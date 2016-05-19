////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _DATABASEWORKERPOOL_H
#define _DATABASEWORKERPOOL_H

#include "Common.h"
#include "Callback.h"
#include "MySQLConnection.h"
#include "Transaction.h"
#include "DatabaseWorker.h"
#include "PreparedStatement.h"
#include "Log.h"
#include "QueryResult.h"
#include "QueryHolder.h"
#include "AdhocStatement.h"
#include "MSCallback.hpp"
#include <forward_list>
#include <mutex>

class PingOperation : public SQLOperation
{
    //! Operation for idle delaythreads
    bool Execute()
    {
        m_conn->Ping();
        return true;
    }
};

struct QueryHolderCallback
{
    QueryHolderCallback(QueryResultHolderFuture p_QueryResultHolderFuture, std::function<void(SQLQueryHolder*)> p_Callback)
    {
        m_QueryResultHolderFuture = p_QueryResultHolderFuture;
        m_Callback = p_Callback;
    }
    
    QueryResultHolderFuture m_QueryResultHolderFuture;
    std::function<void(SQLQueryHolder*)>   m_Callback;
};

template <class T>
class DatabaseWorkerPool
{
    public:
        /* Activity state */
        DatabaseWorkerPool() :
        _queue(new ACE_Activation_Queue())
        {
            memset(_connectionCount, 0, sizeof(_connectionCount));
            
            /// Update queue size limit, 16 kb is not enough
            _queue->queue()->high_water_mark(8 * 1024 * 1024);
            _queue->queue()->low_water_mark(8 * 1024 * 1024);

            WPFatal (mysql_thread_safe(), "Used MySQL library isn't thread-safe.");
    
            m_QueryHolderCallbacks             = std::unique_ptr<QueryHolderCallbacks>(new QueryHolderCallbacks());
            m_QueryHolderCallbacksBuffer       = std::unique_ptr<QueryHolderCallbacks>(new QueryHolderCallbacks());
            m_TransactionCallbacks             = std::unique_ptr<TransactionCallbacks>(new TransactionCallbacks());
            m_TransactionCallbacksBuffer       = std::unique_ptr<TransactionCallbacks>(new TransactionCallbacks());
            m_PreparedStatementCallbacks       = std::unique_ptr<PreparedStatementCallbacks>(new PreparedStatementCallbacks());
            m_PreparedStatementCallbacksBuffer = std::unique_ptr<PreparedStatementCallbacks>(new PreparedStatementCallbacks());
        }

        ~DatabaseWorkerPool()
        {
        }

        bool Open(const std::string& infoString, uint8 async_threads, uint8 synch_threads)
        {
            bool res = true;
            _connectionInfo = MySQLConnectionInfo(infoString);

            sLog->outInfo(LOG_FILTER_SQL_DRIVER, "Opening DatabasePool '%s'. Asynchronous connections: %u, synchronous connections: %u.",
                GetDatabaseName(), async_threads, synch_threads);

            //! Open asynchronous connections (delayed operations)
            _connections[IDX_ASYNC].resize(async_threads);
            for (uint8 i = 0; i < async_threads; ++i)
            {
                T* t = new T(_queue, _connectionInfo);
                res &= t->Open();
                _connections[IDX_ASYNC][i] = t;
                ++_connectionCount[IDX_ASYNC];
            }

            //! Open synchronous connections (direct, blocking operations)
            _connections[IDX_SYNCH].resize(synch_threads);
            for (uint8 i = 0; i < synch_threads; ++i)
            {
                T* t = new T(_connectionInfo);
                res &= t->Open();
                _connections[IDX_SYNCH][i] = t;
                ++_connectionCount[IDX_SYNCH];
            }

            if (res)
                sLog->outInfo(LOG_FILTER_SQL_DRIVER, "DatabasePool '%s' opened successfully. %u total connections running.", GetDatabaseName(),
                    (_connectionCount[IDX_SYNCH] + _connectionCount[IDX_ASYNC]));
            else
                sLog->outError(LOG_FILTER_SQL_DRIVER, "DatabasePool %s NOT opened. There were errors opening the MySQL connections. Check your SQLDriverLogFile "
                    "for specific errors.", GetDatabaseName());
            return res;
        }

        void Close()
        {
            sLog->outInfo(LOG_FILTER_SQL_DRIVER, "Closing down DatabasePool '%s'.", GetDatabaseName());

            //! Shuts down delaythreads for this connection pool by underlying deactivate().
            //! The next dequeue attempt in the worker thread tasks will result in an error,
            //! ultimately ending the worker thread task.
            _queue->queue()->close();

            for (uint8 i = 0; i < _connectionCount[IDX_ASYNC]; ++i)
            {
                T* t = _connections[IDX_ASYNC][i];
                DatabaseWorker* worker = t->m_worker;
                worker->wait();     //! Block until no more threads are running this task.
                delete worker;
                t->Close();         //! Closes the actualy MySQL connection.
            }

            sLog->outInfo(LOG_FILTER_SQL_DRIVER, "Asynchronous connections on DatabasePool '%s' terminated. Proceeding with synchronous connections.",
                GetDatabaseName());

            //! Shut down the synchronous connections
            //! There's no need for locking the connection, because DatabaseWorkerPool<>::Close
            //! should only be called after any other thread tasks in the core have exited,
            //! meaning there can be no concurrent access at this point.
            for (uint8 i = 0; i < _connectionCount[IDX_SYNCH]; ++i)
                _connections[IDX_SYNCH][i]->Close();

            //! Deletes the ACE_Activation_Queue object and its underlying ACE_Message_Queue
            delete _queue;

            sLog->outInfo(LOG_FILTER_SQL_DRIVER, "All connections on DatabasePool '%s' closed.", GetDatabaseName());
        }

        /**
            Delayed one-way statement methods.
        */

        //! Enqueues a one-way SQL operation in string format that will be executed asynchronously.
        //! This method should only be used for queries that are only executed once, e.g during startup.
        void Execute(const char* sql)
        {
            if (!sql)
                return;

            BasicStatementTask* task = new BasicStatementTask(sql);
            Enqueue(task);
        }

        //! Enqueues a one-way SQL operation in string format -with variable args- that will be executed asynchronously.
        //! This method should only be used for queries that are only executed once, e.g during startup.
        void PExecute(const char* sql, ...)
        {
            if (!sql)
                return;

            va_list ap;
            char szQuery[MAX_QUERY_LEN];
            va_start(ap, sql);
            vsnprintf(szQuery, MAX_QUERY_LEN, sql, ap);
            va_end(ap);

            Execute(szQuery);
        }

        //! Enqueues a one-way SQL operation in prepared statement format that will be executed asynchronously.
        //! Statement must be prepared with CONNECTION_ASYNC flag.
        void Execute(PreparedStatement* stmt)
        {
            if (stmt->getIndex() == 0)
            {
                ACE_Stack_Trace l_Stack;
                sLog->outAshran("DatabaseWorkerPool::Execute: Statement index 0");
                sLog->outAshran(l_Stack.c_str());
                return;
            }

            PreparedStatementTask* task = new PreparedStatementTask(stmt);
            Enqueue(task);
        }

        /**
            Direct synchronous one-way statement methods.
        */

        //! Directly executes a one-way SQL operation in string format, that will block the calling thread until finished.
        //! This method should only be used for queries that are only executed once, e.g during startup.
        void DirectExecute(const char* sql)
        {
            if (!sql)
                return;

            T* t = GetFreeConnection();
            t->Execute(sql);
            t->Unlock();
        }

        //! Directly executes a one-way SQL operation in string format -with variable args-, that will block the calling thread until finished.
        //! This method should only be used for queries that are only executed once, e.g during startup.
        void DirectPExecute(const char* sql, ...)
        {
            if (!sql)
                return;

            va_list ap;
            char szQuery[MAX_QUERY_LEN];
            va_start(ap, sql);
            vsnprintf(szQuery, MAX_QUERY_LEN, sql, ap);
            va_end(ap);

            return DirectExecute(szQuery);
        }

        //! Directly executes a one-way SQL operation in prepared statement format, that will block the calling thread until finished.
        //! Statement must be prepared with the CONNECTION_SYNCH flag.
        void DirectExecute(PreparedStatement* stmt)
        {
            if (stmt->getIndex() == 0)
            {
                ACE_Stack_Trace l_Stack;
                sLog->outAshran("DatabaseWorkerPool::DirectExecute: Statement index 0");
                sLog->outAshran(l_Stack.c_str());
                return;
            }

            T* t = GetFreeConnection();
            t->Execute(stmt);
            t->Unlock();

            //! Delete proxy-class. Not needed anymore
            delete stmt;
        }

        bool DirectExecuteWithReturn(PreparedStatement* stmt)
        {
            if (stmt->getIndex() == 0)
            {
                ACE_Stack_Trace l_Stack;
                sLog->outAshran("DatabaseWorkerPool::DirectExecuteWithReturn: Statement index 0");
                sLog->outAshran(l_Stack.c_str());
                return false;
            }

            T* t = GetFreeConnection();
            bool result = t->Execute(stmt);
            t->Unlock();

            //! Delete proxy-class. Not needed anymore
            delete stmt;
            return result;
        }

        /**
            Synchronous query (with resultset) methods.
        */

        //! Directly executes an SQL query in string format that will block the calling thread until finished.
        //! Returns reference counted auto pointer, no need for manual memory management in upper level code.
        QueryResult Query(const char* sql, MySQLConnection* conn = NULL)
        {
            if (!conn)
                conn = GetFreeConnection();

            ResultSet* result = conn->Query(sql);
            conn->Unlock();
            if (!result || !result->GetRowCount())
            {
                delete result;
                return QueryResult(NULL);
            }
            result->NextRow();
            return QueryResult(result);
        }

        //! Directly executes an SQL query in string format -with variable args- that will block the calling thread until finished.
        //! Returns reference counted auto pointer, no need for manual memory management in upper level code.
        QueryResult PQuery(const char* sql, MySQLConnection* conn, ...)
        {
            if (!sql)
                return QueryResult(NULL);

            va_list ap;
            char szQuery[MAX_QUERY_LEN];
            va_start(ap, conn);
            vsnprintf(szQuery, MAX_QUERY_LEN, sql, ap);
            va_end(ap);

            return Query(szQuery, conn);
        }

        //! Directly executes an SQL query in string format -with variable args- that will block the calling thread until finished.
        //! Returns reference counted auto pointer, no need for manual memory management in upper level code.
        QueryResult PQuery(const char* sql, ...)
        {
            if (!sql)
                return QueryResult(NULL);

            va_list ap;
            char szQuery[MAX_QUERY_LEN];
            va_start(ap, sql);
            vsnprintf(szQuery, MAX_QUERY_LEN, sql, ap);
            va_end(ap);

            return Query(szQuery);
        }

        //! Directly executes an SQL query in prepared format that will block the calling thread until finished.
        //! Returns reference counted auto pointer, no need for manual memory management in upper level code.
        //! Statement must be prepared with CONNECTION_SYNCH flag.
        PreparedQueryResult Query(PreparedStatement* stmt)
        {
            if (stmt->getIndex() == 0)
            {
                ACE_Stack_Trace l_Stack;
                sLog->outAshran("DatabaseWorkerPool::Query: Statement index 0");
                sLog->outAshran(l_Stack.c_str());
                return PreparedQueryResult(NULL);
            }

            T* t = GetFreeConnection();
            PreparedResultSet* ret = t->Query(stmt);
            t->Unlock();

            //! Delete proxy-class. Not needed anymore
            delete stmt;

            if (!ret || !ret->GetRowCount())

            {
                delete ret;
                return PreparedQueryResult(NULL);
            }
            return PreparedQueryResult(ret);
        }

        /**
            Asynchronous query (with resultset) methods.
        */

        //! Enqueues a query in string format that will set the value of the QueryResultFuture return object as soon as the query is executed.
        //! The return value is then processed in ProcessQueryCallback methods.
        QueryResultFuture AsyncQuery(const char* sql)
        {
            QueryResultFuture res;
            BasicStatementTask* task = new BasicStatementTask(sql, res);
            Enqueue(task);
            return res;         //! Actual return value has no use yet
        }

        //! Enqueues a query in string format -with variable args- that will set the value of the QueryResultFuture return object as soon as the query is executed.
        //! The return value is then processed in ProcessQueryCallback methods.
        QueryResultFuture AsyncPQuery(const char* sql, ...)
        {
            va_list ap;
            char szQuery[MAX_QUERY_LEN];
            va_start(ap, sql);
            vsnprintf(szQuery, MAX_QUERY_LEN, sql, ap);
            va_end(ap);

            return AsyncQuery(szQuery);
        }

        //! Enqueues a query in prepared format that will set the value of the PreparedQueryResultFuture return object as soon as the query is executed.
        //! The return value is then processed in ProcessQueryCallback methods.
        //! Statement must be prepared with CONNECTION_ASYNC flag.
        PreparedQueryResultFuture AsyncQuery(PreparedStatement* stmt)
        {
            if (stmt->getIndex() == 0)
            {
                ACE_Stack_Trace l_Stack;
                sLog->outAshran("DatabaseWorkerPool::AsyncQuery: Statement index 0");
                sLog->outAshran(l_Stack.c_str());
                return PreparedQueryResultFuture();
            }

            PreparedQueryResultFuture res;
            PreparedStatementTask* task = new PreparedStatementTask(stmt, res);
            Enqueue(task);
            return res;
        }

        //! Enqueues a query in prepared format that will set the value of the PreparedQueryResultFuture return object as soon as the query is executed.
        //! The return value is then processed in ProcessQueryCallback methods.
        //! Statement must be prepared with CONNECTION_ASYNC flag.
        PreparedQueryResultFuture AsyncQuery(PreparedStatement* stmt, std::function<void(PreparedQueryResult)> p_Callback)
        {
            if (stmt->getIndex() == 0)
            {
                ACE_Stack_Trace l_Stack;
                sLog->outAshran("DatabaseWorkerPool::AsyncQuery(callback): Statement index 0");
                sLog->outAshran(l_Stack.c_str());
                return PreparedQueryResultFuture();
            }

            PreparedQueryResultFuture res;
            PreparedStatementTask* task = new PreparedStatementTask(stmt, res);
            Enqueue(task);
            
            AddPrepareStatementCallback(std::make_pair(p_Callback, res));

            return res;
        }

        //! Enqueues a vector of SQL operations (can be both adhoc and prepared) that will set the value of the QueryResultHolderFuture
        //! return object as soon as the query is executed.
        //! The return value is then processed in ProcessQueryCallback methods.
        //! Any prepared statements added to this holder need to be prepared with the CONNECTION_ASYNC flag.
        QueryResultHolderFuture DelayQueryHolder(SQLQueryHolder* holder)
        {
            QueryResultHolderFuture res;
            SQLQueryHolderTask* task = new SQLQueryHolderTask(holder, res);
            Enqueue(task);
            return res;     //! Fool compiler, has no use yet
        }

        /**
            Transaction context methods.
        */

        //! Begins an automanaged transaction pointer that will automatically rollback if not commited. (Autocommit=0)
        SQLTransaction BeginTransaction()
        {
            return SQLTransaction(new Transaction);
        }

        //! Enqueues a collection of one-way SQL operations (can be both adhoc and prepared). The order in which these operations
        //! were appended to the transaction will be respected during execution.
        void CommitTransaction(SQLTransaction transaction, MS::Utilities::CallBackPtr p_Callback = nullptr)
        {
            #ifdef TRINITY_DEBUG
            //! Only analyze transaction weaknesses in Debug mode.
            //! Ideally we catch the faults in Debug mode and then correct them,
            //! so there's no need to waste these CPU cycles in Release mode.
            switch (transaction->GetSize())
            {
                case 0:
                    sLog->outDebug(LOG_FILTER_SQL_DRIVER, "Transaction contains 0 queries. Not executing.");
                    return;
                case 1:
                    sLog->outDebug(LOG_FILTER_SQL_DRIVER, "Warning: Transaction only holds 1 query, consider removing Transaction context in code.");
                    break;
                default:
                    break;
            }
            #endif // TRINITY_DEBUG

            AddTransactionCallback(p_Callback);

            Enqueue(new TransactionTask(transaction, p_Callback));
        }

        //! Directly executes a collection of one-way SQL operations (can be both adhoc and prepared). The order in which these operations
        //! were appended to the transaction will be respected during execution.
        bool DirectCommitTransaction(SQLTransaction& transaction)
        {
            MySQLConnection* con = GetFreeConnection();
            if (con->ExecuteTransaction(transaction))
            {
                con->Unlock();      // OK, operation succesful
                return true;
            }

            bool error = false;

            //! Handle MySQL Errno 1213 without extending deadlock to the core itself
            //! TODO: More elegant way
            if (con->GetLastError() == 1213)
            {
                uint8 loopBreaker = 5;
                for (uint8 i = 0; i < loopBreaker; ++i)
                {
                    if (con->ExecuteTransaction(transaction))
                    {
                        error = true;
                        break;
                    }
                }
            }

            //! Clean up now.
            transaction->Cleanup();

            con->Unlock();
            return error;
        }

        //! Method used to execute prepared statements in a diverse context.
        //! Will be wrapped in a transaction if valid object is present, otherwise executed standalone.
        void ExecuteOrAppend(SQLTransaction& trans, PreparedStatement* stmt)
        {
            if (stmt->getIndex() == 0)
            {
                ACE_Stack_Trace l_Stack;
                sLog->outAshran("DatabaseWorkerPool::ExecuteOrAppend: Statement index 0");
                sLog->outAshran(l_Stack.c_str());
                return;
            }

            if (trans.get() == nullptr)
                Execute(stmt);
            else
                trans->Append(stmt);
        }

        //! Method used to execute ad-hoc statements in a diverse context.
        //! Will be wrapped in a transaction if valid object is present, otherwise executed standalone.
        void ExecuteOrAppend(SQLTransaction& trans, const char* sql)
        {
            if (trans.get() == nullptr)
                Execute(sql);
            else
                trans->Append(sql);
        }

        /**
            Other
        */

        typedef typename T::Statements PreparedStatementIndex;

        //! Automanaged (internally) pointer to a prepared statement object for usage in upper level code.
        //! Pointer is deleted in this->Query(PreparedStatement*) or PreparedStatementTask::~PreparedStatementTask.
        //! This object is not tied to the prepared statement on the MySQL context yet until execution.
        PreparedStatement* GetPreparedStatement(PreparedStatementIndex index)
        {
            return new PreparedStatement(index);
        }

        //! Apply escape string'ing for current collation. (utf8)
        void EscapeString(std::string& str)
        {
            if (str.empty())
                return;

            char* buf = new char[str.size()*2+1];
            EscapeString(buf, str.c_str(), str.size());
            str = buf;
            delete[] buf;
        }

        //! Keeps all our MySQL connections alive, prevent the server from disconnecting us.
        void KeepAlive()
        {
            //! Ping synchronous connections
            for (uint8 i = 0; i < _connectionCount[IDX_SYNCH]; ++i)
            {
                T* t = _connections[IDX_SYNCH][i];
                if (t->LockIfReady())
                {
                    t->Ping();
                    t->Unlock();
                }
            }

            //! Assuming all worker threads are free, every worker thread will receive 1 ping operation request
            //! If one or more worker threads are busy, the ping operations will not be split evenly, but this doesn't matter
            //! as the sole purpose is to prevent connections from idling.
            for (size_t i = 0; i < _connections[IDX_ASYNC].size(); ++i)
                Enqueue(new PingOperation);
        }

        void ProcessQueriesCallback()
        {
            /// - Update querys holders callbacks
            if (!m_QueryHolderCallbacks->empty())
            {
                m_QueryHolderCallbacks->remove_if([](QueryHolderCallback const& p_Callback) -> bool
                {
                    if (p_Callback.m_QueryResultHolderFuture.ready())
                    {
                        SQLQueryHolder* l_QueryHolder;
                        p_Callback.m_QueryResultHolderFuture.get(l_QueryHolder);
                                                          
                        p_Callback.m_Callback(l_QueryHolder);
                        return true;
                    }
                    return false;
                });
            }
            
            /// - Add querys holders callbacks in buffer queue to real queue
            while (!m_QueryHolderCallbacksBuffer->empty())
            {
                m_QueryHolderCallbacks->push_front(m_QueryHolderCallbacksBuffer->front());
                m_QueryHolderCallbacksBuffer->pop_front();
            }
            
            /// - Update transactions callbacks
            if (!m_TransactionCallbacks->empty())
            {
                m_TransactionCallbacks->remove_if([](MS::Utilities::CallBackPtr const& l_Callback) -> bool
                {
                    if (l_Callback->m_State == MS::Utilities::CallBackState::Waiting)
                        return false;
                                                      
                    l_Callback->m_CallBack(l_Callback->m_State == MS::Utilities::CallBackState::Success);
                    return true;
                });
            }
            
            /// - Add transactions callbacks in buffer queue to real queue
            while (!m_TransactionCallbacksBuffer->empty())
            {
                m_TransactionCallbacks->push_front(m_TransactionCallbacksBuffer->front());
                m_TransactionCallbacksBuffer->pop_front();
            }
            
            /// - Update prepared statements callbacks
            if (!m_PreparedStatementCallbacks->empty())
            {
                m_PreparedStatementCallbacks->remove_if([](PrepareStatementCallback const& p_Callback) -> bool
                {
                    /// If the query result is avaiable ...
                    if (p_Callback.second.ready())
                    {
                        /// Then get it
                        PreparedQueryResult l_Result;
                        p_Callback.second.get(l_Result);
                                                                
                        /// Give the result to the callback, and execute it
                        p_Callback.first(l_Result);

                        /// Delete the callback from the forward list
                        return true;
                    }
                                                            
                    /// We havn't the query result yet, we keep the callback and wait for the result!
                    return false;
                });
            }
            
            /// - Add prepared statements in buffer queue to real queue
            while (!m_PreparedStatementCallbacksBuffer->empty())
            {
                m_PreparedStatementCallbacks->push_front(m_PreparedStatementCallbacksBuffer->front());
                m_PreparedStatementCallbacksBuffer->pop_front();
            }
        }
    
    //////////////////////////////////////////////////////////////////////////
    /// New callback system
    //////////////////////////////////////////////////////////////////////////
    void AddTransactionCallback(std::shared_ptr<MS::Utilities::Callback> p_Callback)
    {
        m_TransactionCallbackLock.lock();
        m_TransactionCallbacksBuffer->push_front(p_Callback);
        m_TransactionCallbackLock.unlock();
    }
    
    void AddPrepareStatementCallback(std::pair<std::function<void(PreparedQueryResult)>, PreparedQueryResultFuture> p_Callback)
    {
        m_PreparedStatementCallbackLock.lock();
        m_PreparedStatementCallbacksBuffer->push_front(p_Callback);
        m_PreparedStatementCallbackLock.unlock();
    }
    
    void AddQueryHolderCallback(QueryHolderCallback p_QueryHolderCallback)
    {
        m_QueryHolderCallbackLock.lock();
        m_QueryHolderCallbacksBuffer->push_front(p_QueryHolderCallback);
        m_QueryHolderCallbackLock.unlock();
    }

    private:

        unsigned long EscapeString(char *to, const char *from, unsigned long length)
        {
            if (!to || !from || !length)
                return 0;

            return mysql_real_escape_string(_connections[IDX_SYNCH][0]->GetHandle(), to, from, length);
        }

        void Enqueue(SQLOperation* op)
        {
            _queue->enqueue(op);
        }

        //! Gets a free connection in the synchronous connection pool.
        //! Caller MUST call t->Unlock() after touching the MySQL context to prevent deadlocks.
        T* GetFreeConnection()
        {
            uint8 i = 0;
            size_t num_cons = _connectionCount[IDX_SYNCH];
            //! Block forever until a connection is free
            for (;;)
            {
                T* t = _connections[IDX_SYNCH][++i % num_cons];
                //! Must be matched with t->Unlock() or you will get deadlocks
                if (t->LockIfReady())
                    return t;
            }

            //! This will be called when Celine Dion learns to sing
            return NULL;
        }

        char const* GetDatabaseName() const
        {
            return _connectionInfo.database.c_str();
        }

    private:
        enum _internalIndex
        {
            IDX_ASYNC,
            IDX_SYNCH,
            IDX_SIZE
        };

        ACE_Activation_Queue*           _queue;             //! Queue shared by async worker threads.
        std::vector<T*>                 _connections[IDX_SIZE];
        uint32                          _connectionCount[IDX_SIZE];       //! Counter of MySQL connections;
        MySQLConnectionInfo             _connectionInfo;
    
        //////////////////////////////////////////////////////////////////////////
        /// New query holder callback system
        //////////////////////////////////////////////////////////////////////////
        using QueryHolderCallbacks = std::forward_list<QueryHolderCallback>;
        std::unique_ptr<QueryHolderCallbacks> m_QueryHolderCallbacks;
        std::unique_ptr<QueryHolderCallbacks> m_QueryHolderCallbacksBuffer;
        std::mutex m_QueryHolderCallbackLock;
    
        //////////////////////////////////////////////////////////////////////////
        /// New transaction query callback system
        //////////////////////////////////////////////////////////////////////////
        using TransactionCallbacks = std::forward_list<std::shared_ptr<MS::Utilities::Callback>>;
        std::unique_ptr<TransactionCallbacks> m_TransactionCallbacks;
        std::unique_ptr<TransactionCallbacks> m_TransactionCallbacksBuffer;
        std::mutex m_TransactionCallbackLock;
    
        //////////////////////////////////////////////////////////////////////////
        /// New prepare statement query callback system
        //////////////////////////////////////////////////////////////////////////
        using PrepareStatementCallback = std::pair<std::function<void(PreparedQueryResult)>, PreparedQueryResultFuture>;
        using PreparedStatementCallbacks = std::forward_list<PrepareStatementCallback>;
        std::unique_ptr<PreparedStatementCallbacks> m_PreparedStatementCallbacks;
        std::unique_ptr<PreparedStatementCallbacks> m_PreparedStatementCallbacksBuffer;
        std::mutex m_PreparedStatementCallbackLock;
};

#endif
