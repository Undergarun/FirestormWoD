////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCRIPTING_INTERFACES_PLAYER_HPP_INCLUDED
#define SCRIPTING_INTERFACES_PLAYER_HPP_INCLUDED

#include "InterfaceBase.hpp"

/// Formula script interface
class FormulaScript : public ScriptObjectImpl<false>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        FormulaScript(const char* p_Name);

    public:
        /// Called after calculating honor.
        /// @p_Honor      : Dest honor
        /// @p_Level      : Player level
        /// @p_Multiplier : Honor multiplier
        virtual void OnHonorCalculation(float& p_Honor, uint8 p_Level, float p_Multiplier)
        {
            UNUSED(p_Honor);
            UNUSED(p_Level);
            UNUSED(p_Multiplier);
        }

        /// Called after gray level calculation.
        /// @p_GrayLevel   : Dest gray level
        /// @p_PlayerLevel : Player level
        virtual void OnGrayLevelCalculation(uint8& p_GrayLevel, uint8 p_PlayerLevel)
        {
            UNUSED(p_GrayLevel);
            UNUSED(p_PlayerLevel);
        }

        /// Called after calculating experience color.
        /// @p_Color       : Dest XP Color
        /// @p_PlayerLevel : Player level
        /// @p_MobLevel    : Killed mob level
        virtual void OnColorCodeCalculation(XPColorChar& p_Color, uint8 p_PlayerLevel, uint8 p_MobLevel)
        {
            UNUSED(p_Color);
            UNUSED(p_PlayerLevel);
            UNUSED(p_MobLevel);
        }

        /// Called after calculating zero difference.
        /// @p_Diff        : Level difference
        /// @p_PlayerLevel : Player level
        virtual void OnZeroDifferenceCalculation(uint8& p_Diff, uint8 p_PlayerLevel)
        {
            UNUSED(p_Diff);
            UNUSED(p_PlayerLevel);
        }

        /// Called after calculating base experience gain.
        /// @p_Gain        : Dest XP Gain
        /// @p_PlayerLevel : Rewarded player instance
        /// @p_MobLevel    : Killed mob level
        /// @p_Content     : Content expansion mob
        virtual void OnBaseGainCalculation(uint32& p_Gain, uint8 p_PlayerLevel, uint8 p_MobLevel, ContentLevels p_Content)
        {
            UNUSED(p_Gain);
            UNUSED(p_PlayerLevel);
            UNUSED(p_MobLevel);
            UNUSED(p_Content);
        }

        /// Called after calculating experience gain.
        /// @p_Gain   : Dest XP Gain
        /// @p_Player : Player instance for XP computation
        /// @p_Unit   : Killed unit
        virtual void OnGainCalculation(uint32& p_Gain, Player* p_Player, Unit* p_Unit)
        {
            UNUSED(p_Gain);
            UNUSED(p_Player);
            UNUSED(p_Unit);
        }

        /// Called when calculating the experience rate for group experience.
        /// @p_Rate   : Dest XP rate
        /// @p_Count  : Group member count
        /// @p_IsRaid : Is a raid group
        virtual void OnGroupRateCalculation(float& p_Rate, uint32 p_Count, bool p_IsRaid)
        {
            UNUSED(p_Rate);
            UNUSED(p_Count);
            UNUSED(p_IsRaid);
        }
};

/// Achievement Criteria Script Interface
class AchievementCriteriaScript : public ScriptObjectImpl<true>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        AchievementCriteriaScript(const char* p_Name);

    public:
        /// Called when an additional criteria is checked.
        /// @p_Source : Criteria owner player
        /// @p_Target : Target instance
        virtual bool OnCheck(Player* p_Source, Unit* p_Target) = 0;
};

/// Condition Script Interface
class ConditionScript : public ScriptObjectImpl<true>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        ConditionScript(const char* p_Name);

    public:
        /// Called when a single condition is checked for a player.
        /// @p_Condition  : Condition instance
        /// @p_SourceInfo : Condition  source
        virtual bool OnConditionCheck(Condition const* p_Condition, ConditionSourceInfo& p_SourceInfo)
        {
            UNUSED(p_Condition);
            UNUSED(p_SourceInfo);

            return true;
        }
};

/// Player Script Interface
class PlayerScript : public ScriptObjectImpl<false>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        PlayerScript(const char* p_Name);

    public:
        /// Called just before item is destroyed
        /// @p_Item        : Item to be destroyed
        /// @p_Player      : Player level
        virtual void OnItemDestroyed(Player* p_Player, Item* p_Item)
        {
            UNUSED(p_Item);
            UNUSED(p_Player);
        }
        /// Called when a player kills another player
        /// @p_Killer : Killer instance
        /// @p_Killed : Killed instance
        virtual void OnPVPKill(Player* p_Killer, Player* p_Killed)
        {
            UNUSED(p_Killer);
            UNUSED(p_Killed);
        }

        /// Called when a player kills a Unit
        /// @p_Killer : Killer instance
        /// @p_Killed : Killed instance
        virtual void OnKill(Player* p_Killer, Unit* p_Killed)
        {
            UNUSED(p_Killer);
            UNUSED(p_Killed);
        }

        /// Called when a player kills a creature
        /// @p_Killer : Killer instance
        /// @p_Killed : Killed instance
        virtual void OnCreatureKill(Player* p_Killer, Creature* p_Killed)
        {
            UNUSED(p_Killer);
            UNUSED(p_Killed);
        }

        /// Called when a player is killed by a creature
        /// @p_Killer : Killer instance
        /// @p_Killed : Killed instance
        virtual void OnPlayerKilledByCreature(Creature* p_Killer, Player* p_Killed)
        {
            UNUSED(p_Killer);
            UNUSED(p_Killed);
        }

        /// Called when power change is modify (SetPower)
        /// @p_Player : Player instance
        /// @p_Power  : Power type
        /// @p_OldValue  : Old value
        /// @p_NewValue  : New value
        /// @p_Regen  : If it's a regen modification
        /// @p_After : If it's after modification
        virtual void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen, bool p_After)
        {
            UNUSED(p_Player);
            UNUSED(p_Power);
            UNUSED(p_OldValue);
            UNUSED(p_NewValue);
            UNUSED(p_Regen);
            UNUSED(p_After);
        }

        /// Called when the player switch from indoors to outdoors or from outdoors to indoors
        /// @p_Player : Player instance
        /// @p_IsOutdoors : boolean setting whether player is indoors or outdoors
        virtual void OnSwitchOutdoorsState(Player* p_Player, bool p_IsOutdoors)
        {
            UNUSED(p_Player);
            UNUSED(p_IsOutdoors);
        }

        /// Called when specialisation is modify (SetSpecializationId)
        /// @p_Player : Player instance
        /// @p_OldSpec  : Old Specialisation
        /// @p_NewSpec  : New Specialisation
        virtual void OnModifySpec(Player* p_Player, int32 p_NewSpec)
        {
            UNUSED(p_Player);
            UNUSED(p_NewSpec);
        }

        /// Called when a player kills another player
        /// @p_Player : Player instance
        /// @p_Value  : New value
        virtual void OnModifyHealth(Player* p_Player, int32 p_Value)
        {
            UNUSED(p_Player);
            UNUSED(p_Value);
        }

        /// Called when a player's level changes (right before the level is applied)
        /// @p_Player   : Player instance
        /// @p_OldLevel : Old player Level
        virtual void OnLevelChanged(Player* p_Player, uint8 p_OldLevel)
        {
            UNUSED(p_Player);
            UNUSED(p_OldLevel);
        }

        /// Called when a player's talent points are reset (right before the reset is done)
        /// @p_Player : Player instance
        /// @p_NoCost : Talent was reset without cost
        virtual void OnTalentsReset(Player* p_Player, bool p_NoCost)
        {
            UNUSED(p_Player);
            UNUSED(p_NoCost);
        }

        /// Called when a player's money is modified (before the modification is done)
        /// @p_Player : Player instance
        /// @p_Amount : Modified money amount
        virtual void OnMoneyChanged(Player* p_Player, int64& p_Amount)
        {
            UNUSED(p_Player);
            UNUSED(p_Amount);
        }

        /// Called when a player gains XP (before anything is given)
        /// @p_Player : Player instance
        /// @p_Amount : Modified XP amount
        /// @p_Victim : XP Source
        virtual void OnGiveXP(Player* p_Player, uint32& p_Amount, Unit* p_Victim)
        {
            UNUSED(p_Player);
            UNUSED(p_Amount);
            UNUSED(p_Victim);
        }

        /// Called when a player's reputation changes (before it is actually changed)
        /// @p_Player       : Player instance
        /// @p_FactionID    : Reward faction ID
        /// @p_Standing     : Standing
        /// @p_Incremential : Is incremental
        virtual void OnReputationChange(Player* p_Player, uint32 p_FactionID, int32& p_Standing, bool p_Incremential)
        {
            UNUSED(p_Player);
            UNUSED(p_FactionID);
            UNUSED(p_Standing);
            UNUSED(p_Incremential);
        }

        /// Called when a duel is requested
        /// @p_Target     : Duel target
        /// @p_Challenger : Duel challenger
        virtual void OnDuelRequest(Player* p_Target, Player* p_Challenger)
        {
            UNUSED(p_Target);
            UNUSED(p_Challenger);
        }

        /// Called when a duel starts (after 3s countdown)
        /// @p_Player1 : First player
        /// @p_Player2 : Second player
        virtual void OnDuelStart(Player* p_Player1, Player* p_Player2)
        {
            UNUSED(p_Player1);
            UNUSED(p_Player2);
        }

        /// Called when a duel ends
        /// @p_Winner         : Duel winner
        /// @p_Looser         : Duel looser
        /// @p_CompletionType : Duel Completion Type
        virtual void OnDuelEnd(Player* p_Winner, Player* p_Looser, DuelCompleteType p_CompletionType)
        {
            UNUSED(p_Winner);
            UNUSED(p_Looser);
            UNUSED(p_CompletionType);
        }

        /// Called when a player get teleport by a player
        /// @p_Player : Player
        /// @p_SpellID : SpellID
        virtual void OnTeleport(Player* p_Player, const SpellInfo* p_SpellInfo)
        {
            UNUSED(p_Player);
            UNUSED(p_SpellInfo);
        }

        /// The following methods are called when a player sends a chat message. (World)
        /// @p_Player  : Player instance
        /// @p_Type    : Message type
        /// @p_Lang    : Message language (WoW)
        /// @p_Message : Message content
        virtual void OnChat(Player* p_Player, uint32 p_Type, uint32 p_Lang, std::string& p_Message)
        {
            UNUSED(p_Player);
            UNUSED(p_Type);
            UNUSED(p_Lang);
            UNUSED(p_Message);
        }

        /// The following methods are called when a player sends a chat message. (Whisper)
        /// @p_Player   : Player instance
        /// @p_Type     : Message type
        /// @p_Lang     : Message language (WoW)
        /// @p_Message  : Message content
        /// @p_Receiver : Message receiver
        virtual void OnChat(Player* p_Player, uint32 p_Type, uint32 p_Lang, std::string& p_Message, Player* p_Receiver)
        {
            UNUSED(p_Player);
            UNUSED(p_Type);
            UNUSED(p_Lang);
            UNUSED(p_Message);
            UNUSED(p_Receiver);
        }

        /// The following methods are called when a player sends a chat message. (Party)
        /// @p_Player  : Player instance
        /// @p_Type    : Message type
        /// @p_Lang    : Message language (WoW)
        /// @p_Message : Message content
        /// @p_Group   : Message group target
        virtual void OnChat(Player* p_Player, uint32 p_Type, uint32 p_Lang, std::string& p_Message, Group* p_Group)
        {
            UNUSED(p_Player);
            UNUSED(p_Type);
            UNUSED(p_Lang);
            UNUSED(p_Message);
            UNUSED(p_Group);
        }

        /// The following methods are called when a player sends a chat message. (Guild)
        /// @p_Player  : Player instance
        /// @p_Type    : Message type
        /// @p_Lang    : Message language (WoW)
        /// @p_Message : Message content
        /// @p_Guild   : Message guild target
        virtual void OnChat(Player* p_Player, uint32 p_Type, uint32 p_Lang, std::string& p_Message, Guild* p_Guild)
        {
            UNUSED(p_Player);
            UNUSED(p_Type);
            UNUSED(p_Lang);
            UNUSED(p_Message);
            UNUSED(p_Guild);
        }

        /// The following methods are called when a player sends a chat message. (Channel)
        /// @p_Player  : Player instance
        /// @p_Type    : Message type
        /// @p_Lang    : Message language (WoW)
        /// @p_Message : Message content
        /// @p_Channel : Message channel target
        virtual void OnChat(Player* p_Player, uint32 p_Type, uint32 p_Lang, std::string& p_Message, Channel* p_Channel)
        {
            UNUSED(p_Player);
            UNUSED(p_Type);
            UNUSED(p_Lang);
            UNUSED(p_Message);
            UNUSED(p_Channel);
        }

        /// Both of the below are called on emote opcodes.
        /// @p_Player : Player instance
        /// @p_Emote  : Emote ID
        virtual void OnEmote(Player* p_Player, uint32 p_Emote)
        {
            UNUSED(p_Player);
            UNUSED(p_Emote);
        }

        /// When player start a text emote
        /// @p_Player     : Player instance
        /// @p_TextEmote  : Text emote ID
        /// @p_SoundIndex : Emote sound ID
        /// @p_TargetGUID : Text emote target GUID
        virtual void OnTextEmote(Player* p_Player, uint32 p_TextEmote, uint32 p_SoundIndex, uint64 p_TargetGUID)
        {
            UNUSED(p_Player);
            UNUSED(p_TextEmote);
            UNUSED(p_SoundIndex);
            UNUSED(p_TargetGUID);
        }

        /// Called in Spell::Cast.
        /// @p_Player    : Player instance
        /// @p_Spell     : Casted spell
        /// @p_SkipCheck : Skipped checks
        virtual void OnSpellCast(Player* p_Player, Spell* p_Spell, bool p_SkipCheck)
        {
            UNUSED(p_Player);
            UNUSED(p_Spell);
            UNUSED(p_SkipCheck);
        }

        /// When the player learn a spell
        /// @p_Player  : Player instance
        /// @p_SpellID : Learned spell ID
        virtual void OnSpellLearned(Player* p_Player, uint32 p_SpellID)
        {
            UNUSED(p_Player);
            UNUSED(p_SpellID);
        }

        /// Called when a player logs in.
        /// @p_Player : Player instance
        virtual void OnLogin(Player* p_Player)
        {
            UNUSED(p_Player);
        }

        /// Called when a player logs out.
        /// @p_Player : Player instance
        virtual void OnLogout(Player* p_Player)
        {
            UNUSED(p_Player);
        }

        /// Called when a player is created.
        /// @p_Player : Player instance
        virtual void OnCreate(Player* p_Player)
        {
            UNUSED(p_Player);
        }

        /// Called when a player is deleted.
        /// @p_GUID : Player instance
        virtual void OnDelete(uint64 p_GUID)
        {
            UNUSED(p_GUID);
        }

        /// Called when a update() of a player is done
        /// @p_Player : Player instance
        /// @p_Diff : diff time
        virtual void OnUpdate(Player* p_Player, uint32 p_Diff)
        {
            UNUSED(p_Player);
            UNUSED(p_Diff);
        }

        /// Called when a spline step is done
        /// @p_Player   : Player instance
        /// @p_MoveType : Movement type
        /// @p_ID       : Movement ID
        virtual void OnMovementInform(Player* p_Player, uint32 p_MoveType, uint32 p_ID)
        {
            UNUSED(p_Player);
            UNUSED(p_MoveType);
            UNUSED(p_ID);
        }

        /// Called when a player is bound to an instance
        /// @p_Player     : Player instance
        /// @p_Difficulty : Instance Difficulty ID
        /// @p_MapID      : Instance Map ID
        /// @p_Permanent  : Is a permanent bind
        virtual void OnBindToInstance(Player* p_Player, Difficulty p_Difficulty, uint32 p_MapID, bool p_Permanent)
        {
            UNUSED(p_Player);
            UNUSED(p_Difficulty);
            UNUSED(p_MapID);
            UNUSED(p_Permanent);
        }

        /// Called when a player switches to a new zone
        /// @p_Player : Player instance
        /// @p_NewZoneID : New player zone ID
        /// @p_OldZoneID : Old player zone ID
        /// @p_NewAreaID : New player area ID
        virtual void OnUpdateZone(Player* p_Player, uint32 p_NewZoneID, uint32 p_OldZoneID, uint32 p_NewAreaID)
        {
            UNUSED(p_Player);
            UNUSED(p_NewZoneID);
            UNUSED(p_OldZoneID);
            UNUSED(p_NewAreaID);
        }

        /// Called when a player updates his movement
        /// @p_Player : Player instance
        virtual void OnUpdateMovement(Player* p_Player)
        {
            UNUSED(p_Player);
        }

        /// Called when player rewards some quest
        /// @p_Player : Player instance
        /// @p_Quest  : Rewarded quest
        virtual void OnQuestReward(Player* p_Player, const Quest* p_Quest)
        {
            UNUSED(p_Player);
            UNUSED(p_Quest);
        }

        /// Called when a player validates some quest objective
        /// @p_Player      : Player instance
        /// @p_QuestID     : Quest ID
        /// @p_ObjectiveID : Validated quest objective ID
        virtual void OnObjectiveValidate(Player* p_Player, uint32 p_QuestID, uint32 p_ObjectiveID)
        {
            UNUSED(p_Player);
            UNUSED(p_QuestID);
            UNUSED(p_ObjectiveID);
        }

        /// Called when player accepts some quest
        /// @p_Player : Player instance
        /// @p_Quest  : Accpeted quest
        virtual void OnQuestAccept(Player* p_Player, const Quest* p_Quest)
        {
            UNUSED(p_Player);
            UNUSED(p_Quest);
        }

        /// Called when player completes some quest
        /// @p_Player : Player instance
        /// @p_Quest  : Completed quest
        virtual void OnQuestComplete(Player* p_Player, const Quest* p_Quest)
        {
            UNUSED(p_Player);
            UNUSED(p_Quest);
        }

        /// Called when player has quest removed from questlog (active or rewarded)
        /// @p_Player : Player instance
        /// @p_Quest  : Removed quest
        virtual void OnQuestAbandon(Player* p_Player, const Quest* p_Quest)
        {
            UNUSED(p_Player);
            UNUSED(p_Quest);
        }

        virtual void OnQuestCleared(Player* p_Player, Quest const* p_Quest)
        {
            UNUSED(p_Player);
            UNUSED(p_Quest);
        }

        /// Called when a player shapeshift
        /// @p_Player : Player instance
        /// @p_Form   : New shapeshift from
        virtual void OnChangeShapeshift(Player* p_Player, ShapeshiftForm p_Form)
        {
            UNUSED(p_Player);
            UNUSED(p_Form);
        }

        /// Called when a player loot an item
        /// @p_Player : Player instance
        /// @p_Item   : New looted item instance
        virtual void OnItemLooted(Player* p_Player, Item* p_Item)
        {
            UNUSED(p_Player);
            UNUSED(p_Item);
        }

        /// Called when a player enter in combat
        /// @p_Player : Player instance
        virtual void OnEnterInCombat(Player* p_Player)
        {
            UNUSED(p_Player);
        }

        virtual void OnMount(Player* p_Player, uint32 p_CreatureID)
        {
            UNUSED(p_Player);
            UNUSED(p_CreatureID);
        }

        virtual void OnDismount(Player* p_Player, uint32 p_CreatureID)
        {
            UNUSED(p_Player);
            UNUSED(p_CreatureID);
        }

        /// Called when a player leave combat status
        /// @p_Player : Player instance
        virtual void OnLeaveCombat(Player* p_Player)
        {
            UNUSED(p_Player);
        }

        /// Called when a player receive a scene triggered event
        /// @p_Player          : Player instance
        /// @p_SceneInstanceID : Standalone scene instance ID
        /// @p_Event           : Event string received from client
        virtual void OnSceneTriggerEvent(Player* p_Player, uint32 p_SceneInstanceID, std::string p_Event)
        {
            UNUSED(p_Player);
            UNUSED(p_SceneInstanceID);
            UNUSED(p_Event);
        }

        /// Called when a player regen a power
        /// Return false to prevent default regeneration
        /// @p_Player   : Player instance
        /// @p_Power    : Power to be regenerate
        /// @p_AddValue : amount of power to regenerate
        virtual void OnRegenPower(Player* p_Player, Powers const p_Power, float& l_AddValue, bool& p_PreventDefault)
        {
            UNUSED(p_Player);
            UNUSED(p_Power);
            UNUSED(l_AddValue);
            UNUSED(p_PreventDefault);
        }

        // Called when some faction change is applied on the player.
        virtual void OnFactionChanged(Player* p_Player)
        {
            UNUSED(p_Player);
        }

        /// Called when a player take damage
        /// @p_Player          : Player instance
        /// @p_DamageEffectTyp : Damage type
        /// @p_Damage          : Amount of damage taken
        /// @p_SchoolMask      : school mask of the damage
        virtual void OnTakeDamage(Player* p_Player, DamageEffectType p_DamageEffectType, uint32 p_Damage, SpellSchoolMask p_SchoolMask, CleanDamage const* p_CleanDamage)
        {
            UNUSED(p_Player);
            UNUSED(p_DamageEffectType);
            UNUSED(p_Damage);
            UNUSED(p_SchoolMask);
            UNUSED(p_CleanDamage);
        }

        /// Called when player block attack
        /// @p_Player : Player instance
        /// @p_DamageInfo  : Damage Infos
        virtual void OnBlock(Player* p_Player, Unit* p_Attacker)
        {
            UNUSED(p_Player);
            UNUSED(p_Attacker);
        }

        /// Called when a player cancels some scene
        /// @p_Player          : Player instance
        /// @p_SceneInstanceID : Standalone scene instance ID
        virtual void OnSceneCancel(Player* p_Player, uint32 p_SceneInstanceId)
        {
            UNUSED(p_Player);
            UNUSED(p_SceneInstanceId);
        }

        /// Called when a player enter in bg
        /// @p_Player   : Player instance
        /// @p_MapID    : Map ID
        virtual void OnEnterBG(Player* p_Player, uint32 p_MapID)
        {
            UNUSED(p_Player);
            UNUSED(p_MapID);
        }

        /// Called when a leave a bg
        /// @p_Player   : Player instance
        /// @p_MapID    : Map ID
        virtual void OnLeaveBG(Player* p_Player, uint32 p_MapID)
        {
            UNUSED(p_Player);
            UNUSED(p_MapID);
        }

        /// Called when a player finish a movement like a jump
        /// @p_Player   : Player instance
        /// @p_SpellID  : Spell ID
        /// @p_TargetGUID : Target GUID
        virtual void OnFinishMovement(Player* p_Player, uint32 p_SpellID, uint64 const p_TargetGUID)
        {
            UNUSED(p_Player);
            UNUSED(p_SpellID);
            UNUSED(p_TargetGUID);
        }

        /// Called when player earn achievement
        /// @p_Player : Player instance
        /// @p_Attacker  : Achievement
        /// @p_SendAchievement : Set to false to cancel achievement earned
        virtual void OnAchievementEarned(Player* p_Player, AchievementEntry const* p_Achievement, bool& p_SendAchievement)
        {
            UNUSED(p_Player);
            UNUSED(p_Achievement);
        }

};

#endif  ///< SCRIPTING_INTERFACES_PLAYER_HPP_INCLUDED
