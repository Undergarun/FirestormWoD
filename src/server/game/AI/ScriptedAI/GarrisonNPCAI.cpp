////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "GarrisonNPCAI.hpp"

/// Constructor
GarrisonNPCAI::GarrisonNPCAI(Creature* p_Creature) : CosmeticAI(p_Creature), m_PlotInstanceLocation(nullptr), m_BuildingID(0), m_Owner(nullptr), m_SequenceSize(0) { }

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Set to relative position from building
/// @p_X : Relative X
/// @p_Y : Relative Y
/// @p_Z : Relative Z
void GarrisonNPCAI::MoveBuildingRelative(uint32 p_PointID, float p_X, float p_Y, float p_Z)
{
    if (!m_PlotInstanceLocation)
        return;

    G3D::Vector3 l_Position = G3D::Vector3(p_X, p_Y, 0);

    G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
    l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), m_PlotInstanceLocation->O);

    l_Position.x += m_NonRotatedPlotPosition.x;
    l_Position.y += m_NonRotatedPlotPosition.y;

    l_Position = l_Mat * l_Position;

    l_Position.z = p_Z + m_PlotInstanceLocation->Z;

    me->GetMotionMaster()->MovePoint(p_PointID, l_Position.x, l_Position.y, l_Position.z);
}

/// Set facing to relative angle from the building
/// @p_O : Relative angle
void GarrisonNPCAI::SetFacingBuildingRelative(float p_O)
{
    float l_Angle = p_O;

    if (m_PlotInstanceLocation)
        l_Angle += m_PlotInstanceLocation->O;

    me->SetFacingTo(Position::NormalizeOrientation(l_Angle));
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Set NPC recipes
/// @p_Recipes          : Recipes
/// @p_RecipesSkillID   : Skill line ID
void GarrisonNPCAI::SetRecipes(std::vector<RecipesConditions> p_Recipes, uint32 p_RecipesSkillID)
{
    m_Recipes           = p_Recipes;
    m_RecipesSkillID    = p_RecipesSkillID;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Get building ID
uint32 GarrisonNPCAI::GetBuildingID()
{
    return m_BuildingID;
}

/// Get plot instance ID
uint32 GarrisonNPCAI::GetPlotInstanceID()
{
    return m_PlotInstanceLocation ? m_PlotInstanceLocation->PlotInstanceID : 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Setup action sequence
/// @p_CoordTable       : Coordinates table
/// @p_SequenceTable    : Sequence table
/// @p_SequenceSize     : Size of sequence table,
/// @p_FirstMovePointID : First move point ID
void GarrisonNPCAI::SetupActionSequence(MS::Garrison::SequencePosition* p_CoordTable, uint8* p_SequenceTable, uint32 p_SequenceSize, uint32 p_FirstMovePointID)
{
    m_CoordTable        = p_CoordTable;
    m_SequencePosition  = 0xFF;
    m_SequenceTable     = p_SequenceTable;
    m_SequenceSize      = p_SequenceSize;
    m_FirstMovePointID  = p_FirstMovePointID;
}

/// Do next sequence element
void GarrisonNPCAI::DoNextSequenceAction()
{
    if (!m_SequenceSize)
        return;

    if (m_SequencePosition >= m_SequenceSize)
        m_SequencePosition = 0;

    m_DelayedOperations.push([this]() -> void
    {
        me->SetWalk(true);

        uint32 l_LocationID = m_SequenceTable[m_SequencePosition] - m_FirstMovePointID;
        MoveBuildingRelative( m_SequenceTable[m_SequencePosition], m_CoordTable[l_LocationID].X,
                                                                   m_CoordTable[l_LocationID].Y,
                                                                   m_CoordTable[l_LocationID].Z);

        m_SequencePosition++;
    });
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Spawn a creature with building relative coords
/// @p_Entry      : Creature entry
/// @p_RelX       : X Relative coord
/// @p_RelY       : Y Relative coord
/// @p_RelZ       : Z Relative coord
/// @p_RelO       : Relative orientation coord
/// @p_SummonType : Summon type
Creature* GarrisonNPCAI::SummonRelativeCreature(uint32 p_Entry, float p_RelX, float p_RelY, float p_RelZ, float p_RelO, TempSummonType p_SummonType)
{
    if (!m_PlotInstanceLocation)
        return nullptr;

    G3D::Vector3 l_Position = G3D::Vector3(p_RelX, p_RelY, 0);

    G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
    l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), m_PlotInstanceLocation->O);

    l_Position.x += m_NonRotatedPlotPosition.x;
    l_Position.y += m_NonRotatedPlotPosition.y;

    l_Position = l_Mat * l_Position;

    l_Position.z = p_RelZ + m_PlotInstanceLocation->Z;

    float l_Angle = p_RelO;

    if (m_PlotInstanceLocation)
        l_Angle += m_PlotInstanceLocation->O;

    return me->SummonCreature(p_Entry, l_Position.x, l_Position.y, l_Position.z, l_Angle, p_SummonType);
}

/// Spawn a creature with building relative coords
/// @p_Entry      : Creature entry
/// @p_Position   : Relative position of the creature
/// @p_SummonType : Summon type
Creature* GarrisonNPCAI::SummonRelativeCreature(uint32 p_Entry, MS::Garrison::SequencePosition p_Position, TempSummonType p_SummonType)
{
    return SummonRelativeCreature(p_Entry, p_Position.X, p_Position.Y, p_Position.Z, p_Position.O, p_SummonType);
}

/// Spawn a gameobject with building relative coords
/// @p_Entry      : GameObject entry
/// @p_RelX       : X Relative coord
/// @p_RelY       : Y Relative coord
/// @p_RelZ       : Z Relative coord
/// @p_RelO       : Relative orientation coord
GameObject* GarrisonNPCAI::SummonRelativeGameObject(uint32 p_Entry, float p_RelX, float p_RelY, float p_RelZ, float p_RelO)
{
    if (!m_PlotInstanceLocation)
        return nullptr;

    G3D::Vector3 l_Position = G3D::Vector3(p_RelX, p_RelY, 0);

    G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
    l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), m_PlotInstanceLocation->O);

    l_Position.x += m_NonRotatedPlotPosition.x;
    l_Position.y += m_NonRotatedPlotPosition.y;

    l_Position = l_Mat * l_Position;

    l_Position.z = p_RelZ + m_PlotInstanceLocation->Z;

    float l_Angle = p_RelO;

    if (m_PlotInstanceLocation)
        l_Angle += m_PlotInstanceLocation->O;

    return me->SummonGameObject(p_Entry, l_Position.x, l_Position.y, l_Position.z, l_Angle, 0, 0, 0, 0, 0);
}

/// Spawn a gameobject with building relative coords
/// @p_Entry      : GameObject entry
/// @p_RelX       : X Relative coord
/// @p_RelY       : Y Relative coord
/// @p_RelZ       : Z Relative coord
/// @p_RelO       : Relative orientation coord
GameObject* GarrisonNPCAI::SummonRelativeGameObject(uint32 p_Entry, const Position p_Position)
{
    return SummonRelativeGameObject(p_Entry, p_Position.m_positionX, p_Position.m_positionY, p_Position.m_positionZ, p_Position.m_orientation);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Transform coord
/// @p_X : X coord
/// @p_Y : Y coord
/// @p_Z : Z coord
void GarrisonNPCAI::TransformCoord(float& p_X, float &p_Y, float &p_Z)
{
    if (!m_PlotInstanceLocation)
        return;

    G3D::Vector3 l_Position = G3D::Vector3(p_X, p_Y, 0);

    G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
    l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), m_PlotInstanceLocation->O);

    l_Position.x += m_NonRotatedPlotPosition.x;
    l_Position.y += m_NonRotatedPlotPosition.y;

    l_Position = l_Mat * l_Position;

    l_Position.z = p_Z + m_PlotInstanceLocation->Z;

    p_X = l_Position.x;
    p_Y = l_Position.y;
    p_Z = l_Position.z;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// When the building ID is set
/// @p_BuildingID : Set building ID
void GarrisonNPCAI::OnSetBuildingID(uint32 /*p_BuildingID*/)
{

}

/// When the PlotInstance ID is set
/// @p_BuildingID : Set plot instance ID
void GarrisonNPCAI::OnSetPlotInstanceID(uint32 /*p_PlotInstanceID*/)
{

}

/// When the daily garrison datas are reset
void GarrisonNPCAI::OnDailyDataReset()
{

}

void GarrisonNPCAI::OnPlotInstanceUnload()
{
    for (std::vector<uint64>::iterator l_Guid = m_Summons.begin(); l_Guid != m_Summons.end(); ++l_Guid)
    {
        if (Creature* l_Creature = HashMapHolder<Creature>::Find(*l_Guid))
            l_Creature->DespawnOrUnsummon();
    }

    m_Summons.clear();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Set UInt32 value
/// @p_ID    : Value ID
/// @p_Value : Value
void GarrisonNPCAI::SetData(uint32 p_ID, uint32 p_Value)
{
    switch (p_ID)
    {
        case MS::Garrison::CreatureAIDataIDs::PlotInstanceID:
        {
            m_PlotInstanceLocation = nullptr;

            for (uint32 l_I = 0; l_I < MS::Garrison::Globals::PlotInstanceCount; ++l_I)
            {
                if (MS::Garrison::gGarrisonPlotInstanceInfoLocation[l_I].PlotInstanceID == (p_Value & 0x0000FFFF) && MS::Garrison::gGarrisonPlotInstanceInfoLocation[l_I].SiteLevelID == ((p_Value >> 16) & 0x0000FFFF))
                {
                    m_PlotInstanceLocation = &MS::Garrison::gGarrisonPlotInstanceInfoLocation[l_I];
                    break;
                }
            }

            if (m_PlotInstanceLocation)
            {
                G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
                l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), -m_PlotInstanceLocation->O);

                /// transform plot coord
                m_NonRotatedPlotPosition = l_Mat * G3D::Vector3(m_PlotInstanceLocation->X, m_PlotInstanceLocation->Y, m_PlotInstanceLocation->Z);
        
                OnSetPlotInstanceID(m_PlotInstanceLocation->PlotInstanceID);
            }
            break;
        }
        case MS::Garrison::CreatureAIDataIDs::BuildingID:
        {
            m_BuildingID = p_Value;
            OnSetBuildingID(m_BuildingID);
            break;
        }
        case MS::Garrison::CreatureAIDataIDs::DailyReset:
            OnDailyDataReset();
///                l_GarrisonMgr->UpdatePlot(GetPlotInstanceID()); ///< Disabled cause it causes troubles, m_Owner is not always available
            break;
        case MS::Garrison::CreatureAIDataIDs::DespawnData:
            OnPlotInstanceUnload();
            break;
        default:
            break;
    }
}

void GarrisonNPCAI::SetGUID(uint64 p_Guid, int32 p_Id)
{
    if (p_Id == MS::Garrison::CreatureAIDataIDs::OwnerGuid)
        m_Owner = HashMapHolder<Player>::Find(p_Guid);
}

/// Get UInt32 value
/// @p_ID    : Value ID
uint32 GarrisonNPCAI::GetData(uint32 p_ID)
{
    if ((p_ID & MS::Garrison::CreatureAIDataIDs::HasRecipe) != 0)
    {
        if (m_Recipes.empty())
            return (uint32)-1;

        uint32 l_RecipeID = p_ID & ~MS::Garrison::CreatureAIDataIDs::HasRecipe;

        auto l_It = std::find_if(m_Recipes.begin(), m_Recipes.end(), [l_RecipeID](RecipesConditions const& p_Entry) -> bool
        {
            return p_Entry.RecipeID == l_RecipeID;
        });

        if (l_It == m_Recipes.end())
            return (uint32)-1;

        return l_It->PlayerConditionID;
    }
    
    return (uint32)-1;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Show shipment crafter UI
void GarrisonNPCAI::SendShipmentCrafterUI(Player* p_Player, uint32 p_ShipmentID /*= 0*/)
{
    if (p_Player->IsInGarrison())
    {
        if (!p_ShipmentID && !(p_ShipmentID = sGarrisonShipmentManager->GetShipmentIDForBuilding(m_BuildingID, p_Player, false)))
            p_Player->PlayerTalkClass->SendCloseGossip();
        else
        {
            WorldPacket l_Data(SMSG_OPEN_SHIPMENT_NPCFROM_GOSSIP);
            l_Data.appendPackGUID(me->GetGUID());
            l_Data << uint32(p_ShipmentID);

            p_Player->SendDirectMessage(&l_Data);
        }
    }
}

/// Show Follower recruitment UI
void GarrisonNPCAI::SendFollowerRecruitmentUI(Player* p_Player)
{
    if (p_Player->IsInGarrison())
    {
        MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison();

        if (l_GarrisonMgr == nullptr)
            return;

        WorldPacket l_Data(SMSG_GARRISON_OPEN_RECRUITMENT_NPC);
        l_Data.appendPackGUID(me->GetGUID());

        l_Data << uint32(0);                    ///< Unk1
        l_Data << uint32(0);                    ///< Unk2 162
        l_Data << uint32(0);                    ///< Unk3 2310 || 1281

        std::vector<MS::Garrison::GarrisonFollower> l_Followers = l_GarrisonMgr->GetWeeklyFollowerRecruits(p_Player);

        if (l_Followers.empty())
        {
            for (uint8 l_Itr = 0; l_Itr < 3; ++l_Itr)
            {
                MS::Garrison::GarrisonFollower l_Follower;
                l_Follower.Write(l_Data);
            }
        }
        else
        {
            for (MS::Garrison::GarrisonFollower l_Follower : l_Followers)
                l_Follower.Write(l_Data);
        }

        l_Data.WriteBit(l_GarrisonMgr->CanRecruitFollower());
        l_Data.WriteBit(1);                   ///< unk bit 2

        p_Player->SendDirectMessage(&l_Data);
    }
}

/// Show Follower recruits UI
void GarrisonNPCAI::SendRecruitmentFollowersGenerated(Player* p_Player, uint32 p_AbilityID, uint32 p_ErrorMessage, bool p_IsTrait)
{
    if (p_Player->IsInGarrison())
    {
        MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison();

        if (l_GarrisonMgr == nullptr)
            return;

        WorldPacket l_Data(SMSG_GARRISON_RECRUITMENT_FOLLOWERS_GENERATED);
        
        l_Data << uint32(p_ErrorMessage);                           ///< l_Error ?
        l_Data << uint32(sWorld->GetNextWeeklyQuestsResetTime());   ///< Time left until next reset

        std::vector<MS::Garrison::GarrisonFollower> l_Followers = l_GarrisonMgr->GetWeeklyFollowerRecruits(p_Player);

        if (l_Followers.empty())
        {
            std::list<MS::Garrison::GarrisonFollower> l_FollowersList = l_GarrisonMgr->GetFollowersWithAbility(p_AbilityID, p_IsTrait);
            JadeCore::RandomResizeList(l_FollowersList, 3); ///< Should not happen, List size must never exceed 3

            for (MS::Garrison::GarrisonFollower l_Follower : l_FollowersList)
            {
                l_Follower.Write(l_Data);
                l_GarrisonMgr->SetGarrisonWeeklyTavernData({ l_Follower.FollowerID, l_Follower.Abilities });
            }
        }
        else
        {
            for (MS::Garrison::GarrisonFollower l_Follower : l_Followers)
                l_Follower.Write(l_Data);
        }

        p_Player->SendDirectMessage(&l_Data);
    }
}

/// Show trade skill crafter UI
void GarrisonNPCAI::SendTradeSkillUI(Player* p_Player)
{
    if (p_Player->IsInGarrison())
    {
        if (!m_Recipes.empty())
        {
            WorldPacket l_Data(SMSG_GARRISON_OPEN_TRADESKILL_NPC, 512);
            l_Data.appendPackGUID(me->GetGUID());
            l_Data << uint32(0);                    ///< SpellID
            l_Data << uint32(1);                    ///< Skill line ID count
            l_Data << uint32(0);                    ///< Skill rank count
            l_Data << uint32(0);                    ///< Skill max rank count
            l_Data << uint32(m_Recipes.size());    ///< Skill known ability spell id count

            l_Data << uint32(m_RecipesSkillID);     ///< Skill line ID

            for (uint32 l_I = 0; l_I < m_Recipes.size(); ++l_I)
                l_Data << m_Recipes.at(l_I).RecipeID;

            l_Data << uint32(m_Recipes.size());    ///< Skill known ability spell id condition count

            for (uint32 l_I = 0; l_I < m_Recipes.size(); ++l_I)
            {
                if (m_Recipes.at(l_I).PlayerConditionID && !p_Player->EvalPlayerCondition(m_Recipes.at(l_I).PlayerConditionID).first)
                    l_Data << m_Recipes.at(l_I).PlayerConditionID;
                else
                    l_Data << uint32(0);
            }

            p_Player->SendDirectMessage(&l_Data);
        }
        else
            p_Player->PlayerTalkClass->SendCloseGossip();
    }
}