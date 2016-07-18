////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCRIPTING_INTERFACES_MISC_HPP_INCLUDED
#define SCRIPTING_INTERFACES_MISC_HPP_INCLUDED

#include "InterfaceBase.hpp"

/// Server script interface
class ServerScript : public ScriptObjectImpl<false>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        ServerScript(const char * p_Name);

    public:
        /// Called when reactive socket I/O is started (WorldSocketMgr).
        virtual void OnNetworkStart()
        {

        }
        /// Called when reactive I/O is stopped.
        virtual void OnNetworkStop()
        {

        }

        /// Called when a remote socket establishes a connection to the server. Do not store the socket object.
        /// @p_Socket : Opened socket
        virtual void OnSocketOpen(WorldSocket * p_Socket)
        {
            UNUSED(p_Socket);
        }
        /// Called when a socket is closed. Do not store the socket object, and do not rely on the connection being open; it is not.
        /// @p_Socket : Closed socket
        /// @p_WasNew : Was new ?
        virtual void OnSocketClose(WorldSocket * p_Socket, bool p_WasNew)
        {
            UNUSED(p_Socket);
            UNUSED(p_WasNew);
        }

        /// Called when a packet is sent to a client. The packet object is a copy of the original packet, so reading and modifying it is safe.
        /// @p_Socket : Socket who send the packet
        /// @p_Packet : Sent packet
        virtual void OnPacketSend(WorldSocket * p_Socket, WorldPacket & p_Packet)
        {
            UNUSED(p_Socket);
            UNUSED(p_Packet);
        }

        /// Called when a (valid) packet is received by a client. The packet object is a copy of the original packet, so reading and modifying it is safe.
        /// @p_Socket  : Socket who received the packet
        /// @p_Packet  : Received packet
        /// @p_Session : Session who received the packet /!\ CAN BE NULLPTR
        virtual void OnPacketReceive(WorldSocket* p_Socket, WorldPacket& p_Packet, WorldSession* p_Session)
        {
            UNUSED(p_Socket);
            UNUSED(p_Packet);
            UNUSED(p_Session);
        }

        /// Called when an invalid (unknown opcode) packet is received by a client. The packet is a reference to the original packet; not a copy.
        /// This allows you to actually handle unknown packets (for whatever purpose).
        /// @p_Socket : Socket who received the packet
        /// @p_Packet : Received packet
        virtual void OnUnknownPacketReceive(WorldSocket * p_Socket, WorldPacket & p_Packet)
        {
            UNUSED(p_Socket);
            UNUSED(p_Packet);
        }

};

/// World server script interface
class WorldScript : public ScriptObjectImpl<false>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        WorldScript(const char * p_Name);

    public:
        /// Called when the open/closed state of the world changes.
        /// @p_Open : Open ?
        virtual void OnOpenStateChange(bool p_Open)
        {
            UNUSED(p_Open);
        }

        /// Called after the world configuration is (re)loaded.
        /// @p_Reload : Is the config reload
        virtual void OnConfigLoad(bool p_Reload)
        {
            UNUSED(p_Reload);
        }

        /// Called before the message of the day is changed.
        /// @p_NewMotd : New server message of the day
        virtual void OnMotdChange(std::string & p_NewMotd)
        {
            UNUSED(p_NewMotd);
        }

        /// Called when a world shutdown is initiated.
        /// @p_Code : Server exit code
        /// @p_Mask : Shutdown mask
        virtual void OnShutdownInitiate(ShutdownExitCode p_Code, ShutdownMask p_Mask)
        {
            UNUSED(p_Code);
            UNUSED(p_Mask);
        }

        /// Called when a world shutdown is cancelled.
        virtual void OnShutdownCancel()
        {

        }

        /// Called on every world tick (don't execute too heavy code here).
        /// @p_Diff : Time since last update
        virtual void OnUpdate(uint32 p_Diff)
        {
            UNUSED(p_Diff);
        }

        /// Called when the world is started.
        virtual void OnStartup()
        {

        }
        /// Called when the world is actually shut down.
        virtual void OnShutdown()
        {

        }

};

/// Command Script Interface
class CommandScript : public ScriptObjectImpl<false>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        CommandScript(const char * p_Name);

    public:
        /// Should return a pointer to a valid command table (ChatCommand array) to be used by ChatHandler.
        virtual ChatCommand * GetCommands() const = 0;

};

/// Weather Interface Script
class WeatherScript : public ScriptObjectImpl<true>, public UpdatableScript<Weather>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        WeatherScript(const char * p_Name);

    public:
        /// Called when the weather changes in the zone this script is associated with.
        /// @p_Weather : Weather instance
        /// @p_State   : New weather state
        /// @p_Grade   : New weather grade
        virtual void OnChange(Weather* p_Weather, WeatherState p_State, float p_Grade)
        {
            UNUSED(p_Weather);
            UNUSED(p_State);
            UNUSED(p_Grade);
        }

};

/// Dynamic Object Script Interface
class DynamicObjectScript : public ScriptObjectImpl<false>, public UpdatableScript<DynamicObject>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        DynamicObjectScript(const char * p_Name);

};

#ifndef CROSS
/// Auction House Script Interface
class AuctionHouseScript : public ScriptObjectImpl<false>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        AuctionHouseScript(const char * p_Name);

    public:
        /// Called when an auction is added to an auction house.
        /// @p_AuctionHouseObject : Auction House Object Instance
        /// @p_Entry              : Auction to add
        virtual void OnAuctionAdd(AuctionHouseObject * p_AuctionHouseObject, AuctionEntry * p_Entry)
        {
            UNUSED(p_AuctionHouseObject);
            UNUSED(p_Entry);
        }

        /// Called when an auction is removed from an auction house.
        /// @p_AuctionHouseObject : Auction House Object Instance
        /// @p_Entry              : Auction to remove
        virtual void OnAuctionRemove(AuctionHouseObject * p_AuctionHouseObject, AuctionEntry * p_Entry)
        {
            UNUSED(p_AuctionHouseObject);
            UNUSED(p_Entry);
        }
        /// Called when an auction was successfully completed.
        /// @p_AuctionHouseObject : Auction House Object Instance
        /// @p_Entry              : Auction instance
        virtual void OnAuctionSuccessful(AuctionHouseObject * p_AuctionHouseObject, AuctionEntry * p_Entry)
        {
            UNUSED(p_AuctionHouseObject);
            UNUSED(p_Entry);
        }
        /// Called when an auction expires.
        /// @p_AuctionHouseObject : Auction House Object Instance
        /// @p_Entry              : Auction who expired
        virtual void OnAuctionExpire(AuctionHouseObject * p_AuctionHouseObject, AuctionEntry * p_Entry)
        {
            UNUSED(p_AuctionHouseObject);
            UNUSED(p_Entry);
        }

};

#endif /* not CROSS */
#endif  ///< SCRIPTING_INTERFACES_MISC_HPP_INCLUDED
