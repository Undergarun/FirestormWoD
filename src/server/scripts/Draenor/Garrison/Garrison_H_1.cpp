#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "Garrison.h"

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

            /// When the garrison owner started a quest
            virtual void OnQuestStarted(Player * p_Owner, const Quest * p_Quest) override
            {

            }
            /// When the garrison owner reward a quest
            virtual void OnQuestReward(Player * p_Owner, const Quest * p_Quest) override
            {

            }
            /// Get phase mask
            virtual uint32 GetPhaseMask(Player * p_Owner) override
            {
                uint32 l_PhaseMask = 1;

                return l_PhaseMask;
            }

            /// Owner can use the garrison cache ?
            virtual bool CanUseGarrisonCache(Player * p_Owner) override
            {
                return false;
            }

            void OnCreatureCreate(Creature * p_Creature)
            {

            }
            void OnGameObjectCreate(GameObject * p_Gameobject)
            {

            }

            void OnPlayerEnter(Player* p_Player)
            {
                if (!p_Player->IsInWorld())
                    return;

                InstanceScript::OnPlayerEnter(p_Player);
            }

            void Update(uint32 p_Diff)
            {

            }
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