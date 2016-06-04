////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCRIPTING_INTERFACES_GROUP_HPP_INCLUDED
#define SCRIPTING_INTERFACES_GROUP_HPP_INCLUDED

#include "InterfaceBase.hpp"

/// Group script interface
class GroupScript : public ScriptObjectImpl<false>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        GroupScript(const char * p_Name);

    public:
        /// Called when a member is added to a group.
        /// @p_Group : Group Instance
        /// @p_GUID  : Added member GUID
        virtual void OnAddMember(Group * p_Group, uint64 p_Guid)
        {
            UNUSED(p_Group);
            UNUSED(p_Guid);
        }
        /// Called when a member is invited to join a group.
        /// @p_Group : Group Instance
        /// @p_GUID  : Invited member GUID
        virtual void OnInviteMember(Group * p_Group, uint64 p_GUID)
        {
            UNUSED(p_Group);
            UNUSED(p_GUID);
        }
        /// Called when a member is removed from a group.
        /// @p_Group      : Group Instance
        /// @p_GUID       : Removed member GUID
        /// @p_Method     : Remove method
        /// @p_KickerGUID : Kicker GUID
        /// @p_Reason     : Kick reason
        virtual void OnRemoveMember(Group * p_Group, uint64 p_GUID, RemoveMethod p_Method, uint64 p_KickerGUID, const char * p_Reason)
        {
            UNUSED(p_Group);
            UNUSED(p_GUID);
            UNUSED(p_Method);
            UNUSED(p_KickerGUID);
            UNUSED(p_Reason);
        }

        /// Called when the leader of a group is changed.
        /// @p_Group         : Group Instance
        /// @p_NewLeaderGUID : New group leader GUID
        /// @p_OldLeaderGUID : Old group leader GUID
        virtual void OnChangeLeader(Group * p_Group, uint64 p_NewLeaderGUID, uint64 p_OldLeaderGUID)
        {
            UNUSED(p_Group);
            UNUSED(p_NewLeaderGUID);
            UNUSED(p_OldLeaderGUID);
        }

        /// Called when a group is disbanded.
        /// @p_Group : Group Instance
        virtual void OnDisband(Group * p_Group)
        {
            UNUSED(p_Group);
        }

};

#endif  ///< SCRIPTING_INTERFACES_GROUP_HPP_INCLUDED
