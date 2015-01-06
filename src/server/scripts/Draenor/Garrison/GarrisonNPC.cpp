#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "Garrison.h"

enum 
{
    SPELL_LEFT_ROPE                                 = 164400,
    SPELL_RIGHT_ROPE                                = 164422,

    NPC_GARRISON_ALLIANCE_CART                      = 81627,
    NPC_GARRISON_ALLIANCE_HORSE                     = 81633,

    QUEST_ETABLISH_YOUR_GARRISON_A                  = 34586,
    QUEST_ETABLISH_YOUR_GARRISON_H                  = 34378,
    QUEST_KEEPING_IT_TOGETHER                       = 35176,
    QUEST_SHIP_SALVAGE                              = 35166,
    QUEST_PALE_MOONLIGHT                            = 35174,

    NPC_ASSISTANT_BRIGHTSTONE_TEXT_1                = 85492,
    NPC_ASSISTANT_BRIGHTSTONE_TEXT_2                = 85490,
    NPC_ASSISTANT_BRIGHTSTONE_TEXT_3                = 85493,
    NPC_ASSISTANT_BRIGHTSTONE_TEXT_QUEST            = 83682,
    NPC_SHELLY_HAMBY_TEXT_QUEST                     = 83505,
    NPC_BAROS_ETABLISH_YOUR_GARRISON_CHAT           = 0,
    NPC_BAROS_KEEPING_IT_TOGETHER_START_CHAT        = 1,
    NPC_BAROS_KEEPING_IT_TOGETHER_END_CHAT          = 2,
    NPC_BAROS_SHIP_SALVAGE_START_CHAT               = 3,
    NPC_BAROS_SHIP_SALVAGE_END_CHAT                 = 4,
    NPC_VINDICATOR_MARAAD_PALE_MOONLIGHT_START_CHAT = 0,
    NPC_VINDICATOR_MARAAD_PALE_MOONLIGHT_END_CHAT   = 1,

    ITEM_SHELLY_HAMBY_REPORT                        = 112730,
};

float gGarrisonCreationCoords[][4] =
{
    { 1766.761475f,  191.2846830f,  72.115326f, 0.4649370f },   ///< TEAM_ALLIANCE
    { 5698.020020f, 4512.1635574f, 127.401695f, 2.8622720f }    ///< TEAM_HORDE
};

/// Garrison Ford
class npc_GarrisonFord : public CreatureScript
{
    public:
        /// Constructor
        npc_GarrisonFord()
            : CreatureScript("npc_GarrisonFord")
        { 

        }

        /// Called when a player opens a gossip dialog with the creature.
        bool OnGossipHello(Player * p_Player, Creature * p_Creature) override
        {
            if (!p_Player->GetGarrison())
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Create me a garrison.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
            else
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Delete my garrison.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
            
            p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

            return true;
        }

        /// Called when a player selects a gossip item in the creature's gossip menu.
        bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override
        {
            if (!p_Player->GetGarrison())
            {
                p_Player->CLOSE_GOSSIP_MENU();
                p_Player->CreateGarrison();

                uint32 l_MovieID = p_Player->GetGarrison()->GetGarrisonSiteLevelEntry()->CreationMovie;
                uint32 l_MapID = p_Player->GetGarrison()->GetGarrisonSiteLevelEntry()->MapID;
                uint32 l_TeamID = p_Player->GetTeamId();

                p_Player->AddMovieDelayedTeleport(l_MovieID, l_MapID, gGarrisonCreationCoords[l_TeamID][0], gGarrisonCreationCoords[l_TeamID][1], gGarrisonCreationCoords[l_TeamID][2], gGarrisonCreationCoords[l_TeamID][3]);
                p_Player->SendMovieStart(l_MovieID);

                if (l_TeamID == TEAM_ALLIANCE)
                {
                    p_Player->AddQuest(sObjectMgr->GetQuestTemplate(QUEST_ETABLISH_YOUR_GARRISON_A), p_Creature);
                    p_Player->CompleteQuest(QUEST_ETABLISH_YOUR_GARRISON_A);
                }
                else if (l_TeamID == TEAM_HORDE)
                {
                    p_Player->AddQuest(sObjectMgr->GetQuestTemplate(QUEST_ETABLISH_YOUR_GARRISON_H), p_Creature);
                    p_Player->CompleteQuest(QUEST_ETABLISH_YOUR_GARRISON_H);
                }

                /// HACK until shadowmoon quest are done : add follower Qiana Moonshadow / Olin Umberhide
                p_Player->GetGarrison()->AddFollower(34);
                p_Player->GetGarrison()->AddFollower(89);
                p_Player->GetGarrison()->AddFollower(92);

                /// HACK until quest : add barracks plan
                p_Player->GetGarrison()->LearnBlueprint(26);
            }
            else
            {
                if (p_Player->GetCurrency(GARRISON_CURRENCY_ID, false))
                    p_Player->ModifyCurrency(GARRISON_CURRENCY_ID, -p_Player->GetCurrency(GARRISON_CURRENCY_ID, false));

                p_Player->DeleteGarrison();
            }

            return true;
        }
};

/// Garrison ROPE owner
class npc_GarrisonCartRope : public CreatureScript
{
    public:
        /// Constructor
        npc_GarrisonCartRope() 
            : CreatureScript("npc_GarrisonCartRope")
        { 

        }

        CreatureAI* GetAI(Creature * p_Creature) const
        {
            return new npc_GarrisonCartRopeAI(p_Creature);
        }

        struct npc_GarrisonCartRopeAI : public CreatureAI
        {
            /// Constructor
            npc_GarrisonCartRopeAI(Creature * p_Creature) 
                : CreatureAI(p_Creature)
            {
                HasRope = false;
                me->SetFlag(UNIT_FIELD_FLAGS2, UNIT_FLAG2_DISABLE_TURN);
            }

            void Reset() override
            {
                HasRope = false;
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                if (me->GetEntry() == NPC_GARRISON_ALLIANCE_CART && !HasRope)
                {
                    Creature * l_Horse = me->FindNearestCreature(NPC_GARRISON_ALLIANCE_HORSE, 2.5f);

                    if (l_Horse)
                    {
                        me->CastSpell(l_Horse, SPELL_LEFT_ROPE, TRIGGERED_FULL_MASK);
                        HasRope = true;
                    }
                }
                else if (me->GetEntry() == NPC_GARRISON_ALLIANCE_HORSE && !HasRope)
                {
                    Creature * l_Cart = me->FindNearestCreature(NPC_GARRISON_ALLIANCE_CART, 4.0f);

                    if (l_Cart && l_Cart->GetEntry() == NPC_GARRISON_ALLIANCE_CART)
                    {
                        me->CastSpell(l_Cart, SPELL_RIGHT_ROPE, TRIGGERED_FULL_MASK);
                        HasRope = true;
                    }
                }
            }

            bool HasRope;
        };
};

/// Garrison assistant brightstone
class npc_AssistantBrightstone : public CreatureScript
{
    public:
        /// Constructor
        npc_AssistantBrightstone()
            : CreatureScript("npc_AssistantBrightstone")
        {

        }

        /// Called when a player opens a gossip dialog with the creature.
        bool OnGossipHello(Player * p_Player, Creature * p_Creature) override
        {
            if (p_Player->GetQuestStatus(QUEST_KEEPING_IT_TOGETHER) == QUEST_STATUS_INCOMPLETE && p_Player->GetQuestObjectiveCounter(273535) != 1)
            {
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Time to get back to work.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
                p_Player->SEND_GOSSIP_MENU(NPC_ASSISTANT_BRIGHTSTONE_TEXT_QUEST, p_Creature->GetGUID());
            }
            else
            {
                p_Player->SEND_GOSSIP_MENU(RAND(NPC_ASSISTANT_BRIGHTSTONE_TEXT_1, NPC_ASSISTANT_BRIGHTSTONE_TEXT_2, NPC_ASSISTANT_BRIGHTSTONE_TEXT_3), p_Creature->GetGUID());
            }

            return true;
        }

        /// Called when a player selects a gossip item in the creature's gossip menu.
        bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override
        {
            if (p_Player->GetQuestStatus(QUEST_KEEPING_IT_TOGETHER) == QUEST_STATUS_INCOMPLETE && p_Player->GetQuestObjectiveCounter(273535) != 1)
            {
                p_Player->QuestObjectiveSatisfy(84455, 1, QUEST_OBJECTIVE_TYPE_NPC, p_Creature->GetGUID());

                // @TODO move peon arround

                p_Player->CLOSE_GOSSIP_MENU();
                p_Player->SEND_GOSSIP_MENU(NPC_ASSISTANT_BRIGHTSTONE_TEXT_QUEST, p_Creature->GetGUID());
            }

            return true;
        }

};

/// Garrison shelly hamby
class npc_ShellyHamby : public CreatureScript
{
    public:
        /// Constructor
        npc_ShellyHamby()
            : CreatureScript("npc_ShellyHamby")
        {

        }

        /// Called when a player opens a gossip dialog with the creature.
        bool OnGossipHello(Player * p_Player, Creature * p_Creature) override
        {
            if (!p_Player->HasItemCount(ITEM_SHELLY_HAMBY_REPORT, 1))
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Gather Shelly's report.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

            p_Player->SEND_GOSSIP_MENU(NPC_SHELLY_HAMBY_TEXT_QUEST, p_Creature->GetGUID());

            return true;
        }

        /// Called when a player selects a gossip item in the creature's gossip menu.
        bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override
        {
            p_Player->CLOSE_GOSSIP_MENU();
            p_Creature->AI()->Talk(0);

            if (!p_Player->HasItemCount(ITEM_SHELLY_HAMBY_REPORT, 1))
                p_Player->AddItem(ITEM_SHELLY_HAMBY_REPORT, 1);

            return true;
        }

};

/// Garrison baros alexston
class npc_BarosAlexsom : public CreatureScript
{
    public:
        /// Constructor
        npc_BarosAlexsom()
            : CreatureScript("npc_BarosAlexsom")
        {

        }

        /// Called when a player accepts a quest from the creature.
        virtual bool OnQuestAccept(Player * p_Player, Creature * p_Creature, const Quest * p_Quest) override
        {
            if (p_Quest && p_Quest->GetQuestId() == QUEST_KEEPING_IT_TOGETHER)
                p_Creature->AI()->Talk(NPC_BAROS_KEEPING_IT_TOGETHER_START_CHAT);
            else if (p_Quest && p_Quest->GetQuestId() == QUEST_SHIP_SALVAGE)
                p_Creature->AI()->Talk(NPC_BAROS_SHIP_SALVAGE_START_CHAT);

            return false;
        }
        /// Called when a player completes a quest with the creature.
        virtual bool OnQuestComplete(Player * p_Player, Creature * p_Creature, const Quest * p_Quest) override
        {
            if (p_Quest && p_Quest->GetQuestId() == QUEST_ETABLISH_YOUR_GARRISON_A)
                p_Creature->AI()->Talk(NPC_BAROS_ETABLISH_YOUR_GARRISON_CHAT);
            else if (p_Quest && p_Quest->GetQuestId() == QUEST_KEEPING_IT_TOGETHER)
                p_Creature->AI()->Talk(NPC_BAROS_KEEPING_IT_TOGETHER_END_CHAT);
            else if (p_Quest && p_Quest->GetQuestId() == QUEST_SHIP_SALVAGE)
                p_Creature->AI()->Talk(NPC_BAROS_SHIP_SALVAGE_END_CHAT);

            return false;
        }

};

/// Garrison vindicator maraad
class npc_VindicatorMaraad : public CreatureScript
{
    public:
        /// Constructor
        npc_VindicatorMaraad()
            : CreatureScript("npc_VindicatorMaraad")
        {

        }

        /// Called when a player accepts a quest from the creature.
        virtual bool OnQuestAccept(Player * p_Player, Creature * p_Creature, const Quest * p_Quest) override
        {
            if (p_Quest && p_Quest->GetQuestId() == QUEST_PALE_MOONLIGHT)
                p_Creature->AI()->Talk(NPC_VINDICATOR_MARAAD_PALE_MOONLIGHT_START_CHAT);

            return false;
        }
        /// Called when a player completes a quest with the creature.
        virtual bool OnQuestComplete(Player * p_Player, Creature * p_Creature, const Quest * p_Quest) override
        {
            if (p_Quest && p_Quest->GetQuestId() == QUEST_PALE_MOONLIGHT)
                p_Creature->AI()->Talk(NPC_VINDICATOR_MARAAD_PALE_MOONLIGHT_END_CHAT);

            return false;
        }

};

/// Lunarfall Laborer
class npc_LunarfallLaborer : public CreatureScript
{
    public:
        /// Constructor
        npc_LunarfallLaborer()
            : CreatureScript("npc_LunarfallLaborer")
        {

        }

        CreatureAI* GetAI(Creature * p_Creature) const
        {
            return new npc_LunarfallLaborerAI(p_Creature);
        }

        struct npc_LunarfallLaborerAI : public CreatureAI
        {
            /// Constructor
            npc_LunarfallLaborerAI(Creature * p_Creature)
                : CreatureAI(p_Creature)
            {

            }

            void UpdateAI(const uint32 p_Diff) override
            {

            }

            void SetData(uint32 p_ID, uint32 p_Value) override
            {
                if (p_ID == GARRISON_CREATURE_AI_DATA_BUILDER)
                {
                    me->LoadEquipment(1, true);
                    me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_WORK);
                }
            }
        };
};

/// Frostwall Peon
class npc_FrostwallPeon : public CreatureScript
{
    public:
        /// Constructor
        npc_FrostwallPeon()
            : CreatureScript("npc_FrostwallPeon")
        {

        }

        CreatureAI* GetAI(Creature * p_Creature) const
        {
            return new npc_FrostwallPeonAI(p_Creature);
        }

        struct npc_FrostwallPeonAI : public CreatureAI
        {
            /// Constructor
            npc_FrostwallPeonAI(Creature * p_Creature)
                : CreatureAI(p_Creature)
            {

            }

            void UpdateAI(const uint32 p_Diff) override
            {

            }

            void SetData(uint32 p_ID, uint32 p_Value) override
            {
                if (p_ID == GARRISON_CREATURE_AI_DATA_BUILDER)
                {
                    me->LoadEquipment(1, true);
                    me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_WORK);
                }
            }
        };
};

void AddSC_Garrison_NPC()
{
    new npc_GarrisonFord;
    new npc_GarrisonCartRope;
    new npc_AssistantBrightstone;
    new npc_ShellyHamby;
    new npc_BarosAlexsom;
    new npc_VindicatorMaraad;
    new npc_LunarfallLaborer;
    new npc_FrostwallPeon;
}