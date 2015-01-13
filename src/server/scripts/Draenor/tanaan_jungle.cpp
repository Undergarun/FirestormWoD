/*
* Copyright (C) 2014-20xx AhsranCore <http://www.ashran.com/>
* Copyright (C) 2012-2013 JadeCore <http://www.pandashan.com/>
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "tanaan_jungle.h"
#include "GameObjectAI.h"
#include "NPCHandler.h"

// Archmage Khadgar - 78558 (Main quest giver/taker)
class npc_archmage_khadgar : public CreatureScript
{
    public:
        npc_archmage_khadgar() : CreatureScript("npc_archmage_khadgar")
        {
        }

        bool OnQuestAccept(Player* p_Player, Creature* p_Creature, Quest const* p_Quest)
        {
            if (p_Quest->GetQuestId() == 35933)
            {
                Position l_Pos;
                p_Player->GetPosition(&l_Pos);

                if (Creature* l_Creature = p_Creature->SummonCreature(NPC_IRON_GRUNT, l_Pos))
                {
                    l_Creature->SetReactState(REACT_AGGRESSIVE);
                    l_Creature->GetAI()->AttackStart(p_Player);
                }
            }
            return true;
        }

        bool OnQuestReward(Player* p_Player, Creature* p_Creature, Quest* const p_Quest)
        {
            if (p_Quest->GetQuestId() == 34393)
            {
                if (GameObject* l_Gob = GetClosestGameObjectWithEntry(p_Creature, 234622, 200.0f))
                    l_Gob->DestroyForPlayer(p_Player); // remains there, wtf ? to check soon
            }
            else if (p_Quest->GetQuestId() == 34420)
            {
                if (Quest const* l_Quest = sObjectMgr->GetQuestTemplate(30040))
                    p_Player->AddQuest(l_Quest, p_Creature);
            }

            return true;
        }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            if (p_Player->GetQuestStatus(QUEST_THE_DARK_PORTAL) == QUEST_STATUS_INCOMPLETE)
                p_Player->CompleteQuest(QUEST_THE_DARK_PORTAL);
            else if (p_Player->GetQuestStatus(QUEST_THE_DARK_PORTAL) == QUEST_STATUS_COMPLETE)
            {
                const Quest* l_Quest = sObjectMgr->GetQuestTemplate(QUEST_THE_DARK_PORTAL);
                p_Player->RewardQuest(l_Quest, 0, p_Creature);
            }

            if (p_Player->GetQuestStatus(36881) == QUEST_STATUS_COMPLETE)
            {
                const Quest* l_Quest = sObjectMgr->GetQuestTemplate(36881);
                p_Player->RewardQuest(l_Quest, 0, p_Creature);
            }

            p_Player->PrepareQuestMenu(p_Creature->GetGUID());
            p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());

            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_archmage_khadgarAI(creature);
        }

        struct npc_archmage_khadgarAI : public ScriptedAI
        {
            npc_archmage_khadgarAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                std::list<Creature*> l_CreatureList;
                me->GetCreatureListWithEntryInGrid(l_CreatureList, 78507, 20.0f);

                for (Creature* l_Grunt : l_CreatureList)
                    me->AddAura(29266, l_Grunt);
            }

            void JustDied(Unit* p_Killer)
            {
                std::list<Unit*> l_Members;
                p_Killer->GetPartyMembers(l_Members);

                for (Unit* l_Unit : l_Members)
                {
                    if (l_Unit == nullptr || l_Unit->GetTypeId() != TYPEID_PLAYER)
                        continue;

                    if (l_Unit->ToPlayer()->GetQuestStatus(33334) == QUEST_STATUS_INCOMPLETE)
                        l_Unit->ToPlayer()->KilledMonsterCredit(73983);
                }

                if (Creature* l_FishgorgedCrane = GetClosestCreatureWithEntry(me, 73297, 24.0f))
                    l_FishgorgedCrane->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_29);
            }
        };
};

// 82188 - 81990 - 82007 - 82010 - 81994 - 82011 - 81997 - 82082 - 82191 - 82012 - 82014 - 82002 - 81996 - 81998 - 79062 - 81993 - 81995 - 82000
// 82001 - 82003 - 82004 - 82005 - 82008 - 82025 - 82016 - 82260 - 82264 - 82014 - 82009 - 82075 - 82017 - 82189 - 82263
class npc_generic_tanaan_guardian : public CreatureScript
{
    public:
        npc_generic_tanaan_guardian() : CreatureScript("npc_generic_tanaan_guardian") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_generic_tanaan_guardianAI(creature);
        }

        struct npc_generic_tanaan_guardianAI : public ScriptedAI
        {
            npc_generic_tanaan_guardianAI(Creature* creature) : ScriptedAI(creature) {}

            bool m_EnableCheck;

            void Reset()
            {
                m_EnableCheck = true;
                me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                m_EnableCheck = false;

                if (Creature* l_Grunt = p_Attacker->ToCreature())
                {
                    if (l_Grunt->GetEntry() == NPC_IRON_GRUNT)
                        me->SetAttackTime(WeaponAttackType::BaseAttack, 1000);
                }

                me->SetAttackTime(WeaponAttackType::BaseAttack, 2000);
            }

            void DamageTaken(Unit* p_DoneBy, uint32& p_Damage)
            {
                if (p_DoneBy->ToCreature())
                {
                    if (me->GetHealth() <= p_Damage || me->GetHealthPct() <= 80.0f)
                        me->SetFullHealth();
                }
            }

            void KilledUnit(Unit* p_Victim)
            {
                if (p_Victim->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (p_Victim->ToCreature()->GetEntry() == NPC_IRON_GRUNT)
                    m_EnableCheck = true;
            }

            void EnterEvadeMode()
            {
                m_EnableCheck = true;
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (m_EnableCheck)
                {
                    std::list<Creature*> l_EnemyList;
                    GetCreatureListWithEntryInGrid(l_EnemyList, me, NPC_IRON_GRUNT, 3.0f);

                    for (Creature* l_Grunt : l_EnemyList)
                    {
                        if (!l_Grunt->isMoving())
                        {
                            if (me->GetDistance(l_Grunt) < 3.0f)
                            {
                                AttackStart(l_Grunt);
                                break;
                            }
                        }
                    }
                }

                if (me->HasUnitState(UNIT_STATE_CASTING) || !UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };
};

// 78883 - Iron Grunt (ennemy)
class npc_iron_grunt : public CreatureScript
{
    public:
        npc_iron_grunt() : CreatureScript("npc_iron_grunt") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_gruntAI(creature);
        }

        struct npc_iron_gruntAI : public ScriptedAI
        {
            npc_iron_gruntAI(Creature* creature) : ScriptedAI(creature) {}

            Position m_Pos;
            bool m_HasReset;

            enum eCreatureIds
            {
                MORICCALAS = 1,
                MUMPER     = 2
            };

            void Reset()
            {
                m_HasReset = false;

                if (m_HasReset)
                    return;

                me->SetReactState(REACT_AGGRESSIVE);

                m_Pos = me->GetHomePosition();

                if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE))
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                if (CheckPosition(MORICCALAS))
                {
                    if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE))
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                    me->GetMotionMaster()->MoveCharge(4094.678467f, -2318.3186f, 64.6841f, 10.0f, 1);
                    me->SetReactState(REACT_PASSIVE);
                    me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                }
                else if (CheckPosition(MUMPER))
                {
                    if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE))
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                    me->GetMotionMaster()->MoveCharge(4054.158203f, -2319.444092f, 64.671272f, 10.0f, 1);
                    me->SetReactState(REACT_PASSIVE);
                    me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                }

                m_HasReset = true;
            }

            bool CheckPosition(uint8 p_Id)
            {
                switch (p_Id)
                {
                    case MORICCALAS:
                    {
                        if ((m_Pos.m_positionX <= 4094.0f && m_Pos.m_positionX >= 4093.0f) &&
                            (m_Pos.m_positionY >= -2324.0f && m_Pos.m_positionY <= -2323.0f))
                            return true;
                        else
                            break;
                    }
                    case MUMPER:
                    {
                        if ((m_Pos.m_positionX <= 4043.0f && m_Pos.m_positionX >= 4042.0f) &&
                            (m_Pos.m_positionY >= -2320.0f && m_Pos.m_positionY <= -2319.0f))
                            return true;
                        else
                            break;
                    }
                    default:
                        break;
                }

                return false;
            }

            void EnterEvadeMode()
            {
                m_HasReset = true;
            }

            void MovementInform(uint32 p_Type, uint32 p_Id)
            {
                if (p_Type != POINT_MOTION_TYPE)
                    return;

                if (CheckPosition(MORICCALAS))
                {
                    switch (p_Id)
                    {
                        case 1:
                            me->GetMotionMaster()->MoveCharge(4078.83f, -2318.550293f, 64.69f, 10.0f, 2);
                            break;
                        case 2:
                            me->GetMotionMaster()->MoveCharge(4078.955078f, -2345.177734f, 77.115952f, 10.0f, 3);
                            break;
                        case 3:
                        {
                            if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC))
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);

//                             me->getThreatManager().resetAllAggro();
//                             me->getThreatManager().clearReferences();
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->GetMotionMaster()->Clear();

                            if (Creature* l_Moriccalas = GetClosestCreatureWithEntry(me, NPC_MORICCALAS, 3.0f))
                                AttackStart(l_Moriccalas);

                            break;
                        }
                        default:
                            break;
                    }
                }
                else if (CheckPosition(MUMPER))
                {
                    switch (p_Id)
                    {
                        case 1:
                            me->GetMotionMaster()->MoveCharge(4053.91845f, -2358.014404f, 85.542175f, 10.0f, 2);
                            break;
                        case 2:
                        {
                            if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC))
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);

//                             me->getThreatManager().resetAllAggro();
//                             me->getThreatManager().clearReferences();
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->GetMotionMaster()->Clear();

                            if (Creature* l_Mumper = GetClosestCreatureWithEntry(me, NPC_MUMPER, 3.0f))
                                AttackStart(l_Mumper);

                            break;
                        }
                        default:
                            break;
                    }
                }
            }
        };
};

// 82484 - Iron Gronnling (ennemy)
class npc_iron_gronnling : public CreatureScript
{
    public:
        npc_iron_gronnling() : CreatureScript("npc_iron_gronnling") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_gronnlingAI(creature);
        }

        struct npc_iron_gronnlingAI : public ScriptedAI
        {
            npc_iron_gronnlingAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE || !me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE)))
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }

                if (me->HasAura(166131))
                    me->RemoveAura(166131);
            }

            void MoveInLineOfSight(Unit* p_Who)
            {
            }
        };
};

// 81711 - ShadowMoon Ritualist (ennemy)
class npc_shadowmoon_ritualist : public CreatureScript
{
    public:
        npc_shadowmoon_ritualist() : CreatureScript("npc_shadowmoon_ritualist") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_shadowmoon_ritualistAI(creature);
        }

        struct npc_shadowmoon_ritualistAI : public ScriptedAI
        {
            npc_shadowmoon_ritualistAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE || !me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE)))
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }
            }

            void MoveInLineOfSight(Unit* p_Who)
            {
            }
        };
};

// 81711 - ShadowMoon Ritualist (ennemy)
class npc_gul_dan_trigger : public CreatureScript
{
    public:
        npc_gul_dan_trigger() : CreatureScript("npc_gul_dan_trigger") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_gul_dan_triggerAI(creature);
        }

        struct npc_gul_dan_triggerAI : public ScriptedAI
        {
            npc_gul_dan_triggerAI(Creature* creature) : ScriptedAI(creature) { }

            void UpdateAI(uint32 const diff)
            {
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 5.0f);

                for (Player* l_Player : l_PlayerList)
                {
                    if (l_Player->isInFront(me))
                    {
                        if (l_Player->GetQuestStatus(QUEST_THE_PORTAL_POWER) == QUEST_STATUS_INCOMPLETE && l_Player->GetQuestObjectiveCounter(273930) < 1)
                            l_Player->KilledMonsterCredit(CREDIT_ENTER_GULDAN_PRISON);
                    }
                }
            }
        };
};

// 81711 - ShadowMoon Ritualist (ennemy)
class npc_tormented_soul : public CreatureScript
{
    public:
        npc_tormented_soul() : CreatureScript("npc_tormented_soul") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tormented_soulAI(creature);
        }

        struct npc_tormented_soulAI : public ScriptedAI
        {
            npc_tormented_soulAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                Creature* l_GulDan = GetClosestCreatureWithEntry(me, 78333, 30.0f);

                if (!l_GulDan)
                    return;

                me->setFaction(14);
            }

            void MoveInLineOfSight(Unit* p_Target)
            {
                if (p_Target->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (p_Target->ToPlayer()->GetQuestStatus(QUEST_THE_PORTAL_POWER) == QUEST_STATUS_INCOMPLETE && me->GetDistance(p_Target) <= 3.0f)
                    AttackStart(p_Target);
            }
        };
};

// Ariok - 78556
class npc_tanaan_ariok : public CreatureScript
{
    public:
        npc_tanaan_ariok() : CreatureScript("npc_tanaan_ariok") { }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            if (p_Player->GetQuestStatus(QUEST_ALTAR_ALTERCATION) == QUEST_STATUS_INCOMPLETE || p_Player->GetQuestStatus(QUEST_ALTAR_ALTERCATION) == QUEST_STATUS_FAILED)
            {
                if (p_Player->GetQuestObjectiveCounter(273075) < 1)
                    p_Player->KilledMonsterCredit(78556);

                Position l_Pos;
                p_Creature->GetPosition(&l_Pos);
                p_Creature->SummonCreature(p_Creature->GetEntry(), l_Pos, TEMPSUMMON_MANUAL_DESPAWN);

                if (p_Creature->GetAI())
                    p_Creature->AI()->SetGUID(p_Player->GetGUID());
            }

            return true;
        }

        struct npc_tanaan_ariokAI : public ScriptedAI
        {
            npc_tanaan_ariokAI(Creature* creature) : ScriptedAI(creature) { }

            bool m_IsSummoned;
            uint64 m_PlayerGuid;

            void Reset()
            {
                m_IsSummoned = false;
                m_PlayerGuid = 0;
            }

            void SetGUID(uint64 p_Guid, int32 /*p_Index*/)
            {
                m_PlayerGuid = p_Guid;
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                m_IsSummoned = true;

                if (me->HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP))
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                if (m_PlayerGuid)
                {
                    if (Player* p_Followed = me->GetPlayer(*me, m_PlayerGuid))
                        me->GetMotionMaster()->MoveFollow(p_Followed, 2.0f, 2.0f);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tanaan_ariokAI(creature);
        }
};

// 237670/237667 - Dark Portal
class go_platform_tanaan : public GameObjectScript
{
    public:
        go_platform_tanaan() : GameObjectScript("go_platform_tanaan") { }

        struct go_platform_tanaanAI : public GameObjectAI
        {
            go_platform_tanaanAI(GameObject* p_Go) : GameObjectAI(p_Go) { }

            void Reset()
            {
                go->SetGoState(GO_STATE_ACTIVE);
                go->SetLootState(GO_ACTIVATED);
            }

        };

        GameObjectAI* GetAI(GameObject* p_Go) const
        {
            return new go_platform_tanaanAI(p_Go);
        }
};

class gob_static_rune : public GameObjectScript
{
    public:
        gob_static_rune() : GameObjectScript("gob_static_rune") { }

        struct gob_static_runeAI : public GameObjectAI
        {
            gob_static_runeAI(GameObject* p_Go) : GameObjectAI(p_Go) { }

            bool GossipHello(Player* p_Player)
            {
                if (p_Player->GetQuestStatus(QUEST_THE_PORTAL_POWER) == QUEST_STATUS_INCOMPLETE && p_Player->GetQuestObjectiveCounter(273936) < 1)
                {
                    if (p_Player->GetQuestObjectiveCounter(273438) == 0 || p_Player->GetQuestObjectiveCounter(273556) == 0 || p_Player->GetQuestObjectiveCounter(273557) == 0)
                        return false;

                    p_Player->KilledMonsterCredit(CREDIT_STATIS_RUNE_DESTROYED);
                    p_Player->PlayScene(808, p_Player);
                    p_Player->SetPhaseMask(2, true);
                }
                return true;
            }

        };

        GameObjectAI* GetAI(GameObject* p_Go) const
        {
            return new gob_static_runeAI(p_Go);
        }
};

class go_bleeding_hollow_cage : public GameObjectScript
{
    public:
        go_bleeding_hollow_cage() : GameObjectScript("go_bleeding_hollow_cage") { }

        struct go_bleeding_hollow_cageAI : public GameObjectAI
        {
            go_bleeding_hollow_cageAI(GameObject* p_Go) : GameObjectAI(p_Go) { }

            bool GossipHello(Player* p_Player)
            {
                if (p_Player->GetQuestStatus(QUEST_BLED_DRY) == QUEST_STATUS_INCOMPLETE)
                {
                    if (go->GetGOData()->id == 229350)
                        p_Player->KilledMonsterCredit(82871);
                    else if (go->GetGOData()->id == 229325)
                        p_Player->KilledMonsterCredit(85142);
                }
                return true;
            }

        };

        GameObjectAI* GetAI(GameObject* p_Go) const
        {
            return new go_bleeding_hollow_cageAI(p_Go);
        }
};

class map_dark_portal_entrance : public WorldMapScript
{
    public:
        map_dark_portal_entrance() : WorldMapScript("map_dark_portal_entrance", 1265) { }

        void OnPlayerEnter(Map* p_Map, Player* /*p_Player*/) { p_Map->SetObjectVisibility(350.0f); }
};

// Tanaan entering
class playerScript_enter_tanaan : public PlayerScript
{
    public:
        playerScript_enter_tanaan() : PlayerScript("playerScript_enter_tanaan") { }

        void OnUpdateZone(Player* p_Player, uint32 p_NewZoneId, uint32 /*p_OldZoneID*/, uint32 p_NewAreaId)
        {
            if (p_NewZoneId == ZONE_TANAAN_JUNGLE && p_NewAreaId == AREA_THE_DARK_PORTAL)
            {
                if (p_Player->GetQuestStatus(QUEST_THE_DARK_PORTAL) == QUEST_STATUS_NONE)
                {
                    Quest const* l_Quest = sObjectMgr->GetQuestTemplate(QUEST_THE_DARK_PORTAL);

                    if (l_Quest)
                        p_Player->AddQuest(l_Quest, nullptr);

                    p_Player->SetPhaseMask(1, true);
                }
                else if (p_Player->GetQuestStatus(QUEST_START_DRAENOR) == QUEST_STATUS_INCOMPLETE)
                {
                    Quest const* l_Quest = sObjectMgr->GetQuestTemplate(QUEST_START_DRAENOR);

                    if (l_Quest)
                    {
                        p_Player->CompleteQuest(QUEST_START_DRAENOR);
                        p_Player->RewardQuest(l_Quest, 0, nullptr, false);
                    }
                }
            }
        }

        void OnQuestReward(Player* p_Player, const Quest* p_Quest)
        {
            switch (p_Quest->GetQuestId())
            {
                case QUEST_THE_DARK_PORTAL:
                {
                    if (const Quest* l_Quest = sObjectMgr->GetQuestTemplate(QUEST_ONSLAUGHT_S_END))
                        p_Player->AddQuest(l_Quest, nullptr);

                    break;
                }
                default:
                    break;
            }
        }

        void OnObjectiveValidate(Player* p_Player, uint32 p_QuestId, uint32 p_ObjectiveId)
        {
            if (p_QuestId == QUEST_THE_PORTAL_POWER && p_ObjectiveId == 273930)
                p_Player->PlayScene(925, p_Player);
        }
};

// Inferno - 165988
class spell_tanaan_inferno : public SpellScriptLoader
{
    public:
        spell_tanaan_inferno() : SpellScriptLoader("spell_tanaan_inferno") { }

        class spell_tanaan_inferno_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_tanaan_inferno_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster || l_Caster->GetTypeId() != TYPEID_PLAYER)
                    return;

                std::list<Creature*> l_CreatureList;
                l_Caster->GetCreatureListWithEntryInGrid(l_CreatureList, 300004, 20.0f);
                Quest const* l_Quest = sObjectMgr->GetQuestTemplate(QUEST_BLAZE_OF_GLORY);
                const QuestObjective* l_Objective = l_Quest->GetQuestObjective(l_Quest->GetQuestObjectiveId(QUEST_BLAZE_OF_GLORY, 0));

                if (!l_CreatureList.empty())
                    l_Caster->ToPlayer()->QuestObjectiveSatisfy(l_Objective->ObjectID, 1);
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(spell_tanaan_inferno_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_tanaan_inferno_SpellScript();
        }
};

void AddSC_tanaan_jungle()
{
    new npc_archmage_khadgar();
    new npc_generic_tanaan_guardian();
    new npc_iron_grunt();
    new npc_iron_gronnling();
    new npc_shadowmoon_ritualist();
    new npc_gul_dan_trigger();
    new npc_tormented_soul();
    new npc_tanaan_ariok();
    new gob_static_rune();
    new go_platform_tanaan();
    new go_bleeding_hollow_cage();
    new map_dark_portal_entrance();
    new playerScript_enter_tanaan();
    new spell_tanaan_inferno();
}