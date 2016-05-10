
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "mechanar.h"

/// Gatewatcher Gyro-Kill - 19218
class gatewatcher_gyro_kill : public CreatureScript
{
    public:
        gatewatcher_gyro_kill() : CreatureScript("gatewatcher_gyro_kill") {}

        struct gatewatcher_gyro_killAI : public ScriptedAI
        {
            gatewatcher_gyro_killAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;

            void Reset()
            {
                if (pInstance)
                {
                    if (GameObject* pGob = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_CACHE_LEGION)))
                        pGob->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_LOCKED);
                }
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                if (pInstance)
                    if (GameObject* pGob = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_CACHE_LEGION)))
                            pGob->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_LOCKED);
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new gatewatcher_gyro_killAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_mechanar()
{
    new gatewatcher_gyro_kill();
}
#endif