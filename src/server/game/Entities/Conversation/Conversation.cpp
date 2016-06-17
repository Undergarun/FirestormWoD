////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "Conversation.hpp"
#include "InstanceScript.h"

Conversation::Conversation() :
    WorldObject(false),
    m_Duration(0)
{
    m_objectType        |= TypeMask::TYPEMASK_CONVERSATION;
    m_objectTypeId      = TypeID::TYPEID_CONVERSATION;
    m_updateFlag        = OBJECT_UPDATE_FLAGS::UPDATEFLAG_HAS_POSITION;
    m_valuesCount       = EConversationFields::CONVERSATION_END;
    _dynamicValuesCount = EConversationDynamicFields::CONVERSATION_DYNAMIC_END;
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

void Conversation::Remove()
{
    if (IsInWorld())
    {
        RemoveFromWorld();
        AddObjectToRemoveList();
    }
}

bool Conversation::StartConversation(Unit* p_Source, uint32 p_ConversationEntry)
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

    InstanceScript* l_InstanceScript = p_Source->GetInstanceScript();
    if (l_InstanceScript == nullptr)
    {
        sLog->outError(LogFilterType::LOG_FILTER_GENERAL, "Conversation::StartConversation -> Conversation [%u] isn't in an instanced map.", p_ConversationEntry);
        return false;
    }

    ConversationTemplate const* l_ConvTemplate = sObjectMgr->GetConversationTemplate(p_ConversationEntry);
    if (l_ConvTemplate == nullptr)
    {
        sLog->outError(LogFilterType::LOG_FILTER_GENERAL, "Conversation::StartConversation -> Conversation [%u] doesn't have a template.", p_ConversationEntry);
        return false;
    }

    WorldObject::_Create(sObjectMgr->GenerateLowGuid(HighGuid::HIGHGUID_CONVERSATION), HighGuid::HIGHGUID_CONVERSATION, p_Source->GetPhaseMask());

    SetEntry(p_ConversationEntry);
    SetDuration(l_ConvTemplate->Duration);

    SetObjectScale(1.0f);

    SetUpdateTimerInterval(60);

    for (auto l_Itr = l_ConvTemplate->Actors.cbegin(); l_Itr != l_ConvTemplate->Actors.cend(); ++l_Itr)
    {
        if (Creature* l_Creature = Creature::GetCreature(*p_Source, l_InstanceScript->GetData64(*l_Itr)))
            AddConversationActor(l_Creature->GetGUID());
        else
            AddConversationActor(0);
    }

    for (auto l_Itr = l_ConvTemplate->Lines.cbegin(); l_Itr != l_ConvTemplate->Lines.cend(); ++l_Itr)
        AddConversationLine(*l_Itr);

    if (!GetMap()->AddToMap(this))
        return false;

    return true;
}

void Conversation::AddConversationActor(uint64 p_ActorGuid)
{
    AddDynamicGuidValue(EConversationDynamicFields::CONVERSATION_DYNAMIC_FIELD_ACTORS, p_ActorGuid);
}

void Conversation::AddConversationLine(ConversationLine p_Line)
{
    AddDynamicValue(EConversationDynamicFields::CONVERSATION_DYNAMIC_FIELD_LINES, p_Line.LineID);
    AddDynamicValue(EConversationDynamicFields::CONVERSATION_DYNAMIC_FIELD_LINES, p_Line.BroadcastTextID);
    AddDynamicValue(EConversationDynamicFields::CONVERSATION_DYNAMIC_FIELD_LINES, p_Line.UnkValue);
    AddDynamicValue(EConversationDynamicFields::CONVERSATION_DYNAMIC_FIELD_LINES, p_Line.Timer);
    AddDynamicValue(EConversationDynamicFields::CONVERSATION_DYNAMIC_FIELD_LINES, p_Line.Type);
}

void Conversation::Update(uint32 p_Diff)
{
    /// Don't decrease infinite durations
    if (GetDuration() > int32(p_Diff))
        m_Duration -= p_Diff;
    else if (GetDuration() != -1 && GetDuration() <= 0)
    {
        Remove(); ///< Expired
        return;
    }

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
