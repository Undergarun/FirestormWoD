/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#include "BattlegroundAB.h"
#include "BattlegroundWS.h"
#include "BattlegroundIC.h"
#include "BattlegroundSA.h"
#include "BattlegroundAV.h"
#include "Vehicle.h"

class achievement_resilient_victory : public MS::Game::Scripting::Interfaces::AchievementCriteriaScript
{
    public:
        achievement_resilient_victory() : MS::Game::Scripting::Interfaces::AchievementCriteriaScript("achievement_resilient_victory") { }

        bool OnCheck(Player* source, Unit* /*target*/)
        {
            Battleground* bg = source->GetBattleground();
            if (!bg)
                return false;

            if (bg->GetTypeID(true) != BATTLEGROUND_AB)
                return false;

            if (!static_cast<BattlegroundAB*>(bg)->IsTeamScores500Disadvantage(source->GetTeam()))
                return false;

            return true;
        }
};

class achievement_bg_control_all_nodes : public MS::Game::Scripting::Interfaces::AchievementCriteriaScript
{
    public:
        achievement_bg_control_all_nodes() : MS::Game::Scripting::Interfaces::AchievementCriteriaScript("achievement_bg_control_all_nodes") { }

        bool OnCheck(Player* source, Unit* /*target*/)
        {
            Battleground* bg = source->GetBattleground();
            if (!bg)
                return false;

            if (!bg->IsAllNodesControlledByTeam(source->GetTeam()))
                return false;

            return true;
        }
};

class achievement_save_the_day : public MS::Game::Scripting::Interfaces::AchievementCriteriaScript
{
    public:
        achievement_save_the_day() : MS::Game::Scripting::Interfaces::AchievementCriteriaScript("achievement_save_the_day") { }

        bool OnCheck(Player* source, Unit* target)
        {
            if (!target)
                return false;

            if (Player const* player = target->ToPlayer())
            {
                Battleground* bg = source->GetBattleground();
                if (!bg)
                    return false;

                if (bg->GetTypeID(true) != BATTLEGROUND_WS)
                    return false;

                if (static_cast<BattlegroundWS*>(bg)->GetFlagState(player->GetTeam()) == BG_WS_FLAG_STATE_ON_BASE)
                    return true;
            }
            return false;
        }
};

class achievement_bg_ic_resource_glut : public MS::Game::Scripting::Interfaces::AchievementCriteriaScript
{
    public:
        achievement_bg_ic_resource_glut() : MS::Game::Scripting::Interfaces::AchievementCriteriaScript("achievement_bg_ic_resource_glut") { }

        bool OnCheck(Player* source, Unit* /*target*/)
        {
            if (source->HasAura(SPELL_OIL_REFINERY) && source->HasAura(SPELL_QUARRY))
                return true;

            return false;
        }
};

class achievement_bg_ic_glaive_grave : public MS::Game::Scripting::Interfaces::AchievementCriteriaScript
{
    public:
        achievement_bg_ic_glaive_grave() : MS::Game::Scripting::Interfaces::AchievementCriteriaScript("achievement_bg_ic_glaive_grave") { }

        bool OnCheck(Player* source, Unit* /*target*/)
        {
            if (Creature* vehicle = source->GetVehicleCreatureBase())
            {
                if (vehicle->GetEntry() == NPC_GLAIVE_THROWER_H ||  vehicle->GetEntry() == NPC_GLAIVE_THROWER_A)
                    return true;
            }

            return false;
        }
};

class achievement_bg_ic_mowed_down : public MS::Game::Scripting::Interfaces::AchievementCriteriaScript
{
    public:
        achievement_bg_ic_mowed_down() : MS::Game::Scripting::Interfaces::AchievementCriteriaScript("achievement_bg_ic_mowed_down") { }

        bool OnCheck(Player* source, Unit* /*target*/)
        {
            if (Creature* vehicle = source->GetVehicleCreatureBase())
            {
                if (vehicle->GetEntry() == NPC_KEEP_CANNON)
                    return true;
            }

            return false;
        }
};

class achievement_bg_sa_artillery : public MS::Game::Scripting::Interfaces::AchievementCriteriaScript
{
    public:
        achievement_bg_sa_artillery() : MS::Game::Scripting::Interfaces::AchievementCriteriaScript("achievement_bg_sa_artillery") { }

        bool OnCheck(Player* source, Unit* /*target*/)
        {
            if (Creature* vehicle = source->GetVehicleCreatureBase())
            {
                if (vehicle->GetEntry() == NPC_ANTI_PERSONNAL_CANNON)
                    return true;
            }

            return false;
        }
};

class achievement_arena_kills : public MS::Game::Scripting::Interfaces::AchievementCriteriaScript
{
    public:
        achievement_arena_kills(char const* name, uint8 arenaType) : MS::Game::Scripting::Interfaces::AchievementCriteriaScript(name),
            _arenaType(arenaType)
        {
        }

        bool OnCheck(Player* source, Unit* /*target*/)
        {
            // this checks GetBattleground() for NULL already
            if (!source->InArena())
                return false;

            return source->GetBattleground()->GetArenaType() == _arenaType;
        }

    private:
        uint8 const _arenaType;
};

class achievement_sickly_gazelle : public MS::Game::Scripting::Interfaces::AchievementCriteriaScript
{
public:
    achievement_sickly_gazelle() : MS::Game::Scripting::Interfaces::AchievementCriteriaScript("achievement_sickly_gazelle") { }

    bool OnCheck(Player* /*source*/, Unit* target)
    {
        if (!target)
            return false;

        if (Player* victim = target->ToPlayer())
            if (victim->IsMounted())
                return true;

        return false;
    }
};

class achievement_everything_counts : public MS::Game::Scripting::Interfaces::AchievementCriteriaScript
{
    public:
        achievement_everything_counts() : MS::Game::Scripting::Interfaces::AchievementCriteriaScript("achievement_everything_counts") { }

        bool OnCheck(Player* source, Unit* /*target*/)
        {
            Battleground* bg = source->GetBattleground();
            if (!bg)
                return false;

            if (bg->GetTypeID(true) != BATTLEGROUND_AV)
                return false;

            if (static_cast<BattlegroundAV*>(bg)->IsBothMinesControlledByTeam(source->GetTeam()))
                return true;

            return false;
        }
};

class achievement_bg_av_perfection : public MS::Game::Scripting::Interfaces::AchievementCriteriaScript
{
    public:
        achievement_bg_av_perfection() : MS::Game::Scripting::Interfaces::AchievementCriteriaScript("achievement_bg_av_perfection") { }

        bool OnCheck(Player* source, Unit* /*target*/)
        {
            Battleground* bg = source->GetBattleground();
            if (!bg)
                return false;

            if (bg->GetTypeID(true) != BATTLEGROUND_AV)
                return false;

            if (static_cast<BattlegroundAV*>(bg)->IsAllTowersControlledAndCaptainAlive(source->GetTeam()))
                return true;

            return false;
        }
};

class achievement_bg_sa_defense_of_ancients : public MS::Game::Scripting::Interfaces::AchievementCriteriaScript
{
    public:
        achievement_bg_sa_defense_of_ancients() : MS::Game::Scripting::Interfaces::AchievementCriteriaScript("achievement_bg_sa_defense_of_ancients")
        {
        }

        bool OnCheck(Player* player, Unit* /*target*/)
        {
            if (!player)
                return false;

            Battleground* battleground = player->GetBattleground();
            if (!battleground)
                return false;

            if (player->GetTeamId() == static_cast<BattlegroundSA*>(battleground)->Attackers)
                return false;

            if (!static_cast<BattlegroundSA*>(battleground)->gateDestroyed)
                return true;

            return false;
        }
};

enum ArgentTournamentAreas
{
    AREA_ARGENT_TOURNAMENT_FIELDS  = 4658,
    AREA_RING_OF_ASPIRANTS         = 4670,
    AREA_RING_OF_ARGENT_VALIANTS   = 4671,
    AREA_RING_OF_ALLIANCE_VALIANTS = 4672,
    AREA_RING_OF_HORDE_VALIANTS    = 4673,
    AREA_RING_OF_CHAMPIONS         = 4669,
};

class achievement_tilted : public MS::Game::Scripting::Interfaces::AchievementCriteriaScript
{
    public:
        achievement_tilted() : MS::Game::Scripting::Interfaces::AchievementCriteriaScript("achievement_tilted") {}

        bool OnCheck(Player* player, Unit* /*target*/)
        {
            if (!player)
                return false;

            bool checkArea = player->GetAreaId() == AREA_ARGENT_TOURNAMENT_FIELDS ||
                                player->GetAreaId() == AREA_RING_OF_ASPIRANTS ||
                                player->GetAreaId() == AREA_RING_OF_ARGENT_VALIANTS ||
                                player->GetAreaId() == AREA_RING_OF_ALLIANCE_VALIANTS ||
                                player->GetAreaId() == AREA_RING_OF_HORDE_VALIANTS ||
                                player->GetAreaId() == AREA_RING_OF_CHAMPIONS;

            return checkArea && player->m_Duel && player->m_Duel->isMounted;
        }
};

class achievement_not_even_a_scratch : public MS::Game::Scripting::Interfaces::AchievementCriteriaScript
{
    public:
        achievement_not_even_a_scratch() : MS::Game::Scripting::Interfaces::AchievementCriteriaScript("achievement_not_even_a_scratch") { }

        bool OnCheck(Player* source, Unit* /*target*/)
        {
            if (!source)
                return false;

            Battleground* battleground = source->GetBattleground();
            if (!battleground)
                return false;

            if (static_cast<BattlegroundSA*>(battleground)->notEvenAScratch(source->GetTeam()))
                return true;

            return false;
        }
};

void AddSC_achievement_scripts()
{
    new achievement_resilient_victory();
    new achievement_bg_control_all_nodes();
    new achievement_save_the_day();
    new achievement_bg_ic_resource_glut();
    new achievement_bg_ic_glaive_grave();
    new achievement_bg_ic_mowed_down();
    new achievement_bg_sa_artillery();
    new achievement_sickly_gazelle();
    new achievement_everything_counts();
    new achievement_bg_av_perfection();
    new achievement_arena_kills("achievement_arena_2v2_kills", ArenaType::Arena2v2);
    new achievement_arena_kills("achievement_arena_3v3_kills", ArenaType::Arena3v3);
    new achievement_arena_kills("achievement_arena_5v5_kills", ArenaType::Arena5v5);
    new achievement_bg_sa_defense_of_ancients();
    new achievement_tilted();
    new achievement_not_even_a_scratch();
}
