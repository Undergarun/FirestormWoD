////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "PetBattle.h"

enum
{
    PetBattleTrainerFightActionID = GOSSIP_ACTION_INFO_DEF + 0xABCD
};

class npc_PetBattleTrainer : public CreatureScript
{
    public:
        npc_PetBattleTrainer()
            : CreatureScript("npc_PetBattleTrainer")
        {

        }

        struct npc_PetBattleTrainerAI : CreatureAI
        {
            bool m_IsTrainer;
            uint32 m_QuestID;

            /// Constructor
            /// @p_Me : Creature instance
            npc_PetBattleTrainerAI(Creature* p_Me)
                : CreatureAI(p_Me)
            {
                m_IsTrainer = false;
                m_QuestID = 0;

                if (!sObjectMgr->GetPetBattleTrainerTeam(me->GetEntry()).empty())
                {
                    std::vector<QuestObjective> l_Objs = sObjectMgr->GetQuestObjectivesByType(QUEST_OBJECTIVE_TYPE_PET_BATTLE_TAMER);

                    for (std::size_t l_I = 0; l_I < l_Objs.size(); ++l_I)
                    {
                        if (l_Objs[l_I].ObjectID == p_Me->GetEntry())
                        {
                            m_IsTrainer = true;
                            m_QuestID = l_Objs[l_I].QuestID;
                            break;
                        }
                    }
                }
            }

            /// Update AI
            /// @p_Diff : Time since last update
            void UpdateAI(uint32 const p_Diff) override
            {

            }

            /// On gossip hello
            /// @p_Player : Dialog requester
            void sGossipHello(Player* p_Player) override
            {
                if (me->isQuestGiver())
                    p_Player->PrepareQuestMenu(me->GetGUID());

                ///if (m_IsTrainer && p_Player->HasQuest(m_QuestID))
                ///{
                ///  /*  const char* localizedEntry;
                ///    switch (p_Player->GetSession()->GetSessionDbcLocale())
                ///    {
                ///        case LOCALE_deDE: localizedEntry = LOCALE_INNKEEPER_3; break;
                ///        case LOCALE_enUS: default: localizedEntry = LOCALE_INNKEEPER_0;
                ///    }*/
                ///
                ///    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "GO FIGHT !!", GOSSIP_SENDER_MAIN, PetBattleTrainerFightActionID);
                ///}

                p_Player->TalkedToCreature(me->GetEntry(), me->GetGUID());
                p_Player->SEND_GOSSIP_MENU(p_Player->GetGossipTextId(me), me->GetGUID());
            }

        };

        /// On gossip select
        /// @p_Player : Dialog requester
        /// @p_Sender : Sender entry
        /// @p_Action : Selected action ID
        bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override
        {
            p_Player->PlayerTalkClass->ClearMenus();

            if (p_Action == PetBattleTrainerFightActionID)
            {
                p_Player->CLOSE_GOSSIP_MENU();

                float const l_Distance = 10.f;

                Position l_PlayerPosition;
                {
                    float l_X = p_Creature->m_positionX + (std::cos(p_Creature->m_orientation) * l_Distance);
                    float l_Y = p_Creature->m_positionY + (std::sin(p_Creature->m_orientation) * l_Distance);
                    float l_Z = p_Player->GetMap()->GetHeight(l_X, l_Y, MAX_HEIGHT);

                    l_PlayerPosition.m_positionX = l_X;
                    l_PlayerPosition.m_positionY = l_Y;
                    l_PlayerPosition.m_positionZ = l_Z;

                    l_PlayerPosition.m_orientation = atan2(p_Creature->m_positionY - l_Y, p_Creature->m_positionX - l_X);
                    l_PlayerPosition.m_orientation = (l_PlayerPosition.m_orientation >= 0) ? l_PlayerPosition.m_orientation : 2 * M_PI + l_PlayerPosition.m_orientation;
                }

                Position l_TrainerPosition;
                {
                    l_TrainerPosition.m_positionX    = p_Creature->m_positionX;
                    l_TrainerPosition.m_positionY    = p_Creature->m_positionY;
                    l_TrainerPosition.m_positionZ    = p_Creature->m_positionZ;
                    l_TrainerPosition.m_orientation  = p_Creature->m_orientation;
                }

                Position l_BattleCenterPosition;
                {
                    l_BattleCenterPosition.m_positionX      = (l_PlayerPosition.m_positionX + l_TrainerPosition.m_positionX) / 2;
                    l_BattleCenterPosition.m_positionY      = (l_PlayerPosition.m_positionY + l_TrainerPosition.m_positionY) / 2;
                    l_BattleCenterPosition.m_positionZ      = p_Player->GetMap()->GetHeight(l_BattleCenterPosition.m_positionX, l_BattleCenterPosition.m_positionY, MAX_HEIGHT);
                    l_BattleCenterPosition.m_orientation = l_TrainerPosition.m_orientation + M_PI;
                }

                /// Debug code, show visual position of computed coordinates
                ///p_Player->SummonCreature((uint32)1, l_PlayerPosition, (TempSummonType)TEMPSUMMON_MANUAL_DESPAWN);
                ///p_Player->SummonCreature((uint32)1, l_TrainerPosition, (TempSummonType)TEMPSUMMON_MANUAL_DESPAWN);
                ///p_Player->SummonCreature((uint32)1, l_BattleCenterPosition, (TempSummonType)TEMPSUMMON_MANUAL_DESPAWN);

                PetBattleRequest* l_BattleRequest = sPetBattleSystem->CreateRequest(p_Player->GetGUID());

                l_BattleRequest->OpponentGuid               = p_Creature->GetGUID();
                l_BattleRequest->PetBattleCenterPosition[0] = l_BattleCenterPosition.m_positionX;
                l_BattleRequest->PetBattleCenterPosition[1] = l_BattleCenterPosition.m_positionY;
                l_BattleRequest->PetBattleCenterPosition[2] = l_BattleCenterPosition.m_positionZ;
                l_BattleRequest->BattleFacing               = l_BattleCenterPosition.m_orientation;

                l_BattleRequest->TeamPosition[PETBATTLE_TEAM_1][0] = l_PlayerPosition.m_positionX;
                l_BattleRequest->TeamPosition[PETBATTLE_TEAM_1][1] = l_PlayerPosition.m_positionY;
                l_BattleRequest->TeamPosition[PETBATTLE_TEAM_1][2] = l_PlayerPosition.m_positionZ;

                l_BattleRequest->TeamPosition[PETBATTLE_TEAM_2][0] = l_TrainerPosition.m_positionX;
                l_BattleRequest->TeamPosition[PETBATTLE_TEAM_2][1] = l_TrainerPosition.m_positionY;
                l_BattleRequest->TeamPosition[PETBATTLE_TEAM_2][2] = l_TrainerPosition.m_positionZ;

                l_BattleRequest->RequestType = PETBATTLE_TYPE_PVE;

                eBattlePetRequests l_CanEnterResult = sPetBattleSystem->CanPlayerEnterInPetBattle(p_Player, l_BattleRequest);
                if (l_CanEnterResult != PETBATTLE_REQUEST_OK)
                {
                    p_Player->GetSession()->SendPetBattleRequestFailed(l_CanEnterResult);
                    sPetBattleSystem->RemoveRequest(l_BattleRequest->RequesterGuid);
                    return true;
                }

                ////////////////////////////////
                ////////////////////////////////

                BattlePetInstance::Ptr  l_PlayerPets[MAX_PETBATTLE_SLOTS];
                BattlePetInstance::Ptr  l_WildBattlePets[MAX_PETBATTLE_SLOTS];
                size_t                  l_PlayerPetCount = 0;
                PetBattle*              l_Battle;

                // Temporary pet buffer
                for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
                {
                    l_PlayerPets[l_CurrentPetSlot] = nullptr;
                    l_WildBattlePets[l_CurrentPetSlot] = nullptr;
                }

                ////////////////////////////////
                ////////////////////////////////

                std::vector<BattlePetNpcTeamMember> l_NpcTeam = sObjectMgr->GetPetBattleTrainerTeam(p_Creature->GetEntry());

                uint32 l_WildsPetCount = 0;
                for (BattlePetNpcTeamMember l_Current : l_NpcTeam)
                {
                    if (l_WildsPetCount >= MAX_PETBATTLE_SLOTS)
                        break;

                    std::shared_ptr<BattlePetInstance> l_BattlePetInstance = std::shared_ptr<BattlePetInstance>(new BattlePetInstance());

                    uint32 l_DisplayID = 0;

                    if (sBattlePetSpeciesStore.LookupEntry(l_Current.Specie) && sObjectMgr->GetCreatureTemplate(sBattlePetSpeciesStore.LookupEntry(l_Current.Specie)->entry))
                    {
                        l_DisplayID = sObjectMgr->GetCreatureTemplate(sBattlePetSpeciesStore.LookupEntry(l_Current.Specie)->entry)->Modelid1;

                        if (!l_DisplayID)
                        {
                            l_DisplayID = sObjectMgr->GetCreatureTemplate(sBattlePetSpeciesStore.LookupEntry(l_Current.Specie)->entry)->Modelid2;

                            if (!l_DisplayID)
                            {
                                l_DisplayID = sObjectMgr->GetCreatureTemplate(sBattlePetSpeciesStore.LookupEntry(l_Current.Specie)->entry)->Modelid3;

                                if (!l_DisplayID)
                                {
                                    l_DisplayID = sObjectMgr->GetCreatureTemplate(sBattlePetSpeciesStore.LookupEntry(l_Current.Specie)->entry)->Modelid4;
                                }
                            }
                        }
                    }

                    l_BattlePetInstance->JournalID      = 0;
                    l_BattlePetInstance->Slot           = 0;
                    l_BattlePetInstance->NameTimeStamp  = 0;
                    l_BattlePetInstance->Species        = l_Current.Specie;
                    l_BattlePetInstance->DisplayModelID = l_DisplayID;
                    l_BattlePetInstance->XP             = 0;
                    l_BattlePetInstance->Flags          = 0;
                    l_BattlePetInstance->Health         = 20000;
                    l_BattlePetInstance->Level          = l_Current.Level;
                    l_BattlePetInstance->Breed          = 3;
                    l_BattlePetInstance->Quality        = BATTLEPET_QUALITY_RARE;

                    for (int l_I = 0; l_I < MAX_PETBATTLE_ABILITIES; ++l_I)
                        l_BattlePetInstance->Abilities[l_I] = l_Current.Ability[l_I];

                    l_WildBattlePets[l_WildsPetCount] = l_BattlePetInstance;
                    l_WildBattlePets[l_WildsPetCount]->OriginalCreature = 0;
                    l_WildsPetCount++;
                }

                ////////////////////////////////
                ////////////////////////////////

                // Load player pets
                BattlePet::Ptr * l_PetSlots = p_Player->GetBattlePetCombatTeam();

                for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
                {
                    if (!l_PetSlots[l_CurrentPetSlot])
                        continue;

                    if (l_PlayerPetCount >= MAX_PETBATTLE_SLOTS || l_PlayerPetCount >= p_Player->GetUnlockedPetBattleSlot())
                        break;

                    l_PlayerPets[l_PlayerPetCount] = BattlePetInstance::Ptr(new BattlePetInstance());
                    l_PlayerPets[l_PlayerPetCount]->CloneFrom(l_PetSlots[l_CurrentPetSlot]);
                    l_PlayerPets[l_PlayerPetCount]->Slot = l_PlayerPetCount;
                    l_PlayerPets[l_PlayerPetCount]->OriginalBattlePet = l_PetSlots[l_CurrentPetSlot];

                    ++l_PlayerPetCount;
                }

                ////////////////////////////////
                ////////////////////////////////

                p_Player->GetSession()->SendPetBattleFinalizeLocation(l_BattleRequest);

                // Ok, Create battle
                l_Battle = sPetBattleSystem->CreateBattle();

                // Add player pets
                l_Battle->Teams[PETBATTLE_TEAM_1]->OwnerGuid = p_Player->GetGUID();
                l_Battle->Teams[PETBATTLE_TEAM_1]->PlayerGuid = p_Player->GetGUID();

                l_Battle->Teams[PETBATTLE_TEAM_2]->OwnerGuid = p_Creature->GetGUID();

                for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
                {
                    if (l_PlayerPets[l_CurrentPetSlot])
                        l_Battle->AddPet(PETBATTLE_TEAM_1, l_PlayerPets[l_CurrentPetSlot]);

                    if (l_WildBattlePets[l_CurrentPetSlot])
                        l_Battle->AddPet(PETBATTLE_TEAM_2, l_WildBattlePets[l_CurrentPetSlot]);
                }

                l_Battle->BattleType = l_BattleRequest->RequestType;
                l_Battle->PveBattleType = PVE_PETBATTLE_TRAINER;

                // Launch battle
                p_Player->_petBattleId = l_Battle->ID;

                sPetBattleSystem->RemoveRequest(l_BattleRequest->RequesterGuid);

                for (size_t l_CurrentPetID = 0; l_CurrentPetID < MAX_PETBATTLE_SLOTS; ++l_CurrentPetID)
                {
                    if (l_PlayerPets[l_CurrentPetID])
                        l_PlayerPets[l_CurrentPetID] = nullptr;

                    if (l_WildBattlePets[l_CurrentPetID])
                        l_WildBattlePets[l_CurrentPetID] = nullptr;
                }

                p_Player->GetMotionMaster()->MovePointWithRot(PETBATTLE_ENTER_MOVE_SPLINE_ID, l_PlayerPosition.m_positionX, l_PlayerPosition.m_positionY, l_PlayerPosition.m_positionZ, l_PlayerPosition.m_orientation);
            }
            else
                p_Player->CLOSE_GOSSIP_MENU();

            return true;
        }

        /// Called when a CreatureAI object is needed for the creature.
        /// @p_Creature : Target creature instance
        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_PetBattleTrainerAI(p_Creature);
        }
};

void AddSC_npc_PetBattleTrainer()
{
    new npc_PetBattleTrainer;
}
