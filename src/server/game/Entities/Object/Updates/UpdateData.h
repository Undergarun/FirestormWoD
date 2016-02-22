/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __UPDATEDATA_H
#define __UPDATEDATA_H

#include "ByteBuffer.h"
class WorldPacket;

enum OBJECT_UPDATE_TYPE
{
    UPDATETYPE_VALUES               = 0,
    UPDATETYPE_CREATE_OBJECT        = 1,
    UPDATETYPE_CREATE_OBJECT2       = 2,
    UPDATETYPE_OUT_OF_RANGE_OBJECTS = 3,
};

enum OBJECT_UPDATE_FLAGS
{
    UPDATEFLAG_NONE                     = 0x00000000,
    UPDATEFLAG_NO_BIRTH_ANIM            = 0x00000001,
    UPDATEFLAG_ENABLE_PORTALS           = 0x00000002,
    UPDATEFLAG_PLAY_HOVER_ANIM          = 0x00000004,
    UPDATEFLAG_SUPPRESSED_GREETINGS     = 0x00000008,
    UPDATEFLAG_HAS_MOVEMENT_UPDATE      = 0x00000010,
    UPDATEFLAG_HAS_TRANSPORT_POSITION   = 0x00000020,
    UPDATEFLAG_HAS_POSITION             = 0x00000040,
    UPDATEFLAG_HAS_COMBAT_VICTIM        = 0x00000080,
    UPDATEFLAG_HAS_SERVER_TIME          = 0x00000100,
    UPDATEFLAG_HAS_VEHICLE_CREATE       = 0x00000200,
    UPDATEFLAG_HAS_ANIMKITS_CREATE      = 0x00000400,
    UPDATEFLAG_HAS_ROTATION             = 0x00000800,
    UPDATEFLAG_HAS_AREATRIGGER          = 0x00001000,
    UPDATEFLAG_HAS_GAMEOBJECT           = 0x00002000,
    UPDATEFLAG_THIS_IS_YOU              = 0x00004000,
    UPDATEFLAG_REPLACE_YOU              = 0x00008000,
    UPDATEFLAG_SCENE_OBJECT             = 0x00010000,
    UPDATEFLAG_SCENE_PENDING_INSTANCES  = 0x00020000,
};

class UpdateData
{
    public:
        UpdateData(uint16 map);
        UpdateData(UpdateData&& right) : m_map(right.m_map), m_blockCount(right.m_blockCount),
            m_outOfRangeGUIDs(std::move(right.m_outOfRangeGUIDs)),
            m_data(std::move(right.m_data)) {}

        void AddOutOfRangeGUID(std::set<uint64>& guids);
        void AddOutOfRangeGUID(uint64 guid);
        void AddUpdateBlock(const ByteBuffer &block);
        bool BuildPacket(WorldPacket* packet);
        bool HasData() const { return m_blockCount > 0 || !m_outOfRangeGUIDs.empty(); }
        void Clear();

        std::set<uint64> const& GetOutOfRangeGUIDs() const { return m_outOfRangeGUIDs; }

    protected:
        uint16 m_map;
        uint32 m_blockCount;
        std::set<uint64> m_outOfRangeGUIDs;
        ByteBuffer m_data;

        UpdateData(UpdateData const& right) = delete;
        UpdateData& operator=(UpdateData const& right) = delete;
};
#endif

