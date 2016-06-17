////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "Conversation.hpp"

Conversation::Conversation() :
    WorldObject(false),
    m_Duration(0)
{
    m_objectType    |= TypeMask::TYPEMASK_CONVERSATION;
    m_objectTypeId  = TypeID::TYPEID_CONVERSATION;
    m_updateFlag    = OBJECT_UPDATE_FLAGS::UPDATEFLAG_HAS_POSITION;
    m_valuesCount   = EConversationFields::CONVERSATION_END;
}

Conversation::~Conversation()
{
}

void Conversation::AddToWorld()
{
    /// Register the Conversation for guid lookup
    if (!IsInWorld())
    {
        sObjectAccessor->AddObject(this);
        WorldObject::AddToWorld();
    }
}

void Conversation::RemoveFromWorld()
{
    /// Remove the Conversation from the accessors and from all lists of objects in world
    if (IsInWorld())
    {
        WorldObject::RemoveFromWorld();
        sObjectAccessor->RemoveObject(this);
    }
}

bool Conversation::StartConversation(Unit* p_Source, uint32 p_ConversationEntry, int32 p_Duration)
{
    if (p_Source == nullptr)
        return false;

    SetMap(p_Source->GetMap());

    Position l_Pos = *p_Source;
    Relocate(l_Pos);

    if (!IsPositionValid())
    {
        sLog->outError(LogFilterType::LOG_FILTER_GENERAL, "Conversation::StartConversation -> Invalid coordinates for Conversation [%u] (X: %.3f, Y: %.3f, Z: %.3f)",
            p_ConversationEntry,
            m_positionX,
            m_positionY,
            m_positionZ);

        return false;
    }

    WorldObject::_Create(sObjectMgr->GenerateLowGuid(HighGuid::HIGHGUID_CONVERSATION), HighGuid::HIGHGUID_CONVERSATION, p_Source->GetPhaseMask());

    SetEntry(p_ConversationEntry);
    SetDuration(p_Duration);

    SetObjectScale(1.0f);

    SetUpdateTimerInterval(60);

    if (!GetMap()->AddToMap(this))
        return false;

    AddConversationActor(p_Source->GetGUID());

    return true;
}

void Conversation::AddConversationActor(uint64 p_ActorGuid)
{
    AddDynamicGuidValue(EConversationDynamicFields::CONVERSATION_DYNAMIC_FIELD_ACTORS, p_ActorGuid);
}

void Conversation::Update(uint32 p_Diff)
{
    /// Don't decrease infinite durations
    if (GetDuration() > int32(p_Diff))
        m_Duration -= p_Diff;

    WorldObject::Update(p_Diff);

    m_UpdateTimer.Update(p_Diff);

    if (m_UpdateTimer.Passed())
    {
        m_UpdateTimer.Reset();
    }
}

void Conversation::SetDuration(int32 p_Duration)
{
    m_Duration = p_Duration;
}

int32 Conversation::GetDuration() const
{
    return m_Duration;
}

void Conversation::SetUpdateTimerInterval(uint32 p_Timer)
{
    m_UpdateTimer.SetInterval(p_Timer);
}

uint32 Conversation::GetUpdateInterval() const
{
    return static_cast<uint32>(m_UpdateTimer.GetInterval());
}
