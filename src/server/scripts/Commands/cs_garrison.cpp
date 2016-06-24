#include "ScriptMgr.h"
#include "GameEventMgr.h"
#include "Chat.h"
#include "GarrisonMgr.hpp"
#include "ObjectMgr.h"
#include "../Draenor/Garrison/GarrisonScriptData.hpp"

/// Garrison commands
class garrison_commandscript: public CommandScript
{
    public:
        /// Constructor
        garrison_commandscript()
            : CommandScript("garrison_commandscript")
        {

        }

        /// Get command table
        ChatCommand* GetCommands() const
        {
            static ChatCommand blueprintCommandTable[] =
            {
                { "learn", SEC_ADMINISTRATOR, true,  &HandleBlueprintLearnCommand, "", NULL },
                { NULL,        0,             false, NULL,                         "", NULL }
            };

            static ChatCommand plotCommandTable[] =
            {
                { "info",    SEC_ADMINISTRATOR, true,  &HandlePlotInfoCommand,   "", NULL },
                { "add",     SEC_ADMINISTRATOR, true,  &HandlePlotAddCommand,    "", NULL },
                { "del",     SEC_ADMINISTRATOR, true,  &HandlePlotDelCommand,    "", NULL },
                { "import",  SEC_ADMINISTRATOR, true,  &HandlePlotImportCommand, "", NULL },
                { NULL,        0,               false, NULL,                     "", NULL }
            };

            static ChatCommand followerCommandTable[] =
            {
                { "add",        SEC_ADMINISTRATOR, true,  &HandleFollowerAddCommand,     "", NULL },
                { "addlist",    SEC_ADMINISTRATOR, true,  &HandleFolloweraddlistCommand, "", NULL },
                { "remove",     SEC_ADMINISTRATOR, true,  &HandleFollowerRemoveCommand,  "", NULL },
                { "reroll",     SEC_ADMINISTRATOR, true,  &HandleFollowerRerollCommand,  "", NULL },
                { "list",       SEC_ADMINISTRATOR, true,  &HandleFollowerListCommand,    "", NULL },
                { "promote",    SEC_ADMINISTRATOR, true,  &HandleFollowerPromoteCommand, "", NULL },
                { NULL,        0,                  false, NULL,                          "", NULL }
            };

            static ChatCommand missionCommandTable[] =
            {
                { "add",            SEC_ADMINISTRATOR, true,  &HandleMissionAddCommand,         "", NULL },
                { "completeall",    SEC_ADMINISTRATOR, true,  &HandleMissionCompleteAllCommand, "", NULL },
                { "reroll",         SEC_ADMINISTRATOR, true,  &HandleMissionRerollCommand,      "", NULL },
                { NULL,        0,                      false, NULL,                             "", NULL }
            };

            static ChatCommand buildingCommandTable[] =
            {
                { "complete",         SEC_ADMINISTRATOR, true,  &HandleBuildingCompleteCommand,         "", NULL },
                { NULL,               0,                 false, NULL,                                   "", NULL }
            };

            static ChatCommand shipmentCommandTable[] =
            {
                { "complete",         SEC_ADMINISTRATOR, true,  &HandleShipmentCompleteCommand,         "", NULL },
                { NULL,               0,                 false, NULL,                                   "", NULL }
            };

            static ChatCommand garrisonCommandTable[] =
            {
                { "blueprint", SEC_ADMINISTRATOR,  true,   NULL, "",       blueprintCommandTable },
                { "plot",      SEC_ADMINISTRATOR,  true,   NULL, "",       plotCommandTable      },
                { "follower",  SEC_ADMINISTRATOR,  true,   NULL, "",       followerCommandTable  },
                { "mission" ,  SEC_ADMINISTRATOR,  true,   NULL, "",       missionCommandTable   },
                { "building",  SEC_ADMINISTRATOR,  true,   NULL, "",       buildingCommandTable  },
                { "shipment",  SEC_ADMINISTRATOR,  true,   NULL, "",       shipmentCommandTable  },
                { "info",      SEC_ADMINISTRATOR,  true,   &HandleGarrisonInfo,         "", NULL },
                { "setlevel",  SEC_ADMINISTRATOR,  true,   &HandleGarrisonSetLevel,     "", NULL },
                { "create",    SEC_ADMINISTRATOR,  true,   &HandleGarrisonCreate,       "", NULL },
                { "prepare",   SEC_ADMINISTRATOR,  true,   &HandleGarrisonPrepare,       "", NULL },
                { "delete",    SEC_ADMINISTRATOR,  true,   &HandleGarrisonDelete,       "", NULL },
                { "resetdata", SEC_ADMINISTRATOR,  true,   &HandleGarrisonResetDatas,   "", NULL },
                { NULL,        0,                  false,  NULL,                        "", NULL }
            };

            static ChatCommand shipyardCommandTable[] =
            {
                { "open",       SEC_ADMINISTRATOR, false, &HandleShipyardOpen, NULL      }, ///< Missing field 'ChildCommands' initializer
                { NULL,         0,                 false, NULL, "", NULL                 }
            };

            static ChatCommand commandTable[] =
            {
                { "garrison",   SEC_ADMINISTRATOR, false, NULL, "", garrisonCommandTable },
                { "shipyard",   SEC_ADMINISTRATOR, false, NULL, "", shipyardCommandTable },
                { NULL,         0,                 false, NULL, "", NULL                 }
            };
            return commandTable;
        }

        static bool HandleGarrisonInfo(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            const GarrSiteLevelEntry* l_Entry = l_TargetPlayer->GetGarrison()->GetGarrisonSiteLevelEntry();

            if (!l_Entry)
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            p_Handler->PSendSysMessage("Garrison info");
            p_Handler->PSendSysMessage("Level : %u SiteLevelID : %u MapID : %u FactionID : %u", l_Entry->Level, l_Entry->SiteID, l_Entry->MapID, (uint32)l_TargetPlayer->GetGarrison()->GetGarrisonFactionIndex());
            p_Handler->PSendSysMessage("Cache Resource : %u", l_TargetPlayer->GetGarrison()->GetGarrisonCacheTokenCount());

            return true;
        }

        static bool HandleGarrisonSetLevel(ChatHandler* p_Handler, char const* p_Args)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (p_Args != 0)
            {
                uint32 l_Level = atoi(p_Args);

                if (!l_Level)
                    return false;

                l_TargetPlayer->GetGarrison()->SetLevel(l_Level);
            }

            return true;
        }

        /// This is a debug command, only devs should use it
        static bool HandleGarrisonPrepare(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            Player* l_Target = p_Handler->getSelectedPlayer();
            if (!l_Target)
            {
                p_Handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            /// check online security
            if (p_Handler->HasLowerSecurity(l_Target, 0))
                return false;

            std::string targetNameLink = p_Handler->GetNameLink(l_Target);

            if (l_Target->isInFlight())
            {
                p_Handler->PSendSysMessage(LANG_CHAR_IN_FLIGHT, targetNameLink.c_str());
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            /// Set higher speed
            l_Target->SetSpeed(MOVE_WALK, 5.0f, true);
            l_Target->SetSpeed(MOVE_RUN, 5.0f, true);
            l_Target->SetSpeed(MOVE_SWIM, 5.0f, true);
            l_Target->SetSpeed(MOVE_FLIGHT, 5.0f, true);

            /// Set fly
            l_Target->SetCanFly(true);

            /// Set Garrison resources + money at high values
            l_Target->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_GARRISON_RESSOURCES, 10000, true, true);
            l_Target->ModifyMoney(1000000000);

            /// Set max level to be able to test everything
            if (l_Target->getLevel() < 90)
                l_Target->SetLevel(100);

            /// Create Garrison if player doesn't have it
            HandleGarrisonCreate(p_Handler, "");

            return true;
        }

        static bool HandleGarrisonCreate(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer)
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (l_TargetPlayer->GetGarrison())
            {
                p_Handler->PSendSysMessage("Player already have a garrison");
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            l_TargetPlayer->CreateGarrison();

            uint32 l_MovieID = l_TargetPlayer->GetGarrison()->GetGarrisonSiteLevelEntry()->MovieID;
            uint32 l_MapID   = l_TargetPlayer->GetGarrison()->GetGarrisonSiteLevelEntry()->MapID;
            uint32 l_TeamID  = l_TargetPlayer->GetTeamId();

            l_TargetPlayer->AddMovieDelayedTeleport(l_MovieID, l_MapID, MS::Garrison::gGarrisonCreationCoords[l_TeamID][0],
                                                                        MS::Garrison::gGarrisonCreationCoords[l_TeamID][1],
                                                                        MS::Garrison::gGarrisonCreationCoords[l_TeamID][2],
                                                                        MS::Garrison::gGarrisonCreationCoords[l_TeamID][3]);

            l_TargetPlayer->SendMovieStart(l_MovieID);

            if (l_TeamID == TEAM_ALLIANCE)
            {
                l_TargetPlayer->AddQuest(sObjectMgr->GetQuestTemplate(MS::Garrison::Quests::QUEST_ETABLISH_YOUR_GARRISON_A), l_TargetPlayer);
                l_TargetPlayer->CompleteQuest(MS::Garrison::Quests::QUEST_ETABLISH_YOUR_GARRISON_A);
            }
            else if (l_TeamID == TEAM_HORDE)
            {
                l_TargetPlayer->AddQuest(sObjectMgr->GetQuestTemplate(MS::Garrison::Quests::QUEST_ETABLISH_YOUR_GARRISON_H), l_TargetPlayer);
                l_TargetPlayer->CompleteQuest(MS::Garrison::Quests::QUEST_ETABLISH_YOUR_GARRISON_H);
            }

            /// HACK until shadowmoon quest are done : add follower Qiana Moonshadow / Olin Umberhide
            l_TargetPlayer->GetGarrison()->AddFollower(34);
            l_TargetPlayer->GetGarrison()->AddFollower(89);
            l_TargetPlayer->GetGarrison()->AddFollower(92);

            return true;
        }

        static bool HandleGarrisonDelete(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer)
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (!l_TargetPlayer->GetGarrison())
            {
                p_Handler->PSendSysMessage("Player doesnt have a garrison");
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            l_TargetPlayer->DeleteGarrison();

            if (l_TargetPlayer->GetCurrency(MS::Garrison::Globals::CurrencyID, false))
                l_TargetPlayer->ModifyCurrency(MS::Garrison::Globals::CurrencyID, -(int32)l_TargetPlayer->GetCurrency(MS::Garrison::Globals::CurrencyID, false));

            return true;
        }

        static bool HandleGarrisonResetDatas(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer)
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (!l_TargetPlayer->GetGarrison())
            {
                p_Handler->PSendSysMessage("Player doesnt have a garrison");
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            l_TargetPlayer->ResetDailyGarrisonDatas();

            return true;
        }

        static bool HandleBlueprintLearnCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (p_Args != 0)
            {
                std::string l_Args = p_Args;

                if (l_Args == "all")
                {
                    for (uint32 l_I = 0; l_I < sGarrBuildingStore.GetNumRows(); ++l_I)
                    {
                        const GarrBuildingEntry* l_Entry = sGarrBuildingStore.LookupEntry(l_I);

                        if (l_Entry)
                            l_TargetPlayer->GetGarrison()->LearnBlueprint(l_Entry->ID);
                    }

                    return true;
                }
                else
                {
                    uint32 l_Blueprint = atoi(p_Args);

                    if (!l_Blueprint)
                        return false;

                    return l_TargetPlayer->GetGarrison()->LearnBlueprint(l_Blueprint);
                }
            }

            return true;
        }

        static bool HandlePlotInfoCommand(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            Player* l_TargetPlayer = p_Handler->GetSession()->GetPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage("You don't have a garrison");
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            MS::Garrison::GarrisonPlotInstanceInfoLocation l_Info = l_TargetPlayer->GetGarrison()->GetPlot(l_TargetPlayer->GetPositionX(), l_TargetPlayer->GetPositionY(), l_TargetPlayer->GetPositionZ());
            if (!l_Info.PlotInstanceID)
            {
                p_Handler->SendSysMessage("Plot not found");
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            p_Handler->PSendSysMessage("Plot instance id %u type %u", l_Info.PlotInstanceID, l_TargetPlayer->GetGarrison()->GetPlotType(l_Info.PlotInstanceID));
            p_Handler->PSendSysMessage("Position %f %f %f %f", l_Info.X, l_Info.Y, l_Info.Z, l_Info.O);

            MS::Garrison::GarrisonBuilding l_Building = l_TargetPlayer->GetGarrison()->GetBuilding(l_Info.PlotInstanceID);

            if (l_Building.BuildingID)
            {
                const GarrBuildingEntry* l_Entry = sGarrBuildingStore.LookupEntry(l_Building.BuildingID);

                p_Handler->PSendSysMessage("Building : %u - %s", l_Entry->ID, l_TargetPlayer->GetGarrison()->GetGarrisonFactionIndex() == MS::Garrison::FactionIndex::Alliance ? l_Entry->NameAlliance : l_Entry->NameHorde);
                p_Handler->PSendSysMessage("Active %u Level %u", l_Building.Active, l_Entry->Level);
            }

            float l_X = l_TargetPlayer->GetPositionX();
            float l_Y = l_TargetPlayer->GetPositionY();
            float l_Z = l_TargetPlayer->GetPositionZ();
            float l_O = l_TargetPlayer->GetOrientation();

            G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
            l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), -l_Info.O);

            G3D::Vector3 l_ElementPosition = G3D::Vector3(l_X, l_Y, 0);
            l_ElementPosition  -= G3D::Vector3(l_Info.X, l_Info.Y, 0);
            l_ElementPosition   = l_Mat *l_ElementPosition;
            l_ElementPosition.z = l_Z - l_Info.Z;

            float l_Orientation = Position::NormalizeOrientation((2 * M_PI) - Position::NormalizeOrientation(l_Info.O - l_O));

            p_Handler->PSendSysMessage("Relative location X:%f Y:%f Z:%f O:%f", l_ElementPosition.x, l_ElementPosition.y, l_ElementPosition.z, l_Orientation);

#ifdef WIN32
            char   l_Buffer[120];
            char* lPtrData = nullptr;

            sprintf(l_Buffer, "{ %.4ff, %.4ff, %.4ff, %.4ff },", l_ElementPosition.x, l_ElementPosition.y, l_ElementPosition.z, l_Orientation);

            HANDLE l_Handle;

            int l_BufferSize = strlen(l_Buffer);

            l_Handle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, l_BufferSize + 1);

            lPtrData = (char*)GlobalLock(l_Handle);
            memcpy(lPtrData, l_Buffer, l_BufferSize + 1);

            GlobalUnlock(l_Handle);

            OpenClipboard(nullptr);
            EmptyClipboard();
            SetClipboardData(CF_TEXT, l_Handle);
            CloseClipboard();
#endif

            return true;
        }

        static bool HandlePlotAddCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            Player* l_Player = p_Handler->GetSession()->GetPlayer();

            if (!l_Player || !l_Player->GetGarrison())
            {
                p_Handler->SendSysMessage("You don't have a garrison");
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            MS::Garrison::GarrisonPlotInstanceInfoLocation l_Info = l_Player->GetGarrison()->GetPlot(l_Player->GetPositionX(), l_Player->GetPositionY(), l_Player->GetPositionZ());

            if (!l_Info.PlotInstanceID)
            {
                p_Handler->SendSysMessage("Plot not found");
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            MS::Garrison::GarrisonBuilding l_Building = l_Player->GetGarrison()->GetBuilding(l_Info.PlotInstanceID);

            if (!l_Building.BuildingID)
            {
                p_Handler->SendSysMessage("Building not found");
                p_Handler->SetSentErrorMessage(true);
            }

            int32 l_ElemEntry = atoi(p_Args);
            float l_X = l_Player->GetPositionX();
            float l_Y = l_Player->GetPositionY();
            float l_Z = l_Player->GetPositionZ();
            float l_O = l_Player->GetOrientation();

            G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
            l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), -l_Info.O);

            G3D::Vector3 l_ElementPosition = G3D::Vector3(l_X, l_Y, 0);
            l_ElementPosition -= G3D::Vector3(l_Info.X, l_Info.Y, 0);
            l_ElementPosition = l_Mat *l_ElementPosition;
            l_ElementPosition.z = l_Z - l_Info.Z;

            if (l_ElemEntry > 0)
                l_ElementPosition.z += 0.5f;

            int32 l_TableIndex = l_Player->GetGarrison()->GetPlotType(l_Info.PlotInstanceID);
            if (l_Building.Active)
                l_TableIndex = -int32(l_Building.BuildingID);

            GarrisonPlotBuildingContent l_NewData;
            l_NewData.PlotTypeOrBuilding    = l_TableIndex;
            l_NewData.CreatureOrGob         = l_ElemEntry;
            l_NewData.FactionIndex          = l_Player->GetGarrison()->GetGarrisonFactionIndex();
            l_NewData.X                     = l_ElementPosition.x;
            l_NewData.Y                     = l_ElementPosition.y;
            l_NewData.Z                     = l_ElementPosition.z;
            l_NewData.O                     = Position::NormalizeOrientation((2 * M_PI) - Position::NormalizeOrientation(l_Info.O - l_O));

            sObjectMgr->AddGarrisonPlotBuildingContent(l_NewData);

            /// Test code
            if (true)
            {
                G3D::Vector3 l_NonRotatedPosition;

                l_Mat = G3D::Matrix3::identity();
                l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), -l_Info.O);

                /// transform plot coord
                l_NonRotatedPosition = l_Mat * G3D::Vector3(l_Info.X, l_Info.Y, l_Info.Z);

                G3D::Vector3 l_Position = G3D::Vector3(l_ElementPosition.x, l_ElementPosition.y, 0);

                l_Mat = G3D::Matrix3::identity();
                l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), l_Info.O);

                l_Position.x += l_NonRotatedPosition.x;
                l_Position.y += l_NonRotatedPosition.y;

                l_Position = l_Mat * l_Position;

                l_Position.z = l_ElementPosition.z + l_Info.Z;

                p_Handler->PSendSysMessage("Spawn coord %f %f %f %f", l_X, l_Y, l_Z, l_O);
                p_Handler->PSendSysMessage("Trans coord %f %f %f %f", l_Position.x, l_Position.y, l_Position.z, l_NewData.O + l_Info.O);
            }

            l_Player->GetGarrison()->OnPlayerEnter();

            return true;
        }

        static bool HandlePlotDelCommand(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            Player* l_Player     = p_Handler->GetSession()->GetPlayer();
            WorldObject* l_Target = nullptr;
            
            if (p_Handler->getSelectedCreature())
                l_Target = p_Handler->getSelectedCreature();

            if (!l_Player || !l_Player->GetGarrison())
            {
                p_Handler->SendSysMessage("You don't have a garrison");
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (!l_Target)
            {
                p_Handler->SendSysMessage("No valid target");
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            MS::Garrison::GarrisonPlotInstanceInfoLocation l_Info = l_Player->GetGarrison()->GetPlot(l_Player->GetPositionX(), l_Player->GetPositionY(), l_Player->GetPositionZ());

            if (!l_Info.PlotInstanceID)
            {
                p_Handler->SendSysMessage("Plot not found");
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            G3D::Vector3 l_NonRotatedPosition;

            G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
            l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), -l_Info.O);

            /// transform plot coord
            l_NonRotatedPosition = l_Mat * G3D::Vector3(l_Info.X, l_Info.Y, l_Info.Z);

            std::vector<GarrisonPlotBuildingContent> l_Contents = sObjectMgr->GetGarrisonPlotBuildingContent(l_Player->GetGarrison()->GetPlotType(l_Info.PlotInstanceID), l_Player->GetGarrison()->GetGarrisonFactionIndex());
           
            for (uint32 l_I = 0; l_I < l_Contents.size(); ++l_I)
            {
                if (l_Target->ToCreature() && l_Contents[l_I].CreatureOrGob > 0 && l_Contents[l_I].CreatureOrGob == l_Target->GetEntry())
                {
                    G3D::Vector3 l_Position = G3D::Vector3(l_Contents[l_I].X, l_Contents[l_I].Y, 0);

                    l_Mat = G3D::Matrix3::identity();
                    l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), l_Info.O);

                    l_Position.x += l_NonRotatedPosition.x;
                    l_Position.y += l_NonRotatedPosition.y;

                    l_Position = l_Mat * l_Position;

                    l_Position.z = l_Contents[l_I].Z + l_Info.Z;


                    if (l_Target->GetDistance(l_Position.x, l_Position.y, l_Position.z) < 0.2f)
                    {
                        sObjectMgr->DeleteGarrisonPlotBuildingContent(l_Contents[l_I]);
                        l_Player->GetGarrison()->OnPlayerEnter();
                        return true;
                    }
                }
                else
                {
                    /// @TODO game object case
                }
            }

            return true;
        }
        
        static bool HandlePlotImportCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            Player* l_Player = p_Handler->GetSession()->GetPlayer();

            if (!l_Player || !l_Player->GetGarrison())
            {
                p_Handler->SendSysMessage("You don't have a garrison");
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            MS::Garrison::GarrisonPlotInstanceInfoLocation l_Info = l_Player->GetGarrison()->GetPlot(l_Player->GetPositionX(), l_Player->GetPositionY(), l_Player->GetPositionZ());

            if (!l_Info.PlotInstanceID)
            {
                p_Handler->SendSysMessage("Plot not found");
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            MS::Garrison::GarrisonBuilding l_Building = l_Player->GetGarrison()->GetBuilding(l_Info.PlotInstanceID);

            if (!l_Building.BuildingID)
            {
                p_Handler->SendSysMessage("Building not found");
                p_Handler->SetSentErrorMessage(true);
            }

            int32 l_ElemEntry = 0;
            float l_X = 0;
            float l_Y = 0;
            float l_Z = 0;
            float l_O = 0;

            Creature* l_Target = p_Handler->getSelectedCreature();

            if (l_Target)
            {
                l_ElemEntry = l_Target->GetEntry();
                l_X = l_Target->GetPositionX();
                l_Y = l_Target->GetPositionY();
                l_Z = l_Target->GetPositionZ();
                l_O = l_Target->GetOrientation();
            }
            else if (p_Args != 0)
            {
                uint32 l_GuidLow = atoi(p_Args);

                if (!l_GuidLow)
                {
                    p_Handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, l_GuidLow);
                    p_Handler->SetSentErrorMessage(true);
                    return false;
                }

                GameObject* l_GameObject = nullptr;

                if (GameObjectData const* l_GameObjectData = sObjectMgr->GetGOData(l_GuidLow))
                    l_GameObject = p_Handler->GetObjectGlobalyWithGuidOrNearWithDbGuid(l_GuidLow, l_GameObjectData->id);

                if (!l_GameObject)
                {
                    p_Handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, l_GuidLow);
                    p_Handler->SetSentErrorMessage(true);
                    return false;
                }

                l_ElemEntry = -(int32)l_GameObject->GetEntry();
                l_X = l_GameObject->GetPositionX();
                l_Y = l_GameObject->GetPositionY();
                l_Z = l_GameObject->GetPositionZ();
                l_O = l_GameObject->GetOrientation();
            }
            else
            {
                p_Handler->SendSysMessage("Not creature/gameobject selected");
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
            l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), -l_Info.O);

            G3D::Vector3 l_ElementPosition = G3D::Vector3(l_X, l_Y, 0);
            l_ElementPosition -= G3D::Vector3(l_Info.X, l_Info.Y, 0);
            l_ElementPosition = l_Mat *l_ElementPosition;
            l_ElementPosition.z = l_Z - l_Info.Z;

            if (l_ElemEntry > 0)
                l_ElementPosition.z += 0.5f;

            int32 l_TableIndex = l_Player->GetGarrison()->GetPlotType(l_Info.PlotInstanceID);
            if (l_Building.Active)
                l_TableIndex = -int32(l_Building.BuildingID);

            GarrisonPlotBuildingContent l_NewData;
            l_NewData.PlotTypeOrBuilding    = l_TableIndex;
            l_NewData.CreatureOrGob         = l_ElemEntry;
            l_NewData.FactionIndex          = l_Player->GetGarrison()->GetGarrisonFactionIndex();
            l_NewData.X                     = l_ElementPosition.x;
            l_NewData.Y                     = l_ElementPosition.y;
            l_NewData.Z                     = l_ElementPosition.z;
            l_NewData.O                     = Position::NormalizeOrientation((2 * M_PI) - Position::NormalizeOrientation(l_Info.O - l_O));

            sObjectMgr->AddGarrisonPlotBuildingContent(l_NewData);

            /// Test code
            if (true)
            {
                G3D::Vector3 l_NonRotatedPosition;

                l_Mat = G3D::Matrix3::identity();
                l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), -l_Info.O);

                /// transform plot coord
                l_NonRotatedPosition = l_Mat * G3D::Vector3(l_Info.X, l_Info.Y, l_Info.Z);

                G3D::Vector3 l_Position = G3D::Vector3(l_ElementPosition.x, l_ElementPosition.y, 0);

                l_Mat = G3D::Matrix3::identity();
                l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), l_Info.O);

                l_Position.x += l_NonRotatedPosition.x;
                l_Position.y += l_NonRotatedPosition.y;

                l_Position = l_Mat * l_Position;

                l_Position.z = l_ElementPosition.z + l_Info.Z;

                p_Handler->PSendSysMessage("Spawn coord %f %f %f %f", l_X, l_Y, l_Z, l_O);
                p_Handler->PSendSysMessage("Trans coord %f %f %f %f", l_Position.x, l_Position.y, l_Position.z, l_NewData.O + l_Info.O);
            }

            l_Player->GetGarrison()->OnPlayerEnter();

            return true;
        }

        static bool HandleFollowerPromoteCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (!p_Args)
                return false;

            uint32 l_FollowerID = atoi(p_Args);
            return l_TargetPlayer->GetGarrison()->LevelUpFollower(l_FollowerID);
        }

        static bool HandleFollowerAddCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (p_Args != 0)
            {
                std::string l_Args = p_Args;

                if (l_Args == "all")
                {
                    for (uint32 l_I = 0; l_I < sGarrFollowerStore.GetNumRows(); ++l_I)
                    {
                        const GarrFollowerEntry* l_Entry = sGarrFollowerStore.LookupEntry(l_I);

                        if (l_Entry)
                            l_TargetPlayer->GetGarrison()->AddFollower(l_Entry->ID);
                    }

                    return true;
                }
                else
                {
                    uint32 l_FollowerID = atoi(p_Args);

                    if (!l_FollowerID)
                        return false;

                    return l_TargetPlayer->GetGarrison()->AddFollower(l_FollowerID);
                }
            }

            return true;
        }

        static bool HandleFolloweraddlistCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            std::vector<uint32> l_FollowerIDs = 
            {
                34,
                87,
                89,
                90,
                92,
                97,
                98,
                101,
                103,
                104,
                106,
                107,
                108,
                110,
                112,
                116,
                119,
                120,
                153,
                159,
                179,
                180,
                181,
                182,
                183,
                184,
                185,
                186,
                204,
                207,
                208,
                216,
                463,
                467,
                580,
                581,
                582
            };

            for (uint32 l_ID : l_FollowerIDs)
            {
                const GarrFollowerEntry* l_Entry = sGarrFollowerStore.LookupEntry(l_ID);

                if (l_Entry)
                    l_TargetPlayer->GetGarrison()->AddFollower(l_Entry->ID);
            }

            return true;
        }

        static bool HandleFollowerRemoveCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (!p_Args)
                return false;

            uint32 l_FollowerID = atoi(p_Args);
            return l_TargetPlayer->GetGarrison()->RemoveFollower(l_FollowerID, true);
        }

        static bool HandleFollowerRerollCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (!p_Args)
                return false;

            uint32 l_FollowerID = atoi(p_Args);
            l_TargetPlayer->GetGarrison()->GenerateFollowerAbilities(l_FollowerID, true, true, true, true);

            return true;
        }

        static bool HandleFollowerListCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 l_Type  = 0;

            if (p_Args)
                l_Type = atoi(p_Args);

            auto l_List = l_TargetPlayer->GetGarrison()->GenerateFollowerTextList(l_Type);

            for (auto l_Str  : l_List)
                p_Handler->SendSysMessage(l_Str.c_str());

            return true;
        }

        static bool HandleMissionAddCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (p_Args != 0)
            {
                std::string l_Args = p_Args;

                if (l_Args == "all")
                {
                    for (uint32 l_I = 0; l_I < sGarrMissionStore.GetNumRows(); ++l_I)
                    {
                        const GarrMissionEntry* l_Entry = sGarrMissionStore.LookupEntry(l_I);

                        if (l_Entry)
                            l_TargetPlayer->GetGarrison()->AddMission(l_Entry->MissionRecID);
                    }

                    return true;
                }
                else
                {
                    uint32 l_MissionID = atoi(p_Args);

                    if (!l_MissionID)
                        return false;

                    return l_TargetPlayer->GetGarrison()->AddMission(l_MissionID);
                }
            }

            return true;
        }

        static bool HandleMissionCompleteAllCommand(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            l_TargetPlayer->GetGarrison()->SetAllInProgressMissionAsComplete();

            return true;
        }

        static bool HandleMissionRerollCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            uint32 l_MissionCount = std::min(p_Args ? atoi(p_Args) : 0, 30);
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            l_TargetPlayer->GetGarrison()->UpdateMissionDistribution(true, l_MissionCount);
            return true;
         }

        static bool HandleBuildingCompleteCommand(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (MS::Garrison::Manager* l_Garr = l_TargetPlayer->GetGarrison())
            {
                MS::Garrison::GarrisonPlotInstanceInfoLocation l_Plot = l_Garr->GetPlot(l_TargetPlayer->m_positionX, l_TargetPlayer->m_positionY, l_TargetPlayer->m_positionZ);

                if (l_Plot.PlotInstanceID)
                {
                    if (!l_Garr->PlotIsFree(l_Plot.PlotInstanceID) && !l_Garr->GetBuilding(l_Plot.PlotInstanceID).Active)
                        l_Garr->ActivateBuilding(l_Plot.PlotInstanceID);
                }
            }

            return true;
        }

        static bool HandleShipmentCompleteCommand(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (MS::Garrison::Manager* l_Garr = l_TargetPlayer->GetGarrison())
            {
                MS::Garrison::GarrisonPlotInstanceInfoLocation l_Plot = l_Garr->GetPlot(l_TargetPlayer->m_positionX, l_TargetPlayer->m_positionY, l_TargetPlayer->m_positionZ);

                if (l_Plot.PlotInstanceID)
                {
                    std::vector<MS::Garrison::GarrisonWorkOrder>& l_PlotWorkOrder = l_Garr->GetWorkOrders();

                    if (l_PlotWorkOrder.size() > 0)
                    {
                        uint32 l_CurrentTimeStamp = time(0);

                        for (uint32 l_OrderI = 0; l_OrderI < l_PlotWorkOrder.size(); ++l_OrderI)
                            l_PlotWorkOrder[l_OrderI].CompleteTime = l_CurrentTimeStamp;
                    }
                }
            }
            return true;
        }

        static bool HandleShipyardOpen(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            return true;
        }
};

#ifndef __clang_analyzer__
void AddSC_garrison_commandscript()
{
    new garrison_commandscript();
}
#endif
