////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "ScriptedCosmeticAI.hpp"
#include "GarrisonMgr.hpp"
#include "../../../scripts/Draenor/Garrison/GarrisonScriptData.hpp"

namespace MS
{
    namespace Garrison
    {
        struct SequencePosition;
        struct GarrisonPlotInstanceInfoLocation;
    }
}

class GarrisonNPCAI : public MS::AI::CosmeticAI
{
    public:
        /// Constructor
        GarrisonNPCAI(Creature* p_Creature);

        /// Set to relative position from building
        /// @p_X : Relative X
        /// @p_Y : Relative Y
        /// @p_Z : Relative Z
        void MoveBuildingRelative(uint32 p_PointID, float p_X, float p_Y, float p_Z);
        /// Set facing to relative angle from the building
        /// @p_O : Relative angle
        void SetFacingBuildingRelative(float p_O);

        /// Set NPC recipes
        /// @p_Recipes          : Recipes
        /// @p_RecipesSkillID   : Skill line ID
        void SetRecipes(std::vector<RecipesConditions> p_Recipes, uint32 p_RecipesSkillID);

        /// Show shipment crafter UI
        void SendShipmentCrafterUI(Player* p_Player, uint32 p_ShipmentID = 0);
        /// Show Follower recruitment UI
        void SendFollowerRecruitmentUI(Player* p_Player);
        /// Show trade skill crafter UI
        void SendTradeSkillUI(Player* p_Player);
        /// Show selected followers to recruit
        void SendRecruitmentFollowersGenerated(Player* p_Player, uint32 p_AbilityID, uint32 p_ErrorMessage, bool p_IsTrait);

        /// Get building ID
        uint32 GetBuildingID();
        /// Get plot instance ID
        uint32 GetPlotInstanceID();

        /// Setup action sequence
        /// @p_CoordTable       : Coordinates table
        /// @p_SequenceTable    : Sequence table
        /// @p_SequenceSize     : Size of sequence table,
        /// @p_FirstMovePointID : First move point ID
        void SetupActionSequence(MS::Garrison::SequencePosition* p_CoordTable, uint8* p_SequenceTable, uint32 p_SequenceSize, uint32 p_FirstMovePointID);

        void AddSummonGUID(uint64 p_GUID) { m_Summons.push_back(p_GUID); }
        /// Do next sequence element
        virtual void DoNextSequenceAction();

        /// Spawn a creature with building relative coords
        /// @p_Entry      : Creature entry
        /// @p_RelX       : X Relative coord
        /// @p_RelY       : Y Relative coord
        /// @p_RelZ       : Z Relative coord
        /// @p_RelO       : Relative orientation coord
        /// @p_SummonType : Summon type
        Creature* SummonRelativeCreature(uint32 p_Entry, float p_RelX, float p_RelY, float p_RelZ, float p_RelO, TempSummonType p_SummonType);
        /// Spawn a creature with building relative coords
        /// @p_Entry      : Creature entry
        /// @p_Position   : Relative position of the creature
        /// @p_SummonType : Summon type
        Creature* SummonRelativeCreature(uint32 p_Entry, MS::Garrison::SequencePosition p_Position, TempSummonType p_SummonType);
        /// Spawn a gameobject with building relative coords
        /// @p_Entry      : GameObject entry
        /// @p_RelX       : X Relative coord
        /// @p_RelY       : Y Relative coord
        /// @p_RelZ       : Z Relative coord
        /// @p_RelO       : Relative orientation coord
        GameObject* SummonRelativeGameObject(uint32 p_Entry, float p_RelX, float p_RelY, float p_RelZ, float p_RelO);
        /// Spawn a gameobject with building relative coords
        /// @p_Entry      : GameObject entry
        /// @p_Position   : Relative coords
        GameObject* SummonRelativeGameObject(uint32 p_Entry, const Position p_Position);

        /// Transform coord
        /// @p_X : X coord
        /// @p_Y : Y coord
        /// @p_Z : Z coord
        void TransformCoord(float& p_X, float &p_Y, float &p_Z);

        Player* GetOwner() { return m_Owner; };

    public:
        /// When the building ID is set
        /// @p_BuildingID : Set building ID
        virtual void OnSetBuildingID(uint32 p_BuildingID);
        /// When the PlotInstance ID is set
        /// @p_BuildingID : Set plot instance ID
        virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID);
        /// When the daily garrison datas are reset
        virtual void OnDailyDataReset();
        ///
        virtual void OnPlotInstanceUnload();

    public:
        /// Set UInt32 value
        /// @p_ID    : Value ID
        /// @p_Value : Value
        virtual void SetData(uint32 p_ID, uint32 p_Value) override;
        /// Get UInt32 value
        /// @p_ID    : Value ID
        virtual uint32 GetData(uint32 p_ID) override;

        virtual void SetGUID(uint64 p_Guid, int32 p_Id) override;

    protected:
        MS::Garrison::GarrisonPlotInstanceInfoLocation const* m_PlotInstanceLocation; ///< This creature plot
        G3D::Vector3 m_NonRotatedPlotPosition;                          ///< Cache for coord transformation
        uint32 m_BuildingID;                                            ///< This creature building ID

    private:
        Player* m_Owner;
        MS::Garrison::SequencePosition* m_CoordTable;
        uint8* m_SequenceTable;
        uint32 m_SequenceSize;
        uint32 m_FirstMovePointID;
        uint8 m_SequencePosition;

    private:
        std::vector<uint64> m_Summons;
        std::vector<RecipesConditions> m_Recipes;
        uint32 m_RecipesSkillID;

};
#endif