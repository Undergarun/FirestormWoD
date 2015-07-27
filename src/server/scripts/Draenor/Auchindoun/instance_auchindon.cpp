/*
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
#include "InstanceScript.h"
#include "auchindon.hpp"

class instance_auchindon : public InstanceMapScript
{
public:
	instance_auchindon()
		: InstanceMapScript("instance_auchindon", 1182)
	{
	}
	struct instance_auchindon_InstanceMapScript : public InstanceScript
	{
		instance_auchindon_InstanceMapScript(Map* map) : InstanceScript(map) {}

        InstanceScript* instance = this;
		uint32 m_auiEncounter[4];
        // Creatures
        uint64 Nyami;
        uint64 Tuulani;
        uint64 Warden;
        uint64 Gromtash;
        uint64 Durag;
        uint64 Gulkosh;
        // Bosses
        uint64 Kaathar;
        uint64 Nyamiboss;
        uint64 Azzakel;
        uint64 Teronogor;
        // Objects
        uint64 HolyBarrierKathaarObject;
        uint64 CrystalKaathar;
        uint64 Window;
        uint64 FelBarrierAzzakelObject;
        uint64 FelPortal;
        uint64 soultransportstart;
        uint64 soultransport1;
        uint64 soultransport2;
        uint64 soultransport3;
        // Triggers
        uint64 TriggerBubbleMiddleNyami;
        uint64 TriggerAzzakelFelPortal;

		void Initialize()
		{           
            // Creatures
            Nyami = 0;
            Tuulani = 0;
            Warden = 0;
            Gromtash = 0;
            Durag = 0;
            Gulkosh = 0;
            // Bosses
            Kaathar = 0;
            Nyamiboss = 0;
            Azzakel = 0;
            Teronogor = 0;
            // Objects
            HolyBarrierKathaarObject = 0;
            CrystalKaathar = 0;
            Window = 0;
            soultransportstart = 0;
            soultransport1 = 0;
            soultransport2 = 0;
            soultransport3 = 0;
            // Triggers
            TriggerBubbleMiddleNyami = 0;
		}
		void OnGameObjectCreate(GameObject* go)
		{
			switch (go->GetEntry())
			{
            case GAMEOBJECT_HOLY_BARRIER:
                HolyBarrierKathaarObject = go->GetGUID();
                break;          
            case GAMEOBJECT_AUCHINDON_WINDOW:
                Window = go->GetGUID();
                break;
            case GAMEOBJECT_FEL_BARRIER:
                 FelBarrierAzzakelObject = go->GetGUID();
                break;
            //case GAMEOBJECT_DEMONIC_PORTAL:
               //// FelPortal = go->GetGUID();
               // break;
            case GAMEOBJECT_SOUL_TRANSPORT_START:
                soultransportstart = go->GetGUID();
                //go->SetLootState(GO_READY);
               // go->UseDoorOrButton();
                break;
            case GAMEOBJECT_SOUL_TRANSPORT_1:
                soultransport1 = go->GetGUID();
                //go->SetLootState(GO_READY);
                //go->UseDoorOrButton();
                break;
            case GAMEOBJECT_SOUL_TRANSPORT_2:
                soultransport2 = go->GetGUID();
              //  go->SetLootState(GO_READY);
               // go->UseDoorOrButton();
                break;
            case GAMEOBJECT_SOUL_TRANSPORT_3:
                soultransport3 = go->GetGUID();
              //  go->SetLootState(GO_READY);
              //  go->UseDoorOrButton();
                break;
			}
		}
		void OnCreatureCreate(Creature* creature)
		{
			switch (creature->GetEntry())
			{
            case BOSS_KAATHAR:
                Kaathar = creature->GetGUID();
                break;
            case BOSS_NYAMI:
                Nyamiboss = creature->GetGUID();
                break;
            case BOSS_AZAAKEL:
                Azzakel = creature->GetGUID();
                break;
            case BOSS_TERONOGOR:
                Teronogor = creature->GetGUID();
                break;
            case CREATURE_SOUL_BINDER_TUULANI:
                Tuulani = creature->GetGUID();
                break;
            case CREATURE_SOUL_BINDER_NYAMI:
                Nyami = creature->GetGUID();
                break;
            case TRIGGER_SHIELD_SPOT:
                TriggerBubbleMiddleNyami = creature->GetGUID();
                break;
            case CREATURE_WARDEN_AZZAKAEL:
                Warden = creature->GetGUID();
                break;
            case TRIGGER_DEMONS_SUMMONER:
                TriggerAzzakelFelPortal = creature->GetGUID();
                break;
            case CREATURE_GULKOSH:
                Gulkosh = creature->GetGUID();
                break;
            case CREATURE_GROMTASH_THE_DESTRUCTOR:
                Gromtash = creature->GetGUID();
                break;
            case CREATURE_DURAG_THE_DOMINATOR:
                Durag = creature->GetGUID();
                break;
			}
		}
        void OnUnitDeath(Unit* unit)
        {
            Creature* creature = unit->ToCreature();
            if (!creature)
                return;

            switch (creature->GetEntry())
            {
                case BOSS_KAATHAR:
                {
                    if (GameObject* holybarrier = instance->instance->GetGameObject(instance->GetData64(DATA_HOLY_BARRIER)))
                    {
                        holybarrier->Delete();
                    }
                }
                case BOSS_AZAAKEL:
                {
                    if (GameObject* felbarrier = instance->instance->GetGameObject(instance->GetData64(DATA_FEL_BARRIER)))
                    {
                        felbarrier->Delete();
                    }
                    if (GameObject* soultransport = instance->instance->GetGameObject(instance->GetData64(DATA_SOUL_TRANSPORT_START)))
                    {
                        if (Creature* teronogor = instance->instance->GetCreature(instance->GetData64(DATA_TERONOGOR)))
                        {
                            teronogor->GetAI()->DoAction(ACTION_SOUL_MOVE_1);
                        }
                    }
                }
                break;
                // Soul Transport
                case CREATURE_GROMTASH_THE_DESTRUCTOR:
                    if (GameObject* soultransport = instance->instance->GetGameObject(instance->GetData64(DATA_SOUL_TRANSPORT_3)))
                    {
                        if (Creature* teronogor = instance->instance->GetCreature(instance->GetData64(DATA_TERONOGOR)))
                        {
                           teronogor->GetAI()->DoAction(ACTION_SOUL_MOVE_4);
                        }
                    }
                    break;
                case CREATURE_GULKOSH:
                    if (GameObject* soultransport = instance->instance->GetGameObject(instance->GetData64(DATA_SOUL_TRANSPORT_2)))
                    {
                        if (Creature* teronogor = instance->instance->GetCreature(instance->GetData64(DATA_TERONOGOR)))
                        {
                            teronogor->GetAI()->DoAction(ACTION_SOUL_MOVE_3);
                        }
                    }
                    break;
                case CREATURE_DURAG_THE_DOMINATOR:
                {
                    if (GameObject* soultransport = instance->instance->GetGameObject(instance->GetData64(DATA_SOUL_TRANSPORT_1)))
                    {
                        if (Creature* teronogor = instance->instance->GetCreature(instance->GetData64(DATA_TERONOGOR)))
                        {
                            teronogor->GetAI()->DoAction(ACTION_SOUL_MOVE_2);
                        }
                    }
                    break;
                }
                case BOSS_TERONOGOR:
                {
                    if (creature->GetMap()->IsHeroic())
                    {
                        DoCompleteAchievement(9049);
                    }
                    else
                    {
                        DoCompleteAchievement(9039);
                    }

                    // Curtain flames achievement, No Tags Backs! (9552)
                    UnitList targets;
                    JadeCore::AnyUnitHavingBuffInObjectRangeCheck u_check(creature, creature, 100, 153392, true);
                    JadeCore::UnitListSearcher<JadeCore::AnyUnitHavingBuffInObjectRangeCheck> searcher(creature, targets, u_check);
                    creature->VisitNearbyObject(100, searcher);

                    if (targets.empty())
                        return;
                    else
                    {
                        DoCompleteAchievement(9552);
                    }

                    break;
                }
            }
        }
		void SetData(uint32 type, uint32 data)
		{
			switch (type)
			{
			}
		}
		uint32 GetData(uint32 type)
		{
			switch (type)
			{
			}
			return 0;
		}
		uint64 GetData64(uint32 data)
		{
			switch (data)
			{
                // Objects
            case DATA_SOUL_TRANSPORT_START:
                return soultransportstart;
                break;
            case DATA_SOUL_TRANSPORT_1:
                return soultransport1;
                break;
            case DATA_SOUL_TRANSPORT_2:
                return soultransport2;
                break;
            case DATA_SOUL_TRANSPORT_3:
                return soultransport3;
                break;
            case DATA_HOLY_BARRIER:
                return HolyBarrierKathaarObject;
                break;
            case DATA_AUCHINDON_WINDOW:
                return Window;
                break;
            case DATA_FEL_BARRIER:
                return FelBarrierAzzakelObject;
                break;
            case DATA_FEL_PORTAL:
                return FelPortal;
                break;
                // Bosses
            case DATA_KATHAAR:
                return Kaathar;
                break;
            case DATA_AZZAKAEL:
                return Azzakel;
                break;
            case DATA_BOSS_NYAMI:
                return Nyamiboss;
                break;
            case DATA_TERONOGOR:
                return Teronogor;
                break;
                // Creatures
            case DATA_NYAMI:
                return Nyami;
                break;
            case DATA_TUULANI:
                return Tuulani;
                break;  
            case DATA_WARDEN:
                return Warden;
                break;
            case DATA_GULKOSH:
                return Gulkosh;
                break;
            case DATA_GROMTASH:
                return Gromtash;
                break;
            case DATA_DURAG:
                return Durag;
                break;
                // Triggers
            case DATA_TRIGGER_MIDDLE_NYAMI_FIGHT_BUBBLE:
                return TriggerBubbleMiddleNyami;
                break;
            case DATA_TRIGGER_AZZAKEL_CONTROLLER:
                return TriggerAzzakelFelPortal;
                break;
			}
			return 0;
		}
	};

	InstanceScript* GetInstanceScript(InstanceMap* map) const
	{
		return new instance_auchindon_InstanceMapScript(map);
	}
};

void AddSC_instance_auchindon()
{
	new instance_auchindon;
}

