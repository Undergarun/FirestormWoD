////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Npcs_Special
SD%Complete: 100
SDComment: To be used for special NPCs that are located globally.
SDCategory: NPCs
EndScriptData
*/

/* ContentData
npc_air_force_bots       80%    support for misc (invisible) guard bots in areas where player allowed to fly. Summon guards after a preset time if tagged by spell
npc_lunaclaw_spirit      80%    support for quests 6001/6002 (Body and Heart)
npc_chicken_cluck       100%    support for quest 3861 (Cluck!)
npc_dancing_flames      100%    midsummer event NPC
npc_guardian            100%    guardianAI used to prevent players from accessing off-limits areas. Not in use by SD2
npc_garments_of_quests   80%    NPC's related to all Garments of-quests 5621, 5624, 5625, 5648, 565
npc_injured_patient     100%    patients for triage-quests (6622 and 6624)
npc_doctor              100%    Gustaf Vanhowzen and Gregory Victor, quest 6622 and 6624 (Triage)
npc_mount_vendor        100%    Regular mount vendors all over the world. Display gossip if player doesn't meet the requirements to buy
npc_rogue_trainer        80%    Scripted trainers, so they are able to offer item 17126 for class quest 6681
npc_sayge               100%    Darkmoon event fortune teller, buff player based on answers given
npc_snake_trap_serpents  80%    AI for snakes that summoned by Snake Trap
npc_locksmith            75%    list of keys needs to be confirmed
npc_firework            100%    NPC's summoned by rockets and rocket clusters, for making them cast visual
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "World.h"
#include "PetAI.h"
#include "PassiveAI.h"
#include "CombatAI.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Cell.h"
#include "CellImpl.h"
#include "SpellAuras.h"
#include "Vehicle.h"
#include "Player.h"
#include "SpellScript.h"
#include "Chat.h"

/*########
# npc_air_force_bots
#########*/

enum SpawnType
{
    SPAWNTYPE_TRIPWIRE_ROOFTOP,                             // no warning, summon Creature at smaller range
    SPAWNTYPE_ALARMBOT,                                     // cast guards mark and summon npc - if player shows up with that buff duration < 5 seconds attack
};

struct SpawnAssociation
{
    uint32 thisCreatureEntry;
    uint32 spawnedCreatureEntry;
    SpawnType spawnType;
};

enum eEnums
{
    SPELL_GUARDS_MARK               = 38067,
    AURA_DURATION_TIME_LEFT         = 5000
};

float const RANGE_TRIPWIRE          = 15.0f;
float const RANGE_GUARDS_MARK       = 50.0f;

SpawnAssociation spawnAssociations[] =
{
    {2614,  15241, SPAWNTYPE_ALARMBOT},                     //Air Force Alarm Bot (Alliance)
    {2615,  15242, SPAWNTYPE_ALARMBOT},                     //Air Force Alarm Bot (Horde)
    {21974, 21976, SPAWNTYPE_ALARMBOT},                     //Air Force Alarm Bot (Area 52)
    {21993, 15242, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Horde - Bat Rider)
    {21996, 15241, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Alliance - Gryphon)
    {21997, 21976, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Goblin - Area 52 - Zeppelin)
    {21999, 15241, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Rooftop (Alliance)
    {22001, 15242, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Rooftop (Horde)
    {22002, 15242, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Ground (Horde)
    {22003, 15241, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Ground (Alliance)
    {22063, 21976, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Rooftop (Goblin - Area 52)
    {22065, 22064, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Ethereal - Stormspire)
    {22066, 22067, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Scryer - Dragonhawk)
    {22068, 22064, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Rooftop (Ethereal - Stormspire)
    {22069, 22064, SPAWNTYPE_ALARMBOT},                     //Air Force Alarm Bot (Stormspire)
    {22070, 22067, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Rooftop (Scryer)
    {22071, 22067, SPAWNTYPE_ALARMBOT},                     //Air Force Alarm Bot (Scryer)
    {22078, 22077, SPAWNTYPE_ALARMBOT},                     //Air Force Alarm Bot (Aldor)
    {22079, 22077, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Aldor - Gryphon)
    {22080, 22077, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Rooftop (Aldor)
    {22086, 22085, SPAWNTYPE_ALARMBOT},                     //Air Force Alarm Bot (Sporeggar)
    {22087, 22085, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Sporeggar - Spore Bat)
    {22088, 22085, SPAWNTYPE_TRIPWIRE_ROOFTOP},             //Air Force Trip Wire - Rooftop (Sporeggar)
    {22090, 22089, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Toshley's Station - Flying Machine)
    {22124, 22122, SPAWNTYPE_ALARMBOT},                     //Air Force Alarm Bot (Cenarion)
    {22125, 22122, SPAWNTYPE_ALARMBOT},                     //Air Force Guard Post (Cenarion - Stormcrow)
    {22126, 22122, SPAWNTYPE_ALARMBOT}                      //Air Force Trip Wire - Rooftop (Cenarion Expedition)
};

class npc_air_force_bots : public CreatureScript
{
    public:
        npc_air_force_bots() : CreatureScript("npc_air_force_bots") { }

        struct npc_air_force_botsAI : public ScriptedAI
        {
            npc_air_force_botsAI(Creature* creature) : ScriptedAI(creature)
            {
                SpawnAssoc = NULL;
                SpawnedGUID = 0;

                // find the correct spawnhandling
                static uint32 entryCount = sizeof(spawnAssociations) / sizeof(SpawnAssociation);

                for (uint8 i = 0; i < entryCount; ++i)
                {
                    if (spawnAssociations[i].thisCreatureEntry == creature->GetEntry())
                    {
                        SpawnAssoc = &spawnAssociations[i];
                        break;
                    }
                }

                if (!SpawnAssoc)
                    sLog->outError(LOG_FILTER_SQL, "TCSR: Creature template entry %u has ScriptName npc_air_force_bots, but it's not handled by that script", creature->GetEntry());
                else
                {
                    CreatureTemplate const* spawnedTemplate = sObjectMgr->GetCreatureTemplate(SpawnAssoc->spawnedCreatureEntry);

                    if (!spawnedTemplate)
                    {
                        sLog->outError(LOG_FILTER_SQL, "TCSR: Creature template entry %u does not exist in DB, which is required by npc_air_force_bots", SpawnAssoc->spawnedCreatureEntry);
                        SpawnAssoc = NULL;
                        return;
                    }
                }
            }

            SpawnAssociation* SpawnAssoc;
            uint64 SpawnedGUID;

            void Reset() {}

            Creature* SummonGuard()
            {
                Creature* summoned = me->SummonCreature(SpawnAssoc->spawnedCreatureEntry, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 300000);

                if (summoned)
                    SpawnedGUID = summoned->GetGUID();
                else
                {
                    sLog->outError(LOG_FILTER_SQL, "TCSR: npc_air_force_bots: wasn't able to spawn Creature %u", SpawnAssoc->spawnedCreatureEntry);
                    SpawnAssoc = NULL;
                }

                return summoned;
            }

            Creature* GetSummonedGuard()
            {
                Creature* creature = Unit::GetCreature(*me, SpawnedGUID);

                if (creature && creature->isAlive())
                    return creature;

                return NULL;
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (!SpawnAssoc)
                    return;

                if (me->IsValidAttackTarget(who))
                {
                    Player* playerTarget = who->ToPlayer();

                    // airforce guards only spawn for players
                    if (!playerTarget)
                        return;

                    if (!playerTarget->isAlive())
                        return;

                    Creature* lastSpawnedGuard = SpawnedGUID == 0 ? NULL : GetSummonedGuard();

                    // prevent calling Unit::GetUnit at next MoveInLineOfSight call - speedup
                    if (!lastSpawnedGuard)
                        SpawnedGUID = 0;

                    switch (SpawnAssoc->spawnType)
                    {
                        case SPAWNTYPE_ALARMBOT:
                        {
                            if (!who->IsWithinDistInMap(me, RANGE_GUARDS_MARK))
                                return;

                            Aura* markAura = who->GetAura(SPELL_GUARDS_MARK);
                            if (markAura)
                            {
                                // the target wasn't able to move out of our range within 25 seconds
                                if (!lastSpawnedGuard)
                                {
                                    lastSpawnedGuard = SummonGuard();

                                    if (!lastSpawnedGuard)
                                        return;
                                }

                                if (markAura->GetDuration() < AURA_DURATION_TIME_LEFT)
                                    if (!lastSpawnedGuard->getVictim())
                                        lastSpawnedGuard->AI()->AttackStart(who);
                            }
                            else
                            {
                                if (!lastSpawnedGuard)
                                    lastSpawnedGuard = SummonGuard();

                                if (!lastSpawnedGuard)
                                    return;

                                lastSpawnedGuard->CastSpell(who, SPELL_GUARDS_MARK, true);
                            }
                            break;
                        }
                        case SPAWNTYPE_TRIPWIRE_ROOFTOP:
                        {
                            if (!who->IsWithinDistInMap(me, RANGE_TRIPWIRE))
                                return;

                            if (!lastSpawnedGuard)
                                lastSpawnedGuard = SummonGuard();

                            if (!lastSpawnedGuard)
                                return;

                            // ROOFTOP only triggers if the player is on the ground
                            if (!playerTarget->IsFlying() && !lastSpawnedGuard->getVictim())
                                lastSpawnedGuard->AI()->AttackStart(who);

                            break;
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_air_force_botsAI(creature);
        }
};

/*######
## npc_lunaclaw_spirit
######*/

enum
{
    QUEST_BODY_HEART_A      = 6001,
    QUEST_BODY_HEART_H      = 6002,

    TEXT_ID_DEFAULT         = 4714,
    TEXT_ID_PROGRESS        = 4715
};

#define GOSSIP_ITEM_GRANT   "You have thought well, spirit. I ask you to grant me the strength of your body and the strength of your heart."

class npc_lunaclaw_spirit : public CreatureScript
{
    public:
        npc_lunaclaw_spirit() : CreatureScript("npc_lunaclaw_spirit") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (player->GetQuestStatus(QUEST_BODY_HEART_A) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_BODY_HEART_H) == QUEST_STATUS_INCOMPLETE)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_GRANT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            player->SEND_GOSSIP_MENU(TEXT_ID_DEFAULT, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->SEND_GOSSIP_MENU(TEXT_ID_PROGRESS, creature->GetGUID());
                player->AreaExploredOrEventHappens(player->GetTeam() == ALLIANCE ? QUEST_BODY_HEART_A : QUEST_BODY_HEART_H);
            }
            return true;
        }
};

/*########
# npc_chicken_cluck
#########*/

#define EMOTE_HELLO         -1070004
#define EMOTE_CLUCK_TEXT    -1070006

#define QUEST_CLUCK         3861
#define FACTION_FRIENDLY    35
#define FACTION_CHICKEN     31

class npc_chicken_cluck : public CreatureScript
{
    public:
        npc_chicken_cluck() : CreatureScript("npc_chicken_cluck") { }

        struct npc_chicken_cluckAI : public ScriptedAI
        {
            npc_chicken_cluckAI(Creature* creature) : ScriptedAI(creature) {}

            uint32 ResetFlagTimer;

            void Reset()
            {
                ResetFlagTimer = 120000;
                me->setFaction(FACTION_CHICKEN);
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            }

            void EnterCombat(Unit* /*who*/) {}

            void UpdateAI(uint32 const diff)
            {
                // Reset flags after a certain time has passed so that the next player has to start the 'event' again
                if (me->HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
                {
                    if (ResetFlagTimer <= diff)
                    {
                        EnterEvadeMode();
                        return;
                    }
                    else
                        ResetFlagTimer -= diff;
                }

                if (UpdateVictim())
                    DoMeleeAttackIfReady();
            }

            void ReceiveEmote(Player* player, uint32 emote)
            {
                switch (emote)
                {
                    case TEXT_EMOTE_CHICKEN:
                        if (player->GetQuestStatus(QUEST_CLUCK) == QUEST_STATUS_NONE && rand() % 30 == 1)
                        {
                            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                            me->setFaction(FACTION_FRIENDLY);
                            DoScriptText(EMOTE_HELLO, me);
                        }
                        break;
                    case TEXT_EMOTE_CHEER:
                        if (player->GetQuestStatus(QUEST_CLUCK) == QUEST_STATUS_COMPLETE)
                        {
                            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                            me->setFaction(FACTION_FRIENDLY);
                            DoScriptText(EMOTE_CLUCK_TEXT, me);
                        }
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_chicken_cluckAI(creature);
        }

        bool OnQuestAccept(Player* /*player*/, Creature* creature, Quest const* quest)
        {
            if (quest->GetQuestId() == QUEST_CLUCK)
                CAST_AI(npc_chicken_cluck::npc_chicken_cluckAI, creature->AI())->Reset();

            return true;
        }

        bool OnQuestComplete(Player* /*player*/, Creature* creature, Quest const* quest)
        {
            if (quest->GetQuestId() == QUEST_CLUCK)
                CAST_AI(npc_chicken_cluck::npc_chicken_cluckAI, creature->AI())->Reset();

            return true;
        }
};

/*######
## npc_dancing_flames
######*/

#define SPELL_BRAZIER       45423
#define SPELL_SEDUCTION     47057
#define SPELL_FIERY_AURA    45427

class npc_dancing_flames : public CreatureScript
{
    public:
        npc_dancing_flames() : CreatureScript("npc_dancing_flames") { }

        struct npc_dancing_flamesAI : public ScriptedAI
        {
            npc_dancing_flamesAI(Creature* creature) : ScriptedAI(creature) {}

            bool Active;
            uint32 CanIteract;

            void Reset()
            {
                Active = true;
                CanIteract = 3500;
                DoCast(me, SPELL_BRAZIER, true);
                DoCast(me, SPELL_FIERY_AURA, false);
                float x, y, z;
                me->GetPosition(x, y, z);
                me->Relocate(x, y, z + 0.94f);
                me->SetDisableGravity(true);
                me->HandleEmoteCommand(EMOTE_ONESHOT_DANCE);
                WorldPacket data;                       //send update position to client
                me->BuildHeartBeatMsg(&data);
                me->SendMessageToSet(&data, true);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!Active)
                {
                    if (CanIteract <= diff)
                    {
                        Active = true;
                        CanIteract = 3500;
                        me->HandleEmoteCommand(EMOTE_ONESHOT_DANCE);
                    }
                    else
                        CanIteract -= diff;
                }
            }

            void EnterCombat(Unit* /*who*/){}

            void ReceiveEmote(Player* player, uint32 emote)
            {
                if (me->IsWithinLOS(player->GetPositionX(), player->GetPositionY(), player->GetPositionZ()) && me->IsWithinDistInMap(player, 30.0f))
                {
                    me->SetInFront(player);
                    Active = false;

                    WorldPacket data;
                    me->BuildHeartBeatMsg(&data);
                    me->SendMessageToSet(&data, true);
                    switch (emote)
                    {
                        case TEXT_EMOTE_KISS:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_SHY);
                            break;
                        case TEXT_EMOTE_WAVE:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_WAVE);
                            break;
                        case TEXT_EMOTE_BOW:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_BOW);
                            break;
                        case TEXT_EMOTE_JOKE:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
                            break;
                        case TEXT_EMOTE_DANCE:
                            if (!player->HasAura(SPELL_SEDUCTION))
                                DoCast(player, SPELL_SEDUCTION, true);
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_dancing_flamesAI(creature);
        }
};

/*######
## Triage quest
######*/

//signed for 9623
#define SAY_DOC1    -1000201
#define SAY_DOC2    -1000202
#define SAY_DOC3    -1000203

#define DOCTOR_ALLIANCE     12939
#define DOCTOR_HORDE        12920
#define ALLIANCE_COORDS     7
#define HORDE_COORDS        6

struct Location
{
    float x, y, z, o;
};

static Location AllianceCoords[]=
{
    {-3757.38f, -4533.05f, 14.16f, 3.62f},                      // Top-far-right bunk as seen from entrance
    {-3754.36f, -4539.13f, 14.16f, 5.13f},                      // Top-far-left bunk
    {-3749.54f, -4540.25f, 14.28f, 3.34f},                      // Far-right bunk
    {-3742.10f, -4536.85f, 14.28f, 3.64f},                      // Right bunk near entrance
    {-3755.89f, -4529.07f, 14.05f, 0.57f},                      // Far-left bunk
    {-3749.51f, -4527.08f, 14.07f, 5.26f},                      // Mid-left bunk
    {-3746.37f, -4525.35f, 14.16f, 5.22f},                      // Left bunk near entrance
};

//alliance run to where
#define A_RUNTOX -3742.96f
#define A_RUNTOY -4531.52f
#define A_RUNTOZ 11.91f

static Location HordeCoords[]=
{
    {-1013.75f, -3492.59f, 62.62f, 4.34f},                      // Left, Behind
    {-1017.72f, -3490.92f, 62.62f, 4.34f},                      // Right, Behind
    {-1015.77f, -3497.15f, 62.82f, 4.34f},                      // Left, Mid
    {-1019.51f, -3495.49f, 62.82f, 4.34f},                      // Right, Mid
    {-1017.25f, -3500.85f, 62.98f, 4.34f},                      // Left, front
    {-1020.95f, -3499.21f, 62.98f, 4.34f}                       // Right, Front
};

//horde run to where
#define H_RUNTOX -1016.44f
#define H_RUNTOY -3508.48f
#define H_RUNTOZ 62.96f

uint32 const AllianceSoldierId[3] =
{
    12938,                                                  // 12938 Injured Alliance Soldier
    12936,                                                  // 12936 Badly injured Alliance Soldier
    12937                                                   // 12937 Critically injured Alliance Soldier
};

uint32 const HordeSoldierId[3] =
{
    12923,                                                  //12923 Injured Soldier
    12924,                                                  //12924 Badly injured Soldier
    12925                                                   //12925 Critically injured Soldier
};

/*######
## npc_doctor (handles both Gustaf Vanhowzen and Gregory Victor)
######*/

class npc_doctor : public CreatureScript
{
    public:
        npc_doctor() : CreatureScript("npc_doctor") {}

        struct npc_doctorAI : public ScriptedAI
        {
            npc_doctorAI(Creature* creature) : ScriptedAI(creature) {}

            uint64 PlayerGUID;

            uint32 SummonPatientTimer;
            uint32 SummonPatientCount;
            uint32 PatientDiedCount;
            uint32 PatientSavedCount;

            bool Event;

            std::list<uint64> Patients;
            std::vector<Location*> Coordinates;

            void Reset()
            {
                PlayerGUID = 0;

                SummonPatientTimer = 10000;
                SummonPatientCount = 0;
                PatientDiedCount = 0;
                PatientSavedCount = 0;

                Patients.clear();
                Coordinates.clear();

                Event = false;

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            void BeginEvent(Player* player)
            {
                PlayerGUID = player->GetGUID();

                SummonPatientTimer = 10000;
                SummonPatientCount = 0;
                PatientDiedCount = 0;
                PatientSavedCount = 0;

                switch (me->GetEntry())
                {
                    case DOCTOR_ALLIANCE:
                        for (uint8 i = 0; i < ALLIANCE_COORDS; ++i)
                            Coordinates.push_back(&AllianceCoords[i]);
                        break;
                    case DOCTOR_HORDE:
                        for (uint8 i = 0; i < HORDE_COORDS; ++i)
                            Coordinates.push_back(&HordeCoords[i]);
                        break;
                }

                Event = true;
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            void PatientDied(Location* point)
            {
                Player* player = Unit::GetPlayer(*me, PlayerGUID);
                if (player && ((player->GetQuestStatus(6624) == QUEST_STATUS_INCOMPLETE) || (player->GetQuestStatus(6622) == QUEST_STATUS_INCOMPLETE)))
                {
                    ++PatientDiedCount;

                    if (PatientDiedCount > 5 && Event)
                    {
                        if (player->GetQuestStatus(6624) == QUEST_STATUS_INCOMPLETE)
                            player->FailQuest(6624);
                        else if (player->GetQuestStatus(6622) == QUEST_STATUS_INCOMPLETE)
                            player->FailQuest(6622);

                        Reset();
                        return;
                    }

                    Coordinates.push_back(point);
                }
                else
                    // If no player or player abandon quest in progress
                    Reset();
            }

            void PatientSaved(Creature* /*soldier*/, Player* player, Location* point)
            {
                if (player && PlayerGUID == player->GetGUID())
                {
                    if ((player->GetQuestStatus(6624) == QUEST_STATUS_INCOMPLETE) || (player->GetQuestStatus(6622) == QUEST_STATUS_INCOMPLETE))
                    {
                        ++PatientSavedCount;

                        if (PatientSavedCount == 15)
                        {
                            if (!Patients.empty())
                            {
                                std::list<uint64>::const_iterator itr;
                                for (itr = Patients.begin(); itr != Patients.end(); ++itr)
                                {
                                    if (Creature* patient = Unit::GetCreature((*me), *itr))
                                        patient->setDeathState(JUST_DIED);
                                }
                            }

                            if (player->GetQuestStatus(6624) == QUEST_STATUS_INCOMPLETE)
                                player->AreaExploredOrEventHappens(6624);
                            else if (player->GetQuestStatus(6622) == QUEST_STATUS_INCOMPLETE)
                                player->AreaExploredOrEventHappens(6622);

                            Reset();
                            return;
                        }

                        Coordinates.push_back(point);
                    }
                }
            }

            void UpdateAI(uint32 const diff);

            void EnterCombat(Unit* /*who*/){}
        };

        bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
        {
            if ((quest->GetQuestId() == 6624) || (quest->GetQuestId() == 6622))
                CAST_AI(npc_doctor::npc_doctorAI, creature->AI())->BeginEvent(player);

            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_doctorAI(creature);
        }
};

/*#####
## npc_injured_patient (handles all the patients, no matter Horde or Alliance)
#####*/

class npc_injured_patient : public CreatureScript
{
    public:
        npc_injured_patient() : CreatureScript("npc_injured_patient") { }

        struct npc_injured_patientAI : public ScriptedAI
        {
            npc_injured_patientAI(Creature* creature) : ScriptedAI(creature) {}

            uint64 DoctorGUID;
            Location* Coord;

            void Reset()
            {
                DoctorGUID = 0;
                Coord = NULL;

                //no select
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                //no regen health
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);

                //to make them lay with face down
                me->SetStandState(UNIT_STAND_STATE_DEAD);

                uint32 mobId = me->GetEntry();

                switch (mobId)
                {                                                   //lower max health
                    case 12923:
                    case 12938:                                     //Injured Soldier
                        me->SetHealth(me->CountPctFromMaxHealth(75));
                        break;
                    case 12924:
                    case 12936:                                     //Badly injured Soldier
                        me->SetHealth(me->CountPctFromMaxHealth(50));
                        break;
                    case 12925:
                    case 12937:                                     //Critically injured Soldier
                        me->SetHealth(me->CountPctFromMaxHealth(25));
                        break;
                }
            }

            void EnterCombat(Unit* /*who*/){}

            void SpellHit(Unit* caster, SpellInfo const* spell)
            {
                if (caster->IsPlayer() && me->isAlive() && spell->Id == 20804)
                {
                    if ((CAST_PLR(caster)->GetQuestStatus(6624) == QUEST_STATUS_INCOMPLETE) || (CAST_PLR(caster)->GetQuestStatus(6622) == QUEST_STATUS_INCOMPLETE))
                        if (DoctorGUID)
                            if (Creature* doctor = Unit::GetCreature(*me, DoctorGUID))
                                CAST_AI(npc_doctor::npc_doctorAI, doctor->AI())->PatientSaved(me, CAST_PLR(caster), Coord);

                    //make not selectable
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                    //regen health
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);

                    //stand up
                    me->SetStandState(UNIT_STAND_STATE_STAND);

                    DoScriptText(RAND(SAY_DOC1, SAY_DOC2, SAY_DOC3), me);

                    uint32 mobId = me->GetEntry();
                    me->SetWalk(false);

                    switch (mobId)
                    {
                        case 12923:
                        case 12924:
                        case 12925:
                            me->GetMotionMaster()->MovePoint(0, H_RUNTOX, H_RUNTOY, H_RUNTOZ);
                            break;
                        case 12936:
                        case 12937:
                        case 12938:
                            me->GetMotionMaster()->MovePoint(0, A_RUNTOX, A_RUNTOY, A_RUNTOZ);
                            break;
                    }
                }
            }

            void UpdateAI(uint32 const /*diff*/)
            {
                //lower HP on every world tick makes it a useful counter, not officlone though
                if (me->isAlive() && me->GetHealth() > 6)
                    me->ModifyHealth(-5);

                if (me->isAlive() && me->GetHealth() <= 6)
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->setDeathState(JUST_DIED);
                    me->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, 32);

                    if (DoctorGUID)
                        if (Creature* doctor = Unit::GetCreature((*me), DoctorGUID))
                            CAST_AI(npc_doctor::npc_doctorAI, doctor->AI())->PatientDied(Coord);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_injured_patientAI(creature);
        }
};

void npc_doctor::npc_doctorAI::UpdateAI(uint32 const diff)
{
    if (Event && SummonPatientCount >= 20)
    {
        Reset();
        return;
    }

    if (Event)
    {
        if (SummonPatientTimer <= diff)
        {
            if (Coordinates.empty())
                return;

            std::vector<Location*>::iterator itr = Coordinates.begin() + rand() % Coordinates.size();
            uint32 patientEntry = 0;

            switch (me->GetEntry())
            {
                case DOCTOR_ALLIANCE:
                    patientEntry = AllianceSoldierId[rand() % 3];
                    break;
                case DOCTOR_HORDE:
                    patientEntry = HordeSoldierId[rand() % 3];
                    break;
                default:
                    sLog->outError(LOG_FILTER_TSCR, "Invalid entry for Triage doctor. Please check your database");
                    return;
            }

            if (Location* point = *itr)
            {
                if (Creature* Patient = me->SummonCreature(patientEntry, point->x, point->y, point->z, point->o, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000))
                {
                    //303, this flag appear to be required for client side item->spell to work (TARGET_SINGLE_FRIEND)
                    Patient->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);

                    Patients.push_back(Patient->GetGUID());
                    CAST_AI(npc_injured_patient::npc_injured_patientAI, Patient->AI())->DoctorGUID = me->GetGUID();
                    CAST_AI(npc_injured_patient::npc_injured_patientAI, Patient->AI())->Coord = point;

                    Coordinates.erase(itr);
                }
            }
            SummonPatientTimer = 10000;
            ++SummonPatientCount;
        }
        else
            SummonPatientTimer -= diff;
    }
}

/*######
## npc_garments_of_quests
######*/

//TODO: get text for each NPC

enum eGarments
{
    SPELL_LESSER_HEAL_R2    = 2052,
    SPELL_FORTITUDE_R1      = 1243,

    QUEST_MOON              = 5621,
    QUEST_LIGHT_1           = 5624,
    QUEST_LIGHT_2           = 5625,
    QUEST_SPIRIT            = 5648,
    QUEST_DARKNESS          = 5650,

    ENTRY_SHAYA             = 12429,
    ENTRY_ROBERTS           = 12423,
    ENTRY_DOLF              = 12427,
    ENTRY_KORJA             = 12430,
    ENTRY_DG_KEL            = 12428,

    //used by 12429, 12423, 12427, 12430, 12428, but signed for 12429
    SAY_COMMON_HEALED       = -1000164,
    SAY_DG_KEL_THANKS       = -1000165,
    SAY_DG_KEL_GOODBYE      = -1000166,
    SAY_ROBERTS_THANKS      = -1000167,
    SAY_ROBERTS_GOODBYE     = -1000168,
    SAY_KORJA_THANKS        = -1000169,
    SAY_KORJA_GOODBYE       = -1000170,
    SAY_DOLF_THANKS         = -1000171,
    SAY_DOLF_GOODBYE        = -1000172,
    SAY_SHAYA_THANKS        = -1000173,
    SAY_SHAYA_GOODBYE       = -1000174, //signed for 21469
};

class npc_garments_of_quests : public CreatureScript
{
    public:
        npc_garments_of_quests() : CreatureScript("npc_garments_of_quests") { }

        struct npc_garments_of_questsAI : public npc_escortAI
        {
            npc_garments_of_questsAI(Creature* creature) : npc_escortAI(creature) {Reset();}

            uint64 CasterGUID;

            bool IsHealed;
            bool CanRun;

            uint32 RunAwayTimer;

            void Reset()
            {
                CasterGUID = 0;

                IsHealed = false;
                CanRun = false;

                RunAwayTimer = 5000;

                me->SetStandState(UNIT_STAND_STATE_KNEEL);
                //expect database to have RegenHealth=0
                me->SetHealth(me->CountPctFromMaxHealth(70));
            }

            void EnterCombat(Unit* /*who*/) {}

            void SpellHit(Unit* caster, SpellInfo const* Spell)
            {
                if (Spell->Id == SPELL_LESSER_HEAL_R2 || Spell->Id == SPELL_FORTITUDE_R1)
                {
                    //not while in combat
                    if (me->isInCombat())
                        return;

                    //nothing to be done now
                    if (IsHealed && CanRun)
                        return;

                    if (Player* player = caster->ToPlayer())
                    {
                        switch (me->GetEntry())
                        {
                            case ENTRY_SHAYA:
                                if (player->GetQuestStatus(QUEST_MOON) == QUEST_STATUS_INCOMPLETE)
                                {
                                    if (IsHealed && !CanRun && Spell->Id == SPELL_FORTITUDE_R1)
                                    {
                                        DoScriptText(SAY_SHAYA_THANKS, me, caster);
                                        CanRun = true;
                                    }
                                    else if (!IsHealed && Spell->Id == SPELL_LESSER_HEAL_R2)
                                    {
                                        CasterGUID = caster->GetGUID();
                                        me->SetStandState(UNIT_STAND_STATE_STAND);
                                        DoScriptText(SAY_COMMON_HEALED, me, caster);
                                        IsHealed = true;
                                    }
                                }
                                break;
                            case ENTRY_ROBERTS:
                                if (player->GetQuestStatus(QUEST_LIGHT_1) == QUEST_STATUS_INCOMPLETE)
                                {
                                    if (IsHealed && !CanRun && Spell->Id == SPELL_FORTITUDE_R1)
                                    {
                                        DoScriptText(SAY_ROBERTS_THANKS, me, caster);
                                        CanRun = true;
                                    }
                                    else if (!IsHealed && Spell->Id == SPELL_LESSER_HEAL_R2)
                                    {
                                        CasterGUID = caster->GetGUID();
                                        me->SetStandState(UNIT_STAND_STATE_STAND);
                                        DoScriptText(SAY_COMMON_HEALED, me, caster);
                                        IsHealed = true;
                                    }
                                }
                                break;
                            case ENTRY_DOLF:
                                if (player->GetQuestStatus(QUEST_LIGHT_2) == QUEST_STATUS_INCOMPLETE)
                                {
                                    if (IsHealed && !CanRun && Spell->Id == SPELL_FORTITUDE_R1)
                                    {
                                        DoScriptText(SAY_DOLF_THANKS, me, caster);
                                        CanRun = true;
                                    }
                                    else if (!IsHealed && Spell->Id == SPELL_LESSER_HEAL_R2)
                                    {
                                        CasterGUID = caster->GetGUID();
                                        me->SetStandState(UNIT_STAND_STATE_STAND);
                                        DoScriptText(SAY_COMMON_HEALED, me, caster);
                                        IsHealed = true;
                                    }
                                }
                                break;
                            case ENTRY_KORJA:
                                if (player->GetQuestStatus(QUEST_SPIRIT) == QUEST_STATUS_INCOMPLETE)
                                {
                                    if (IsHealed && !CanRun && Spell->Id == SPELL_FORTITUDE_R1)
                                    {
                                        DoScriptText(SAY_KORJA_THANKS, me, caster);
                                        CanRun = true;
                                    }
                                    else if (!IsHealed && Spell->Id == SPELL_LESSER_HEAL_R2)
                                    {
                                        CasterGUID = caster->GetGUID();
                                        me->SetStandState(UNIT_STAND_STATE_STAND);
                                        DoScriptText(SAY_COMMON_HEALED, me, caster);
                                        IsHealed = true;
                                    }
                                }
                                break;
                            case ENTRY_DG_KEL:
                                if (player->GetQuestStatus(QUEST_DARKNESS) == QUEST_STATUS_INCOMPLETE)
                                {
                                    if (IsHealed && !CanRun && Spell->Id == SPELL_FORTITUDE_R1)
                                    {
                                        DoScriptText(SAY_DG_KEL_THANKS, me, caster);
                                        CanRun = true;
                                    }
                                    else if (!IsHealed && Spell->Id == SPELL_LESSER_HEAL_R2)
                                    {
                                        CasterGUID = caster->GetGUID();
                                        me->SetStandState(UNIT_STAND_STATE_STAND);
                                        DoScriptText(SAY_COMMON_HEALED, me, caster);
                                        IsHealed = true;
                                    }
                                }
                                break;
                        }

                        //give quest credit, not expect any special quest objectives
                        if (CanRun)
                            player->TalkedToCreature(me->GetEntry(), me->GetGUID());
                    }
                }
            }

            void WaypointReached(uint32 /*waypointId*/)
            {

            }

            void UpdateAI(uint32 const diff)
            {
                if (CanRun && !me->isInCombat())
                {
                    if (RunAwayTimer <= diff)
                    {
                        if (Unit* unit = Unit::GetUnit(*me, CasterGUID))
                        {
                            switch (me->GetEntry())
                            {
                                case ENTRY_SHAYA:
                                    DoScriptText(SAY_SHAYA_GOODBYE, me, unit);
                                    break;
                                case ENTRY_ROBERTS:
                                    DoScriptText(SAY_ROBERTS_GOODBYE, me, unit);
                                    break;
                                case ENTRY_DOLF:
                                    DoScriptText(SAY_DOLF_GOODBYE, me, unit);
                                    break;
                                case ENTRY_KORJA:
                                    DoScriptText(SAY_KORJA_GOODBYE, me, unit);
                                    break;
                                case ENTRY_DG_KEL:
                                    DoScriptText(SAY_DG_KEL_GOODBYE, me, unit);
                                    break;
                            }

                            Start(false, true, true);
                        }
                        else
                            EnterEvadeMode();                       //something went wrong

                        RunAwayTimer = 30000;
                    }
                    else
                        RunAwayTimer -= diff;
                }

                npc_escortAI::UpdateAI(diff);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_garments_of_questsAI(creature);
        }
};

/*######
## npc_guardian
######*/

#define SPELL_DEATHTOUCH                5

class npc_guardian : public CreatureScript
{
    public:
        npc_guardian() : CreatureScript("npc_guardian") { }

        struct npc_guardianAI : public ScriptedAI
        {
            npc_guardianAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }

            void EnterCombat(Unit* /*who*/)
            {
            }

            void UpdateAI(uint32 const /*diff*/)
            {
                if (!UpdateVictim())
                    return;

                if (me->isAttackReady())
                {
                    DoCast(me->getVictim(), SPELL_DEATHTOUCH, true);
                    me->resetAttackTimer();
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_guardianAI(creature);
        }
};

/////////////////////////
/// npc_mount_vendor
/////////////////////////

/// Used for faction restriction on mount vendor.
class npc_mount_vendor : public CreatureScript
{
    public:
        npc_mount_vendor() : CreatureScript("npc_mount_vendor") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (creature->isQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            bool canBuy = false;
            uint32 vendor = creature->GetEntry();
            uint8 race = player->getRace();

            switch (vendor)
            {
                case 384:                                           ///< Katie Hunter
                case 1460:                                          ///< Unger Statforth
                case 2357:                                          ///< Merideth Carlson
                case 4885:                                          ///< Gregor MacVince
                    if (player->GetReputationRank(72) != REP_EXALTED && race != RACE_HUMAN)
                        player->SEND_GOSSIP_MENU(5855, creature->GetGUID());
                    else canBuy = true;
                    break;
                case 1261:                                          ///< Veron Amberstill
                    if (player->GetReputationRank(47) != REP_EXALTED && race != RACE_DWARF)
                        player->SEND_GOSSIP_MENU(5856, creature->GetGUID());
                    else canBuy = true;
                    break;
                case 3362:                                          ///< Ogunaro Wolfrunner
                    if (player->GetReputationRank(76) != REP_EXALTED && race != RACE_ORC)
                        player->SEND_GOSSIP_MENU(5841, creature->GetGUID());
                    else canBuy = true;
                    break;
                case 3685:                                          ///< Harb Clawhoof
                    if (player->GetReputationRank(81) != REP_EXALTED && race != RACE_TAUREN)
                        player->SEND_GOSSIP_MENU(5843, creature->GetGUID());
                    else canBuy = true;
                    break;
                case 4730:                                          ///< Lelanai
                    if (player->GetReputationRank(69) != REP_EXALTED && race != RACE_NIGHTELF)
                        player->SEND_GOSSIP_MENU(5844, creature->GetGUID());
                    else canBuy = true;
                    break;
                case 4731:                                          ///< Zachariah Post
                    if (player->GetReputationRank(68) != REP_EXALTED && race != RACE_UNDEAD_PLAYER)
                        player->SEND_GOSSIP_MENU(5840, creature->GetGUID());
                    else canBuy = true;
                    break;
                case 7952:                                          ///< Zjolnir
                    if (player->GetReputationRank(530) != REP_EXALTED && race != RACE_TROLL)
                        player->SEND_GOSSIP_MENU(5842, creature->GetGUID());
                    else canBuy = true;
                    break;
                case 7955:                                          ///< Milli Featherwhistle
                    if (player->GetReputationRank(54) != REP_EXALTED && race != RACE_GNOME)
                        player->SEND_GOSSIP_MENU(5857, creature->GetGUID());
                    else canBuy = true;
                    break;
                case 16264:                                         ///< Winaestra
                    if (player->GetReputationRank(911) != REP_EXALTED && race != RACE_BLOODELF)
                        player->SEND_GOSSIP_MENU(10305, creature->GetGUID());
                    else canBuy = true;
                    break;
                case 17584:                                         ///< Torallius the Pack Handler
                    if (player->GetReputationRank(930) != REP_EXALTED && race != RACE_DRAENEI)
                        player->SEND_GOSSIP_MENU(10239, creature->GetGUID());
                    else canBuy = true;
                    break;
                case 48510:                                         ///< Kall Worthalon
                    if (player->GetReputationRank(1133) != REP_EXALTED && race != RACE_GOBLIN)
                        player->SEND_GOSSIP_MENU(30002, creature->GetGUID());
                    else canBuy = true;
                    break;
                case 55285:                                         ///< Astrid Langstrump
                    if (player->GetReputationRank(1134) != REP_EXALTED && race != RACE_WORGEN)
                        player->SEND_GOSSIP_MENU(30002, creature->GetGUID());
                    else canBuy = true;
                    break;
                case 65068:                                         ///< Old Whitenose
                    if (player->GetReputationRank(1353) != REP_EXALTED && race != RACE_PANDAREN_ALLI)
                        player->SEND_GOSSIP_MENU(300002, creature->GetGUID()); ///< unk id
                    else canBuy = true;
                    break;
                case 66022:                                         ///< Turtlemaster Odai
                    if (player->GetReputationRank(1352) != REP_EXALTED && race != RACE_PANDAREN_HORDE)
                        player->SEND_GOSSIP_MENU(300002, creature->GetGUID()); ///< unk id
                    else canBuy = true;
                    break;
            }

            if (canBuy)
            {
                if (creature->isVendor())
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
                player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            }
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            if (action == GOSSIP_ACTION_TRADE)
                player->GetSession()->SendListInventory(creature->GetGUID());

            return true;
        }
};

/*######
## npc_rogue_trainer
######*/

#define GOSSIP_HELLO_ROGUE1 "I wish to unlearn my talents"
#define GOSSIP_HELLO_ROGUE2 "<Take the letter>"
#define GOSSIP_HELLO_ROGUE3 "Purchase a Dual Talent Specialization."
#define GOSSIP_HELLO_ROGUE4 "I wish to unlearn my specialization"

class npc_rogue_trainer : public CreatureScript
{
    public:
        npc_rogue_trainer() : CreatureScript("npc_rogue_trainer") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (creature->isQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (creature->isTrainer())
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, GOSSIP_TEXT_TRAIN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRAIN);

            if (creature->isCanTrainingAndResetTalentsOf(player))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, GOSSIP_HELLO_ROGUE1, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_UNLEARNTALENTS);

            if (creature->isCanTrainingAndResetTalentsOf(player))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, GOSSIP_HELLO_ROGUE4, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_UNLEARNSPECIALIZATION);

            if (player->GetSpecsCount() == 1 && creature->isCanTrainingAndResetTalentsOf(player) && player->getLevel() >= sWorld->getIntConfig(CONFIG_MIN_DUALSPEC_LEVEL))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, GOSSIP_HELLO_ROGUE3, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_LEARNDUALSPEC);

            if (player->getClass() == CLASS_ROGUE && player->getLevel() >= 24 && !player->HasItemCount(17126) && !player->GetQuestRewardStatus(6681))
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO_ROGUE2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->SEND_GOSSIP_MENU(5996, creature->GetGUID());
            } else
                player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                    player->CLOSE_GOSSIP_MENU();
                    player->CastSpell(player, 21100, false);
                    break;
                case GOSSIP_ACTION_TRAIN:
                    player->GetSession()->SendTrainerList(creature->GetGUID());
                    break;
                case GOSSIP_OPTION_UNLEARNTALENTS:
                    player->CLOSE_GOSSIP_MENU();
                    player->SendTalentWipeConfirm(creature->GetGUID(), false);
                    break;
                case GOSSIP_OPTION_UNLEARNSPECIALIZATION:
                    player->CLOSE_GOSSIP_MENU();
                    player->SendTalentWipeConfirm(creature->GetGUID(), true);
                    break;
                case GOSSIP_OPTION_LEARNDUALSPEC:
                    if (player->GetSpecsCount() == 1 && !(player->getLevel() < sWorld->getIntConfig(CONFIG_MIN_DUALSPEC_LEVEL)))
                    {
                        if (!player->HasEnoughMoney(uint64(10000000)))
                        {
                            player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, 0, 0, 0);
                            player->PlayerTalkClass->SendCloseGossip();
                            break;
                        }
                        else
                        {
                            player->ModifyMoney(int64(-10000000));

                            // Cast spells that teach dual spec
                            // Both are also ImplicitTarget self and must be cast by player
                            player->CastSpell(player, 63680, true, NULL, nullptr, player->GetGUID());
                            player->CastSpell(player, 63624, true, NULL, nullptr, player->GetGUID());

                            // Should show another Gossip text with "Congratulations..."
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;
            }
            return true;
        }
};

/*######
## npc_sayge
######*/

#define SPELL_DMG       23768                               //dmg
#define SPELL_RES       23769                               //res
#define SPELL_ARM       23767                               //arm
#define SPELL_SPI       23738                               //spi
#define SPELL_INT       23766                               //int
#define SPELL_STM       23737                               //stm
#define SPELL_STR       23735                               //str
#define SPELL_AGI       23736                               //agi
#define SPELL_FORTUNE   23765                               //faire fortune

#define GOSSIP_HELLO_SAYGE  "Yes"
#define GOSSIP_SENDACTION_SAYGE1    "Slay the Man"
#define GOSSIP_SENDACTION_SAYGE2    "Turn him over to liege"
#define GOSSIP_SENDACTION_SAYGE3    "Confiscate the corn"
#define GOSSIP_SENDACTION_SAYGE4    "Let him go and have the corn"
#define GOSSIP_SENDACTION_SAYGE5    "Execute your friend painfully"
#define GOSSIP_SENDACTION_SAYGE6    "Execute your friend painlessly"
#define GOSSIP_SENDACTION_SAYGE7    "Let your friend go"
#define GOSSIP_SENDACTION_SAYGE8    "Confront the diplomat"
#define GOSSIP_SENDACTION_SAYGE9    "Show not so quiet defiance"
#define GOSSIP_SENDACTION_SAYGE10   "Remain quiet"
#define GOSSIP_SENDACTION_SAYGE11   "Speak against your brother openly"
#define GOSSIP_SENDACTION_SAYGE12   "Help your brother in"
#define GOSSIP_SENDACTION_SAYGE13   "Keep your brother out without letting him know"
#define GOSSIP_SENDACTION_SAYGE14   "Take credit, keep gold"
#define GOSSIP_SENDACTION_SAYGE15   "Take credit, share the gold"
#define GOSSIP_SENDACTION_SAYGE16   "Let the knight take credit"
#define GOSSIP_SENDACTION_SAYGE17   "Thanks"

class npc_sayge : public CreatureScript
{
    public:
        npc_sayge() : CreatureScript("npc_sayge") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (creature->isQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (player->HasSpellCooldown(SPELL_INT) ||
                player->HasSpellCooldown(SPELL_ARM) ||
                player->HasSpellCooldown(SPELL_DMG) ||
                player->HasSpellCooldown(SPELL_RES) ||
                player->HasSpellCooldown(SPELL_STR) ||
                player->HasSpellCooldown(SPELL_AGI) ||
                player->HasSpellCooldown(SPELL_STM) ||
                player->HasSpellCooldown(SPELL_SPI))
                player->SEND_GOSSIP_MENU(7393, creature->GetGUID());
            else
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO_SAYGE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->SEND_GOSSIP_MENU(7339, creature->GetGUID());
            }

            return true;
        }

        void SendAction(Player* player, Creature* creature, uint32 action)
        {
            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE1,            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE2,            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE3,            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE4,            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                    player->SEND_GOSSIP_MENU(7340, creature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF + 2:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE5,            GOSSIP_SENDER_MAIN + 1, GOSSIP_ACTION_INFO_DEF);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE6,            GOSSIP_SENDER_MAIN + 2, GOSSIP_ACTION_INFO_DEF);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE7,            GOSSIP_SENDER_MAIN + 3, GOSSIP_ACTION_INFO_DEF);
                    player->SEND_GOSSIP_MENU(7341, creature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF + 3:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE8,            GOSSIP_SENDER_MAIN + 4, GOSSIP_ACTION_INFO_DEF);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE9,            GOSSIP_SENDER_MAIN + 5, GOSSIP_ACTION_INFO_DEF);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE10,           GOSSIP_SENDER_MAIN + 2, GOSSIP_ACTION_INFO_DEF);
                    player->SEND_GOSSIP_MENU(7361, creature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF + 4:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE11,           GOSSIP_SENDER_MAIN + 6, GOSSIP_ACTION_INFO_DEF);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE12,           GOSSIP_SENDER_MAIN + 7, GOSSIP_ACTION_INFO_DEF);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE13,           GOSSIP_SENDER_MAIN + 8, GOSSIP_ACTION_INFO_DEF);
                    player->SEND_GOSSIP_MENU(7362, creature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF + 5:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE14,           GOSSIP_SENDER_MAIN + 5, GOSSIP_ACTION_INFO_DEF);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE15,           GOSSIP_SENDER_MAIN + 4, GOSSIP_ACTION_INFO_DEF);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE16,           GOSSIP_SENDER_MAIN + 3, GOSSIP_ACTION_INFO_DEF);
                    player->SEND_GOSSIP_MENU(7363, creature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SENDACTION_SAYGE17,           GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                    player->SEND_GOSSIP_MENU(7364, creature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF + 6:
                    creature->CastSpell(player, SPELL_FORTUNE, false);
                    player->SEND_GOSSIP_MENU(7365, creature->GetGUID());
                    break;
            }
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            switch (sender)
            {
                case GOSSIP_SENDER_MAIN:
                    SendAction(player, creature, action);
                    break;
                case GOSSIP_SENDER_MAIN + 1:
                    creature->CastSpell(player, SPELL_DMG, false);
                    player->AddSpellCooldown(SPELL_DMG, 0, 7200 * IN_MILLISECONDS);
                    SendAction(player, creature, action);
                    break;
                case GOSSIP_SENDER_MAIN + 2:
                    creature->CastSpell(player, SPELL_RES, false);
                    player->AddSpellCooldown(SPELL_DMG, 0, 7200 * IN_MILLISECONDS);
                    SendAction(player, creature, action);
                    break;
                case GOSSIP_SENDER_MAIN + 3:
                    creature->CastSpell(player, SPELL_ARM, false);
                    player->AddSpellCooldown(SPELL_DMG, 0, 7200 * IN_MILLISECONDS);
                    SendAction(player, creature, action);
                    break;
                case GOSSIP_SENDER_MAIN + 4:
                    creature->CastSpell(player, SPELL_SPI, false);
                    player->AddSpellCooldown(SPELL_DMG, 0, 7200 * IN_MILLISECONDS);
                    SendAction(player, creature, action);
                    break;
                case GOSSIP_SENDER_MAIN + 5:
                    creature->CastSpell(player, SPELL_INT, false);
                    player->AddSpellCooldown(SPELL_DMG, 0, 7200 * IN_MILLISECONDS);
                    SendAction(player, creature, action);
                    break;
                case GOSSIP_SENDER_MAIN + 6:
                    creature->CastSpell(player, SPELL_STM, false);
                    player->AddSpellCooldown(SPELL_DMG, 0, 7200 * IN_MILLISECONDS);
                    SendAction(player, creature, action);
                    break;
                case GOSSIP_SENDER_MAIN + 7:
                    creature->CastSpell(player, SPELL_STR, false);
                    player->AddSpellCooldown(SPELL_DMG, 0, 7200 * IN_MILLISECONDS);
                    SendAction(player, creature, action);
                    break;
                case GOSSIP_SENDER_MAIN + 8:
                    creature->CastSpell(player, SPELL_AGI, false);
                    player->AddSpellCooldown(SPELL_DMG, 0, 7200 * IN_MILLISECONDS);
                    SendAction(player, creature, action);
                    break;
            }
            return true;
        }
};

class npc_steam_tonk : public CreatureScript
{
    public:
        npc_steam_tonk() : CreatureScript("npc_steam_tonk") { }

        struct npc_steam_tonkAI : public ScriptedAI
        {
            npc_steam_tonkAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset() {}
            void EnterCombat(Unit* /*who*/) {}

            void OnPossess(bool apply)
            {
                if (apply)
                {
                    // Initialize the action bar without the melee attack command
                    me->InitCharmInfo();
                    me->GetCharmInfo()->InitEmptyActionBar(false);

                    me->SetReactState(REACT_PASSIVE);
                }
                else
                    me->SetReactState(REACT_AGGRESSIVE);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_steam_tonkAI(creature);
        }
};

#define SPELL_TONK_MINE_DETONATE 25099

class npc_tonk_mine : public CreatureScript
{
    public:
        npc_tonk_mine() : CreatureScript("npc_tonk_mine") { }

        struct npc_tonk_mineAI : public ScriptedAI
        {
            npc_tonk_mineAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            uint32 ExplosionTimer;

            void Reset()
            {
                ExplosionTimer = 3000;
            }

            void EnterCombat(Unit* /*who*/) {}
            void AttackStart(Unit* /*who*/) {}
            void MoveInLineOfSight(Unit* /*who*/) {}

            void UpdateAI(uint32 const diff)
            {
                if (ExplosionTimer <= diff)
                {
                    DoCast(me, SPELL_TONK_MINE_DETONATE, true);
                    me->setDeathState(DEAD); // unsummon it
                }
                else
                    ExplosionTimer -= diff;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tonk_mineAI(creature);
        }
};

/*####
## npc_brewfest_reveler
####*/

class npc_brewfest_reveler : public CreatureScript
{
    public:
        npc_brewfest_reveler() : CreatureScript("npc_brewfest_reveler") { }

        struct npc_brewfest_revelerAI : public ScriptedAI
        {
            npc_brewfest_revelerAI(Creature* creature) : ScriptedAI(creature) {}
            void ReceiveEmote(Player* player, uint32 emote)
            {
                if (!IsHolidayActive(HOLIDAY_BREWFEST))
                    return;

                if (emote == TEXT_EMOTE_DANCE)
                    me->CastSpell(player, 41586, false);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_brewfest_revelerAI(creature);
        }
};

/*####
## npc_snake_trap_serpents
####*/

#define SPELL_MIND_NUMBING_POISON    25810   //Viper
#define SPELL_DEADLY_POISON          34655   //Venomous Snake
#define SPELL_CRIPPLING_POISON       30981   //Viper

#define VENOMOUS_SNAKE_TIMER 1500
#define VIPER_TIMER 3000

#define C_VIPER 19921

class npc_snake_trap : public CreatureScript
{
    public:
        npc_snake_trap() : CreatureScript("npc_snake_trap_serpents") { }

        struct npc_snake_trap_serpentsAI : public ScriptedAI
        {
            npc_snake_trap_serpentsAI(Creature* creature) : ScriptedAI(creature) {}

            uint32 SpellTimer;
            bool IsViper;

            void EnterCombat(Unit* /*who*/) {}

            void Reset()
            {
                SpellTimer = 0;

                CreatureTemplate const* Info = me->GetCreatureTemplate();

                IsViper = Info->Entry == C_VIPER ? true : false;

                me->SetMaxHealth(uint32(107 * (me->getLevel() - 40) * 0.025f));
                //Add delta to make them not all hit the same time
                uint32 delta = (rand() % 7) * 100;
                me->SetStatFloatValue(UNIT_FIELD_ATTACK_ROUND_BASE_TIME, float(Info->baseattacktime + delta));

                // Start attacking attacker of owner on first ai update after spawn - move in line of sight may choose better target
                if (!me->getVictim() && me->isSummon())
                    if (Unit* Owner = me->ToTempSummon()->GetSummoner())
                        if (Owner->getAttackerForHelper())
                            AttackStart(Owner->getAttackerForHelper());
            }

            //Redefined for random target selection:
            void MoveInLineOfSight(Unit* who)
            {
                if (!me->getVictim() && me->canCreatureAttack(who))
                {
                    if (me->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                        return;

                    float attackRadius = me->GetAttackDistance(who);
                    if (me->IsWithinDistInMap(who, attackRadius) && me->IsWithinLOSInMap(who))
                    {
                        if (!(rand() % 5))
                        {
                            me->setAttackTimer(WeaponAttackType::BaseAttack, (rand() % 10) * 100);
                            SpellTimer = (rand() % 10) * 100;
                            AttackStart(who);
                        }
                    }
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->getVictim()->HasBreakableByDamageCrowdControlAura(me))
                {
                    me->InterruptNonMeleeSpells(false);
                    return;
                }

                if (me->getVictim()->isInStun() && me->getVictim()->HasAura(3355))
                    return;

                if (SpellTimer <= diff)
                {
                    if (IsViper) //Viper
                    {
                        if (urand(0, 2) == 0) //33% chance to cast
                        {
                            uint32 spell;
                            spell = SPELL_CRIPPLING_POISON;

                            if (!me->getVictim()->HasAura(spell))
                                DoCast(me->getVictim(), spell);
                        }

                        SpellTimer = VIPER_TIMER;
                    }
                    else //Venomous Snake
                    {
                        if (urand(0, 2) == 0) //33% chance to cast
                        {
                            if (!me->getVictim()->HasAura(SPELL_CRIPPLING_POISON))
                                DoCast(me->getVictim(), SPELL_CRIPPLING_POISON);
                        }
                        SpellTimer = VENOMOUS_SNAKE_TIMER + (rand() % 5) * 100;
                    }
                }
                else
                    SpellTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_snake_trap_serpentsAI(creature);
        }
};

#define SAY_RANDOM_MOJO0    "Now that's what I call froggy-style!"
#define SAY_RANDOM_MOJO1    "Your lily pad or mine?"
#define SAY_RANDOM_MOJO2    "This won't take long, did it?"
#define SAY_RANDOM_MOJO3    "I thought you'd never ask!"
#define SAY_RANDOM_MOJO4    "I promise not to give you warts..."
#define SAY_RANDOM_MOJO5    "Feelin' a little froggy, are ya?"
#define SAY_RANDOM_MOJO6a   "Listen, "
#define SAY_RANDOM_MOJO6b   ", I know of a little swamp not too far from here...."
#define SAY_RANDOM_MOJO7    "There's just never enough Mojo to go around..."

class mob_mojo : public CreatureScript
{
    public:
        mob_mojo() : CreatureScript("mob_mojo") { }

        struct mob_mojoAI : public ScriptedAI
        {
            mob_mojoAI(Creature* creature) : ScriptedAI(creature) {Reset();}
            uint32 hearts;
            uint64 victimGUID;
            void Reset()
            {
                victimGUID = 0;
                hearts = 15000;
                if (Unit* own = me->GetOwner())
                    me->GetMotionMaster()->MoveFollow(own, 0, 0);
            }

            void EnterCombat(Unit* /*who*/){}

            void UpdateAI(uint32 const diff)
            {
                if (me->HasAura(20372))
                {
                    if (hearts <= diff)
                    {
                        me->RemoveAurasDueToSpell(20372);
                        hearts = 15000;
                    } hearts -= diff;
                }
            }

            void ReceiveEmote(Player* player, uint32 emote)
            {
                me->HandleEmoteCommand(emote);
                Unit* own = me->GetOwner();
                if (!own || own->GetTypeId() != TYPEID_PLAYER || CAST_PLR(own)->GetTeam() != player->GetTeam())
                    return;
                if (emote == TEXT_EMOTE_KISS)
                {
                    std::string whisp = "";
                    switch (rand() % 8)
                    {
                        case 0:
                            whisp.append(SAY_RANDOM_MOJO0);
                            break;
                        case 1:
                            whisp.append(SAY_RANDOM_MOJO1);
                            break;
                        case 2:
                            whisp.append(SAY_RANDOM_MOJO2);
                            break;
                        case 3:
                            whisp.append(SAY_RANDOM_MOJO3);
                            break;
                        case 4:
                            whisp.append(SAY_RANDOM_MOJO4);
                            break;
                        case 5:
                            whisp.append(SAY_RANDOM_MOJO5);
                            break;
                        case 6:
                            whisp.append(SAY_RANDOM_MOJO6a);
                            whisp.append(player->GetName());
                            whisp.append(SAY_RANDOM_MOJO6b);
                            break;
                        case 7:
                            whisp.append(SAY_RANDOM_MOJO7);
                            break;
                    }

                    me->MonsterWhisper(whisp.c_str(), player->GetGUID());
                    if (victimGUID)
                        if (Player* victim = Unit::GetPlayer(*me, victimGUID))
                            victim->RemoveAura(43906);//remove polymorph frog thing
                    me->AddAura(43906, player);//add polymorph frog thing
                    victimGUID = player->GetGUID();
                    DoCast(me, 20372, true);//tag.hearts
                    me->GetMotionMaster()->MoveFollow(player, 0, 0);
                    hearts = 15000;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_mojoAI(creature);
        }
};

class npc_mirror_image : public CreatureScript
{
    public:
        npc_mirror_image() : CreatureScript("npc_mirror_image") { }

        enum eSpells
        {
            SPELL_MAGE_FROSTBOLT    = 59638,
            SPELL_MAGE_FIREBALL     = 133,
            SPELL_MAGE_ARCANE_BLAST = 30451,
            SPELL_MAGE_GLYPH        = 63093,
            SPELL_INITIALIZE_IMAGES = 102284,
            SPELL_CLONE_CASTER      = 102288
        };

        struct npc_mirror_imageAI : CasterAI
        {
            npc_mirror_imageAI(Creature* creature) :
            CasterAI(creature)
            {
            }

            void IsSummonedBy(Unit* p_Owner) override
            {
                if (!p_Owner || p_Owner->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                if (!me->HasUnitState(UnitState::UNIT_STATE_FOLLOW))
                {
                    me->GetMotionMaster()->Clear(false);
                    me->GetMotionMaster()->MoveFollow(p_Owner, PET_FOLLOW_DIST, me->GetFollowAngle(), MovementSlot::MOTION_SLOT_ACTIVE);
                }

                me->SetMaxPower(me->getPowerType(), p_Owner->GetMaxPower(me->getPowerType()));
                me->SetPower(me->getPowerType(), p_Owner->GetPower(me->getPowerType()));
                me->SetMaxHealth(p_Owner->GetMaxHealth());
                me->SetHealth(p_Owner->GetHealth());
                me->SetReactState(ReactStates::REACT_DEFENSIVE);

                // Inherit Master's Threat List
                me->CastSpell(p_Owner, 58838, true);

                // here mirror image casts on summoner spell (not present in client dbc) 49866
                // here should be auras (not present in client dbc): 35657, 35658, 35659, 35660 selfcasted by mirror images (stats related?)

                for (uint32 l_AttackType = 0; l_AttackType < WeaponAttackType::MaxAttack; l_AttackType++)
                {
                    WeaponAttackType l_AttackTypeEnum = static_cast<WeaponAttackType>(l_AttackType);
                    me->SetBaseWeaponDamage(l_AttackTypeEnum, WeaponDamageRange::MAXDAMAGE, p_Owner->GetWeaponDamageRange(l_AttackTypeEnum, WeaponDamageRange::MAXDAMAGE));
                    me->SetBaseWeaponDamage(l_AttackTypeEnum, WeaponDamageRange::MINDAMAGE, p_Owner->GetWeaponDamageRange(l_AttackTypeEnum, WeaponDamageRange::MINDAMAGE));
                }

                me->UpdateAttackPowerAndDamage();
            }

            void EnterCombat(Unit* /*p_Who*/) override
            {
                if (!me->GetOwner())
                    return;

                Player* l_Owner = me->GetOwner()->ToPlayer();
                if (!l_Owner)
                    return;

                eSpells l_Spell = eSpells::SPELL_MAGE_FROSTBOLT;
                if (l_Owner->HasAura(SPELL_MAGE_GLYPH))
                {
                    switch (l_Owner->GetSpecializationId(l_Owner->GetActiveSpec()))
                    {
                        case SPEC_MAGE_ARCANE:
                            l_Spell = eSpells::SPELL_MAGE_ARCANE_BLAST;
                            break;

                        case SPEC_MAGE_FIRE:
                            l_Spell = eSpells::SPELL_MAGE_FIREBALL;
                            break;
                    }
                }

                events.ScheduleEvent(l_Spell, 0); ///< Schedule cast
                me->GetMotionMaster()->Clear(false);
            }

            void EnterEvadeMode() override
            {
                if (me->IsInEvadeMode() || !me->isAlive())
                    return;

                Unit* l_Owner = me->GetOwner();

                me->CombatStop(true);
                if (l_Owner && !me->HasUnitState(UNIT_STATE_FOLLOW))
                {
                    me->GetMotionMaster()->Clear(false);
                    me->GetMotionMaster()->MoveFollow(l_Owner, PET_FOLLOW_DIST, me->GetFollowAngle(), MovementSlot::MOTION_SLOT_ACTIVE);
                }
            }

            void Reset() override
            {
                if (Unit* l_Owner = me->GetOwner())
                {
                    l_Owner->CastSpell(me, SPELL_INITIALIZE_IMAGES, true);
                    l_Owner->CastSpell(me, SPELL_CLONE_CASTER, true);
                }
            }

            bool CanAIAttack(Unit const* l_Target) const override
            {
                /// Am I supposed to attack this target? (ie. do not attack polymorphed target)
                return l_Target && !l_Target->HasAuraType(SPELL_AURA_MOD_CONFUSE);
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                events.Update(p_Diff);

                Unit* l_Victim = me->getVictim();
                Player* l_Owner = me->GetOwner()->ToPlayer();
                if (!l_Owner)
                    return;

                if (l_Victim)
                {
                    if (CanAIAttack(l_Victim))
                    {
                        /// If not already casting, cast! ("I'm a cast machine")
                        if (!me->HasUnitState(UNIT_STATE_CASTING))
                        {
                            if (uint32 l_SpellId = events.ExecuteEvent())
                            {
                                me->CastSpell(l_Victim, l_SpellId, false);
                                uint32 l_CastTime = me->GetCurrentSpellCastTime(l_SpellId);
                                events.ScheduleEvent(l_SpellId, 0);
                            }
                        }
                    }
                    else
                    {
                        /// My victim has changed state, I shouldn't attack it anymore
                        if (me->HasUnitState(UNIT_STATE_CASTING))
                            me->CastStop();

                        me->AI()->EnterEvadeMode();
                    }
                }
                else
                {
                    /// Let's choose a new target
                    Unit* l_Target = me->SelectVictim();
                    if (!l_Target)
                    {
                        /// No target? Let's see if our owner has a better target for us
                        if (Unit* l_Owner = me->GetOwner())
                        {
                            Unit* l_OwnerVictim = nullptr;
                            if (Player* l_Player = l_Owner->ToPlayer())
                                l_OwnerVictim = l_Player->GetSelectedUnit();
                            else
                                l_OwnerVictim = l_Owner->getVictim();

                            if (l_OwnerVictim && me->canCreatureAttack(l_OwnerVictim))
                                l_Target = l_OwnerVictim;
                        }
                    }

                    if (l_Target)
                        me->AI()->AttackStart(l_Target);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_mirror_imageAI(creature);
        }
};

class npc_ebon_gargoyle : public CreatureScript
{
    public:
        npc_ebon_gargoyle() : CreatureScript("npc_ebon_gargoyle") { }

        struct npc_ebon_gargoyleAI : CasterAI
        {
            npc_ebon_gargoyleAI(Creature* creature) : CasterAI(creature) {}

            uint32 despawnTimer;

            void InitializeAI()
            {
                CasterAI::InitializeAI();
                uint64 ownerGuid = me->GetOwnerGUID();
                if (!ownerGuid)
                    return;
                // Not needed to be despawned now
                despawnTimer = 0;
            }

            void DoSearchTarget()
            {
                Unit* owner = me->GetOwner();
                if (!owner)
                    return;

                // Find victim of Summon Gargoyle spell
                std::list<Unit*> targets;
                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck u_check(me, me, 30);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> searcher(me, targets, u_check);
                me->VisitNearbyObject(30, searcher);
                for (std::list<Unit*>::const_iterator iter = targets.begin(); iter != targets.end(); ++iter)
                    if ((*iter)->GetAura(49206, owner->GetGUID()))
                    {
                        me->Attack((*iter), false);
                        break;
                    }
                    else
                    {
                        owner->AddAura(49206, (*iter));
                        break;
                    }
            }

            void EnterEvadeMode() { }

            void JustDied(Unit* /*killer*/)
            {
                // Stop Feeding Gargoyle when it dies
                if (Unit* owner = me->GetOwner())
                    owner->RemoveAurasDueToSpell(50514);
            }

            void AttackStart(Unit *target)
            {
                if (target && !target->HasAura(49206))
                {
                    DoSearchTarget();
                    return;
                }

                AttackStartCaster(target, 30.0f);
            }

            // Fly away when dismissed
            void SpellHit(Unit* source, SpellInfo const* spell)
            {
                if (spell->Id != 50515 || !me->isAlive())
                    return;

                Unit* owner = me->GetOwner();

                if (!owner || owner != source)
                    return;

                // Stop Fighting
                me->ApplyModFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE, true);
                // Sanctuary
                me->CastSpell(me, 54661, true);
                me->SetReactState(REACT_PASSIVE);

                //! HACK: Creature's can't have MOVEMENTFLAG_FLYING
                // Fly Away
                me->AddUnitMovementFlag(MOVEMENTFLAG_CAN_FLY|MOVEMENTFLAG_ASCENDING|MOVEMENTFLAG_FLYING);
                me->SetSpeed(MOVE_FLIGHT, 0.75f, true);
                me->SetSpeed(MOVE_RUN, 0.75f, true);
                float x = me->GetPositionX() + 20 * std::cos(me->GetOrientation());
                float y = me->GetPositionY() + 20 * std::sin(me->GetOrientation());
                float z = me->GetPositionZ() + 40;
                me->GetMotionMaster()->Clear(false);
                me->GetMotionMaster()->MovePoint(0, x, y, z);

                // Despawn as soon as possible
                despawnTimer = 4 * IN_MILLISECONDS;
            }

            void UpdateAI(const uint32 diff)
            {
                if (despawnTimer > 0)
                {
                    if (despawnTimer > diff)
                        despawnTimer -= diff;
                    else
                        me->DespawnOrUnsummon();
                    return;
                }
                CasterAI::UpdateAI(diff);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_ebon_gargoyleAI(creature);
        }
};

/// Last Build 6.2.3
/// Lightwell - 31897 & 64571
class npc_lightwell : public CreatureScript
{
    public:
        npc_lightwell() : CreatureScript("npc_lightwell") { }

        struct npc_lightwellAI : public PassiveAI
        {
            npc_lightwellAI(Creature* p_Creature) : PassiveAI(p_Creature) { }

            enum eSpells
            {
                GlyphOfDeepWells  = 55673,
                ChargeAura        = 59907,
                LightWellHeal     = 60123,
                GlyphOfLightwell  = 126133,
                LightWellHealAura = 126154
            };

            uint64 m_OwnerGUID = 0;
            uint32 m_RenewTimer;

            void IsSummonedBy(Unit* p_Owner)
            {
                if (!p_Owner)
                    return;

                m_RenewTimer = 1000;

                if (p_Owner->IsPlayer())
                    m_OwnerGUID = p_Owner->GetGUID();

                me->SetMaxHealth(p_Owner->GetMaxHealth());
                me->SetHealth(p_Owner->GetHealth());

                if (Aura* l_Charges = me->AddAura(eSpells::ChargeAura, me))
                {
                    l_Charges->SetCharges(15);
                    l_Charges->GetEffect(EFFECT_0)->ChangeAmount(15);

                    if (p_Owner->HasAura(eSpells::GlyphOfDeepWells))
                    {
                        l_Charges->SetCharges(17);
                        l_Charges->GetEffect(EFFECT_0)->ChangeAmount(17);
                    }
                }
            }

            void EnterEvadeMode()
            {
                if (!me->isAlive())
                    return;

                me->DeleteThreatList();
                me->CombatStop(true);
                me->ResetPlayerDamageReq();
            }

            void UpdateAI(const uint32 diff)
            {
                if (m_RenewTimer)
                {
                    if (m_RenewTimer <= diff)
                    {
                        Unit* l_Owner = ObjectAccessor::FindUnit(m_OwnerGUID);
                        if (l_Owner != nullptr)
                        {
                            if (l_Owner->HasAura(eSpells::GlyphOfLightwell))
                                return;

                            if (Player* l_Player = l_Owner->ToPlayer())
                            {
                                std::list<Unit*> l_Party;
                                std::list<Unit*> l_TempList;
                                l_Player->GetRaidMembers(l_Party);

                                for (auto itr : l_Party)
                                {
                                    if (itr->GetHealthPct() >= 50.0f ||
                                        itr->GetDistance(me) >= 40.0f ||
                                        itr->HasAura(eSpells::LightWellHealAura))
                                        continue;

                                    l_TempList.push_back(itr);
                                }

                                for (auto itr : l_TempList)
                                {
                                    me->CastSpell(itr, eSpells::LightWellHeal, true);
                                    if (Aura* l_Charges = me->GetAura(eSpells::ChargeAura))
                                    {
                                        l_Charges->DropCharge();
                                        l_Charges->GetEffect(0)->ChangeAmount(l_Charges->GetCharges());
                                    }
                                }
                            }
                        }
                        m_RenewTimer = 1000;
                    }
                    else
                        m_RenewTimer -= diff;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_lightwellAI(p_Creature);
        }
};

enum eTrainingDummy
{
    NPC_ADVANCED_TARGET_DUMMY                  = 2674,
    NPC_TARGET_DUMMY                           = 2673
};

struct npc_training_dummyAI : Scripted_NoMovementAI
{
    npc_training_dummyAI(Creature* creature) : Scripted_NoMovementAI(creature)
    {
        entry = creature->GetEntry();
    }

    uint32 entry;
    uint32 resetTimer;
    uint32 despawnTimer;

    void Reset()
    {
        me->SetControlled(true, UNIT_STATE_STUNNED);//disable rotate
        me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);        // Immune to knock aways like blast wave
        me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);   // Immune to knock back effects like Whiplash

        resetTimer = 5000;
        despawnTimer = 15000;
    }

    void EnterEvadeMode()
    {
        if (!_EnterEvadeMode())
            return;

        Reset();
    }

    void MoveInLineOfSight(Unit* p_Who)
    {
        if (!me->IsWithinDistInMap(p_Who, 25.0f) && p_Who->isInCombat())
        {
            me->RemoveAllAurasByCaster(p_Who->GetGUID());
            me->getHostileRefManager().deleteReference(p_Who);
        }
    }

    void DamageTaken(Unit* doneBy, uint32& damage, SpellInfo const* p_SpellInfo)
    {
        resetTimer = 5000;
        damage = 0;

        me->SetInCombatWith(doneBy);
        doneBy->SetInCombatWith(me);
    }

    void EnterCombat(Unit* /*who*/)
    {
        if (entry != NPC_ADVANCED_TARGET_DUMMY && entry != NPC_TARGET_DUMMY)
            return;
    }

    void UpdateAI(uint32 const diff)
    {
        if (!UpdateVictim())
            return;

        if (!me->HasUnitState(UNIT_STATE_STUNNED))
            me->SetControlled(true, UNIT_STATE_STUNNED);//disable rotate

        if (entry != NPC_ADVANCED_TARGET_DUMMY && entry != NPC_TARGET_DUMMY)
        {
            if (resetTimer <= diff)
            {
                EnterEvadeMode();
                resetTimer = 5000;
            }
            else
                resetTimer -= diff;
            return;
        }
        else
        {
            if (despawnTimer <= diff)
                me->DespawnOrUnsummon();
            else
                despawnTimer -= diff;
        }
    }

};

class npc_training_dummy : public CreatureScript
{
    public:
        npc_training_dummy() : CreatureScript("npc_training_dummy") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_training_dummyAI(creature);
        }
};

class npc_pvp_training_dummy : public CreatureScript
{
    public:
        npc_pvp_training_dummy() : CreatureScript("npc_pvp_training_dummy") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_training_dummyAI(creature);
        }
};

/*######
# npc_wormhole
######*/

#define GOSSIP_ENGINEERING1   "Borean Tundra"
#define GOSSIP_ENGINEERING2   "Howling Fjord"
#define GOSSIP_ENGINEERING3   "Sholazar Basin"
#define GOSSIP_ENGINEERING4   "Icecrown"
#define GOSSIP_ENGINEERING5   "Storm Peaks"
#define GOSSIP_ENGINEERING6   "Underground..."

enum WormholeSpells
{
    SPELL_BOREAN_TUNDRA         = 67834,
    SPELL_SHOLAZAR_BASIN        = 67835,
    SPELL_ICECROWN              = 67836,
    SPELL_STORM_PEAKS           = 67837,
    SPELL_HOWLING_FJORD         = 67838,
    SPELL_UNDERGROUND           = 68081,

    TEXT_WORMHOLE               = 907,

    DATA_SHOW_UNDERGROUND       = 1
};

class npc_wormhole : public CreatureScript
{
    public:
        npc_wormhole() : CreatureScript("npc_wormhole") {}

        struct npc_wormholeAI : public PassiveAI
        {
            npc_wormholeAI(Creature* creature) : PassiveAI(creature) {}

            void InitializeAI()
            {
                _showUnderground = urand(0, 100) == 0; // Guessed value, it is really rare though
            }

            uint32 GetData(uint32 type)
            {
                return (type == DATA_SHOW_UNDERGROUND && _showUnderground) ? 1 : 0;
            }

        private:
            bool _showUnderground;
        };

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (creature->isSummon())
            {
                if (player == creature->ToTempSummon()->GetSummoner())
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ENGINEERING1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ENGINEERING2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ENGINEERING3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ENGINEERING4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ENGINEERING5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);

                    if (creature->AI()->GetData(DATA_SHOW_UNDERGROUND))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ENGINEERING6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);

                    player->PlayerTalkClass->SendGossipMenu(TEXT_WORMHOLE, creature->GetGUID());
                }
            }

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1: // Borean Tundra
                    player->CLOSE_GOSSIP_MENU();
                    creature->CastSpell(player, SPELL_BOREAN_TUNDRA, false);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 2: // Howling Fjord
                    player->CLOSE_GOSSIP_MENU();
                    creature->CastSpell(player, SPELL_HOWLING_FJORD, false);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 3: // Sholazar Basin
                    player->CLOSE_GOSSIP_MENU();
                    creature->CastSpell(player, SPELL_SHOLAZAR_BASIN, false);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 4: // Icecrown
                    player->CLOSE_GOSSIP_MENU();
                    creature->CastSpell(player, SPELL_ICECROWN, false);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 5: // Storm peaks
                    player->CLOSE_GOSSIP_MENU();
                    creature->CastSpell(player, SPELL_STORM_PEAKS, false);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 6: // Underground
                    player->CLOSE_GOSSIP_MENU();
                    creature->CastSpell(player, SPELL_UNDERGROUND, false);
                    break;
            }

            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_wormholeAI(creature);
        }
};

/*######
## npc_pet_trainer
######*/

enum ePetTrainer
{
    TEXT_ISHUNTER               = 5838,
    TEXT_NOTHUNTER              = 5839,
    TEXT_PETINFO                = 13474,
    TEXT_CONFIRM                = 7722
};

#define GOSSIP_PET1             "How do I train my pet?"
#define GOSSIP_PET2             "I wish to untrain my pet."
#define GOSSIP_PET_CONFIRM      "Yes, please do."

class npc_pet_trainer : public CreatureScript
{
    public:
        npc_pet_trainer() : CreatureScript("npc_pet_trainer") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (creature->isQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (player->getClass() == CLASS_HUNTER)
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_PET1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                if (player->GetPet() && player->GetPet()->getPetType() == HUNTER_PET)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_PET2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

                player->PlayerTalkClass->SendGossipMenu(TEXT_ISHUNTER, creature->GetGUID());
                return true;
            }
            player->PlayerTalkClass->SendGossipMenu(TEXT_NOTHUNTER, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                    player->PlayerTalkClass->SendGossipMenu(TEXT_PETINFO, creature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF + 2:
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_PET_CONFIRM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                        player->PlayerTalkClass->SendGossipMenu(TEXT_CONFIRM, creature->GetGUID());
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 3:
                    {
                        player->CLOSE_GOSSIP_MENU();
                    }
                    break;
            }
            return true;
        }
};

/*######
## npc_locksmith
######*/

enum eLockSmith
{
    QUEST_HOW_TO_BRAKE_IN_TO_THE_ARCATRAZ = 10704,
    QUEST_DARK_IRON_LEGACY                = 3802,
    QUEST_THE_KEY_TO_SCHOLOMANCE_A        = 5505,
    QUEST_THE_KEY_TO_SCHOLOMANCE_H        = 5511,
    QUEST_HOTTER_THAN_HELL_A              = 10758,
    QUEST_HOTTER_THAN_HELL_H              = 10764,
    QUEST_RETURN_TO_KHAGDAR               = 9837,
    QUEST_CONTAINMENT                     = 13159,
    QUEST_ETERNAL_VIGILANCE               = 11011,
    QUEST_KEY_TO_THE_FOCUSING_IRIS        = 13372,
    QUEST_HC_KEY_TO_THE_FOCUSING_IRIS     = 13375,

    ITEM_ARCATRAZ_KEY                     = 31084,
    ITEM_SHADOWFORGE_KEY                  = 11000,
    ITEM_SKELETON_KEY                     = 13704,
    ITEM_SHATTERED_HALLS_KEY              = 28395,
    ITEM_THE_MASTERS_KEY                  = 24490,
    ITEM_VIOLET_HOLD_KEY                  = 42482,
    ITEM_ESSENCE_INFUSED_MOONSTONE        = 32449,
    ITEM_KEY_TO_THE_FOCUSING_IRIS         = 44582,
    ITEM_HC_KEY_TO_THE_FOCUSING_IRIS      = 44581,

    SPELL_ARCATRAZ_KEY                    = 54881,
    SPELL_SHADOWFORGE_KEY                 = 54882,
    SPELL_SKELETON_KEY                    = 54883,
    SPELL_SHATTERED_HALLS_KEY             = 54884,
    SPELL_THE_MASTERS_KEY                 = 54885,
    SPELL_VIOLET_HOLD_KEY                 = 67253,
    SPELL_ESSENCE_INFUSED_MOONSTONE       = 40173
};

#define GOSSIP_LOST_ARCATRAZ_KEY                "I've lost my key to the Arcatraz."
#define GOSSIP_LOST_SHADOWFORGE_KEY             "I've lost my key to the Blackrock Depths."
#define GOSSIP_LOST_SKELETON_KEY                "I've lost my key to the Scholomance."
#define GOSSIP_LOST_SHATTERED_HALLS_KEY         "I've lost my key to the Shattered Halls."
#define GOSSIP_LOST_THE_MASTERS_KEY             "I've lost my key to the Karazhan."
#define GOSSIP_LOST_VIOLET_HOLD_KEY             "I've lost my key to the Violet Hold."
#define GOSSIP_LOST_ESSENCE_INFUSED_MOONSTONE   "I've lost my Essence-Infused Moonstone."
#define GOSSIP_LOST_KEY_TO_THE_FOCUSING_IRIS    "I've lost my Key to the Focusing Iris."
#define GOSSIP_LOST_HC_KEY_TO_THE_FOCUSING_IRIS "I've lost my Heroic Key to the Focusing Iris."

class npc_locksmith : public CreatureScript
{
    public:
        npc_locksmith() : CreatureScript("npc_locksmith") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            // Arcatraz Key
            if (player->GetQuestRewardStatus(QUEST_HOW_TO_BRAKE_IN_TO_THE_ARCATRAZ) && !player->HasItemCount(ITEM_ARCATRAZ_KEY, 1, true))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOST_ARCATRAZ_KEY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            // Shadowforge Key
            if (player->GetQuestRewardStatus(QUEST_DARK_IRON_LEGACY) && !player->HasItemCount(ITEM_SHADOWFORGE_KEY, 1, true))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOST_SHADOWFORGE_KEY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

            // Skeleton Key
            if ((player->GetQuestRewardStatus(QUEST_THE_KEY_TO_SCHOLOMANCE_A) || player->GetQuestRewardStatus(QUEST_THE_KEY_TO_SCHOLOMANCE_H)) &&
                !player->HasItemCount(ITEM_SKELETON_KEY, 1, true))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOST_SKELETON_KEY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

            // Shatered Halls Key
            if ((player->GetQuestRewardStatus(QUEST_HOTTER_THAN_HELL_A) || player->GetQuestRewardStatus(QUEST_HOTTER_THAN_HELL_H)) &&
                !player->HasItemCount(ITEM_SHATTERED_HALLS_KEY, 1, true))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOST_SHATTERED_HALLS_KEY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);

            // Master's Key
            if (player->GetQuestRewardStatus(QUEST_RETURN_TO_KHAGDAR) && !player->HasItemCount(ITEM_THE_MASTERS_KEY, 1, true))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOST_THE_MASTERS_KEY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);

            // Violet Hold Key
            if (player->GetQuestRewardStatus(QUEST_CONTAINMENT) && !player->HasItemCount(ITEM_VIOLET_HOLD_KEY, 1, true))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOST_VIOLET_HOLD_KEY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);

            // Essence-Infused Moonstone
            if (player->GetQuestRewardStatus(QUEST_ETERNAL_VIGILANCE) && !player->HasItemCount(ITEM_ESSENCE_INFUSED_MOONSTONE, 1, true))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOST_ESSENCE_INFUSED_MOONSTONE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);

            // Key to the Focusing Iris
            if (player->GetQuestRewardStatus(QUEST_KEY_TO_THE_FOCUSING_IRIS) && !player->HasItemCount(ITEM_KEY_TO_THE_FOCUSING_IRIS, 1, true))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOST_KEY_TO_THE_FOCUSING_IRIS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);

            // Heroic Key to the Focusing Iris
            if (player->GetQuestRewardStatus(QUEST_HC_KEY_TO_THE_FOCUSING_IRIS) && !player->HasItemCount(ITEM_HC_KEY_TO_THE_FOCUSING_IRIS, 1, true))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOST_HC_KEY_TO_THE_FOCUSING_IRIS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                    player->CLOSE_GOSSIP_MENU();
                    player->CastSpell(player, SPELL_ARCATRAZ_KEY, false);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 2:
                    player->CLOSE_GOSSIP_MENU();
                    player->CastSpell(player, SPELL_SHADOWFORGE_KEY, false);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 3:
                    player->CLOSE_GOSSIP_MENU();
                    player->CastSpell(player, SPELL_SKELETON_KEY, false);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 4:
                    player->CLOSE_GOSSIP_MENU();
                    player->CastSpell(player, SPELL_SHATTERED_HALLS_KEY, false);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 5:
                    player->CLOSE_GOSSIP_MENU();
                    player->CastSpell(player, SPELL_THE_MASTERS_KEY, false);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 6:
                    player->CLOSE_GOSSIP_MENU();
                    player->CastSpell(player, SPELL_VIOLET_HOLD_KEY, false);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 7:
                    player->CLOSE_GOSSIP_MENU();
                    player->CastSpell(player, SPELL_ESSENCE_INFUSED_MOONSTONE, false);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 8:
                    player->CLOSE_GOSSIP_MENU();
                    player->AddItem(ITEM_KEY_TO_THE_FOCUSING_IRIS,1);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 9:
                    player->CLOSE_GOSSIP_MENU();
                    player->AddItem(ITEM_HC_KEY_TO_THE_FOCUSING_IRIS,1);
                    break;
            }
            return true;
        }
};

/*######
## npc_experience
######*/

#define EXP_COST                100000 //10 00 00 copper (10golds)
#define GOSSIP_TEXT_EXP         14736
#define GOSSIP_XP_OFF           "I no longer wish to gain experience."
#define GOSSIP_XP_ON            "I wish to start gaining experience again."

class npc_experience : public CreatureScript
{
    public:
        npc_experience() : CreatureScript("npc_experience") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_XP_OFF, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_XP_ON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->PlayerTalkClass->SendGossipMenu(GOSSIP_TEXT_EXP, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            bool noXPGain = player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN);
            bool doSwitch = false;

            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1://xp off
                    {
                        if (!noXPGain)//does gain xp
                            doSwitch = true;//switch to don't gain xp
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 2://xp on
                    {
                        if (noXPGain)//doesn't gain xp
                            doSwitch = true;//switch to gain xp
                    }
                    break;
            }
            if (doSwitch)
            {
                if (!player->HasEnoughMoney(uint64(EXP_COST)))
                    player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, 0, 0, 0);
                else if (noXPGain)
                {
                    player->ModifyMoney(-int64(EXP_COST));
                    player->RemoveFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN);
                }
                else if (!noXPGain)
                {
                    player->ModifyMoney(-EXP_COST);
                    player->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN);
                }
            }
            player->PlayerTalkClass->SendCloseGossip();
            return true;
        }
};

enum Fireworks
{
    NPC_OMEN                = 15467,
    NPC_MINION_OF_OMEN      = 15466,
    NPC_FIREWORK_BLUE       = 15879,
    NPC_FIREWORK_GREEN      = 15880,
    NPC_FIREWORK_PURPLE     = 15881,
    NPC_FIREWORK_RED        = 15882,
    NPC_FIREWORK_YELLOW     = 15883,
    NPC_FIREWORK_WHITE      = 15884,
    NPC_FIREWORK_BIG_BLUE   = 15885,
    NPC_FIREWORK_BIG_GREEN  = 15886,
    NPC_FIREWORK_BIG_PURPLE = 15887,
    NPC_FIREWORK_BIG_RED    = 15888,
    NPC_FIREWORK_BIG_YELLOW = 15889,
    NPC_FIREWORK_BIG_WHITE  = 15890,

    NPC_CLUSTER_BLUE        = 15872,
    NPC_CLUSTER_RED         = 15873,
    NPC_CLUSTER_GREEN       = 15874,
    NPC_CLUSTER_PURPLE      = 15875,
    NPC_CLUSTER_WHITE       = 15876,
    NPC_CLUSTER_YELLOW      = 15877,
    NPC_CLUSTER_BIG_BLUE    = 15911,
    NPC_CLUSTER_BIG_GREEN   = 15912,
    NPC_CLUSTER_BIG_PURPLE  = 15913,
    NPC_CLUSTER_BIG_RED     = 15914,
    NPC_CLUSTER_BIG_WHITE   = 15915,
    NPC_CLUSTER_BIG_YELLOW  = 15916,
    NPC_CLUSTER_ELUNE       = 15918,

    GO_FIREWORK_LAUNCHER_1  = 180771,
    GO_FIREWORK_LAUNCHER_2  = 180868,
    GO_FIREWORK_LAUNCHER_3  = 180850,
    GO_CLUSTER_LAUNCHER_1   = 180772,
    GO_CLUSTER_LAUNCHER_2   = 180859,
    GO_CLUSTER_LAUNCHER_3   = 180869,
    GO_CLUSTER_LAUNCHER_4   = 180874,

    SPELL_ROCKET_BLUE       = 26344,
    SPELL_ROCKET_GREEN      = 26345,
    SPELL_ROCKET_PURPLE     = 26346,
    SPELL_ROCKET_RED        = 26347,
    SPELL_ROCKET_WHITE      = 26348,
    SPELL_ROCKET_YELLOW     = 26349,
    SPELL_ROCKET_BIG_BLUE   = 26351,
    SPELL_ROCKET_BIG_GREEN  = 26352,
    SPELL_ROCKET_BIG_PURPLE = 26353,
    SPELL_ROCKET_BIG_RED    = 26354,
    SPELL_ROCKET_BIG_WHITE  = 26355,
    SPELL_ROCKET_BIG_YELLOW = 26356,
    SPELL_LUNAR_FORTUNE     = 26522,

    ANIM_GO_LAUNCH_FIREWORK = 3,
    ZONE_MOONGLADE          = 493
};

Position omenSummonPos = {7558.993f, -2839.999f, 450.0214f, 4.46f};

class npc_firework : public CreatureScript
{
    public:
        npc_firework() : CreatureScript("npc_firework") { }

        struct npc_fireworkAI : public ScriptedAI
        {
            npc_fireworkAI(Creature* creature) : ScriptedAI(creature) {}

            bool isCluster()
            {
                switch (me->GetEntry())
                {
                    case NPC_FIREWORK_BLUE:
                    case NPC_FIREWORK_GREEN:
                    case NPC_FIREWORK_PURPLE:
                    case NPC_FIREWORK_RED:
                    case NPC_FIREWORK_YELLOW:
                    case NPC_FIREWORK_WHITE:
                    case NPC_FIREWORK_BIG_BLUE:
                    case NPC_FIREWORK_BIG_GREEN:
                    case NPC_FIREWORK_BIG_PURPLE:
                    case NPC_FIREWORK_BIG_RED:
                    case NPC_FIREWORK_BIG_YELLOW:
                    case NPC_FIREWORK_BIG_WHITE:
                        return false;
                    case NPC_CLUSTER_BLUE:
                    case NPC_CLUSTER_GREEN:
                    case NPC_CLUSTER_PURPLE:
                    case NPC_CLUSTER_RED:
                    case NPC_CLUSTER_YELLOW:
                    case NPC_CLUSTER_WHITE:
                    case NPC_CLUSTER_BIG_BLUE:
                    case NPC_CLUSTER_BIG_GREEN:
                    case NPC_CLUSTER_BIG_PURPLE:
                    case NPC_CLUSTER_BIG_RED:
                    case NPC_CLUSTER_BIG_YELLOW:
                    case NPC_CLUSTER_BIG_WHITE:
                    case NPC_CLUSTER_ELUNE:
                    default:
                        return true;
                }
            }

            GameObject* FindNearestLauncher()
            {
                GameObject* launcher = NULL;

                if (isCluster())
                {
                    GameObject* launcher1 = GetClosestGameObjectWithEntry(me, GO_CLUSTER_LAUNCHER_1, 0.5f);
                    GameObject* launcher2 = GetClosestGameObjectWithEntry(me, GO_CLUSTER_LAUNCHER_2, 0.5f);
                    GameObject* launcher3 = GetClosestGameObjectWithEntry(me, GO_CLUSTER_LAUNCHER_3, 0.5f);
                    GameObject* launcher4 = GetClosestGameObjectWithEntry(me, GO_CLUSTER_LAUNCHER_4, 0.5f);

                    if (launcher1)
                        launcher = launcher1;
                    else if (launcher2)
                        launcher = launcher2;
                    else if (launcher3)
                        launcher = launcher3;
                    else if (launcher4)
                        launcher = launcher4;
                }
                else
                {
                    GameObject* launcher1 = GetClosestGameObjectWithEntry(me, GO_FIREWORK_LAUNCHER_1, 0.5f);
                    GameObject* launcher2 = GetClosestGameObjectWithEntry(me, GO_FIREWORK_LAUNCHER_2, 0.5f);
                    GameObject* launcher3 = GetClosestGameObjectWithEntry(me, GO_FIREWORK_LAUNCHER_3, 0.5f);

                    if (launcher1)
                        launcher = launcher1;
                    else if (launcher2)
                        launcher = launcher2;
                    else if (launcher3)
                        launcher = launcher3;
                }

                return launcher;
            }

            uint32 GetFireworkSpell(uint32 entry)
            {
                switch (entry)
                {
                    case NPC_FIREWORK_BLUE:
                        return SPELL_ROCKET_BLUE;
                    case NPC_FIREWORK_GREEN:
                        return SPELL_ROCKET_GREEN;
                    case NPC_FIREWORK_PURPLE:
                        return SPELL_ROCKET_PURPLE;
                    case NPC_FIREWORK_RED:
                        return SPELL_ROCKET_RED;
                    case NPC_FIREWORK_YELLOW:
                        return SPELL_ROCKET_YELLOW;
                    case NPC_FIREWORK_WHITE:
                        return SPELL_ROCKET_WHITE;
                    case NPC_FIREWORK_BIG_BLUE:
                        return SPELL_ROCKET_BIG_BLUE;
                    case NPC_FIREWORK_BIG_GREEN:
                        return SPELL_ROCKET_BIG_GREEN;
                    case NPC_FIREWORK_BIG_PURPLE:
                        return SPELL_ROCKET_BIG_PURPLE;
                    case NPC_FIREWORK_BIG_RED:
                        return SPELL_ROCKET_BIG_RED;
                    case NPC_FIREWORK_BIG_YELLOW:
                        return SPELL_ROCKET_BIG_YELLOW;
                    case NPC_FIREWORK_BIG_WHITE:
                        return SPELL_ROCKET_BIG_WHITE;
                    default:
                        return 0;
                }
            }

            uint32 GetFireworkGameObjectId()
            {
                uint32 spellId = 0;

                switch (me->GetEntry())
                {
                    case NPC_CLUSTER_BLUE:
                        spellId = GetFireworkSpell(NPC_FIREWORK_BLUE);
                        break;
                    case NPC_CLUSTER_GREEN:
                        spellId = GetFireworkSpell(NPC_FIREWORK_GREEN);
                        break;
                    case NPC_CLUSTER_PURPLE:
                        spellId = GetFireworkSpell(NPC_FIREWORK_PURPLE);
                        break;
                    case NPC_CLUSTER_RED:
                        spellId = GetFireworkSpell(NPC_FIREWORK_RED);
                        break;
                    case NPC_CLUSTER_YELLOW:
                        spellId = GetFireworkSpell(NPC_FIREWORK_YELLOW);
                        break;
                    case NPC_CLUSTER_WHITE:
                        spellId = GetFireworkSpell(NPC_FIREWORK_WHITE);
                        break;
                    case NPC_CLUSTER_BIG_BLUE:
                        spellId = GetFireworkSpell(NPC_FIREWORK_BIG_BLUE);
                        break;
                    case NPC_CLUSTER_BIG_GREEN:
                        spellId = GetFireworkSpell(NPC_FIREWORK_BIG_GREEN);
                        break;
                    case NPC_CLUSTER_BIG_PURPLE:
                        spellId = GetFireworkSpell(NPC_FIREWORK_BIG_PURPLE);
                        break;
                    case NPC_CLUSTER_BIG_RED:
                        spellId = GetFireworkSpell(NPC_FIREWORK_BIG_RED);
                        break;
                    case NPC_CLUSTER_BIG_YELLOW:
                        spellId = GetFireworkSpell(NPC_FIREWORK_BIG_YELLOW);
                        break;
                    case NPC_CLUSTER_BIG_WHITE:
                        spellId = GetFireworkSpell(NPC_FIREWORK_BIG_WHITE);
                        break;
                    case NPC_CLUSTER_ELUNE:
                        spellId = GetFireworkSpell(urand(NPC_FIREWORK_BLUE, NPC_FIREWORK_WHITE));
                        break;
                }

                const SpellInfo* spellInfo = sSpellMgr->GetSpellInfo(spellId);

                if (spellInfo && spellInfo->Effects[0].Effect == SPELL_EFFECT_SUMMON_OBJECT_WILD)
                    return spellInfo->Effects[0].MiscValue;

                return 0;
            }

            void Reset()
            {
                if (GameObject* launcher = FindNearestLauncher())
                {
                    launcher->SendCustomAnim(ANIM_GO_LAUNCH_FIREWORK);
                    me->SetOrientation(launcher->GetOrientation() + M_PI/2);
                }
                else
                    return;

                if (isCluster())
                {
                    // Check if we are near Elune'ara lake south, if so try to summon Omen or a minion
                    if (me->GetZoneId() == ZONE_MOONGLADE)
                    {
                        if (!me->FindNearestCreature(NPC_OMEN, 100.0f, false) && me->GetDistance2d(omenSummonPos.GetPositionX(), omenSummonPos.GetPositionY()) <= 100.0f)
                        {
                            switch (urand(0,9))
                            {
                                case 0:
                                case 1:
                                case 2:
                                case 3:
                                    if (Creature* minion = me->SummonCreature(NPC_MINION_OF_OMEN, me->GetPositionX()+frand(-5.0f, 5.0f), me->GetPositionY()+frand(-5.0f, 5.0f), me->GetPositionZ(), 0.0f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 20000))
                                        minion->AI()->AttackStart(me->SelectNearestPlayer(20.0f));
                                    break;
                                case 9:
                                    me->SummonCreature(NPC_OMEN, omenSummonPos);
                                    break;
                            }
                        }
                    }
                    if (me->GetEntry() == NPC_CLUSTER_ELUNE)
                        DoCast(SPELL_LUNAR_FORTUNE);

                    float displacement = 0.7f;
                    for (uint8 i = 0; i < 4; i++)
                        me->SummonGameObject(GetFireworkGameObjectId(), me->GetPositionX() + (i%2 == 0 ? displacement : -displacement), me->GetPositionY() + (i > 1 ? displacement : -displacement), me->GetPositionZ() + 4.0f, me->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 1);
                }
                else
                    //me->CastSpell(me, GetFireworkSpell(me->GetEntry()), true);
                    me->CastSpell(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), GetFireworkSpell(me->GetEntry()), true);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_fireworkAI(creature);
        }
};

/*#####
# npc_spring_rabbit
#####*/

enum rabbitSpells
{
    SPELL_SPRING_FLING          = 61875,
    SPELL_SPRING_RABBIT_JUMP    = 61724,
    SPELL_SPRING_RABBIT_WANDER  = 61726,
    SPELL_SUMMON_BABY_BUNNY     = 61727,
    SPELL_SPRING_RABBIT_IN_LOVE = 61728,
    NPC_SPRING_RABBIT           = 32791
};

class npc_spring_rabbit : public CreatureScript
{
    public:
        npc_spring_rabbit() : CreatureScript("npc_spring_rabbit") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_spring_rabbitAI(creature);
        }

        struct npc_spring_rabbitAI : public ScriptedAI
        {
            npc_spring_rabbitAI(Creature* creature) : ScriptedAI(creature) { }

            bool inLove;
            uint32 jumpTimer;
            uint32 bunnyTimer;
            uint32 searchTimer;
            uint64 rabbitGUID;

            void Reset()
            {
                inLove = false;
                rabbitGUID = 0;
                jumpTimer = urand(5000, 10000);
                bunnyTimer = urand(10000, 20000);
                searchTimer = urand(5000, 10000);
                if (Unit* owner = me->GetOwner())
                    me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
            }

            void EnterCombat(Unit * /*who*/) { }

            void DoAction(const int32 /*param*/)
            {
                inLove = true;
                if (Unit* owner = me->GetOwner())
                    owner->CastSpell(owner, SPELL_SPRING_FLING, true);
            }

            void UpdateAI(const uint32 diff)
            {
                if (inLove)
                {
                    if (jumpTimer <= diff)
                    {
                        if (Unit* rabbit = Unit::GetUnit(*me, rabbitGUID))
                            DoCast(rabbit, SPELL_SPRING_RABBIT_JUMP);
                        jumpTimer = urand(5000, 10000);
                    } else jumpTimer -= diff;

                    if (bunnyTimer <= diff)
                    {
                        DoCast(SPELL_SUMMON_BABY_BUNNY);
                        bunnyTimer = urand(20000, 40000);
                    } else bunnyTimer -= diff;
                }
                else
                {
                    if (searchTimer <= diff)
                    {
                        if (Creature* rabbit = me->FindNearestCreature(NPC_SPRING_RABBIT, 10.0f))
                        {
                            if (rabbit == me || rabbit->HasAura(SPELL_SPRING_RABBIT_IN_LOVE))
                                return;

                            me->AddAura(SPELL_SPRING_RABBIT_IN_LOVE, me);
                            DoAction(1);
                            rabbit->AddAura(SPELL_SPRING_RABBIT_IN_LOVE, rabbit);
                            rabbit->AI()->DoAction(1);
                            rabbit->CastSpell(rabbit, SPELL_SPRING_RABBIT_JUMP, true);
                            rabbitGUID = rabbit->GetGUID();
                        }
                        searchTimer = urand(5000, 10000);
                    } else searchTimer -= diff;
                }
            }
        };
};

/*######
## npc_generic_harpoon_cannon
######*/

class npc_generic_harpoon_cannon : public CreatureScript
{
    public:
        npc_generic_harpoon_cannon() : CreatureScript("npc_generic_harpoon_cannon") { }

        struct npc_generic_harpoon_cannonAI : public ScriptedAI
        {
            npc_generic_harpoon_cannonAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                me->SetUnitMovementFlags(MOVEMENTFLAG_ROOT);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_generic_harpoon_cannonAI(creature);
        }
};

/// Toran <Experience Rate Master> - 159753
class npc_rate_xp_modifier : public CreatureScript
{
    public:
        npc_rate_xp_modifier() : CreatureScript("npc_rate_xp_modifier") { }

        enum eOptions
        {
            Rate1,
            Rate3,
            Rate5,
            OriginalRate
        };

        struct npc_rate_xp_modifierAI : public ScriptedAI
        {
            npc_rate_xp_modifierAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void sGossipSelect(Player* p_Player, uint32 /*p_MenuID*/, uint32 p_Action) override
            {
                switch (p_Action)
                {
                    case eOptions::Rate1:
                        p_Player->SetPersonnalXpRate(1.0f);
                        break;
                    case eOptions::Rate3:
                        p_Player->SetPersonnalXpRate(3.0f);
                        break;
                    case eOptions::Rate5:
                        p_Player->SetPersonnalXpRate(5.0f);
                        break;
                    case eOptions::OriginalRate:
                        p_Player->SetPersonnalXpRate(sWorld->getRate(RATE_XP_KILL));
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_rate_xp_modifierAI(p_Creature);
        }
};

/// Send current rate XP at login
class RatesXPWarner : public PlayerScript
{
    public:
        RatesXPWarner() : PlayerScript("RatesXPWarner") { }

        void OnLogin(Player* p_Player)
        {
            uint32 l_Rate = p_Player->GetPersonnalXpRate();

            ChatHandler(p_Player).PSendSysMessage(TrinityStrings::CharXPRateWarn, l_Rate);
        }
};

/*######
# npc_demoralizing_banner
######*/

class npc_demoralizing_banner : public CreatureScript
{
    public:
        npc_demoralizing_banner() : CreatureScript("npc_demoralizing_banner") { }

        struct npc_demoralizing_bannerAI : public ScriptedAI
        {
            uint32 demoralizingTimer;

            npc_demoralizing_bannerAI(Creature* creature) : ScriptedAI(creature)
            {
                demoralizingTimer = 1000;

                Unit* owner = creature->GetOwner();

                if (owner)
                    owner->CastSpell(creature, 114205, true);
            }

            void UpdateAI(const uint32 diff)
            {
                Unit* owner = me->GetOwner();

                if (!owner)
                    return;

                if (demoralizingTimer <= diff)
                {
                    owner->CastSpell(me, 114205, true);
                    demoralizingTimer = 0;
                }
                else
                    demoralizingTimer -= diff;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_demoralizing_bannerAI(creature);
        }
};

/*######
# npc_guardian_of_ancient_kings
######*/

enum GuardianSpellsAndEntries
{
    NPC_PROTECTION_GUARDIAN         = 46490,
    NPC_HOLY_GUARDIAN               = 46499,
    NPC_RETRI_GUARDIAN              = 46506,
    SPELL_ANCIENT_GUARDIAN_VISUAL   = 86657,
    SPELL_ANCIENT_HEALER            = 86674
};

class npc_guardian_of_ancient_kings : public CreatureScript
{
    public:
        npc_guardian_of_ancient_kings() : CreatureScript("npc_guardian_of_ancient_kings") { }

        struct npc_guardian_of_ancient_kingsAI : public ScriptedAI
        {
            npc_guardian_of_ancient_kingsAI(Creature *creature) : ScriptedAI(creature) {}

            uint32 despawnTimer;

            void Reset()
            {
                despawnTimer = 30 * IN_MILLISECONDS;

                if (me->GetEntry() == NPC_RETRI_GUARDIAN || me->GetEntry() == NPC_HOLY_GUARDIAN)
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
                else if (me->GetEntry() == NPC_PROTECTION_GUARDIAN)
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_DISABLE_MOVE);

                if (me->GetEntry() == NPC_RETRI_GUARDIAN)
                    me->SetReactState(REACT_DEFENSIVE);
                else
                    me->SetReactState(REACT_PASSIVE);

                if (me->GetEntry() == NPC_PROTECTION_GUARDIAN)
                {
                    if (me->GetOwner())
                        DoCast(me->GetOwner(), SPELL_ANCIENT_GUARDIAN_VISUAL);
                }
                else if (me->GetEntry() == NPC_RETRI_GUARDIAN)
                {
                    if (me->GetOwner())
                    {
                        if (me->GetOwner()->getVictim())
                            AttackStart(me->GetOwner()->getVictim());

                        DoCast(me, 86703, true);
                    }
                }
                else if (me->GetEntry() == NPC_HOLY_GUARDIAN)
                    if (me->GetOwner())
                        me->GetOwner()->CastSpell(me->GetOwner(), SPELL_ANCIENT_HEALER, true);
            }

            void UpdateAI(const uint32 diff)
            {
                if (despawnTimer)
                {
                    if (despawnTimer <= diff)
                    {
                        me->DespawnOrUnsummon();
                        return;
                    }
                    else
                        despawnTimer -= diff;
                }

                if (!UpdateVictim())
                    return;

                if (me->GetOwner())
                    if (Unit* newVictim = me->GetOwner()->getVictim())
                        if (me->getVictim() != newVictim)
                            AttackStart(newVictim);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_guardian_of_ancient_kingsAI(creature);
        }
};

/*######
# npc_dire_beast
######*/

class npc_dire_beast : public CreatureScript
{
    public:
        npc_dire_beast() : CreatureScript("npc_dire_beast") { }

        struct npc_dire_beastAI : public ScriptedAI
        {
            npc_dire_beastAI(Creature *creature) : ScriptedAI(creature) {}

            void Reset()
            {
                me->SetReactState(REACT_DEFENSIVE);

                if (me->GetOwner())
                    if (me->GetOwner()->getVictim() || me->GetOwner()->getAttackerForHelper())
                        AttackStart(me->GetOwner()->getVictim() ? me->GetOwner()->getVictim() : me->GetOwner()->getAttackerForHelper());
            }

            void UpdateAI(const uint32 /*diff*/)
            {
                if (me->GetReactState() != REACT_DEFENSIVE)
                    me->SetReactState(REACT_DEFENSIVE);

                if (!UpdateVictim())
                {
                    if (Unit* owner = me->GetOwner())
                        if (Unit* newVictim = owner->getAttackerForHelper())
                            AttackStart(newVictim);

                    return;
                }

                if (me->getVictim())
                    if (Unit* owner = me->GetOwner())
                        if (Unit* ownerVictim = owner->getAttackerForHelper())
                            if (me->getVictim()->GetGUID() != ownerVictim->GetGUID())
                                AttackStart(ownerVictim);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_dire_beastAI(creature);
        }
};
/*######
## npc_ring_of_frost
######*/

float const g_RingOfFrostMaxRadius = 5.0f;

class npc_ring_of_frost : public CreatureScript
{
    public:
        npc_ring_of_frost() : CreatureScript("npc_ring_of_frost") { }

        enum Constants
        {
            RingOfFrostFreeze = 82691,
            RingOfFrostImmune = 91264
        };

        struct npc_ring_of_frostAI : public Scripted_NoMovementAI
        {
            npc_ring_of_frostAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            }

            void Reset()
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            }

            void InitializeAI()
            {
                ScriptedAI::InitializeAI();
                Unit* l_Owner = me->GetOwner();
                if (!l_Owner || l_Owner->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                std::list<Creature*> l_RingList;
                me->GetCreatureListWithEntryInGrid(l_RingList, me->GetEntry(), 200.0f);

                for (Creature* l_Ring : l_RingList)
                {
                    if (l_Ring->GetOwner() == me->GetOwner() && l_Ring != me)
                        l_Ring->DisappearAndDie();
                }
            }

            void CheckIfMoveInRing(Unit* p_Who)
            {
                if (p_Who->isAlive() && me->IsInRange(p_Who, 2.0f, 5.0f) && me->IsWithinLOSInMap(p_Who))
                {
                    if (p_Who->HasAura(Constants::RingOfFrostImmune) || p_Who->HasAura(Constants::RingOfFrostFreeze))
                        return;

                    if (Unit* l_Owner = me->GetOwner())
                        l_Owner->CastSpell(p_Who, Constants::RingOfFrostFreeze, true);
                }
            }

            void UpdateAI(uint32 const /*p_Diff*/)
            {
                /// Find all the enemies
                std::list<Unit*> l_Targets;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(me, me, g_RingOfFrostMaxRadius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(me, l_Targets, l_Check);
                me->VisitNearbyObject(g_RingOfFrostMaxRadius, l_Searcher);

                for (Unit* l_Target : l_Targets)
                    CheckIfMoveInRing(l_Target);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ring_of_frostAI(p_Creature);
        }
};

/*######
# npc_wild_mushroom
######*/

#define WILD_MUSHROOM_INVISIBILITY   92661

class npc_wild_mushroom : public CreatureScript
{
    public:
        npc_wild_mushroom() : CreatureScript("npc_wild_mushroom") { }

        struct npc_wild_mushroomAI : public ScriptedAI
        {
            uint32 CastTimer;
            bool stealthed;

            npc_wild_mushroomAI(Creature *creature) : ScriptedAI(creature)
            {
                CastTimer = 6000;
                stealthed = false;
                me->SetReactState(REACT_PASSIVE);
            }

            void UpdateAI(const uint32 diff)
            {
                if (CastTimer <= diff && !stealthed)
                {
                    DoCast(me, WILD_MUSHROOM_INVISIBILITY, true);
                    stealthed = true;
                }
                else
                {
                    CastTimer -= diff;

                    if (!stealthed)
                        me->RemoveAura(WILD_MUSHROOM_INVISIBILITY);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_wild_mushroomAI(creature);
        }
};

/*######
## npc_fungal_growth
######*/

#define FUNGAL_GROWTH_PERIODIC  81282
#define FUNGAL_GROWTH_AREA      94339

class npc_fungal_growth : public CreatureScript
{
    public:
        npc_fungal_growth() : CreatureScript("npc_fungal_growth") { }

        struct npc_fungal_growthAI : public Scripted_NoMovementAI
        {
            npc_fungal_growthAI(Creature *c) : Scripted_NoMovementAI(c)
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            void InitializeAI()
            {
                ScriptedAI::InitializeAI();
                Unit * owner = me->GetOwner();
                if (!owner || owner->GetTypeId() != TYPEID_PLAYER)
                    return;

                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                me->CastSpell(me, FUNGAL_GROWTH_PERIODIC, true);    // Periodic Trigger spell : decrease speed
                me->CastSpell(me, FUNGAL_GROWTH_AREA, true);        // Persistent Area
            }

            void UpdateAI(const uint32 /*diff*/)
            {
                if (!me->HasAura(FUNGAL_GROWTH_PERIODIC))
                    me->CastSpell(me, FUNGAL_GROWTH_PERIODIC, true);
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_fungal_growthAI(pCreature);
        }
};

/*######
## npc_bloodworm
######*/

#define BLOODWORM_BLOOD_GORGED  50453
#define BLOODWORM_BLOOD_STACKS  81277
#define BLOODWORM_BLOOD_BURST   81280 ///< @todo spell id removed

class npc_bloodworm : public CreatureScript
{
    public:
        npc_bloodworm() : CreatureScript("npc_bloodworm") { }

        struct npc_bloodwormAI : public ScriptedAI
        {
            npc_bloodwormAI(Creature* c) : ScriptedAI(c)
            {
            }

            uint32 uiBurstTimer;
            uint32 uiCheckBloodChargesTimer;

            void Burst()
            {
                if (Aura* bloodGorged = me->GetAura(BLOODWORM_BLOOD_STACKS))
                {
                    uint32 stacks = std::min<uint32>(bloodGorged->GetStackAmount(), 10);
                    int32 damage = stacks *  10;
                    me->CastCustomSpell(me, BLOODWORM_BLOOD_BURST, &damage, NULL, NULL, true);
                    me->DespawnOrUnsummon(500);
                }
            }

            void JustDied(Unit* /*killer*/)
            {
                Burst();
            }

            void Reset()
            {
                DoCast(me, BLOODWORM_BLOOD_GORGED, true);

                uiBurstTimer = 19000;
                uiCheckBloodChargesTimer = 1500;
            }

            void UpdateAI(const uint32 diff)
            {
                if (uiBurstTimer <= diff)
                    Burst();
                else
                    uiBurstTimer -= diff;

                if (uiCheckBloodChargesTimer <= diff)
                {
                    if (me->GetOwner())
                    {
                        if (Aura* bloodGorged = me->GetAura(BLOODWORM_BLOOD_STACKS))
                        {
                            // 10% per stack
                            int32 stacks = bloodGorged->GetStackAmount() * 10;
                            int32 masterPct = int32(100.0f - me->GetOwner()->GetHealthPct());
                            AddPct(stacks, masterPct);

                            if (stacks > 100)
                                stacks = 100;

                            if (roll_chance_i(stacks))
                                Burst();
                            else
                                uiCheckBloodChargesTimer = 1500;
                        }
                    }
                }
                else
                    uiCheckBloodChargesTimer -= diff;

                if (!UpdateVictim())
                {
                    if (Unit* target = me->SelectVictim())
                        me->Attack(target, true);
                    return;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature *creature) const
        {
            return new npc_bloodwormAI(creature);
        }
};

/*######
## npc_past_self
######*/

enum PastSelfSpells
{
    SPELL_FADING                    = 107550,
    SPELL_ALTER_TIME                = 110909,
    SPELL_ENCHANTED_REFLECTION      = 102284,
    SPELL_ENCHANTED_REFLECTION_2    = 102288
};

#define ACTION_ALTER_TIME   1

class npc_past_self : public CreatureScript
{
    public:
        npc_past_self() : CreatureScript("npc_past_self") { }

        struct npc_past_selfAI : public Scripted_NoMovementAI
        {
            npc_past_selfAI(Creature* c) : Scripted_NoMovementAI(c)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_PASSIVE);
                me->SetMaxHealth(500);
                me->SetHealth(me->GetMaxHealth());
                m_Mana = 0;
                m_Health = 0;
            }

            int32 m_Mana;
            int32 m_Health;

            void Reset()
            {
                if (!me->HasAura(PastSelfSpells::SPELL_FADING))
                    me->AddAura(PastSelfSpells::SPELL_FADING, me);
            }

            void IsSummonedBy(Unit* p_Owner)
            {
                if (p_Owner && p_Owner->IsPlayer())
                {
                    m_Mana = p_Owner->GetPower(Powers::POWER_MANA);
                    m_Health = p_Owner->GetHealth();

                    p_Owner->AddAura(PastSelfSpells::SPELL_ENCHANTED_REFLECTION, me);
                    p_Owner->AddAura(PastSelfSpells::SPELL_ENCHANTED_REFLECTION_2, me);
                }
                else
                    me->DespawnOrUnsummon();
            }

            void DoAction(const int32 p_Action)
            {
                if (p_Action == ACTION_ALTER_TIME)
                {
                    if (TempSummon* l_PastSelf = me->ToTempSummon())
                    {
                        if (Unit* l_Owner = l_PastSelf->GetSummoner())
                        {
                            if (l_Owner->ToPlayer())
                            {
                                if (!l_Owner->isAlive())
                                    return;

                                l_Owner->SetPower(POWER_MANA, m_Mana);
                                l_Owner->SetHealth(m_Health);

                                l_Owner->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), true);
                                me->DespawnOrUnsummon(100);
                            }
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_past_selfAI(p_Creature);
        }
};

/*######
## npc_transcendence_spirit -- 54569
######*/

enum TranscendenceSpiritSpells
{
    SPELL_MEDITATE          = 124416,
    SPELL_ROOT_FOR_EVER     = 31366,
    SPELL_VISUAL_SPIRIT     = 119053,
    SPELL_INITIALIZE_IMAGES = 102284,
    SPELL_CLONE_CASTER      = 102288
};

enum transcendenceActions
{
    ACTION_TELEPORT = 1
};

class npc_transcendence_spirit : public CreatureScript
{
    public:
        npc_transcendence_spirit() : CreatureScript("npc_transcendence_spirit") { }

        struct npc_transcendence_spiritAI : public Scripted_NoMovementAI
        {
            npc_transcendence_spiritAI(Creature* c) : Scripted_NoMovementAI(c)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                me->CastSpell(me, SPELL_MEDITATE, true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
            }

            void IsSummonedBy(Unit* owner)
            {
                if (!owner || owner->GetTypeId() != TYPEID_PLAYER)
                    return;

                me->SetMaxHealth(owner->GetMaxHealth() / 2);
                me->SetHealth(me->GetMaxHealth());

                me->CastSpell(me, SPELL_VISUAL_SPIRIT, true);
                owner->CastSpell(me, SPELL_INITIALIZE_IMAGES, true);
                owner->CastSpell(me, SPELL_CLONE_CASTER, true);
                owner->AddAura(SPELL_MEDITATE, me);
                me->AddAura(SPELL_ROOT_FOR_EVER, me);
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_TELEPORT:
                    {
                        if (Unit* owner = me->GetOwner())
                        {
                            Position ownerPos;
                            owner->GetPosition(&ownerPos);

                            owner->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                            me->NearTeleportTo(ownerPos.m_positionX, ownerPos.m_positionY, ownerPos.m_positionZ, ownerPos.m_orientation);
                        }

                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature *creature) const
        {
            return new npc_transcendence_spiritAI(creature);
        }
};

/*######
## npc_void_tendrils -- 65282
######*/

enum voidTendrilsSpells
{
    SPELL_VOID_TENDRILS_ROOT = 108920,
    SPELL_VOID_TENDRILS_GRASP = 114404
};

class npc_void_tendrils : public CreatureScript
{
    public:
        npc_void_tendrils() : CreatureScript("npc_void_tendrils") { }

        struct npc_void_tendrilsAI : public Scripted_NoMovementAI
        {
            npc_void_tendrilsAI(Creature* c) : Scripted_NoMovementAI(c)
            {
                me->SetReactState(REACT_AGGRESSIVE);
                targetGUID = 0;
            }

            uint64 targetGUID;

            void Reset()
            {
                if (!me->HasAura(SPELL_ROOT_FOR_EVER))
                    me->AddAura(SPELL_ROOT_FOR_EVER, me);
            }

            void SetGUID(uint64 guid, int32)
            {
                targetGUID = guid;

                if (Unit* l_Target = ObjectAccessor::FindUnit(targetGUID))
                    me->CastSpell(l_Target, SPELL_VOID_TENDRILS_GRASP, true);
            }

            void JustDied(Unit* /*killer*/)
            {
                if (Unit* m_target = ObjectAccessor::FindUnit(targetGUID))
                {
                    m_target->RemoveAura(SPELL_VOID_TENDRILS_ROOT);
                    m_target->RemoveAura(SPELL_VOID_TENDRILS_GRASP);
                }
            }

            void IsSummonedBy(Unit* owner)
            {

                if (owner && owner->IsPlayer())
                {
                    me->SetLevel(owner->getLevel());

                    me->SetMaxHealth(owner->CountPctFromMaxHealth(10));
                    me->SetHealth(me->GetMaxHealth());

                    // Set no damage
                    me->SetBaseWeaponDamage(WeaponAttackType::BaseAttack, MINDAMAGE, 0.0f);
                    me->SetBaseWeaponDamage(WeaponAttackType::BaseAttack, MAXDAMAGE, 0.0f);
                    me->setFaction(owner->getFaction());
                    me->AddAura(SPELL_ROOT_FOR_EVER, me);
                }
                else
                    me->DespawnOrUnsummon();
            }

            void UpdateAI(uint32 const /*diff*/)
            {
                if (!(ObjectAccessor::FindUnit(targetGUID)))
                    me->DespawnOrUnsummon();

                if (Unit* l_Target = ObjectAccessor::FindUnit(targetGUID))
                    if (!l_Target->HasAura(SPELL_VOID_TENDRILS_GRASP) && !l_Target->HasAura(SPELL_VOID_TENDRILS_ROOT))
                        me->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature *creature) const
        {
            return new npc_void_tendrilsAI(creature);
        }
};

/*######
## npc_spectral_guise -- 59607
######*/

enum spectralGuiseSpells
{
    SPELL_SPECTRAL_GUISE_CLONE      = 119012,
    SPELL_SPECTRAL_GUISE_CHARGES    = 119030,
    SPELL_SPECTRAL_GUISE_STEALTH    = 119032
};

class npc_spectral_guise : public CreatureScript
{
    public:
        npc_spectral_guise() : CreatureScript("npc_spectral_guise") { }

        struct npc_spectral_guiseAI : public Scripted_NoMovementAI
        {
            npc_spectral_guiseAI(Creature* c) : Scripted_NoMovementAI(c)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                if (!me->HasAura(SPELL_ROOT_FOR_EVER))
                    me->AddAura(SPELL_ROOT_FOR_EVER, me);
            }

            void IsSummonedBy(Unit* owner)
            {
                if (owner && owner->IsPlayer())
                {
                    me->SetLevel(owner->getLevel());
                    me->SetMaxHealth(owner->GetMaxHealth() / 2);
                    me->SetHealth(me->GetMaxHealth());

                    owner->AddAura(SPELL_INITIALIZE_IMAGES, me);
                    owner->AddAura(SPELL_SPECTRAL_GUISE_CLONE, me);

                    me->CastSpell(me, SPELL_SPECTRAL_GUISE_CHARGES, true);
                    Aura::TryRefreshStackOrCreate(sSpellMgr->GetSpellInfo(SPELL_SPECTRAL_GUISE_STEALTH), MAX_EFFECT_MASK, owner, owner);

                    std::list<HostileReference*> threatList = owner->getThreatManager().getThreatList();
                    for (std::list<HostileReference*>::const_iterator itr = threatList.begin(); itr != threatList.end(); ++itr)
                        if (Unit* unit = (*itr)->getTarget())
                            if (unit->GetTypeId() == TYPEID_UNIT)
                                if (Creature* creature = unit->ToCreature())
                                    if (creature->canStartAttack(me, false))
                                        creature->Attack(me, true);

                    // Set no damage
                    me->SetBaseWeaponDamage(WeaponAttackType::BaseAttack, MINDAMAGE, 0.0f);
                    me->SetBaseWeaponDamage(WeaponAttackType::BaseAttack, MAXDAMAGE, 0.0f);

                    me->AddAura(SPELL_ROOT_FOR_EVER, me);
                }
                else
                    me->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature *creature) const
        {
            return new npc_spectral_guiseAI(creature);
        }
};

/*######
## npc_force_of_nature
######*/

enum eForceOfNatureSpells
{
    SPELL_TREANT_TAUNT          = 113830,
    SPELL_TREANT_ROOT           = 113770,
    SPELL_TREANT_SWIFTMEND      = 142421,
    SPELL_TREANT_HEAL           = 113828,
    SPELL_TREANT_WRATH          = 113769
};

class npc_force_of_nature : public CreatureScript
{
    public:
        npc_force_of_nature() : CreatureScript("npc_force_of_nature") { }

        struct npc_force_of_natureAI : public ScriptedAI
        {
            npc_force_of_natureAI(Creature* pCreature) : ScriptedAI(pCreature) { }

            void Reset()
            {
                Unit* owner = me->ToTempSummon() ? me->ToTempSummon()->GetSummoner() : NULL;
                Unit* target = owner ? (owner->getVictim() ? owner->getVictim() : (owner->ToPlayer() ? owner->ToPlayer()->GetSelectedUnit() : NULL)) : NULL;

                if (!owner || !target)
                    return;

                me->setFaction(owner->getFaction());
                me->SetLevel(owner->getLevel());
                me->SetMaxHealth(owner->GetMaxHealth() / 2);
                me->SetFullHealth();

                switch (me->GetEntry())
                {
                    case ENTRY_TREANT_GUARDIAN:
                        if (me->IsValidAttackTarget(target))
                        {
                            me->CastSpell(target, SPELL_TREANT_TAUNT, false); // Taunt
                            AttackStart(target);
                        }
                        break;
                    case ENTRY_TREANT_FERAL:
                    case ENTRY_TREANT_BALANCE:
                        if (me->IsValidAttackTarget(target))
                        {
                            me->CastSpell(target, SPELL_TREANT_ROOT, false); // Root
                            AttackStart(target);
                        }
                        break;
                    case ENTRY_TREANT_RESTO:
                        if (me->IsValidAssistTarget(target))
                            me->CastSpell(target, SPELL_TREANT_SWIFTMEND, false); // Heal
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 /*diff*/)
            {
                switch (me->GetEntry())
                {
                    case ENTRY_TREANT_BALANCE:
                    {
                        if (!UpdateVictim())
                            return;

                        if (me->HasUnitState(UNIT_STATE_CASTING))
                            return;

                        if (Unit* target = me->getVictim())
                            me->CastSpell(target, SPELL_TREANT_WRATH, false);
                        return;
                    }
                    case ENTRY_TREANT_RESTO:
                    {
                        if (me->HasUnitState(UNIT_STATE_CASTING))
                            return;

                        if (Unit* target = me->SelectNearbyAlly(NULL, 30.0f, true))
                            me->CastSpell(target, SPELL_TREANT_HEAL, false);
                        return;
                    }
                    default:
                    {
                        if (!UpdateVictim())
                            return;

                        if (me->HasUnitState(UNIT_STATE_CASTING))
                            return;

                        break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_force_of_natureAI(pCreature);
        }
};

/*######
## npc_luo_meng
######*/

#define MAGIC_BAMBOO_SHOOT 93314

class npc_luo_meng : public CreatureScript
{
    public:
        npc_luo_meng() : CreatureScript("npc_luo_meng") { }

        struct npc_luo_mengAI : public ScriptedAI
        {
            npc_luo_mengAI(Creature* pCreature) : ScriptedAI(pCreature) { }

            void ReceiveEmote(Player* player, uint32 emote)
            {
                if (emote != TEXT_EMOTE_HUG)
                    return;

                ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(MAGIC_BAMBOO_SHOOT);
                if (!itemTemplate)
                    return;

                // Adding items
                uint32 noSpaceForCount = 0;
                uint32 count = 1;

                // check space and find places
                ItemPosCountVec dest;
                InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, MAGIC_BAMBOO_SHOOT, count, &noSpaceForCount);
                if (msg != EQUIP_ERR_OK)                               // convert to possible store amount
                    count -= noSpaceForCount;

                if (count == 0 || dest.empty())                         // can't add any
                    return;

                Item* item = player->StoreNewItem(dest, MAGIC_BAMBOO_SHOOT, true, Item::GenerateItemRandomPropertyId(MAGIC_BAMBOO_SHOOT));
                if (count > 0 && item)
                    player->SendNewItem(item, count, true, false);
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_luo_mengAI(pCreature);
        }
};

/*######
## npc_monk_spirit - 69791
######*/

enum eSpiritEntries
{
    NPC_STORM_SPIRIT    = 69680,
    NPC_EARTH_SPIRIT    = 69792,
    NPC_FIRE_SPIRIT     = 69791
};

enum eSpiritEquips
{
    EQUIP_STORM_TWO_HANDS   = 25197,
    EQUIP_EARTH_STAFF       = 86218,
    EQUIP_FIRE_TWO_HANDS    = 82224
};

enum eSpiritActions
{
    ACTION_DESPAWN_SPIRIT
};

enum eSpiritMoves
{
    MOVE_DESPAWN    = 1
};

const uint32 visualMorph[3] = { 138080, 138083, 138081 }; // Storm, Earth and Fire

class npc_monk_spirit : public CreatureScript
{
    public:
        npc_monk_spirit() : CreatureScript("npc_monk_spirit") { }

        struct npc_monk_spiritAI : public ScriptedAI
        {
            npc_monk_spiritAI(Creature* creature) : ScriptedAI(creature)
            {
                targetGuid = 0;
            }

            uint64 targetGuid;

            void Reset()
            {
                me->SetDisplayId(38551);

                Unit* owner = me->GetOwner();
                if (!owner)
                    return;

                me->SetShapeshiftForm(owner->GetShapeshiftForm());

                switch (me->GetEntry())
                {
                    case NPC_STORM_SPIRIT:
                        me->CastSpell(me, visualMorph[0], true);
                        SetEquipmentSlots(false, EQUIP_STORM_TWO_HANDS, EQUIP_STORM_TWO_HANDS, EQUIP_NO_CHANGE);
                        me->SetBaseWeaponDamage(WeaponAttackType::BaseAttack, MINDAMAGE, owner->GetWeaponDamageRange(WeaponAttackType::BaseAttack, MINDAMAGE));
                        me->SetBaseWeaponDamage(WeaponAttackType::BaseAttack, MAXDAMAGE, owner->GetWeaponDamageRange(WeaponAttackType::BaseAttack, MAXDAMAGE));
                        me->SetBaseWeaponDamage(WeaponAttackType::OffAttack, MINDAMAGE, owner->GetWeaponDamageRange(WeaponAttackType::OffAttack, MINDAMAGE) / 2);
                        me->SetBaseWeaponDamage(WeaponAttackType::OffAttack, MAXDAMAGE, owner->GetWeaponDamageRange(WeaponAttackType::OffAttack, MAXDAMAGE) / 2);
                        break;
                    case NPC_EARTH_SPIRIT:
                        me->CastSpell(me, visualMorph[1], true);
                        SetEquipmentSlots(false, EQUIP_EARTH_STAFF, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
                        me->SetBaseWeaponDamage(WeaponAttackType::BaseAttack, MINDAMAGE, owner->GetWeaponDamageRange(WeaponAttackType::BaseAttack, MINDAMAGE));
                        me->SetBaseWeaponDamage(WeaponAttackType::BaseAttack, MAXDAMAGE, owner->GetWeaponDamageRange(WeaponAttackType::BaseAttack, MAXDAMAGE));
                        break;
                    case NPC_FIRE_SPIRIT:
                        me->CastSpell(me, visualMorph[2], true);
                        SetEquipmentSlots(false, EQUIP_FIRE_TWO_HANDS, EQUIP_FIRE_TWO_HANDS, EQUIP_NO_CHANGE);
                        me->SetBaseWeaponDamage(WeaponAttackType::BaseAttack, MINDAMAGE, owner->GetWeaponDamageRange(WeaponAttackType::BaseAttack, MINDAMAGE));
                        me->SetBaseWeaponDamage(WeaponAttackType::BaseAttack, MAXDAMAGE, owner->GetWeaponDamageRange(WeaponAttackType::BaseAttack, MAXDAMAGE));
                        me->SetBaseWeaponDamage(WeaponAttackType::OffAttack, MINDAMAGE, owner->GetWeaponDamageRange(WeaponAttackType::OffAttack, MINDAMAGE) / 2);
                        me->SetBaseWeaponDamage(WeaponAttackType::OffAttack, MAXDAMAGE, owner->GetWeaponDamageRange(WeaponAttackType::OffAttack, MAXDAMAGE) / 2);
                        break;
                }
            }

            void SetGUID(uint64 guid, int32 /*data*/ /*= 0*/)
            {
                targetGuid = guid;

                if (Unit* victim = Unit::GetUnit(*me, targetGuid))
                {
                    me->CastSpell(victim, 138104, true);    // Jump
                    me->CastSpell(me, 138130, true);
                    AttackStart(victim);
                }
            }

            uint64 GetGUID(int32 /*data*/ /*= 0*/)
            {
                return targetGuid;
            }

            void IsSummonedBy(Unit* summoner)
            {
                if (!summoner || summoner->GetTypeId() != TYPEID_PLAYER)
                    return;

                summoner->CastSpell(me, 119051, true);
                me->SetLevel(summoner->getLevel());
                me->SetHealth(summoner->CountPctFromMaxHealth(10));
                me->SetMaxHealth(summoner->CountPctFromMaxHealth(10));
                me->SetFullHealth();
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_DESPAWN_SPIRIT:
                    {
                        if (Unit* owner = me->GetOwner())
                        {
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MoveJump(owner->GetPositionX(), owner->GetPositionY(), owner->GetPositionZ(), 15.0f, 10.0f, me->GetOrientation(), MOVE_DESPAWN);
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->GetGUID() == targetGuid)
                    DoAction(ACTION_DESPAWN_SPIRIT);
            }

            void MovementInform(uint32 /*type*/, uint32 id)
            {
                if (id == MOVE_DESPAWN)
                    me->DespawnOrUnsummon(500);
            }

            void UpdateAI(const uint32 /*diff*/)
            {
                if (targetGuid)
                {
                    if (me->getVictim() && me->getVictim()->GetGUID() != targetGuid)
                        DoAction(0);
                    else if (!me->getVictim())
                        DoAction(0);
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_monk_spiritAI(creature);
        }
};

/*######
## npc_rogue_decoy - 62261
######*/

class npc_rogue_decoy : public CreatureScript
{
    public:
        npc_rogue_decoy() : CreatureScript("npc_rogue_decoy") { }

        struct npc_rogue_decoyAI : public Scripted_NoMovementAI
        {
            npc_rogue_decoyAI(Creature* c) : Scripted_NoMovementAI(c)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            uint32 m_DespawnTimer;

            void Reset() override
            {
                m_DespawnTimer = 0;
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_RENAME);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
            }

            void IsSummonedBy(Unit* p_Owner) override
            {
                if (!p_Owner || p_Owner->GetTypeId() != TYPEID_PLAYER)
                    return;

                m_DespawnTimer = 5000;

                me->SetMaxHealth(p_Owner->GetMaxHealth() / 2);
                me->SetHealth(me->GetMaxHealth());

                p_Owner->CastSpell(me, SPELL_INITIALIZE_IMAGES, true);
                p_Owner->CastSpell(me, SPELL_CLONE_CASTER, true);
                me->AddAura(SPELL_ROOT_FOR_EVER, me);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (m_DespawnTimer)
                {
                    if (m_DespawnTimer <= p_Diff)
                    {
                        m_DespawnTimer = 0;
                        me->DespawnOrUnsummon();
                    }
                    else
                        m_DespawnTimer -= p_Diff;
                }
            }
        };

        CreatureAI* GetAI(Creature *creature) const
        {
            return new npc_rogue_decoyAI(creature);
        }
};

/*######
## npc_army_of_the_dead - 24207
######*/

const uint32 displayIds[6] = { 26079, 25286, 10971, 201, 28292, 27870 };

class npc_army_of_the_dead : public CreatureScript
{
    public:
        npc_army_of_the_dead() : CreatureScript("npc_army_of_the_dead") { }

        struct npc_army_of_the_deadAI : public ScriptedAI
        {
            npc_army_of_the_deadAI(Creature* c) : ScriptedAI(c) { }

            void IsSummonedBy(Unit* owner)
            {
                if (!owner)
                    return;

                if (owner->HasAura(58642))
                    me->SetDisplayId(displayIds[urand(0, 5)]);
            }
        };

        CreatureAI* GetAI(Creature *creature) const
        {
            return new npc_army_of_the_deadAI(creature);
        }
};

/// Training Dummy <Healing> - 88835
/// Training Dummy <Healing> - 89321
/// Training Dummy <Healing> - 88289
/// Training Dummy <Healing> - 87321
/// Training Dummy <Healing> - 88316
class npc_training_dummy_healing : public CreatureScript
{
    public:
        npc_training_dummy_healing() : CreatureScript("npc_training_dummy_healing") { }

        struct npc_training_dummy_healingAI : Scripted_NoMovementAI
        {
            npc_training_dummy_healingAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature) { }

            uint32 m_ResetTimer;

            void Reset() override
            {
                m_ResetTimer = 0;

                if (!me->isAlive())
                    me->Respawn(true);

                me->SetHealth(1);
                me->DisableHealthRegen();
                me->AddUnitState(UnitState::UNIT_STATE_STUNNED);
            }

            void HealReceived(Unit* /*p_Healer*/, uint32& /*p_HealAmount*/) override
            {
                m_ResetTimer = 20 * TimeConstants::IN_MILLISECONDS;
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const* /*p_SpellInfo*/) override
            {
                p_Damage = 0;
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (m_ResetTimer)
                {
                    if (m_ResetTimer <= p_Diff)
                        Reset();
                    else
                        m_ResetTimer -= p_Diff;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_training_dummy_healingAI(p_Creature);
        }
};

/// Training Dummy <Damage> - 87760
/// Training Dummy <Damage> - 87317
/// Training Dummy <Damage> - 87320
/// Training Dummy <Damage> - 87761
/// Training Dummy <Damage> - 87318
/// Training Dummy <Damage> - 87762
class npc_training_dummy_damage : public CreatureScript
{
    public:
        npc_training_dummy_damage() : CreatureScript("npc_training_dummy_damage") { }

        struct npc_training_dummy_damageAI : Scripted_NoMovementAI
        {
            npc_training_dummy_damageAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature) { }

            uint32 m_ResetTimer;

            void Reset() override
            {
                m_ResetTimer = 0;

                if (!me->isAlive())
                    me->Respawn(true);

                me->SetFullHealth();
                me->ReenableHealthRegen();

                me->AddUnitState(UnitState::UNIT_STATE_STUNNED);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);        ///< Immune to knock aways like blast wave
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);   ///< Immune to knock back effects like Whiplash
            }

            void EnterEvadeMode() override
            {
                if (!_EnterEvadeMode())
                    return;

                Reset();
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const* /*p_SpellInfo*/) override
            {
                m_ResetTimer = 10 * TimeConstants::IN_MILLISECONDS;
                p_Damage = 0;
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                if (!me->HasUnitState(UnitState::UNIT_STATE_STUNNED))
                    me->AddUnitState(UnitState::UNIT_STATE_STUNNED);

                if (m_ResetTimer)
                {
                    if (m_ResetTimer <= p_Diff)
                        EnterEvadeMode();
                    else
                        m_ResetTimer -= p_Diff;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_training_dummy_damageAI(p_Creature);
        }
};

/// Training Dummy <Tanking> - 87329
/// Training Dummy <Tanking> - 88288
/// Training Dummy <Tanking> - 88836
/// Training Dummy <Tanking> - 87322
/// Training Dummy <Tanking> - 88837
/// Training Dummy <Tanking> - 88314
class npc_training_dummy_tanking : public CreatureScript
{
    public:
        npc_training_dummy_tanking() : CreatureScript("npc_training_dummy_tanking") { }

        struct npc_training_dummy_tankingAI : Scripted_NoMovementAI
        {
            npc_training_dummy_tankingAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature) { }

            uint32 m_ResetTimer;

            void Reset() override
            {
                m_ResetTimer = 0;

                if (!me->isAlive())
                    me->Respawn(true);

                me->AddUnitState(UnitState::UNIT_STATE_ROOT);

                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);        ///< Immune to knock aways like blast wave
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);   ///< Immune to knock back effects like Whiplash
            }

            void DamageDealt(Unit* /*p_Victim*/, uint32& /*p_Damage*/, DamageEffectType /*p_DamageType*/) override
            {
                m_ResetTimer = 10 * TimeConstants::IN_MILLISECONDS;
            }

            void EnterEvadeMode() override
            {
                if (!_EnterEvadeMode())
                    return;

                Reset();
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                if (!me->HasUnitState(UnitState::UNIT_STATE_ROOT))
                    me->AddUnitState(UnitState::UNIT_STATE_ROOT);

                if (m_ResetTimer)
                {
                    if (m_ResetTimer <= p_Diff)
                        EnterEvadeMode();
                    else
                        m_ResetTimer -= p_Diff;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_training_dummy_tankingAI(p_Creature);
        }
};

/// Consecration - 43499
class npc_consecration : public CreatureScript
{
    public:
        npc_consecration() : CreatureScript("npc_consecration") { }

        enum Constants : uint32
        {
            TickPeriod = 1 * IN_MILLISECONDS
        };

        enum eSpells
        {
            ConsecrationVisual = 81298,
            ConsecrationDamage = 81297
        };

        struct npc_consecrationAI : public PassiveAI
        {
            uint32 m_Counter;

            npc_consecrationAI(Creature* creature) :
                PassiveAI(creature),
                m_Counter(0)
            {
            }

            void EnterEvadeMode() override
            {
                if (!me->isAlive())
                    return;

                me->DeleteThreatList();
                me->CombatStop(true);
                me->ResetPlayerDamageReq();
            }

            void IsSummonedBy(Unit* p_Owner) override
            {
                p_Owner->CastSpell(*me, eSpells::ConsecrationVisual, true);
            }

            void Reset() override
            {
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_Counter += p_Diff;

                if (m_Counter >= Constants::TickPeriod)
                {
                    if (Unit* l_Owner = me->GetOwner())
                    {
                        if (DynamicObject* dynObj = l_Owner->GetDynObject(eSpells::ConsecrationVisual))
                        {
                            l_Owner->CastSpell(*dynObj, eSpells::ConsecrationDamage, true);
                        }
                    }

                    m_Counter -= Constants::TickPeriod;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_consecrationAI(creature);
        }
};
/*######
# npc_xuen_the_white_tiger
######*/

class npc_xuen_the_white_tiger : public CreatureScript
{
    public:
        npc_xuen_the_white_tiger() : CreatureScript("npc_xuen_the_white_tiger") { }

        enum eSpells
        {
            INVOKE_XUEN_THE_WHITE_TIGER = 123995,
            CRACKLING_TIGER_LIGHTNING = 123999
        };
        struct npc_xuen_the_white_tigerAI : public PetAI
        {
            EventMap events;

            npc_xuen_the_white_tigerAI(Creature* creature) : PetAI(creature)
            {
                me->SetReactState(ReactStates::REACT_HELPER);

                Unit* l_Owner = me->ToTempSummon() ? me->ToTempSummon()->GetSummoner() : NULL;
                Unit* l_Target = l_Owner ? (l_Owner->getVictim() ? l_Owner->getVictim() : (l_Owner->ToPlayer() ? l_Owner->ToPlayer()->GetSelectedUnit() : NULL)) : NULL;

                if (!l_Owner || !l_Target)
                    return;

                /// Start attack
                if (me->IsValidAttackTarget(l_Target))
                {
                    AttackStart(l_Target);
                    ///me->GetMotionMaster()->MoveJump(l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ(), 15.0f, 10.0f, l_Target->GetOrientation(), MOVE_DESPAWN);
                    me->CastSpell(l_Target, eSpells::INVOKE_XUEN_THE_WHITE_TIGER, true);
                }
            }

            void Reset() override
            {
                PetAI::Reset();

                me->CastSpell(me, eSpells::CRACKLING_TIGER_LIGHTNING, true);
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_xuen_the_white_tigerAI(creature);
        }
};

/// Frozen Trail Packer - 64227
class npc_frozen_trail_packer : public CreatureScript
{
    public:
        npc_frozen_trail_packer() : CreatureScript("npc_frozen_trail_packer") { }

        struct npc_frozen_trail_packerAI : public ScriptedAI
        {
            npc_frozen_trail_packerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void sGossipSelect(Player* p_Player, uint32 /*p_MenuID*/, uint32 /*p_Action*/) override
            {
                if (p_Player->AddItem(86125, 1)) ///< Kafa Press
                    me->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_frozen_trail_packerAI(p_Creature);
        }
};

/////////////////////////////////////////////////////////////
/// Ethereal Soul-Trader - 27914
class npc_ethereal_soul_trader : public CreatureScript
{
    public:
        npc_ethereal_soul_trader() : CreatureScript("npc_ethereal_soul_trader") { }

        struct npc_ethereal_soul_traderAI : public PassiveAI
        {
            npc_ethereal_soul_traderAI(Creature* p_Creature) : PassiveAI(p_Creature) { }

            enum eSpells
            {
                EtherealPetAura                 = 50051,
                EtherealPetOnKillStealEssence   = 50101
            };

            std::queue<uint64> m_PendingTargets;

            void IsSummonedBy(Unit* p_Owner) override
            {
                me->GetMotionMaster()->Clear(false);
                me->GetMotionMaster()->MoveFollow(p_Owner, PET_FOLLOW_DIST, (3 * M_PI) / 2);

                p_Owner->CastSpell(p_Owner, eSpells::EtherealPetAura, true);
            }

            void JustDespawned() override
            {
                if (Unit* l_Owner = me->GetOwner())
                    l_Owner->RemoveAura(eSpells::EtherealPetAura);
            }

            void SetGUID(uint64 p_Guid, int32 /*p_ID*/) override
            {
                m_PendingTargets.push(p_Guid);
            }

            void UpdateAI(uint32 const /*p_Diff*/) override
            {
                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING) || m_PendingTargets.empty())
                    return;

                if (uint64 l_Guid = m_PendingTargets.front())
                {
                    m_PendingTargets.pop();

                    if (Unit* l_Target = Unit::GetUnit(*me, l_Guid))
                        me->CastSpell(l_Target, eSpells::EtherealPetOnKillStealEssence, false);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ethereal_soul_traderAI(p_Creature);
        }
};

/// Ethereal Pet Aura - 50051
class spell_ethereal_pet_aura : public SpellScriptLoader
{
    public:
        spell_ethereal_pet_aura() : SpellScriptLoader("spell_ethereal_pet_aura") { }

        class spell_ethereal_pet_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ethereal_pet_aura_AuraScript);

            enum eDatas
            {
                EtherealSoulTrader = 27914
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (!GetTarget() || !p_EventInfo.GetActionTarget() || !GetTarget()->IsPlayer())
                    return;

                Player* l_Player = GetTarget()->ToPlayer();

                Creature* l_BattlePet = l_Player->GetSummonedBattlePet();
                if (l_BattlePet == nullptr || l_BattlePet->GetEntry() != eDatas::EtherealSoulTrader)
                    return;

                if (l_BattlePet->IsAIEnabled)
                    l_BattlePet->AI()->SetGUID(p_EventInfo.GetActionTarget()->GetGUID(), 0);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_ethereal_pet_aura_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_ethereal_pet_aura_AuraScript();
        }
};

/// Ethereal Pet OnKill Steal Essence - 50101
class spell_ethereal_pet_onkill_steal_essence : public SpellScriptLoader
{
    public:
        spell_ethereal_pet_onkill_steal_essence() : SpellScriptLoader("spell_ethereal_pet_onkill_steal_essence") { }

        class spell_ethereal_pet_onkill_steal_essence_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ethereal_pet_onkill_steal_essence_AuraScript);

            enum eData
            {
                EtherealPetOnKillGiveToken = 50063
            };

            void HandleRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, eData::EtherealPetOnKillGiveToken, true);
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_ethereal_pet_onkill_steal_essence_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_ethereal_pet_onkill_steal_essence_AuraScript();
        }
};
/////////////////////////////////////////////////////////////

#ifndef __clang_analyzer__
void AddSC_npcs_special()
{
    new npc_air_force_bots();
    new npc_lunaclaw_spirit();
    new npc_chicken_cluck();
    new npc_dancing_flames();
    new npc_doctor();
    new npc_injured_patient();
    new npc_garments_of_quests();
    new npc_guardian();
    new npc_mount_vendor();
    new npc_rogue_trainer();
    new npc_sayge();
    new npc_steam_tonk();
    new npc_tonk_mine();
    new npc_brewfest_reveler();
    new npc_snake_trap();
    new npc_mirror_image();
    new npc_ebon_gargoyle();
    new npc_lightwell();
    new mob_mojo();
    new npc_training_dummy();
    new npc_pvp_training_dummy();
    new npc_wormhole();
    new npc_pet_trainer();
    new npc_locksmith();
    new npc_experience();
    new npc_firework();
    new npc_spring_rabbit();
    new npc_generic_harpoon_cannon();
    new npc_rate_xp_modifier();
    new RatesXPWarner();
    new npc_demoralizing_banner();
    new npc_guardian_of_ancient_kings();
    new npc_dire_beast();
    new npc_ring_of_frost();
    new npc_wild_mushroom();
    new npc_fungal_growth();
    new npc_bloodworm();
    new npc_past_self();
    new npc_transcendence_spirit();
    new npc_void_tendrils();
    new npc_spectral_guise();
    new npc_force_of_nature();
    new npc_luo_meng();
    new npc_monk_spirit();
    new npc_rogue_decoy();
    new npc_army_of_the_dead();
    new npc_training_dummy_healing();
    new npc_training_dummy_damage();
    new npc_training_dummy_tanking();
    new npc_consecration();
    new npc_xuen_the_white_tiger();
    new npc_frozen_trail_packer();

    /////////////////////////////////////////////////////////////
    /// Ethereal Soul-Trader
    new npc_ethereal_soul_trader();
    new spell_ethereal_pet_aura();
    new spell_ethereal_pet_onkill_steal_essence();
    /////////////////////////////////////////////////////////////
}
#endif
