////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCRIPTING_INTERFACES_MAP_HPP_INCLUDED
#define SCRIPTING_INTERFACES_MAP_HPP_INCLUDED

#include "InterfaceBase.hpp"

/// Map script interface
template<class TMap> class MapScript : public UpdatableScript<TMap>
{
    protected:
        /// Constructor
        /// @p_MapID : Map Script map ID
        MapScript(uint32 p_MapID)
            : m_MapEntry(sMapStore.LookupEntry(p_MapID))
        {
            if (!m_MapEntry)
                sLog->outError(LOG_FILTER_TSCR, "Invalid MapScript for %u; no such map ID.", p_MapID);
        }

    public:
        /// Gets the MapEntry structure associated with this script. Can return nullptr.
        MapEntry const* GetEntry()
        {
            return m_MapEntry;
        }

        /// Called when the map is created.
        /// @p_Map : Created map instance
        virtual void OnCreate(TMap * p_Map)
        {
            UNUSED(p_Map);
        }
        /// Called just before the map is destroyed.
        /// @p_Map : Destroyed map instance
        virtual void OnDestroy(TMap * p_Map)
        {
            UNUSED(p_Map);
        }

        /// Called when a grid map is loaded.
        /// @p_Map     : Context map
        /// @p_GridMap : Grid map container
        /// @p_GridX   : Loaded grid X offset
        /// @p_GridY   : Loaded grid Y offset
        virtual void OnLoadGridMap(TMap * p_Map, GridMap * p_GripMap, uint32 p_GridX, uint32 p_GridY)
        {
            UNUSED(p_Map);
            UNUSED(p_GripMap);
            UNUSED(p_GridX);
            UNUSED(p_GridY);
        }
        /// Called when a grid map is unloaded.
        /// @p_Map     : Context map
        /// @p_GridMap : Grid map container
        /// @p_GridX   : Unloaded grid X offset
        /// @p_GridY   : Unloaded grid Y offset
        virtual void OnUnloadGridMap(TMap * p_Map, GridMap * p_GripMap, uint32 p_GridX, uint32 p_GridY)
        {
            UNUSED(p_Map);
            UNUSED(p_GripMap);
            UNUSED(p_GridX);
            UNUSED(p_GridY);
        }

        /// Called when a player enters the map.
        /// @p_Map    : Context map
        /// @p_Player : Entered player instance
        virtual void OnPlayerEnter(TMap * p_Map, Player * p_Player)
        {
            UNUSED(p_Map);
            UNUSED(p_Player);
        }
        /// Called when a player leaves the map.
        /// @p_Map    : Context map
        /// @p_Player : Leaved player instance
        virtual void OnPlayerLeave(TMap * p_Map, Player * p_Player)
        {
            UNUSED(p_Map);
            UNUSED(p_Player);
        }

        /// Called on every map update tick.
        /// @p_Map  : Context map
        /// @p_Diff : Time since last update
        virtual void OnUpdate(TMap * p_Map, uint32 p_Diff)
        {
            UNUSED(p_Map);
            UNUSED(p_Diff);
        }

    private:
        /// DBC Map Entry
        const MapEntry * m_MapEntry;

};

/// World map script interface
class WorldMapScript : public ScriptObjectImpl<false>, public MapScript<Map>
{
    protected:
        /// Constructor
        /// @p_Name  : Script name
        /// @p_MapID : Map ID
        WorldMapScript(const char* p_Name, uint32 p_MapID);

};

/// Instance map script interface
class InstanceMapScript : public ScriptObjectImpl<true>, public MapScript<InstanceMap>
{
    protected:
        /// Constructor
        /// @p_Name  : Script name
        /// @p_MapID : Map ID
        InstanceMapScript(const char* p_Name, uint32 p_MapID);

    public:
        /// Gets an InstanceScript object for this instance.
        /// @p_Map : Context map
        virtual InstanceScript * GetInstanceScript(InstanceMap * p_Map) const
        {
            UNUSED(p_Map);

            return nullptr;
        }

};

/// Battle ground map script interface
class BattlegroundMapScript : public ScriptObjectImpl<false>, public MapScript<BattlegroundMap>
{
    protected:
        /// Constructor
        /// @p_Name  : Script name
        /// @p_MapID : Map ID
        BattlegroundMapScript(const char* p_Name, uint32 p_MapID);

};

#endif  ///< SCRIPTING_INTERFACES_MAP_HPP_INCLUDED
