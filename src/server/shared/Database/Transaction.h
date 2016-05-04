////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _TRANSACTION_H
#define _TRANSACTION_H

#include "SQLOperation.h"
#include "MSCallback.hpp"

//- Forward declare (don't include header to prevent circular includes)
class PreparedStatement;

/*! Transactions, high level class. */
class Transaction
{
    friend class TransactionTask;
    friend class MySQLConnection;
    friend class DatabaseWokerPool;

    public:
        Transaction() : _cleanedUp(false) {}
        ~Transaction() { Cleanup(); }

        void Append(PreparedStatement* statement);
        void Append(const char* sql);
        void PAppend(const char* sql, ...);

        size_t GetSize() const { return m_queries.size(); }

    //protected:
        void Cleanup();
        std::list<SQLElementData> m_queries;

    private:
        bool _cleanedUp;

};
typedef std::shared_ptr<Transaction> SQLTransaction;

/*! Low level class*/
class TransactionTask : public SQLOperation
{
    template <class T> friend class DatabaseWorkerPool;
    friend class DatabaseWorker;

    public:
        TransactionTask(SQLTransaction trans, MS::Utilities::CallBackPtr p_Callback) : m_trans(trans), m_Callback(p_Callback) {};
        ~TransactionTask(){};

    protected:
        bool Execute();

        SQLTransaction                m_trans;
        MS::Utilities::CallBackPtr    m_Callback;
};

#endif
