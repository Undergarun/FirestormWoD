////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CONVERSATION_HPP
# define CONVERSATION_HPP

# include "Object.h"
# include "CellImpl.h"
# include "GridNotifiers.h"
# include "GridNotifiersImpl.h"
# include "Map.h"

class Conversation : public WorldObject
{
    protected:
        Conversation();

    public:
        virtual ~Conversation();

        void AddConversationActor(uint64 p_ActorGuid);
};

#endif ///< CONVERSATION_HPP
