#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "SpellScript.h"
#include "Vehicle.h"

#define GOSSIP_WIND     "I would like to go back on the top of the temple"
#define ACTION_TALK 1
class mob_master_shang_xi_temple : public CreatureScript
{
public:
    mob_master_shang_xi_temple() : CreatureScript("mob_master_shang_xi_temple") { }

    bool OnQuestAccept(Player* player, Creature* /*creature*/, Quest const* quest)
    {
        if (quest->GetQuestId() == 29776) // Brise du matin
        {
            if (Creature* vehicle = player->SummonCreature(55685, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation()))
            {
                player->AddAura(99385, vehicle);
                player->EnterVehicle(vehicle);
            }
        }

        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestStatus(29776) != QUEST_STATUS_NONE)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_WIND, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 action)
    {
        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            /* The vehicle bug for now on TaranZhu, too much lags
            if (Creature* vehicle = player->SummonCreature(55685, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation()))
            {
            player->AddAura(99385, vehicle);
            player->EnterVehicle(vehicle);
            }*/

            player->NearTeleportTo(926.58f, 3605.33f, 251.63f, 3.114f);
        }

        player->PlayerTalkClass->SendCloseGossip();
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_master_shang_xi_templeAI(creature);
    }

    struct mob_master_shang_xi_templeAI : public ScriptedAI
    {
        mob_master_shang_xi_templeAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 l_CheckTimer;
        uint32 l_TalkRefill;
        bool hasSaid1;

        void Reset()
        {
            hasSaid1 = false;
            l_CheckTimer = 1500;
            l_TalkRefill = 30000;
        }

        void DoAction(int32 const action)
        {
            switch (action)
            {
            case ACTION_TALK:
                if (!hasSaid1)
                {
                    Talk(0);
                    hasSaid1 = true;
                }
                break;
            }
        }

        void UpdateAI(const uint32 diff)
        {

            if (l_CheckTimer)
            {
                if (l_CheckTimer <= diff)
                {
                    std::list<Player*> l_PlayerList;
                    GetPlayerListInGrid(l_PlayerList, me, 15.0f);

                    for (Player* l_Player : l_PlayerList)
                    {
                        if (l_Player->GetQuestStatus(29775) == QUEST_STATUS_INCOMPLETE && l_Player->GetQuestObjectiveCounter(276326) < 1)
                            l_Player->KilledMonsterCredit(59497);
                        else if (l_Player->GetQuestStatus(29423) == QUEST_STATUS_COMPLETE)
                            DoAction(ACTION_TALK);
                    }

                    l_CheckTimer = 1500;
                }
                else
                    l_CheckTimer -= diff;
            }

            if (l_TalkRefill)
            {
                if (l_TalkRefill <= diff)
                {
                    hasSaid1 = false;
                    l_TalkRefill = 30000;
                }
                else
                    l_TalkRefill -= diff;
            }

            DoMeleeAttackIfReady();
        }
    };
};

class npc_wind_vehicle : public CreatureScript
{
public:
    npc_wind_vehicle() : CreatureScript("npc_wind_vehicle") { }

    struct npc_wind_vehicleAI : public npc_escortAI
    {        
        npc_wind_vehicleAI(Creature* creature) : npc_escortAI(creature)
        {}

        uint32 IntroTimer;

        void Reset()
        {
            IntroTimer = 100;
        }

        void WaypointReached(uint32 waypointId)
        {
            if (waypointId == 6)
            {
                if (me->GetVehicleKit())
                    me->GetVehicleKit()->RemoveAllPassengers();

                me->DespawnOrUnsummon();
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (IntroTimer)
            {
                if (IntroTimer <= diff)
                {
                    Start(false, true);
                    IntroTimer = 0;
                }
                else
                    IntroTimer -= diff;
            }

            npc_escortAI::UpdateAI(diff);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_wind_vehicleAI(creature);
    }

};

class AreaTrigger_at_wind_temple_entrance : public AreaTriggerScript
{
public:
    AreaTrigger_at_wind_temple_entrance() : AreaTriggerScript("AreaTrigger_at_wind_temple_entrance")
    {}

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
    {
        if (player->GetQuestStatus(29785) == QUEST_STATUS_INCOMPLETE)
        {
            if (Creature* aysa = player->SummonCreature(55744, 665.60f, 4220.66f, 201.93f, 1.93f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID()))
                aysa->AI()->SetGUID(player->GetGUID());
        }

        return true;
    }
};

class mob_aysa_wind_temple_escort : public CreatureScript
{
public:
    mob_aysa_wind_temple_escort() : CreatureScript("mob_aysa_wind_temple_escort") { }

    struct mob_aysa_wind_temple_escortAI : public npc_escortAI
    {        
        mob_aysa_wind_temple_escortAI(Creature* creature) : npc_escortAI(creature)
        {}

        uint32 IntroTimer;

        uint64 playerGuid;

        void Reset()
        {
            IntroTimer = 100;
            me->SetReactState(REACT_PASSIVE);
        }

        void SetGUID(uint64 guid, int32)
        {
            playerGuid = guid;
        }

        void DoAction(int32 const /*param*/)
        {
            SetEscortPaused(false);
        }

        void WaypointReached(uint32 waypointId)
        {
            switch (waypointId)
            {
            case 1:
//                SetEscortPaused(true);  <-- Who did this ? And why ?
//                me->SetFacingTo(2.38f);
                break;
            case 6:
//                SetEscortPaused(true);  <-- Who did this ? And why ?
                break;
            case 8:
                if (Player* player = ObjectAccessor::GetPlayer(*me, playerGuid))
                    player->KilledMonsterCredit(55666);
                break;
            default:
                break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (IntroTimer)
            {
                if (IntroTimer <= diff)
                {
                    Start(false, true);
                    IntroTimer = 0;
                }
                else
                    IntroTimer -= diff;
            }

            npc_escortAI::UpdateAI(diff);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_aysa_wind_temple_escortAI(creature);
    }
};

class mob_frightened_wind : public CreatureScript
{
public:
    mob_frightened_wind() : CreatureScript("mob_frightened_wind") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_frightened_windAI(creature);
    }

    struct mob_frightened_windAI : public ScriptedAI
    {
        mob_frightened_windAI(Creature* creature) : ScriptedAI(creature)
        {}

        uint32 tornadeTimer;

        enum Spells
        {
            SPELL_TORNADE    = 107278,
        };

        void Reset()
        {
            me->SetReactState(REACT_PASSIVE);
            tornadeTimer = 8 * IN_MILLISECONDS;
        }

        void UpdateAI(const uint32 diff)
        {
            if (tornadeTimer <= diff)
            {
                me->ToggleAura(SPELL_TORNADE, me);

                if (!me->HasAura(SPELL_TORNADE))
                {
                    std::list<Creature*> aysaList;
                    GetCreatureListWithEntryInGrid(aysaList, me, 55744, 35.0f);

                    for (auto aysa: aysaList)
                        aysa->AI()->DoAction(1);
                }
                tornadeTimer = 8 * IN_MILLISECONDS;
            }
            else
                tornadeTimer -= diff;
        }
    };
};

class npc_aysa_in_wind_temple : public CreatureScript
{
public:
    npc_aysa_in_wind_temple() : CreatureScript("npc_aysa_in_wind_temple") { }

    bool OnQuestAccept(Player* player, Creature* /*creature*/, Quest const* quest)
    {
        if (quest->GetQuestId() == 29786) // Bataille Pyrotechnique
        {
            if (Creature* aysa = player->SummonCreature(64543, 543.94f, 4317.31f, 212.24f, 1.675520f, TEMPSUMMON_TIMED_DESPAWN, 10000, player->GetGUID()))
                aysa->GetMotionMaster()->MovePoint(1, 643.45f, 4228.66f, 202.90f);

            if (Creature* dafeng = player->SummonCreature(64532, 543.56f, 4320.97f, 212.24f, 5.445430f, TEMPSUMMON_TIMED_DESPAWN, 10000, player->GetGUID()))
                dafeng->GetMotionMaster()->MovePoint(1, 643.45f, 4228.66f, 202.90f);
        }

        return true;
    }
};

enum Enums
{
    NPC_ROCKET_LAUNCHER = 64507,
    SPELL_ROCKET_LAUNCH = 104855,

    EVENT_NEXT_MOVEMENT = 1,
    EVENT_STUNNED       = 2,
    EVENT_LIGHTNING     = 3,

    SPELL_SERPENT_SWEEP = 125990,
    SPELL_STUNNED       = 125992,
    SPELL_LIGHTNING     = 126006
};

Position ZhaoPos[] =
{
    {719.36f, 4164.60f, 216.06f, 0.0f}, // Center
    {745.91f, 4154.35f, 223.48f, 0.0f},
    {717.04f, 4141.16f, 219.83f, 0.0f},
    {689.62f, 4153.16f, 217.63f, 0.0f},
    {684.53f, 4173.24f, 216.98f, 0.0f},
    {704.77f, 4190.16f, 218.24f, 0.0f},
    {736.90f, 4183.85f, 221.41f, 0.0f}
};

class boss_zhao_ren : public CreatureScript
{
public:
    boss_zhao_ren() : CreatureScript("boss_zhao_ren") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_zhao_renAI(creature);
    }

    struct boss_zhao_renAI : public ScriptedAI
    {
        boss_zhao_renAI(Creature* creature) : ScriptedAI(creature)
        {}

        EventMap _events;
        bool eventStarted;
        uint8 hitCount;
        uint8 currentPos;

        void Reset()
        {
            _events.Reset();
            me->SetReactState(REACT_PASSIVE);

            eventStarted = false;
            hitCount = 0;
            currentPos = 0;

            me->SetFullHealth();
            me->RemoveAurasDueToSpell(SPELL_STUNNED);

            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MovePoint(0, ZhaoPos[0].GetPositionX(), ZhaoPos[0].GetPositionY(), ZhaoPos[0].GetPositionZ());
        }

        void SpellHit(Unit* /*caster*/, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_ROCKET_LAUNCH)
            {
                if (++hitCount >= 5)
                {
                    me->CastSpell(me, SPELL_STUNNED, true);
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MoveFall();
                    _events.ScheduleEvent(EVENT_STUNNED, 12000);
                    hitCount = 0;
                }
            }
        }

        bool checkPlayers()
        {
            std::list<Player*> playerList;
            GetPlayerListInGrid(playerList, me, 80.0f);

            for (auto player : playerList)
                if (player->GetQuestStatus(29786) == QUEST_STATUS_INCOMPLETE)
                    if (player->isAlive())
                        return true;

            return false;
        }

        void GoToNextPos()
        {
            if (++currentPos > 6)
                currentPos = 1;

            me->GetMotionMaster()->MovePoint(currentPos, ZhaoPos[currentPos].GetPositionX(), ZhaoPos[currentPos].GetPositionY(), ZhaoPos[currentPos].GetPositionZ());
        }

        Player* GetRandomPlayer()
        {
            std::list<Player*> playerList;
            GetPlayerListInGrid(playerList, me, 50.0f);

            if (playerList.empty())
                return NULL;

            JadeCore::Containers::RandomResizeList(playerList, 1);

            return *playerList.begin();
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (!id)
                return;

            _events.ScheduleEvent(EVENT_NEXT_MOVEMENT, 200);
        }

        void JustDied(Unit* /*attacker*/)
        {
            std::list<Player*> playerList;
            GetPlayerListInGrid(playerList, me, 50.0f);

            for (auto player : playerList)
                if (player->GetQuestStatus(29786) == QUEST_STATUS_INCOMPLETE)
                    if (player->isAlive())
                        player->KilledMonsterCredit(me->GetEntry());
        }

        void UpdateAI(const uint32 diff)
        {
            if (checkPlayers())
            {
                if (!eventStarted)  // Event not started, player found
                {
                    _events.ScheduleEvent(EVENT_NEXT_MOVEMENT, 1000);
                    _events.ScheduleEvent(EVENT_LIGHTNING, 5000);
                    eventStarted = true;
                }
            }
            else
            {
                if (eventStarted)  // Event started, no player found
                    Reset();

                return;
            }

            _events.Update(diff);

            switch (_events.ExecuteEvent())
            {
            case EVENT_NEXT_MOVEMENT:
                {
                    if (me->HasAura(SPELL_STUNNED))
                        _events.ScheduleEvent(EVENT_NEXT_MOVEMENT, 2000);

                    GoToNextPos();
                    break;
                }
            case EVENT_STUNNED:
                {
                    me->RemoveAurasDueToSpell(SPELL_STUNNED);
                    me->CastSpell(me, SPELL_SERPENT_SWEEP, false);
                    _events.ScheduleEvent(EVENT_NEXT_MOVEMENT, 3000);
                    break;
                }
            case EVENT_LIGHTNING:
                {
                    if (Player* player = GetRandomPlayer())
                        me->CastSpell(player, SPELL_LIGHTNING, false);

                    _events.ScheduleEvent(EVENT_LIGHTNING, 5000);
                    break;
                }
            }
        }
    };
};

class npc_rocket_launcher : public CreatureScript
{
public:
    npc_rocket_launcher() : CreatureScript("npc_rocket_launcher") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_rocket_launcherAI (creature);
    }

    struct npc_rocket_launcherAI : public ScriptedAI
    {
        npc_rocket_launcherAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 cooldown;

        void Reset()
        {
            cooldown = 0;
            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        }

        void OnSpellClick(Unit* /*Clicker*/)
        {
            if (cooldown)
                return;

            if (Creature* zhao = GetClosestCreatureWithEntry(me, 55786, 50.0f))
                me->CastSpell(zhao, SPELL_ROCKET_LAUNCH, false);

            cooldown = 5000;
            me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        }

        void EnterCombat(Unit* /*who*/)
        {
            return;
        }

        void UpdateAI(const uint32 diff)
        {
            if (cooldown)
            {
                if (cooldown <= diff)
                {
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    cooldown = 0;
                }
                else
                    cooldown -= diff;
            }
        }
    };
};

class mob_master_shang_xi_after_zhao : public CreatureScript
{
public:
    mob_master_shang_xi_after_zhao() : CreatureScript("mob_master_shang_xi_after_zhao") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == 29787) // Digne de passer
            if (Creature* master = player->SummonCreature(56159, creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ(), creature->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID()))
                master->AI()->SetGUID(player->GetGUID());

        return true;
    }
};

class mob_master_shang_xi_after_zhao_escort : public CreatureScript
{
public:
    mob_master_shang_xi_after_zhao_escort() : CreatureScript("mob_master_shang_xi_after_zhao_escort") { }

    struct mob_master_shang_xi_after_zhao_escortAI : public npc_escortAI
    {
        mob_master_shang_xi_after_zhao_escortAI(Creature* creature) : npc_escortAI(creature)
        {}

        uint32 IntroTimer;

        uint64 playerGuid;

        void Reset()
        {
            IntroTimer = 250;
            me->SetReactState(REACT_PASSIVE);
        }

        void SetGUID(uint64 guid, int32)
        {
            playerGuid = guid;
        }

        void WaypointReached(uint32 waypointId)
        {
            switch (waypointId)
            {
//             case 11:
//                 me->SummonCreature(56274, 845.89f, 4372.62f, 223.98f, 4.78f, TEMPSUMMON_CORPSE_DESPAWN, 0, playerGuid); // TODO : set template
//                 break;
                case 12:
                    me->SetFacingTo(0.0f);
                    SetEscortPaused(true);
                    break;
                case 17:
                    me->SetFacingTo(4.537860f);
                    me->DespawnOrUnsummon(1000);

                    if (Player* owner = ObjectAccessor::GetPlayer(*me, playerGuid))
                        owner->AddAura(59074, owner);
                    break;
                default:
                    break;
            }
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            if (summon->GetEntry() == 56274)
                SetEscortPaused(false);
        }

        void UpdateAI(const uint32 diff)
        {
            if (IntroTimer)
            {
                if (IntroTimer <= diff)
                {
                    Start(false, true);
                    IntroTimer = 0;
                }
                else
                    IntroTimer -= diff;
            }

            npc_escortAI::UpdateAI(diff);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_master_shang_xi_after_zhao_escortAI(creature);
    }
};

class mob_master_shang_xi_thousand_staff : public CreatureScript
{
public:
    mob_master_shang_xi_thousand_staff() : CreatureScript("mob_master_shang_xi_thousand_staff") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == 29790) // Digne de passer
            if (Creature* master = player->SummonCreature(56686, creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ(), creature->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID()))
                master->AI()->SetGUID(player->GetGUID());

        return true;
    }
};

class mob_master_shang_xi_thousand_staff_escort : public CreatureScript
{
public:
    mob_master_shang_xi_thousand_staff_escort() : CreatureScript("mob_master_shang_xi_thousand_staff_escort") { }

    struct mob_master_shang_xi_thousand_staff_escortAI : public npc_escortAI
    {
        mob_master_shang_xi_thousand_staff_escortAI(Creature* creature) : npc_escortAI(creature)
        {}

        uint32 IntroTimer;
        uint32 DespawnTimer;

        uint32 talkTimer1;
        uint32 talkTimer2;
        uint32 talkTimer3;
        uint32 talkTimer4;
        uint32 talkTimer5;
        uint32 talkTimer6;

        uint64 playerGuid;

        void Reset()
        {
            IntroTimer = 250;
            DespawnTimer = 0;
            me->SetReactState(REACT_PASSIVE);
            SetRun(false);
        }

        void SetGUID(uint64 guid, int32)
        {
            playerGuid = guid;
        }

        void WaypointReached(uint32 waypointId)
        {
            switch (waypointId)
            {
            case 4:
                SetEscortPaused(true);
                me->SetFacingTo(4.522332f);
                DespawnTimer = 58000;
                talkTimer1 = 3000;
                talkTimer2 = 10000;
                talkTimer3 = 20000;
                talkTimer4 = 30000;
                talkTimer5 = 40000;
                talkTimer6 = 50000;
                break;
            default:
                break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (IntroTimer)
            {
                if (IntroTimer <= diff)
                {
                    Start(false, true);
                    IntroTimer = 0;
                }
                else
                    IntroTimer -= diff;
            }

            if (talkTimer1)
            {
                if (talkTimer1 <= diff)
                {
                    Talk(0);
                    talkTimer1 = 0;
                }
                else
                    talkTimer1 -= diff;
            }

            if (talkTimer2)
            {
                if (talkTimer2 <= diff)
                {
                    Talk(1);
                    talkTimer2 = 0;
                }
                else
                    talkTimer2 -= diff;
            }

            if (talkTimer3)
            {
                if (talkTimer3 <= diff)
                {
                    Talk(2);
                    talkTimer3 = 0;
                }
                else
                    talkTimer3 -= diff;
            }

            if (talkTimer4)
            {
                if (talkTimer4 <= diff)
                {
                    Talk(3);
                    talkTimer4 = 0;
                }
                else
                    talkTimer4 -= diff;
            }

            if (talkTimer5)
            {
                if (talkTimer5 <= diff)
                {
                    Talk(4);
                    talkTimer5 = 0;
                }
                else
                    talkTimer5 -= diff;
            }

            if (talkTimer6)
            {
                if (talkTimer6 <= diff)
                {
                    Talk(5);
                    talkTimer6 = 0;
                }
                else
                    talkTimer6 -= diff;
            }

            if (DespawnTimer)
            {
                if (DespawnTimer <= diff)
                {
                    me->DespawnOrUnsummon();
                    me->SummonCreature(57874, 873.09f, 4462.25f, 241.27f, 3.80f, TEMPSUMMON_MANUAL_DESPAWN, 0, playerGuid);

                    if (Player* owner = ObjectAccessor::GetPlayer(*me, playerGuid))
                        owner->KilledMonsterCredit(56688);

                    DespawnTimer = 0;
                }
                else
                    DespawnTimer -= diff;
            }

            npc_escortAI::UpdateAI(diff);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_master_shang_xi_thousand_staff_escortAI(creature);
    }
};

class mob_shang_xi_air_balloon : public VehicleScript
{
public:
    mob_shang_xi_air_balloon() : VehicleScript("mob_shang_xi_air_balloon") { }

    void OnAddPassenger(Vehicle* /*veh*/, Unit* passenger, int8 seatId)
    {
        if (seatId == 1)
            if (Player* player = passenger->ToPlayer())
                player->KilledMonsterCredit(56378);
    }

    struct mob_shang_xi_air_balloonAI : public npc_escortAI
    {
        mob_shang_xi_air_balloonAI(Creature* creature) : npc_escortAI(creature)
        {}

        uint32 IntroTimer;

        void Reset()
        {
            IntroTimer = 250;
            me->setActive(true);
            me->SetReactState(REACT_PASSIVE);
        }

        void WaypointReached(uint32 waypointId)
        {
            switch (waypointId)
            {
            case 19:
                if (me->GetVehicleKit())
                {
                    if (Unit* passenger = me->GetVehicleKit()->GetPassenger(1))
                        if (Player* player = passenger->ToPlayer())
                        {
                            player->KilledMonsterCredit(55939);
                            player->AddAura(50550, player);
                        }

                        me->GetVehicleKit()->RemoveAllPassengers();
                }

                me->DespawnOrUnsummon();
                break;
            default:
                break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (IntroTimer)
            {
                if (IntroTimer <= diff)
                {
                    if (me->GetVehicleKit())
                    {
                        if (Unit* passenger = me->GetVehicleKit()->GetPassenger(1))
                        {
                            Start(false, true);
                            IntroTimer = 0;
                        }
                    }
                }
                else
                    IntroTimer -= diff;
            }

            npc_escortAI::UpdateAI(diff);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_shang_xi_air_balloonAI(creature);
    }
};

class npc_ji_firepaw : public CreatureScript
{
public:
    npc_ji_firepaw() : CreatureScript("npc_ji_firepaw")
    {
        isSummoned = false;
    }

    bool isSummoned;

    void SummonHiFirepawHelper(Player* summoner, uint32 entry)
    {
        uint32 phase = summoner->GetPhaseMask();
        uint32 team = summoner->GetTeam();
        Position pos;

        summoner->GetPosition(&pos);

        Guardian* summon = new Guardian(NULL, summoner, false);

        if (!summon->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_UNIT), summoner->GetMap(), phase, entry, 0, team, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation()))
        {
            delete summon;
            return;
        }



        summon->SetHomePosition(pos);
        summon->InitStats(0);
        summoner->GetMap()->AddToMap(summon->ToCreature());
        summon->InitSummon();
        summon->InitStatsForLevel(10);
        summon->SetFollowAngle(summoner->GetAngle(summon));
        summon->SetReactState(REACT_AGGRESSIVE);

    }

    bool OnQuestAccept(Player* player, Creature* /*creature*/, Quest const* quest)
    {
        std::list<Creature*> summonList;
        GetCreatureListWithEntryInGrid(summonList, player, 59960, 6.0f);

        for (auto summoned: summonList)
            isSummoned = true;

        if (isSummoned == false)
        {
            if (quest->GetQuestId() == 29779)
            {
                SummonHiFirepawHelper(player, 59960);
                isSummoned = true;
            }

            if (quest->GetQuestId() == 29780)
            {
                SummonHiFirepawHelper(player, 59960);
                isSummoned = true;
            }

            if (quest->GetQuestId() == 29781)
            {
                SummonHiFirepawHelper(player, 59960);
                isSummoned = true;
            }
        }

        return true;
    }
};

class npc_ji_firepaw_escort : public CreatureScript
{
public:
    npc_ji_firepaw_escort() : CreatureScript("npc_ji_firepaw_escort") { }

    struct npc_ji_firepaw_escortAI : public ScriptedAI
    {
        npc_ji_firepaw_escortAI(Creature* creature) : ScriptedAI(creature)
        {
            playerGuid = 0;
        }

        uint64 playerGuid;

        void IsSummonedBy(Unit* summoner)
        {
            playerGuid = summoner->GetGUID();
            me->GetMotionMaster()->MoveFollow(summoner, 1.0f, 1.0f, MOTION_SLOT_ACTIVE);
        }

        void UpdateAI(const uint32 /*diff*/)
        {
            Player* summoner = sObjectAccessor->FindPlayer(playerGuid);
            if (!summoner)
                return;

            if (Unit* target = summoner->getVictim())
            {
                me->Attack(target, true);
                DoMeleeAttackIfReady();
                return;
            }

            if ((summoner->GetQuestStatus(29780) == QUEST_STATUS_COMPLETE || summoner->GetQuestStatus(29780) == QUEST_STATUS_REWARDED) && (summoner->GetQuestStatus(29779) == QUEST_STATUS_COMPLETE
                || summoner->GetQuestStatus(29779) == QUEST_STATUS_REWARDED) && (summoner->GetQuestStatus(29781) == QUEST_STATUS_COMPLETE || summoner->GetQuestStatus(29781) == QUEST_STATUS_REWARDED))
                me->DespawnOrUnsummon();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ji_firepaw_escortAI(creature);
    }
};

class mob_guardian_of_the_elders : public CreatureScript
{
    public:
        mob_guardian_of_the_elders() : CreatureScript("mob_guardian_of_the_elders") { }

        struct mob_guardian_of_the_eldersAI : public ScriptedAI
        {
            mob_guardian_of_the_eldersAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            uint64 playerGuid;

            void JustDied(Unit* p_Killer)
            {
                if (p_Killer->GetTypeId() != TYPEID_PLAYER)
                    return;

                std::list<GameObject*> l_GobList;
                GetGameObjectListWithEntryInGrid(l_GobList, me, 209922, 20.0f);

                for (GameObject* l_Gob : l_GobList)
                    l_Gob->UseDoorOrButton(me->GetRespawnTime());
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_guardian_of_the_eldersAI(creature);
        }
};

class gob_defaced_scroll_of_wisdom : public GameObjectScript
{
public:
    gob_defaced_scroll_of_wisdom() : GameObjectScript("gob_defaced_scroll_of_wisdom")
    {
    }

    bool OnGossipHello(Player* player, GameObject* go)
    {
        if (player->GetQuestStatus(29778) == QUEST_STATUS_INCOMPLETE)
            player->KilledMonsterCredit(55600);

        go->DestroyForPlayer(player, false);
        return true;
    }
};

class mob_shang_xi_second_air_balloon : public VehicleScript
{
public:
    mob_shang_xi_second_air_balloon() : VehicleScript("mob_shang_xi_second_air_balloon") { }

    void OnAddPassenger(Vehicle* /*veh*/, Unit* passenger, int8 seatId)
    {
        if (seatId == -1)
            if (Player* player = passenger->ToPlayer())
            {
                player->ExitVehicle();

                if (Creature* balloon = player->SummonCreature(55649, 915.55f, 4563.66f, 230.68f, 2.298090f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID()))
                    player->EnterVehicle(balloon, 1);
            }
    }
};

#ifndef __clang_analyzer__
void AddSC_WanderingIsland_West()
{
    new mob_guardian_of_the_elders();
    new mob_master_shang_xi_temple();
    new npc_wind_vehicle();
    new AreaTrigger_at_wind_temple_entrance();
    new mob_aysa_wind_temple_escort();
    new mob_frightened_wind();
    new npc_aysa_in_wind_temple();
    new boss_zhao_ren();
    new npc_rocket_launcher();
    new mob_master_shang_xi_after_zhao();
    new mob_master_shang_xi_after_zhao_escort();
    new mob_master_shang_xi_thousand_staff();
    new mob_master_shang_xi_thousand_staff_escort();
    new mob_shang_xi_air_balloon();
    new npc_ji_firepaw();
    new npc_ji_firepaw_escort();
    new gob_defaced_scroll_of_wisdom();
    new mob_shang_xi_second_air_balloon();
}
#endif
