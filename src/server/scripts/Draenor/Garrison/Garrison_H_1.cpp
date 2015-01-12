#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "Garrison.h"

enum
{
    QUEST_ETABLISH_YOUR_GARRISON_H  = 34378,
    QUEST_WHAT_WE_GOT               = 34824,
};

enum
{
    GARRISON_PHASE_BASE             = 0x0001,
};

/// TerrainSwap : See WorldMapArea.dbc
enum
{
    TERRAIN_SWAP_GARRISON_FF_HORDE_TIER_1 = 980
};

class instance_Garrison_H1 : public InstanceMapScript
{
    public:
        /// Constructor
        instance_Garrison_H1()
            : InstanceMapScript("instance_Garrison_H1", 1158)
        {

        }

        struct instance_Garrison_H1MapScript : public InstanceScript, public GarrisonInstanceScriptBase
        {
            /// Constructor
            instance_Garrison_H1MapScript(Map* p_Map)
                : InstanceScript(p_Map)
            {

            }
            /// Destructor
            ~instance_Garrison_H1MapScript()
            {
                for (uint64 p_Guid : m_Players)
                {
                    Player * l_Player = sObjectAccessor->FindPlayer(p_Guid);

                    if (l_Player && l_Player->GetGarrison())
                        l_Player->GetGarrison()->_SetGarrisonScript(nullptr);
                }
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            /// When the garrison owner started a quest
            virtual void OnQuestStarted(Player * p_Owner, const Quest * p_Quest) override
            {

            }
            /// When the garrison owner reward a quest
            virtual void OnQuestReward(Player * p_Owner, const Quest * p_Quest) override
            {

            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            /// Get phase mask
            virtual uint32 GetPhaseMask(Player * p_Owner) override
            {
                uint32 l_PhaseMask = GARRISON_PHASE_BASE;

                return l_PhaseMask;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            /// Owner can use the garrison cache ?
            virtual bool CanUseGarrisonCache(Player * p_Owner) override
            {
                /// Temp hack
                return true;
                if (p_Owner->GetQuestStatus(QUEST_WHAT_WE_GOT) == QUEST_STATUS_REWARDED
                    || p_Owner->GetQuestStatus(QUEST_WHAT_WE_GOT) == QUEST_STATUS_INCOMPLETE)
                    return true;

                return false;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            /// When a mission start
            virtual void OnMissionStart(uint32 p_MissionID, std::vector<uint32> p_Followers) override
            {

            }
            /// When a construction start
            virtual void OnPurchaseBuilding(uint32 p_BuildingID) override
            {

            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            virtual void OnCreatureCreate(Creature * p_Creature) override
            {

            }
            virtual void OnGameObjectCreate(GameObject * p_Gameobject) override
            {

            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            /// Called when a player successfully enters the instance.
            virtual void OnPlayerEnter(Player* p_Player) override
            {
                if (!p_Player->IsInWorld())
                    return;

                if (!p_Player->GetGarrison())
                    return;

                InstanceScript::OnPlayerEnter(p_Player);

                p_Player->GetGarrison()->_SetGarrisonScript(this);
                m_Players.emplace(p_Player->GetGUID());
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            virtual void Update(uint32 p_Diff) override
            {

            }

            std::set<uint64> m_Players;
        };

        InstanceScript * GetInstanceScript(InstanceMap* p_Map) const
        {
            return new instance_Garrison_H1MapScript(p_Map);
        }
};

void AddSC_Garrison_H1()
{
    new instance_Garrison_H1();
}