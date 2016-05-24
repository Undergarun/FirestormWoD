////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "GameObjectAI.h"
#include "NPCHandler.h"
#include "Vehicle.h"
#include "PhaseMgr.h"
#include <random>
#include "ObjectMgr.h"
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
#include "MapManager.h"

/// 300007 - World Boss Teleporter
/// No SQL commit with this script, must stay on PTR.
/*
REPLACE INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`) VALUES('300007','0','0','35231','0','0','0','World Boss Teleporter','','','','0','100','100','6','0','35','1','0','1','1.14286','1.14286','1','0','0','1','0','0','1','1','1','518','0','0','0','0','0','0','0','0','0','10','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','1','1','1','1','0','0','0','0','0','0','0','0','0','0','1','0','0','0','npc_world_boss_gossip','1');
DELETE FROM creature WHERE id = 300007;
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES('300007','0','12','7486','1','4294967295','0','0','-9083.16','396.379','93.6052','2.18698','300','0','0','0','0','0','0','0','0','2048','0','0','0','0',NULL);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES('300007','1','14','4982','1','4294967295','0','0','1366.45','-4355.5','26.5794','4.62173','300','0','0','0','0','0','0','0','0','2048','0','0','0','0',NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc2`) VALUE ('14097','Hey, great adventurer, come to me in your faction capital, to access fastly our last testable content ! Newest raids, bosses, get directly teleported to the destination you are looking for.',NULL,'Hey, cher aventurier, venez me voir dans la capitale de votre faction, pour accéder rapidement à notre dernier contenu testable ! Derniers raids, boss, soyez directement téléporté à la destination de votre choix.');
*/
class npc_world_boss_gossip : public CreatureScript
{
    public:
        npc_world_boss_gossip() : CreatureScript("npc_world_boss_gossip")
        {
        }

        enum eActions
        {
            SelectRukhmar     = 1001,
            SelectTarlna      = 1002,
            SelectDrov        = 1003,
            SelectBlackrock   = 1004,
            TpAloneRukhmar    = 1005,
            TpGroupRukhmar    = 1006,
            TpAloneTarlna     = 1007,
            TpGroupTarlna     = 1008,
            TpAloneDrov       = 1009,
            TpGroupDrov       = 1010,
            TpAloneBlackrock  = 1011,
            TpGroupBlackrock  = 1012,
        };

        enum Destinations
        {
            DestinationRukhmar          = 1,
            DestinationTarlna           = 2,
            DestinationDrov             = 3,
            DestinationBlackRockFoundry = 4
        };

        void TeleportPlayer(Player* p_Player, uint8 p_Destination)
        {
            switch (p_Destination)
            {
                case Destinations::DestinationRukhmar:
                    p_Player->TeleportTo(1116, 167.2388f, 2655.39f, 68.58f, 4.105f);
                    break;
                case Destinations::DestinationTarlna:
                    p_Player->TeleportTo(1116, 4918.54f, 1292.16f, 120.28f, 4.018f);
                    break;
                case Destinations::DestinationDrov:
                    p_Player->TeleportTo(1116, 7330.3f, 1455.87f, 81.76f, 6.08f);
                    break;
                case Destinations::DestinationBlackRockFoundry:
                    p_Player->TeleportTo(1116, 7856.9667f, 555.5454f, 124.1314f, 6.08f);
                    break;
            }
        }

        void TeleportGroup(Player* p_Player, uint8 p_Destination)
        {
            std::list<Unit*> l_PlayerList;
            p_Player->GetPartyMembers(l_PlayerList);

            for (Unit* l_GroupMember : l_PlayerList)
            {
                if (l_GroupMember->IsPlayer())
                {
                    switch (p_Destination)
                    {
                        case Destinations::DestinationRukhmar:
                            TeleportPlayer(l_GroupMember->ToPlayer(), Destinations::DestinationRukhmar);
                            break;
                        case Destinations::DestinationTarlna:
                            TeleportPlayer(l_GroupMember->ToPlayer(), Destinations::DestinationTarlna);
                            break;
                        case Destinations::DestinationDrov:
                            TeleportPlayer(l_GroupMember->ToPlayer(), Destinations::DestinationDrov);
                            break;
                        case Destinations::DestinationBlackRockFoundry:
                            p_Player->TeleportTo(1116, 8084.64f, 855.51f, 34.3623f, 6.037f);
                            break;
                    }
                }
            }
        }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
        {
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to go to Rukhmar, please.", GOSSIP_SENDER_MAIN, eActions::SelectRukhmar);
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to go to Tarlna, please.", GOSSIP_SENDER_MAIN, eActions::SelectTarlna);
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to go to Drov, please.", GOSSIP_SENDER_MAIN, eActions::SelectDrov);
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to go Blackrock Foundry, please.", GOSSIP_SENDER_MAIN, eActions::SelectBlackrock);
            p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 p_Action) override
        {
            p_Player->PlayerTalkClass->ClearMenus();
            p_Player->PlayerTalkClass->SendCloseGossip();

            switch (p_Action)
            {
                case eActions::SelectRukhmar:
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I want to fight Rukhmar alone.", GOSSIP_SENDER_MAIN, eActions::TpAloneRukhmar);
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I want to fight Rukhmar with my group.", GOSSIP_SENDER_MAIN, eActions::TpGroupRukhmar);
                    p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());
                    break;
                case eActions::SelectTarlna:
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I want to fight Tarlna alone.", GOSSIP_SENDER_MAIN, eActions::TpAloneTarlna);
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I want to fight Tarlna with my group.", GOSSIP_SENDER_MAIN, eActions::TpGroupTarlna);
                    p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());
                    break;
                case eActions::SelectDrov:
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I want to fight Drov alone.", GOSSIP_SENDER_MAIN, eActions::TpAloneDrov);
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I want to fight Drov with my group.", GOSSIP_SENDER_MAIN, eActions::TpGroupDrov);
                    p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());
                    break;
                case eActions::SelectBlackrock:
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I want to test Blackrock Foundry alone.", GOSSIP_SENDER_MAIN, eActions::TpAloneBlackrock);
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I want to fight Blackrock Foundry with my group.", GOSSIP_SENDER_MAIN, eActions::TpGroupBlackrock);
                    p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());
                    break;
                case eActions::TpAloneRukhmar:
                    TeleportPlayer(p_Player, Destinations::DestinationRukhmar);
                    break;
                case eActions::TpGroupRukhmar:
                    TeleportGroup(p_Player, Destinations::DestinationRukhmar);
                    break;
                case eActions::TpAloneTarlna:
                    TeleportPlayer(p_Player, Destinations::DestinationTarlna);
                    break;
                case eActions::TpGroupTarlna:
                    TeleportGroup(p_Player, Destinations::DestinationTarlna);
                    break;
                case eActions::TpAloneDrov:
                    TeleportPlayer(p_Player, Destinations::DestinationDrov);
                    break;
                case eActions::TpGroupDrov:
                    TeleportGroup(p_Player, Destinations::DestinationDrov);
                    break;
                case eActions::TpAloneBlackrock:
                    TeleportPlayer(p_Player, Destinations::DestinationBlackRockFoundry);
                    break;
                case eActions::TpGroupBlackrock:
                    TeleportGroup(p_Player, Destinations::DestinationBlackRockFoundry);
                    break;
                default:
                    break;
            }

            return true;
        }

        struct npc_world_boss_gossipAI : public ScriptedAI
        {
            npc_world_boss_gossipAI(Creature* creature) : ScriptedAI(creature)
            {
                m_YellTimer = 300 * IN_MILLISECONDS;
            }

            uint32 m_YellTimer;

            void UpdateAI(uint32 const p_Diff) override
            {
                if (m_YellTimer)
                {
                    if (m_YellTimer <= p_Diff)
                    {
                        /// "Hey $N, come to me in your faction capital, to access fastly our last testable content ! Newest raids, boss, get directly teleported to the destination you are looking for."
                        me->YellToZone(14097, LANG_UNIVERSAL, 0);
                        m_YellTimer = 300 * IN_MILLISECONDS;
                    }
                    else
                        m_YellTimer -= p_Diff;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_world_boss_gossipAI(creature);
        }
};

enum eHighmaulDests
{
    DestinationKargath,
    DestinationButcher,
    DestinationBrackenspore,
    DestinationTectus,
    DestinationTwinOgron,
    DestinationKoragh,
    DestinationMargok,
    MaxHighmaulDests
};

Position const g_HighmaulDests[eHighmaulDests::MaxHighmaulDests] =
{
    { 3475.60f, 7590.64f, 55.30f, 4.062f }, ///< Kargath
    { 3698.73f, 7601.06f, 30.03f, 1.178f }, ///< Butcher
    { 3983.92f, 7742.95f, 5.009f, 0.019f }, ///< Brackenspore
    { 3492.97f, 7907.10f, 68.40f, 0.679f }, ///< Tectus
    { 3923.18f, 8310.01f, 322.1f, 0.836f }, ///< Twin Ogron
    { 3864.26f, 8554.24f, 367.6f, 0.903f }, ///< Ko'ragh
    { 4067.41f, 8582.56f, 572.6f, 3.084f }  ///< Imperator Mar'gok
};

/// This class is used to activate tests for Highmaul
class HighmaulTestEnable : public BasicEvent
{
    enum eHighmaulData
    {
        TestsActivated = 1
    };

    public:
        HighmaulTestEnable(uint64 p_Guid) : m_Guid(p_Guid), BasicEvent() { }
        virtual ~HighmaulTestEnable() {}

        virtual bool Execute(uint64 p_EndTime, uint32 p_Time)
        {
            if (Player* l_Player = HashMapHolder<Player>::Find(m_Guid))
            {
                if (InstanceScript* l_Highmaul = l_Player->GetInstanceScript())
                {
                    if (l_Highmaul->GetData(eHighmaulData::TestsActivated))
                        return false;

                    l_Highmaul->SetData(eHighmaulData::TestsActivated, true);
                }
            }

            return true;
        }

        virtual void Abort(uint64 p_EndTime) { }

    private:
        uint64 m_Guid;
};

/// 300008 - Highmaul Tests Manager
/// No SQL commit with this script, must stay on PTR.
/// REPLACE INTO creature_template VALUE ('300008','0','0','35231','0','0','0','PvE Tests Manager','','','','0','103','103','6','0','35','1','0','1','1.14286','1.14286','1','0','0','1','0','0','1','1','1','518','0','0','0','0','0','0','0','0','0','10','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','1','1','1','1','0','0','0','0','0','0','0','0','0','0','1','0','0','0','npc_pve_tests_manager','1');
/// DELETE FROM creature WHERE id = 300008;
/// INSERT INTO creature (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES
/// ('300008','0','12','7486','1','4294967295','0','0','-8932.997','537.873413','94.678825','0.676821','300','0','0','0','0','0','0','0','0','2048','0','0','0','0',NULL),
/// ('300008','1','1637','5170','1','4294967295','0','0','1606.946167','-4384.196289','20.702406','3.469189','300','0','0','0','0','0','0','0','0','2048','0','0','0','0',NULL),
/// ('300008','1116','6755','7367','1','4294967295','0','0','3477.581543','7437.905762','31.542486','4.521338','300','0','0','0','0','0','0','0','0','2048','0','0','0','0',NULL),
/// ('300008','1228','6996','7395','1','4294967295','0','0','3486.217041','7592.922852','10.485300','1.551457','300','0','0','0','0','0','0','0','0','2048','0','0','0','0',NULL);
/// REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc2`) VALUE ('20000','Hey, great adventurer, come to me in your faction capital, to access fastly our last testable content! Newest raids, bosses, get directly teleported to the destination you are looking for.','Hé, cher aventurier, venez me voir dans la capitale de votre faction, pour accéder rapidement à notre dernier contenu testable ! Derniers raids, boss, soyez directement téléporté à la destination de votre choix.');
class npc_pve_tests_manager : public CreatureScript
{
    public:
        npc_pve_tests_manager() : CreatureScript("npc_pve_tests_manager") { }

        enum eActions
        {
            /// Gossip select
            /// Highmaul
            SelectHighmaul      = 1000,
            SelectKargath       = 1001,
            SelectButcher       = 1002,
            SelectBrackenspore  = 1003,
            SelectTectus        = 1004,
            SelectTwinOgron     = 1005,
            SelectKoragh        = 1006,
            SelectMargok        = 1007,
            /// Blackrock Foundry
            SelectBlackrock     = 1008,
            SelectBRFAlone      = 1009,
            SelectBRFGroup      = 1010,
            /// Boss select
            /// Highmaul
            KargathGroup        = 10001,
            ButcherGroup        = 10002,
            BrackensporeGroup   = 10003,
            TectusGroup         = 10004,
            TwinOgronGroup      = 10005,
            KoraghGroup         = 10006,
            MargokGroup         = 10007
        };

        enum eData
        {
            HighmaulMapID = 1228
        };

        bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
        {
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to do some tests in Highmaul.", GOSSIP_SENDER_MAIN, eActions::SelectHighmaul);
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to do some tests in Blackrock Foundry.", GOSSIP_SENDER_MAIN, eActions::SelectBlackrock);
            p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 p_Action) override
        {
            p_Player->PlayerTalkClass->ClearMenus();
            p_Player->PlayerTalkClass->SendCloseGossip();

            if (!p_Creature->IsAIEnabled)
                return true;

            npc_pve_tests_manager::npc_pve_tests_managerAI* l_AI = CAST_AI(npc_pve_tests_manager::npc_pve_tests_managerAI, p_Creature->GetAI());
            if (l_AI == nullptr)
                return true;

            switch (p_Action)
            {
                case eActions::SelectHighmaul:
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to fight against Kargath Bladefist.", GOSSIP_SENDER_MAIN, eActions::SelectKargath);
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to fight against The Butcher.", GOSSIP_SENDER_MAIN, eActions::SelectButcher);
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to fight against Brackenspore.", GOSSIP_SENDER_MAIN, eActions::SelectBrackenspore);
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to fight against Tectus.", GOSSIP_SENDER_MAIN, eActions::SelectTectus);
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to fight against the Twin Ogron.", GOSSIP_SENDER_MAIN, eActions::SelectTwinOgron);
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to fight against Ko'ragh.", GOSSIP_SENDER_MAIN, eActions::SelectKoragh);
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to fight against Imperator Mar'gok.", GOSSIP_SENDER_MAIN, eActions::SelectMargok);
                    p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());
                    break;
                case eActions::SelectKargath:
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to fight Kargath Bladefist with my group.", GOSSIP_SENDER_MAIN, eActions::KargathGroup);
                    p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());
                    break;
                case eActions::SelectButcher:
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to fight The Butcher with my group.", GOSSIP_SENDER_MAIN, eActions::ButcherGroup);
                    p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());
                    break;
                case eActions::SelectBrackenspore:
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to fight Brackenspore with my group.", GOSSIP_SENDER_MAIN, eActions::BrackensporeGroup);
                    p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());
                    break;
                case eActions::SelectTectus:
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to fight Tectus with my group.", GOSSIP_SENDER_MAIN, eActions::TectusGroup);
                    p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());
                    break;
                case eActions::SelectTwinOgron:
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to fight the Twin Ogron with my group.", GOSSIP_SENDER_MAIN, eActions::TwinOgronGroup);
                    p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());
                    break;
                case eActions::SelectKoragh:
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to fight Ko'ragh with my group.", GOSSIP_SENDER_MAIN, eActions::KoraghGroup);
                    p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());
                    break;
                case eActions::SelectMargok:
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to fight Imperator Mar'gok with my group.", GOSSIP_SENDER_MAIN, eActions::MargokGroup);
                    p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());
                    break;
                case eActions::SelectBlackrock:
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to be teleported to Blackrock Foundry alone.", GOSSIP_SENDER_MAIN, eActions::SelectBRFAlone);
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "I want to be teleported to Blackrock Foundry with my group.", GOSSIP_SENDER_MAIN, eActions::SelectBRFGroup);
                    p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());
                    break;
                case eActions::KargathGroup:
                    l_AI->TeleportGroup(p_Player, eHighmaulDests::DestinationKargath);
                    break;
                case eActions::ButcherGroup:
                    l_AI->TeleportGroup(p_Player, eHighmaulDests::DestinationButcher);
                    break;
                case eActions::BrackensporeGroup:
                    l_AI->TeleportGroup(p_Player, eHighmaulDests::DestinationBrackenspore);
                    break;
                case eActions::TectusGroup:
                    l_AI->TeleportGroup(p_Player, eHighmaulDests::DestinationTectus);
                    break;
                case eActions::TwinOgronGroup:
                    l_AI->TeleportGroup(p_Player, eHighmaulDests::DestinationTwinOgron);
                    break;
                case eActions::KoraghGroup:
                    l_AI->TeleportGroup(p_Player, eHighmaulDests::DestinationKoragh);
                    break;
                case eActions::MargokGroup:
                    l_AI->TeleportGroup(p_Player, eHighmaulDests::DestinationMargok);
                    break;
                case eActions::SelectBRFAlone:
                    p_Player->TeleportTo(1116, 8084.64f, 855.51f, 34.3623f, 6.037f);
                    break;
                case eActions::SelectBRFGroup:
                    l_AI->TeleportGroup(p_Player, 8084.64f, 855.51f, 34.3623f, 6.037f);
                    break;
                default:
                    break;
            }

            return true;
        }

        struct npc_pve_tests_managerAI : public ScriptedAI
        {
            npc_pve_tests_managerAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_YellTimer = 300 * IN_MILLISECONDS;
            }

            uint32 m_YellTimer;

            void UpdateAI(uint32 const p_Diff) override
            {
                if (m_YellTimer)
                {
                    if (m_YellTimer <= p_Diff)
                    {
                        /// "Hey $N, come to me in your faction capital, to access fastly our last testable content ! Newest raids, boss, get directly teleported to the destination you are looking for."
                        me->YellToZone(TrinityStrings::WarnRaidTests, LANG_UNIVERSAL, 0);
                        m_YellTimer = 300 * IN_MILLISECONDS;
                    }
                    else
                        m_YellTimer -= p_Diff;
                }
            }

            void TeleportPlayer(Player* p_Player, uint8 p_Destination)
            {
                if (p_Destination >= eHighmaulDests::MaxHighmaulDests)
                    return;

                /// This will prevent players to be re-teleported depending on bosses states
                p_Player->BeginSummon();
                p_Player->TeleportTo(eData::HighmaulMapID, g_HighmaulDests[p_Destination]);

                p_Player->m_Events.AddEvent(new HighmaulTestEnable(p_Player->GetGUID()), 3 * TimeConstants::IN_MILLISECONDS);
            }

            void TeleportGroup(Player* p_Player, uint8 p_Destination)
            {
                if (p_Player->GetGroup() == nullptr)
                {
                    me->MonsterSay("Sorry, but you must be in a raid group to test this boss.", LANG_UNIVERSAL, 0);
                    return;
                }

                std::list<Unit*> l_PlayerList;
                p_Player->GetRaidMembers(l_PlayerList);

                for (Unit* l_GroupMember : l_PlayerList)
                {
                    if (l_GroupMember->GetTypeId() == TypeID::TYPEID_PLAYER)
                        TeleportPlayer(l_GroupMember->ToPlayer(), p_Destination);
                }
            }

            void TeleportGroup(Player* p_Player, float p_X, float p_Y, float p_Z, float p_O)
            {
                if (p_Player->GetGroup() == nullptr)
                {
                    me->MonsterSay("Sorry, but you must be in a raid group to test this boss.", LANG_UNIVERSAL, 0);
                    return;
                }

                std::list<Unit*> l_PlayerList;
                p_Player->GetRaidMembers(l_PlayerList);

                for (Unit* l_GroupMember : l_PlayerList)
                {
                    if (l_GroupMember->GetTypeId() == TypeID::TYPEID_PLAYER)
                        l_GroupMember->ToPlayer()->TeleportTo(1116, p_X, p_Y, p_Z, p_O);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_pve_tests_managerAI(p_Creature);
        }
};

/// Boris <Season 2 Premade Master> - 179317
class npc_season_2_premade_master : public CreatureScript
{
    public:
        npc_season_2_premade_master() : CreatureScript("npc_season_2_premade_master") { }

        enum eOptions
        {
            LevelUp,
            FullEquipment
        };

        enum eItems
        {
            HexweaveBag                     = 114821,
            TomeOfTheClearMind              = 79249,
            ReinsOfTheIllidariFelstalker    = 128425
        };

        enum eMenuIDs
        {
            MenuBaseLevel   = 179317,
            MenuBaseStuff   = 179318,
            MenuUnable      = 179319
        };

        enum eTalks
        {
            TalkSpec,
            TalkError,
            TalkNoSpace,
            TalkStuffOK,
            TalkLevelOK
        };

        enum eVisual
        {
            MountID = 29344
        };

        bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
        {
            if (!p_Player->GetSession()->HasServiceFlags(ServiceFlags::Season2Gold) &&
                !p_Player->GetSession()->HasServiceFlags(ServiceFlags::Season2Item))
            {
                p_Player->PlayerTalkClass->ClearMenus();
                p_Player->SEND_GOSSIP_MENU(eMenuIDs::MenuUnable, p_Creature->GetGUID());
                return true;
            }

            if (p_Player->GetSession()->HasServiceFlags(ServiceFlags::Season2Gold))
            {
                p_Player->PlayerTalkClass->ClearMenus();
                p_Player->ADD_GOSSIP_ITEM_DB(eMenuIDs::MenuBaseLevel, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                p_Player->SEND_GOSSIP_MENU(eMenuIDs::MenuBaseLevel, p_Creature->GetGUID());
                return true;
            }
            else if (p_Player->GetSession()->HasServiceFlags(ServiceFlags::Season2Item))
            {
                p_Player->PlayerTalkClass->ClearMenus();
                p_Player->ADD_GOSSIP_ITEM_DB(eMenuIDs::MenuBaseStuff, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                p_Player->SEND_GOSSIP_MENU(eMenuIDs::MenuBaseStuff, p_Creature->GetGUID());
                return true;
            }

            return false;
        }

        struct npc_season_2_premade_masterAI : public ScriptedAI
        {
            npc_season_2_premade_masterAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->SetUInt32Value(EUnitFields::UNIT_FIELD_MOUNT_DISPLAY_ID, eVisual::MountID);

                me->SetReactState(ReactStates::REACT_PASSIVE);
            }

            void sGossipSelect(Player* p_Player, uint32 p_MenuID, uint32 p_Action) override
            {
                if (p_Player->GetSession()->HasServiceFlags(ServiceFlags::Season2Gold))
                {
                    /// Player doesn't have enough space
                    if (p_Player->GetBagsFreeSlots() < 6)
                    {
                        Talk(eTalks::TalkNoSpace, p_Player->GetGUID());
                        p_Player->PlayerTalkClass->SendCloseGossip();
                        return;
                    }

                    p_Player->GiveLevel(MAX_LEVEL);
                    p_Player->ModifyMoney(50000 * MoneyConstants::GOLD);

                    p_Player->AddItem(eItems::HexweaveBag, 4);
                    p_Player->AddItem(eItems::TomeOfTheClearMind, 200);
                    p_Player->AddItem(eItems::ReinsOfTheIllidariFelstalker, 1);

                    p_Player->GetSession()->UnsetServiceFlags(ServiceFlags::Season2Gold);
                    Talk(eTalks::TalkLevelOK, p_Player->GetGUID());
                }
                else if (p_Player->GetSession()->HasServiceFlags(ServiceFlags::Season2Item))
                {
                    if (!p_Player->GetSpecializationId())
                    {
                        Talk(eTalks::TalkSpec, p_Player->GetGUID());
                        p_Player->PlayerTalkClass->SendCloseGossip();
                        return;
                    }

                    if (!GiveFullPvPEquipment(p_Player))
                    {
                        p_Player->PlayerTalkClass->SendCloseGossip();
                        return;
                    }

                    p_Player->GetSession()->UnsetServiceFlags(ServiceFlags::Season2Item);
                    Talk(eTalks::TalkStuffOK, p_Player->GetGUID());
                }

                p_Player->PlayerTalkClass->SendCloseGossip();
            }

            bool GiveFullPvPEquipment(Player* p_Player)
            {
                LootStore& l_LootStore = LootTemplates_Creature;
                LootTemplate const* l_LootTemplate = l_LootStore.GetLootFor(me->GetCreatureTemplate()->lootid);
                if (l_LootTemplate == nullptr)
                {
                    Talk(eTalks::TalkError, p_Player->GetGUID());
                    return false;
                }

                std::list<ItemTemplate const*>      l_LootTable;
                std::vector<std::vector<uint32>>    l_Items;
                std::set<uint32>                    l_ItemsToAdd;

                l_LootTemplate->FillAutoAssignationLoot(l_LootTable, p_Player, false);

                uint32 l_SpecID     = p_Player->GetSpecializationId();
                uint32 l_ItemCount  = 0;

                l_Items.resize(InventoryType::MAX_INVTYPE);

                /// Fill possible equipment for each slots
                for (uint8 l_Iter = 0; l_Iter < InventoryType::MAX_INVTYPE; ++l_Iter)
                {
                    l_Items[l_Iter].clear();

                    for (ItemTemplate const* l_Template : l_LootTable)
                    {
                        if (l_Template->InventoryType != l_Iter)
                            continue;

                        if (p_Player->CanUseItem(l_Template) != InventoryResult::EQUIP_ERR_OK)
                            continue;

                        if (!l_Template->HasSpec((SpecIndex)l_SpecID, p_Player->getLevel()))
                            continue;

                        l_Items[l_Iter].push_back(l_Template->ItemId);
                    }

                    std::random_shuffle(l_Items[l_Iter].begin(), l_Items[l_Iter].end());
                }

                for (uint8 l_Iter = 0; l_Iter < InventoryType::MAX_INVTYPE; ++l_Iter)
                {
                    uint8 l_Count = 1;

                    switch (l_Iter)
                    {
                        case InventoryType::INVTYPE_FINGER:
                        case InventoryType::INVTYPE_TRINKET:
                        case InventoryType::INVTYPE_WEAPON:
                            l_Count++;
                            break;
                        default:
                            break;
                    }

                    for (uint32 l_Item : l_Items[l_Iter])
                    {
                        if (!l_Count)
                            break;

                        if (l_ItemsToAdd.find(l_Item) != l_ItemsToAdd.end())
                            continue;

                        l_ItemsToAdd.insert(l_Item);
                        --l_Count;
                        ++l_ItemCount;
                    }
                }

                /// Player doesn't have enough space
                if (p_Player->GetBagsFreeSlots() < l_ItemCount)
                {
                    Talk(eTalks::TalkNoSpace, p_Player->GetGUID());
                    return false;
                }

                for (uint32 l_Item : l_ItemsToAdd)
                    p_Player->AddItem(l_Item, 1);

                return true;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_season_2_premade_masterAI(p_Creature);
        }
}; 

class npc_fun_gold_vendor : public CreatureScript
{
    public:
        npc_fun_gold_vendor() : CreatureScript("npc_fun_gold_vendor") { }
    
        bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Give me 50 000 golds! ", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
            return true;
        }
        
        struct npc_fun_gold_vendorAI : public ScriptedAI
        {
            npc_fun_gold_vendorAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }
            
            void sGossipSelect(Player* p_Player, uint32 p_MenuID, uint32 p_Action) override
            {
                p_Player->ModifyMoney(50000 * MoneyConstants::GOLD);
                p_Player->PlayerTalkClass->SendCloseGossip();
            }
        }
        
        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_fun_gold_vendorAI(p_Creature);
        }
};

void AddSC_npc_custom()
{
///    new npc_world_boss_gossip();
    new npc_pve_tests_manager();
    new npc_season_2_premade_master();
    new npc_fun_gold_vendor();
}