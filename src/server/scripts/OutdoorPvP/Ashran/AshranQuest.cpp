////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AshranMgr.hpp"

/// Commander Anne Dunworthy - 84173
class npc_ashran_commander_anne_dunworthy : public CreatureScript
{
    public:
        npc_ashran_commander_anne_dunworthy() : CreatureScript("npc_ashran_commander_anne_dunworthy") { }

        enum eQuest
        {
            WelcomeToAshran = 36119
        };

        bool OnGossipHello(Player* p_Player, Creature* /*p_Creature*/) override
        {
            if (p_Player->GetQuestStatus(eQuest::WelcomeToAshran) == QuestStatus::QUEST_STATUS_INCOMPLETE)
                p_Player->CompleteQuest(eQuest::WelcomeToAshran);

            return false;
        }
};

/// General Ushet Wolfbarger - 84473
class npc_ashran_general_ushet_wolfbarger : public CreatureScript
{
    public:
        npc_ashran_general_ushet_wolfbarger() : CreatureScript("npc_ashran_general_ushet_wolfbarger") { }

        enum eDatas
        {
            WelcomeToAshran = 36196
        };

        bool OnGossipHello(Player* p_Player, Creature* /*p_Creature*/) override
        {
            if (p_Player->GetQuestStatus(eDatas::WelcomeToAshran) == QuestStatus::QUEST_STATUS_INCOMPLETE)
                p_Player->CompleteQuest(eDatas::WelcomeToAshran);

            return false;
        }
};

/// Farseer Kylanda <Stormshield Shaman Leader> - 82901
class npc_ashran_farseer_kylanda : public CreatureScript
{
    public:
        npc_ashran_farseer_kylanda() : CreatureScript("npc_ashran_farseer_kylanda") { }

        enum eDatas
        {
            ReportingForDuty    = 36130,
            FarseerKylanda      = 84256
        };

        bool OnGossipHello(Player* p_Player, Creature* /*p_Creature*/) override
        {
            if (p_Player->GetQuestStatus(eDatas::ReportingForDuty) == QuestStatus::QUEST_STATUS_INCOMPLETE)
                p_Player->KilledMonsterCredit(eDatas::FarseerKylanda);

            return false;
        }
};

/// Chris Clarkie <SI:7 Operative> - 82909
class npc_ashran_chris_clarkie : public CreatureScript
{
    public:
        npc_ashran_chris_clarkie() : CreatureScript("npc_ashran_chris_clarkie") { }

        enum eDatas
        {
            ReportingForDuty    = 36130,
            ChrisClarkie        = 84257
        };

        bool OnGossipHello(Player* p_Player, Creature* /*p_Creature*/) override
        {
            if (p_Player->GetQuestStatus(eDatas::ReportingForDuty) == QuestStatus::QUEST_STATUS_INCOMPLETE)
                p_Player->KilledMonsterCredit(eDatas::ChrisClarkie);

            return false;
        }
};

/// Harrison Jones - 84223
class npc_ashran_harrison_jones : public CreatureScript
{
    public:
        npc_ashran_harrison_jones() : CreatureScript("npc_ashran_harrison_jones") { }

        enum eDatas
        {
            ReportingForDuty    = 36130,
            HarrisonJones       = 84226
        };

        bool OnGossipHello(Player* p_Player, Creature* /*p_Creature*/) override
        {
            if (p_Player->GetQuestStatus(eDatas::ReportingForDuty) == QuestStatus::QUEST_STATUS_INCOMPLETE)
                p_Player->KilledMonsterCredit(eDatas::HarrisonJones);

            return false;
        }

        struct npc_ashran_harrison_jonesAI : public ScriptedAI
        {
            npc_ashran_harrison_jonesAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void sGossipSelect(Player* p_Player, uint32 /*p_Sender*/, uint32 p_Action) override
            {
                /// "Take all of my Artifact Fragments" is always 0
                if (p_Action)
                    return;

                ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(me->GetZoneId());
                if (l_ZoneScript == nullptr)
                    return;

                uint32 l_ArtifactCount = p_Player->GetCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, true);
                p_Player->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, -int32(l_ArtifactCount * CURRENCY_PRECISION), false);

                uint32 l_Modulo = l_ArtifactCount % eArtifactsDatas::MaxArtifactCounts;
                uint32 l_Count = l_ArtifactCount / eArtifactsDatas::MaxArtifactCounts;
                uint8 l_Rand = urand(0, eArtifactsDatas::MaxArtifactCounts - 1);

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
                {
                    for (uint8 l_I = 0; l_I < eArtifactsDatas::MaxArtifactCounts; ++l_I)
                    {
                        if (l_I == l_Rand)
                            l_Ashran->AddCollectedArtifacts(TeamId::TEAM_ALLIANCE, l_I, l_Count + l_Modulo);
                        else
                            l_Ashran->AddCollectedArtifacts(TeamId::TEAM_ALLIANCE, l_I, l_Count);
                    }

                    l_Ashran->RewardHonorAndReputation(l_ArtifactCount, p_Player);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_harrison_jonesAI(p_Creature);
        }
};

/// Belloc Brightblade <The Reliquary> - 88448
class npc_ashran_belloc_brightblade : public CreatureScript
{
    public:
        npc_ashran_belloc_brightblade() : CreatureScript("npc_ashran_belloc_brightblade") { }

        enum eDatas
        {
            ReportingForDuty    = 36197,
            BellocBrightblade   = 84474
        };

        bool OnGossipHello(Player* p_Player, Creature* /*p_Creature*/) override
        {
            if (p_Player->GetQuestStatus(eDatas::ReportingForDuty) == QuestStatus::QUEST_STATUS_INCOMPLETE)
                p_Player->KilledMonsterCredit(eDatas::BellocBrightblade);

            return false;
        }

        struct npc_ashran_belloc_brightbladeAI : public ScriptedAI
        {
            npc_ashran_belloc_brightbladeAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void sGossipSelect(Player* p_Player, uint32 /*p_Sender*/, uint32 p_Action) override
            {
                /// "Take all of my Artifact Fragments" is always 0
                if (p_Action)
                    return;

                ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(me->GetZoneId());
                if (l_ZoneScript == nullptr)
                    return;

                uint32 l_ArtifactCount = p_Player->GetCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, true);
                p_Player->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, -int32(l_ArtifactCount * CURRENCY_PRECISION), false);

                uint32 l_Modulo = l_ArtifactCount % eArtifactsDatas::MaxArtifactCounts;
                uint32 l_Count = l_ArtifactCount / eArtifactsDatas::MaxArtifactCounts;
                uint8 l_Rand = urand(0, eArtifactsDatas::MaxArtifactCounts - 1);

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
                {
                    for (uint8 l_I = 0; l_I < eArtifactsDatas::MaxArtifactCounts; ++l_I)
                    {
                        if (l_I == l_Rand)
                            l_Ashran->AddCollectedArtifacts(TeamId::TEAM_HORDE, l_I, l_Count + l_Modulo);
                        else
                            l_Ashran->AddCollectedArtifacts(TeamId::TEAM_HORDE, l_I, l_Count);
                    }

                    l_Ashran->RewardHonorAndReputation(l_ArtifactCount, p_Player);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_belloc_brightbladeAI(p_Creature);
        }
};

/// Angry Zurge - 83869
class npc_ashran_angry_zurge : public CreatureScript
{
    public:
        npc_ashran_angry_zurge() : CreatureScript("npc_ashran_angry_zurge") { }

        enum eDatas
        {
            ReportingForDuty    = 36197,
            AngryZurge          = 84659
        };

        bool OnGossipHello(Player* p_Player, Creature* /*p_Creature*/) override
        {
            if (p_Player->GetQuestStatus(eDatas::ReportingForDuty) == QuestStatus::QUEST_STATUS_INCOMPLETE)
                p_Player->KilledMonsterCredit(eDatas::AngryZurge);

            return false;
        }
};

/// Mare Wildrunner <Warspear Farseer> - 84660
class npc_ashran_mare_wildrunner : public CreatureScript
{
    public:
        npc_ashran_mare_wildrunner() : CreatureScript("npc_ashran_mare_wildrunner") { }

        enum eDatas
        {
            ReportingForDuty    = 36197,
            MareWildrunner      = 84661
        };

        bool OnGossipHello(Player* p_Player, Creature* /*p_Creature*/) override
        {
            if (p_Player->GetQuestStatus(eDatas::ReportingForDuty) == QuestStatus::QUEST_STATUS_INCOMPLETE)
                p_Player->KilledMonsterCredit(eDatas::MareWildrunner);

            return false;
        }
};

#ifndef __clang_analyzer__
void AddSC_AshranQuest()
{
    new npc_ashran_commander_anne_dunworthy();
    new npc_ashran_general_ushet_wolfbarger();
    new npc_ashran_farseer_kylanda();
    new npc_ashran_chris_clarkie();
    new npc_ashran_harrison_jones();
    new npc_ashran_belloc_brightblade();
    new npc_ashran_angry_zurge();
    new npc_ashran_mare_wildrunner();
}
#endif
