////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "Conversation.hpp"

Conversation::Conversation() : WorldObject(false)
{
    m_objectType    |= TypeMask::TYPEMASK_CONVERSATION;
    m_objectTypeId  = TypeID::TYPEID_CONVERSATION;
    m_updateFlag    = 0;
    m_valuesCount   = EConversationFields::CONVERSATION_END;
}

Conversation::~Conversation()
{
}

void Conversation::AddConversationActor(uint64 p_ActorGuid)
{
    AddDynamicGuidValue(EConversationDynamicFields::CONVERSATION_DYNAMIC_FIELD_ACTORS, p_ActorGuid);
}
