////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "Config.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "ObjectMgr.h"
#include "OutdoorPvPMgr.h"
#include "ScriptLoader.h"
#include "ScriptSystem.h"
#include "Transport.h"
#include "Vehicle.h"
#include "SpellInfo.h"
#include "SpellScript.h"
#include "GossipDef.h"
#include "CreatureAIImpl.h"
#include "SpellAuraEffects.h"
#include "BattlepayMgr.h"

void DoScriptText(int32 p_ItemTextEntry, WorldObject* p_Source, Unit* p_Target)
{
    if (!p_Source)
    {
        sLog->outError(LOG_FILTER_TSCR, "DoScriptText entry %i, invalid Source pointer.", p_ItemTextEntry);
        return;
    }

    if (p_ItemTextEntry >= 0)
    {
        sLog->outError(LOG_FILTER_TSCR, "DoScriptText with source entry %u (TypeId=%u, guid=%u) attempts to process text entry %i, but text entry must be negative.", p_Source->GetEntry(), p_Source->GetTypeId(), p_Source->GetGUIDLow(), p_ItemTextEntry);
        return;
    }

    StringTextData const* l_TextDatas = sScriptSystemMgr->GetTextData(p_ItemTextEntry);
    if (!l_TextDatas)
    {
        sLog->outError(LOG_FILTER_TSCR, "DoScriptText with source entry %u (TypeId=%u, guid=%u) could not find text entry %i.", p_Source->GetEntry(), p_Source->GetTypeId(), p_Source->GetGUIDLow(), p_ItemTextEntry);
        return;
    }

    sLog->outDebug(LOG_FILTER_TSCR, "DoScriptText: text entry=%i, Sound=%u, Type=%u, Language=%u, Emote=%u", p_ItemTextEntry, l_TextDatas->uiSoundId, l_TextDatas->uiType, l_TextDatas->uiLanguage, l_TextDatas->uiEmote);

    if (l_TextDatas->uiSoundId)
    {
        if (sSoundEntriesStore.LookupEntry(l_TextDatas->uiSoundId))
            p_Source->SendPlaySound(l_TextDatas->uiSoundId, false);
        else
            sLog->outError(LOG_FILTER_TSCR, "DoScriptText entry %i tried to process invalid sound id %u.", p_ItemTextEntry, l_TextDatas->uiSoundId);
    }

    if (l_TextDatas->uiEmote)
    {
        if (p_Source->GetTypeId() == TYPEID_UNIT || p_Source->IsPlayer())
            ((Unit*)p_Source)->HandleEmoteCommand(l_TextDatas->uiEmote);
        else
            sLog->outError(LOG_FILTER_TSCR, "DoScriptText entry %i tried to process emote for invalid TypeId (%u).", p_ItemTextEntry, p_Source->GetTypeId());
    }

    switch (l_TextDatas->uiType)
    {
        case CHAT_TYPE_SAY:
            p_Source->MonsterYell(p_ItemTextEntry, l_TextDatas->uiLanguage, p_Target ? p_Target->GetGUID() : 0);
            break;
        case CHAT_TYPE_YELL:
            p_Source->MonsterYell(p_ItemTextEntry, l_TextDatas->uiLanguage, p_Target ? p_Target->GetGUID() : 0);
            break;
        case CHAT_TYPE_TEXT_EMOTE:
            p_Source->MonsterTextEmote(p_ItemTextEntry, p_Target ? p_Target->GetGUID() : 0);
            break;
        case CHAT_TYPE_BOSS_EMOTE:
            p_Source->MonsterTextEmote(p_ItemTextEntry, p_Target ? p_Target->GetGUID() : 0, true);
            break;
        case CHAT_TYPE_WHISPER:
        {
            if (p_Target && p_Target->IsPlayer())
                p_Source->MonsterWhisper(p_ItemTextEntry, p_Target->GetGUID());
            else
                sLog->outError(LOG_FILTER_TSCR, "DoScriptText entry %i cannot whisper without target unit (TYPEID_PLAYER).", p_ItemTextEntry);

            break;
        }
        case CHAT_TYPE_BOSS_WHISPER:
        {
            if (p_Target && p_Target->IsPlayer())
                p_Source->MonsterWhisper(p_ItemTextEntry, p_Target->GetGUID(), true);
            else
                sLog->outError(LOG_FILTER_TSCR, "DoScriptText entry %i cannot whisper without target unit (TYPEID_PLAYER).", p_ItemTextEntry);

            break;
        }
        case CHAT_TYPE_ZONE_YELL:
            p_Source->MonsterYellToZone(p_ItemTextEntry, l_TextDatas->uiLanguage, p_Target ? p_Target->GetGUID() : 0);
            break;
    }
}

struct TSpellSummary
{
    uint8 Targets;                                          ///< Set of enum SelectTarget
    uint8 Effects;                                          ///< Set of enum SelectEffect
} *SpellSummary;

namespace
{
    typedef std::set<ScriptObject*> ExampleScriptContainer;
    ExampleScriptContainer ExampleScripts;
}

/// This is the global static registry of scripts.
template<class TScript> class ScriptRegistry
{
    public:

        typedef std::map<uint32, TScript*> ScriptMap;
        typedef typename ScriptMap::iterator ScriptMapIterator;

        /// The actual list of scripts. This will be accessed concurrently, so it must not be modified
        /// after server startup.
        static ScriptMap ScriptPointerList;

        static void Clear()
        {
            for (auto l_ScriptPair : ScriptPointerList)
                delete l_ScriptPair.second;

            ScriptPointerList.clear();
        }

        static void AddScript(TScript* const p_Script)
        {
            ASSERT(p_Script);

            // See if the script is using the same memory as another script. If this happens, it means that
            // someone forgot to allocate new memory for a script.
            for (ScriptMapIterator l_It = ScriptPointerList.begin(); l_It != ScriptPointerList.end(); ++l_It)
            {
                if (l_It->second == p_Script)
                {
                    sLog->outError(LOG_FILTER_TSCR, "Script '%s' has same memory pointer as '%s'.",
                        p_Script->GetName().c_str(), l_It->second->GetName().c_str());

                    return;
                }
            }

            if (p_Script->IsDatabaseBound())
            {
                // Get an ID for the script. An ID only exists if it's a script that is assigned in the database
                // through a script name (or similar).
                uint32 l_ID = sObjectMgr->GetScriptId(p_Script->GetName().c_str());
                if (l_ID)
                {
                    // Try to find an existing script.
                    bool l_Exists = false;
                    for (ScriptMapIterator l_It = ScriptPointerList.begin(); l_It != ScriptPointerList.end(); ++l_It)
                    {
                        // If the script names match...
                        if (l_It->second->GetName() == p_Script->GetName())
                        {
                            // ... It exists.
                            l_Exists = true;
                            break;
                        }
                    }

                    // If the script isn't assigned -> assign it!
                    if (!l_Exists)
                    {
                        ScriptPointerList[l_ID] = p_Script;
                        sScriptMgr->IncrementScriptCount();
                    }
                }
                else
                {
                    // The script uses a script name from database, but isn't assigned to anything.
                    if (p_Script->GetName().find("example") == std::string::npos && p_Script->GetName().find("Smart") == std::string::npos)
                        sLog->outError(LOG_FILTER_SQL, "Script named '%s' does not have a script name assigned in database.",
                            p_Script->GetName().c_str());

                        // These scripts don't get stored anywhere so throw them into this to avoid leaking memory
                    ExampleScripts.insert(p_Script);
                }
            }
            else
            {
                // We're dealing with a code-only script; just add it.
                ScriptPointerList[_scriptIdCounter++] = p_Script;
                sScriptMgr->IncrementScriptCount();
            }
        }

        /// Gets a script by its ID (assigned by ObjectMgr).
        static TScript* GetScriptById(uint32 p_ID)
        {
            ScriptMapIterator l_It = ScriptPointerList.find(p_ID);
            if (l_It != ScriptPointerList.end())
                return l_It->second;

            return nullptr;
        }

    private:

        /// Counter used for code-only scripts.
        static uint32 _scriptIdCounter;
};

/// Utility macros to refer to the script registry.
#define SCR_REG_MAP(T) ScriptRegistry<T>::ScriptMap
#define SCR_REG_ITR(T) ScriptRegistry<T>::ScriptMapIterator
#define SCR_REG_LST(T) ScriptRegistry<T>::ScriptPointerList

/// Utility macros for looping over scripts.
#define FOR_SCRIPTS(T, C, E) \
    if (SCR_REG_LST(T).empty()) \
        return; \
    for (SCR_REG_ITR(T) C = SCR_REG_LST(T).begin(); \
        C != SCR_REG_LST(T).end(); ++C)
#define FOR_SCRIPTS_RET(T, C, E, R) \
    if (SCR_REG_LST(T).empty()) \
        return R; \
    for (SCR_REG_ITR(T) C = SCR_REG_LST(T).begin(); \
        C != SCR_REG_LST(T).end(); ++C)
#define FOREACH_SCRIPT(T) \
    FOR_SCRIPTS(T, itr, end) \
    itr->second

/// Utility macros for finding specific scripts.
#define GET_SCRIPT_NO_RET(T, I, V) \
    T* V = ScriptRegistry<T>::GetScriptById(I);

#define GET_SCRIPT(T, I, V) \
    T* V = ScriptRegistry<T>::GetScriptById(I); \
    if (!V) \
        return;

#define GET_SCRIPT_RET(T, I, V, R) \
    T* V = ScriptRegistry<T>::GetScriptById(I); \
    if (!V) \
        return R;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Constructor
ScriptMgr::ScriptMgr() : m_ScriptCount(0), m_ScheduledScripts(0)
{

}

/// Destructor
ScriptMgr::~ScriptMgr()
{

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Initialize Script Mgr and bind all script
void ScriptMgr::Initialize()
{
    uint32 l_OldMSTime = getMSTime();

    LoadDatabase();

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading C++ scripts");

    FillSpellSummary();
    AddScripts();

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u C++ scripts in %u ms", GetScriptCount(), GetMSTimeDiffToNow(l_OldMSTime));
}

/// Unload all script
void ScriptMgr::Unload()
{
    /// Clear scripts for every script type.
    ScriptRegistry<SpellScriptLoader>::Clear();
    ScriptRegistry<ServerScript>::Clear();
    ScriptRegistry<WorldScript>::Clear();
    ScriptRegistry<FormulaScript>::Clear();
    ScriptRegistry<WorldMapScript>::Clear();
    ScriptRegistry<InstanceMapScript>::Clear();
    ScriptRegistry<BattlegroundMapScript>::Clear();
    ScriptRegistry<ItemScript>::Clear();
    ScriptRegistry<CreatureScript>::Clear();
    ScriptRegistry<GameObjectScript>::Clear();
    ScriptRegistry<AreaTriggerScript>::Clear();
    ScriptRegistry<BattlegroundScript>::Clear();
    ScriptRegistry<OutdoorPvPScript>::Clear();
    ScriptRegistry<CommandScript>::Clear();
    ScriptRegistry<WeatherScript>::Clear();
    ScriptRegistry<AuctionHouseScript>::Clear();
    ScriptRegistry<ConditionScript>::Clear();
    ScriptRegistry<VehicleScript>::Clear();
    ScriptRegistry<DynamicObjectScript>::Clear();
    ScriptRegistry<TransportScript>::Clear();
    ScriptRegistry<AchievementCriteriaScript>::Clear();
    ScriptRegistry<PlayerScript>::Clear();
    ScriptRegistry<GuildScript>::Clear();
    ScriptRegistry<GroupScript>::Clear();
    ScriptRegistry<AreaTriggerEntityScript>::Clear();
    ScriptRegistry<EncounterScript>::Clear();

    for (ExampleScriptContainer::iterator l_It = ExampleScripts.begin(); l_It != ExampleScripts.end(); ++l_It)
        delete *l_It;

    ExampleScripts.clear();

    delete[] UnitAI::AISpellInfo;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Load all script data (text, waypoint, ...)
void ScriptMgr::LoadDatabase()
{
    sScriptSystemMgr->LoadScriptTexts();
    sScriptSystemMgr->LoadScriptTextsCustom();
    sScriptSystemMgr->LoadScriptWaypoints();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Increase script count
void ScriptMgr::IncrementScriptCount()
{
    ++m_ScriptCount;
}

/// Get script count
uint32 ScriptMgr::GetScriptCount() const
{
    return m_ScriptCount;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Initialize some spell date for scripted creature
void ScriptMgr::FillSpellSummary()
{
    UnitAI::FillAISpellInfo();

    SpellSummary = new TSpellSummary[sSpellMgr->GetSpellInfoStoreSize()];

    SpellInfo const* l_TempSpell;

    for (uint32 l_I = 0; l_I < sSpellMgr->GetSpellInfoStoreSize(); ++l_I)
    {
        SpellSummary[l_I].Effects = 0;
        SpellSummary[l_I].Targets = 0;

        l_TempSpell = sSpellMgr->GetSpellInfo(l_I);
        /// This spell doesn't exist.
        if (!l_TempSpell)
            continue;

        for (uint32 l_J = 0; l_J < l_TempSpell->EffectCount; ++l_J)
        {
            /// Spell targets self.
            if (l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_UNIT_CASTER)
                SpellSummary[l_I].Targets |= 1 << (SELECT_TARGET_SELF - 1);

            /// Spell targets a single enemy.
            if (l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_UNIT_TARGET_ENEMY ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_DEST_TARGET_ENEMY)
                SpellSummary[l_I].Targets |= 1 << (SELECT_TARGET_SINGLE_ENEMY - 1);

            /// Spell targets AoE at enemy.
            if (l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_UNIT_SRC_AREA_ENEMY ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_UNIT_DEST_AREA_ENEMY ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_SRC_CASTER ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_DEST_DYNOBJ_ENEMY)
                SpellSummary[l_I].Targets |= 1 << (SELECT_TARGET_AOE_ENEMY - 1);

            /// Spell targets an enemy.
            if (l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_UNIT_TARGET_ENEMY ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_DEST_TARGET_ENEMY ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_UNIT_SRC_AREA_ENEMY ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_UNIT_DEST_AREA_ENEMY ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_SRC_CASTER ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_DEST_DYNOBJ_ENEMY)
                SpellSummary[l_I].Targets |= 1 << (SELECT_TARGET_ANY_ENEMY - 1);

            /// Spell targets a single friend (or self).
            if (l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_UNIT_CASTER ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_UNIT_TARGET_ALLY ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_UNIT_TARGET_PARTY)
                SpellSummary[l_I].Targets |= 1 << (SELECT_TARGET_SINGLE_FRIEND - 1);

            /// Spell targets AoE friends.
            if (l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_UNIT_CASTER_AREA_PARTY ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_UNIT_LASTTARGET_AREA_PARTY ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_SRC_CASTER)
                SpellSummary[l_I].Targets |= 1 << (SELECT_TARGET_AOE_FRIEND - 1);

            /// Spell targets any friend (or self).
            if (l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_UNIT_CASTER ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_UNIT_TARGET_ALLY ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_UNIT_TARGET_PARTY ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_UNIT_CASTER_AREA_PARTY ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_UNIT_LASTTARGET_AREA_PARTY ||
                l_TempSpell->Effects[l_J].TargetA.GetTarget() == TARGET_SRC_CASTER)
                SpellSummary[l_I].Targets |= 1 << (SELECT_TARGET_ANY_FRIEND - 1);

            /// Make sure that this spell includes a damage effect.
            if (l_TempSpell->Effects[l_J].Effect == SPELL_EFFECT_SCHOOL_DAMAGE ||
                l_TempSpell->Effects[l_J].Effect == SPELL_EFFECT_INSTAKILL ||
                l_TempSpell->Effects[l_J].Effect == SPELL_EFFECT_ENVIRONMENTAL_DAMAGE ||
                l_TempSpell->Effects[l_J].Effect == SPELL_EFFECT_HEALTH_LEECH)
                SpellSummary[l_I].Effects |= 1 << (SELECT_EFFECT_DAMAGE - 1);

            /// Make sure that this spell includes a healing effect (or an apply aura with a periodic heal).
            if (l_TempSpell->Effects[l_J].Effect == SPELL_EFFECT_HEAL ||
                l_TempSpell->Effects[l_J].Effect == SPELL_EFFECT_HEAL_MAX_HEALTH ||
                l_TempSpell->Effects[l_J].Effect == SPELL_EFFECT_HEAL_MECHANICAL ||
                ((l_TempSpell->Effects[l_J].Effect == SPELL_EFFECT_APPLY_AURA || l_TempSpell->Effects[l_J].Effect == SPELL_EFFECT_APPLY_AURA_ON_PET)
                && l_TempSpell->Effects[l_J].ApplyAuraName == 8))
                SpellSummary[l_I].Effects |= 1 << (SELECT_EFFECT_HEALING - 1);

            /// Make sure that this spell applies an aura.
            if (l_TempSpell->Effects[l_J].Effect == SPELL_EFFECT_APPLY_AURA ||
                l_TempSpell->Effects[l_J].Effect == SPELL_EFFECT_APPLY_AURA_ON_PET)
                SpellSummary[l_I].Effects |= 1 << (SELECT_EFFECT_AURA - 1);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Increase scheduled script count
uint32 ScriptMgr::IncreaseScheduledScriptsCount()
{
    return ++m_ScheduledScripts;
}

/// Decrease scheduled script count
uint32 ScriptMgr::DecreaseScheduledScriptCount()
{
    return --m_ScheduledScripts;
}

/// Decrease scheduled script count
uint32 ScriptMgr::DecreaseScheduledScriptCount(size_t count)
{
    return m_ScheduledScripts -= count;
}

/// Is script scheduled
bool ScriptMgr::IsScriptScheduled() const
{
    return m_ScheduledScripts > 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Called when the area trigger is activated by a player.
/// @p_Player  : Player who trigger this area trigger
/// @p_Trigger : Area Trigger
bool ScriptMgr::OnAreaTrigger(Player* p_Player, const AreaTriggerEntry* p_Trigger)
{
    ASSERT(p_Player);
    ASSERT(p_Trigger);

    GET_SCRIPT_RET(AreaTriggerScript, sObjectMgr->GetAreaTriggerScriptId(p_Trigger->ID), tmpscript, false);
    return tmpscript->OnTrigger(p_Player, p_Trigger);
}

void ScriptMgr::OnEnterAreaTrigger(Player* p_Player, AreaTriggerEntry const* p_AreatriggerEntry)
{
    ASSERT(p_Player);
    ASSERT(p_AreatriggerEntry);

    uint32 l_ID = sObjectMgr->GetAreaTriggerScriptId(p_AreatriggerEntry->ID);
    if (!l_ID)
        return;

    AreaTriggerScript* l_Script = ScriptRegistry<AreaTriggerScript>::GetScriptById(l_ID);
    if (l_Script)
        l_Script->OnEnter(p_Player, p_AreatriggerEntry);
}

void ScriptMgr::OnExitAreaTrigger(Player* p_Player, AreaTriggerEntry const* p_AreatriggerEntry)
{
    ASSERT(p_Player);
    ASSERT(p_AreatriggerEntry);

    uint32 l_ID = sObjectMgr->GetAreaTriggerScriptId(p_AreatriggerEntry->ID);
    if (!l_ID)
        return;

    AreaTriggerScript* l_Script = ScriptRegistry<AreaTriggerScript>::GetScriptById(l_ID);
    if (l_Script)
        l_Script->OnExit(p_Player, p_AreatriggerEntry);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Assign script to Areatrigger
void ScriptMgr::InitScriptEntity(AreaTrigger* p_AreaTrigger)
{
    ASSERT(p_AreaTrigger);

    // On creation, we look for instantiating a new script, locally to the AreaTrigger.
    if (p_AreaTrigger->GetScript())
        return;

    AreaTriggerEntityScript* l_AreaTriggerScript = ScriptRegistry<AreaTriggerEntityScript>::GetScriptById(p_AreaTrigger->GetMainTemplate()->m_ScriptId);
    if (l_AreaTriggerScript == nullptr)
        return;

    p_AreaTrigger->SetScript(l_AreaTriggerScript->GetAI());
}

/// Proc when AreaTrigger is created.
/// @p_AreaTrigger : AreaTrigger instance
void ScriptMgr::OnCreateAreaTriggerEntity(AreaTrigger* p_AreaTrigger)
{
    ASSERT(p_AreaTrigger);
    
    AreaTriggerEntityScript* l_Script = p_AreaTrigger->GetScript();
    if (!l_Script)
        return;

    l_Script->OnCreate(p_AreaTrigger);
}

/// Procs before creation to specify position and linear destination of the areatrigger
/// @p_AreaTrigger: Areatrigger Instance
/// @p_Caster: Caster because he the Areatrigger is not spawned so caster is not defined
/// @p_SourcePosition: Spawn location of the Areatrigger
/// @p_DestinationPostion: Linear destination of the Areatrigger
/// @p_PathToLinearDestination: Linear path without the endpoint
void ScriptMgr::OnSetCreatePositionEntity(AreaTrigger* p_AreaTrigger, Unit* p_Caster, Position& p_SourcePosition, Position& p_DestinationPosition, std::list<Position>& p_PathToLinearDestination)
{
    ASSERT(p_AreaTrigger);

    AreaTriggerEntityScript* l_Script = p_AreaTrigger->GetScript();
    if (!l_Script)
        return;

    l_Script->OnSetCreatePosition(p_AreaTrigger, p_Caster, p_SourcePosition, p_DestinationPosition, p_PathToLinearDestination);
}

/// Proc when AreaTrigger is updated.
/// @p_AreaTrigger : AreaTrigger instance
/// @p_Time        : Diff since last update
void ScriptMgr::OnUpdateAreaTriggerEntity(AreaTrigger* p_AreaTrigger, uint32 p_Time)
{
    ASSERT(p_AreaTrigger);

    if (!p_AreaTrigger->GetScript())
        return;

    p_AreaTrigger->GetScript()->OnUpdate(p_AreaTrigger, p_Time);
}

/// Proc when AreaTrigger is removed.
/// @p_AreaTrigger : AreaTrigger instance
/// @p_Time        : Diff since last update
void ScriptMgr::OnRemoveAreaTriggerEntity(AreaTrigger* p_AreaTrigger, uint32 p_Time)
{
    ASSERT(p_AreaTrigger);

    if (!p_AreaTrigger->GetScript())
        return;

    p_AreaTrigger->GetScript()->OnRemove(p_AreaTrigger, p_Time);
}

void ScriptMgr::OnDestinationReached(AreaTrigger* p_AreaTrigger)
{
    ASSERT(p_AreaTrigger);

    if (!p_AreaTrigger->GetScript())
        return;

    p_AreaTrigger->GetScript()->OnDestinationReached(p_AreaTrigger);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Get Creature script by ScriptID
/// @p_ScriptID : ScriptID from the creature template
CreatureScript* ScriptMgr::GetCreatureScriptByID(uint32 p_ScriptID)
{
    return ScriptRegistry<CreatureScript>::GetScriptById(p_ScriptID);
}

/// Called when a dummy spell effect is triggered on the creature.
/// @p_Caster      : Spell Caster
/// @p_SpellID     : Casted spell ID
/// @p_EffectIndex : Dummy effect index
/// @p_Target      : Spell target
bool ScriptMgr::OnDummyEffect(Unit* p_Caster, uint32 p_SpellID, SpellEffIndex p_EffectIndex, Creature* p_Target)
{
    ASSERT(p_Caster);
    ASSERT(p_Target);

    GET_SCRIPT_RET(CreatureScript, p_Target->GetScriptId(), tmpscript, false);
    return tmpscript->OnDummyEffect(p_Caster, p_SpellID, p_EffectIndex, p_Target);
}

/// Called when a player opens a gossip dialog with the creature.
/// @p_Player   : Source player instance
/// @p_Creature : Target creature instance
bool ScriptMgr::OnGossipHello(Player* p_Player, Creature* p_Creature)
{
    ASSERT(p_Player);
    ASSERT(p_Creature);

    GET_SCRIPT_RET(CreatureScript, p_Creature->GetScriptId(), tmpscript, false);
    p_Player->PlayerTalkClass->ClearMenus();
    return tmpscript->OnGossipHello(p_Player, p_Creature);
}

/// Called when a player selects a gossip item in the creature's gossip menu.
/// @p_Player   : Source player instance
/// @p_Creature : Target creature instance
/// @p_Sender   : Sender menu
/// @p_Action   : Action
bool ScriptMgr::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action)
{
    ASSERT(p_Player);
    ASSERT(p_Creature);

    GET_SCRIPT_RET(CreatureScript, p_Creature->GetScriptId(), tmpscript, false);
    return tmpscript->OnGossipSelect(p_Player, p_Creature, p_Sender, p_Action);
}

/// Called when a player selects a gossip with a code in the creature's gossip menu.
/// @p_Player   : Source player instance
/// @p_Creature : Target creature instance
/// @p_Sender   : Sender menu
/// @p_Action   : Action
/// @p_Code     : Player input code
bool ScriptMgr::OnGossipSelectCode(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action, const char* p_Code)
{
    ASSERT(p_Player);
    ASSERT(p_Creature);
    ASSERT(p_Code);

    GET_SCRIPT_RET(CreatureScript, p_Creature->GetScriptId(), tmpscript, false);
    return tmpscript->OnGossipSelectCode(p_Player, p_Creature, p_Sender, p_Action, p_Code);
}

/// Called when a player accepts a quest from the creature.
/// @p_Player   : Source player instance
/// @p_Creature : Target creature instance
/// @p_Quest    : Accepted quest
bool ScriptMgr::OnQuestAccept(Player* p_Player, Creature* p_Creature, const Quest* p_Quest)
{
    ASSERT(p_Player);
    ASSERT(p_Creature);
    ASSERT(p_Quest);

    GET_SCRIPT_RET(CreatureScript, p_Creature->GetScriptId(), tmpscript, false);
    p_Player->PlayerTalkClass->ClearMenus();
    return tmpscript->OnQuestAccept(p_Player, p_Creature, p_Quest);
}

/// Called when a player selects a quest in the creature's quest menu.
/// @p_Player   : Source player instance
/// @p_Creature : Target creature instance
/// @p_Quest    : Selected quest
bool ScriptMgr::OnQuestSelect(Player* p_Player, Creature* p_Creature, const Quest* p_Quest)
{
    ASSERT(p_Player);
    ASSERT(p_Creature);
    ASSERT(p_Quest);

    GET_SCRIPT_RET(CreatureScript, p_Creature->GetScriptId(), tmpscript, false);
    p_Player->PlayerTalkClass->ClearMenus();
    return tmpscript->OnQuestSelect(p_Player, p_Creature, p_Quest);
}

/// Called when a player completes a quest with the creature.
/// @p_Player   : Source player instance
/// @p_Creature : Target creature instance
/// @p_Quest    : Completed quest
bool ScriptMgr::OnQuestComplete(Player* p_Player, Creature* p_Creature, const Quest* p_Quest)
{
    ASSERT(p_Player);
    ASSERT(p_Creature);
    ASSERT(p_Quest);

    GET_SCRIPT_RET(CreatureScript, p_Creature->GetScriptId(), tmpscript, false);
    p_Player->PlayerTalkClass->ClearMenus();
    return tmpscript->OnQuestComplete(p_Player, p_Creature, p_Quest);
}

/// Called when a player selects a quest reward.
/// @p_Player   : Source player instance
/// @p_Creature : Target creature instance
/// @p_Quest    : Reward quest
/// @p_Option   : Reward
bool ScriptMgr::OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option)
{
    ASSERT(p_Player);
    ASSERT(p_Creature);
    ASSERT(p_Quest);

    GET_SCRIPT_RET(CreatureScript, p_Creature->GetScriptId(), tmpscript, false);
    p_Player->PlayerTalkClass->ClearMenus();
    return tmpscript->OnQuestReward(p_Player, p_Creature, p_Quest, p_Option);
}

/// Called when the dialog status between a player and the creature is requested.
/// @p_Player   : Source player instance
/// @p_Creature : Target creature instance
uint32 ScriptMgr::GetDialogStatus(Player* p_Player, Creature* p_Creature)
{
    ASSERT(p_Player);
    ASSERT(p_Creature);

    /// TODO: 100 is a funny magic number to have hanging around here...
    GET_SCRIPT_RET(CreatureScript, p_Creature->GetScriptId(), tmpscript, 100);
    p_Player->PlayerTalkClass->ClearMenus();
    return tmpscript->GetDialogStatus(p_Player, p_Creature);
}

/// Called when player asks from a creature gossip to create a shipment.
/// @p_Player       : Player that created the shipment
/// @p_Creature     : NPC that got the gossip
void ScriptMgr::OnShipmentCreated(Player* p_Player, Creature* p_Creature, uint32 p_BuildingID)
{
    ASSERT(p_Player);
    ASSERT(p_Creature);
    ASSERT(p_BuildingID);

    GET_SCRIPT(CreatureScript, p_Creature->GetScriptId(), tmpscript);
    tmpscript->OnShipmentCreated(p_Player, p_Creature, p_BuildingID);
}

/// Called when a CreatureAI object is needed for the creature.
/// @p_Creature : Target creature instance
CreatureAI* ScriptMgr::GetCreatureAI(Creature* p_Creature)
{
    ASSERT(p_Creature);

    GET_SCRIPT_NO_RET(CreatureScript, p_Creature->GetScriptId(), tmpscript);
    GET_SCRIPT_NO_RET(VehicleScript, p_Creature->GetScriptId(), tmpVehiclescript);

    if (tmpscript)
        return tmpscript->GetAI(p_Creature);
    else if (tmpVehiclescript)
        return tmpVehiclescript->GetAI(p_Creature);
    else
        return NULL;
}

/// On update
/// @p_Object : Updated object instance
/// @p_Diff   : Time since last update
void ScriptMgr::OnCreatureUpdate(Creature* p_Object, uint32 p_Diff)
{
    ASSERT(p_Object);

    GET_SCRIPT(CreatureScript, p_Object->GetScriptId(), tmpscript);
    tmpscript->OnUpdate(p_Object, p_Diff);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Called when a dummy spell effect is triggered on the GameObject.
/// @p_Caster      : Spell Caster
/// @p_SpellID     : Casted spell ID
/// @p_EffectIndex : Dummy effect index
/// @p_Target      : Spell target
bool ScriptMgr::OnDummyEffect(Unit* p_Caster, uint32 p_SpellID, SpellEffIndex p_EffectIndex, GameObject* p_Target)
{
    ASSERT(p_Caster);
    ASSERT(p_Target);

    GET_SCRIPT_RET(GameObjectScript, p_Target->GetScriptId(), tmpscript, false);
    return tmpscript->OnDummyEffect(p_Caster, p_SpellID, p_EffectIndex, p_Target);
}

/// Called when a player opens a gossip dialog with the GameObject.
/// @p_Player     : Source player instance
/// @p_GameObject : Target GameObject instance
bool ScriptMgr::OnGossipHello(Player* p_Player, GameObject* p_GameObject)
{
    ASSERT(p_Player);
    ASSERT(p_GameObject);

    GET_SCRIPT_RET(GameObjectScript, p_GameObject->GetScriptId(), tmpscript, false);
    p_Player->PlayerTalkClass->ClearMenus();
    return tmpscript->OnGossipHello(p_Player, p_GameObject);
}

/// Called when a player selects a gossip item in the GameObject's gossip menu.
/// @p_Player     : Source player instance
/// @p_GameObject : Target GameObject instance
/// @p_Sender     : Sender menu
/// @p_Action     : Action
bool ScriptMgr::OnGossipSelect(Player* p_Player, GameObject* p_GameObject, uint32 p_Sender, uint32 p_Action)
{
    ASSERT(p_Player);
    ASSERT(p_GameObject);

    GET_SCRIPT_RET(GameObjectScript, p_GameObject->GetScriptId(), tmpscript, false);
    return tmpscript->OnGossipSelect(p_Player, p_GameObject, p_Sender, p_Action);
}

/// Called when a player selects a gossip with a code in the GameObject's gossip menu.
/// @p_Player     : Source player instance
/// @p_GameObject : Target GameObject instance
/// @p_Sender     : Sender menu
/// @p_Action     : Action
/// @p_Code       : Player input code
bool ScriptMgr::OnGossipSelectCode(Player* p_Player, GameObject* p_GameObject, uint32 p_Sender, uint32 p_Action, const char* p_Code)
{
    ASSERT(p_Player);
    ASSERT(p_GameObject);
    ASSERT(p_Code);

    GET_SCRIPT_RET(GameObjectScript, p_GameObject->GetScriptId(), tmpscript, false);
    return tmpscript->OnGossipSelectCode(p_Player, p_GameObject, p_Sender, p_Action, p_Code);
}

/// Called when a player accepts a quest from the game object.
/// @p_Player     : Source player instance
/// @p_GameObject : Target GameObject instance
/// @p_Quest      : Accepted quest
bool ScriptMgr::OnQuestAccept(Player* p_Player, GameObject* p_GameObject, const Quest* p_Quest)
{
    ASSERT(p_Player);
    ASSERT(p_GameObject);
    ASSERT(p_Quest);

    GET_SCRIPT_RET(GameObjectScript, p_GameObject->GetScriptId(), tmpscript, false);
    p_Player->PlayerTalkClass->ClearMenus();
    return tmpscript->OnQuestAccept(p_Player, p_GameObject, p_Quest);
}

/// Called when a player selects a quest reward.
/// @p_Player     : Source player instance
/// @p_GameObject : Target GameObject instance
/// @p_Quest      : Reward quest
/// @p_Option     : Reward
bool ScriptMgr::OnQuestReward(Player* p_Player, GameObject* p_GameObject, Quest const* p_Quest, uint32 p_Option)
{
    ASSERT(p_Player);
    ASSERT(p_GameObject);
    ASSERT(p_Quest);

    GET_SCRIPT_RET(GameObjectScript, p_GameObject->GetScriptId(), tmpscript, false);
    p_Player->PlayerTalkClass->ClearMenus();
    return tmpscript->OnQuestReward(p_Player, p_GameObject, p_Quest, p_Option);
}

/// Called when the dialog status between a player and the GameObject is requested.
/// @p_Player     : Source player instance
/// @p_GameObject : Target GameObject instance
uint32 ScriptMgr::GetDialogStatus(Player* p_Player, GameObject* p_GameObject)
{
    ASSERT(p_Player);
    ASSERT(p_GameObject);

    // TODO: 100 is a funny magic number to have hanging around here...
    GET_SCRIPT_RET(GameObjectScript, p_GameObject->GetScriptId(), tmpscript, 100);
    p_Player->PlayerTalkClass->ClearMenus();
    return tmpscript->GetDialogStatus(p_Player, p_GameObject);
}

// Called when the game object is destroyed (destructible buildings only).
/// @p_GameObject : Destroyed GameObject
/// @p_Player     : Destroyer player instance
void ScriptMgr::OnGameObjectDestroyed(GameObject* p_GameObject, Player* player)
{
    ASSERT(p_GameObject);

    GET_SCRIPT(GameObjectScript, p_GameObject->GetScriptId(), tmpscript);
    tmpscript->OnDestroyed(p_GameObject, player);
}

/// Called when the game object is damaged (destructible buildings only).
/// @p_GameObject : Damaged GameObject
/// @p_Player     : Damager player instance
void ScriptMgr::OnGameObjectDamaged(GameObject* p_GameObject, Player* p_Player)
{
    ASSERT(p_GameObject);

    GET_SCRIPT(GameObjectScript, p_GameObject->GetScriptId(), tmpscript);
    tmpscript->OnDamaged(p_GameObject, p_Player);
}

/// Called when the game object loot state is changed.
/// @p_GameObject : Looted GameObject
/// @p_State      : Loot state
/// @p_Unit       : Unit
void ScriptMgr::OnGameObjectLootStateChanged(GameObject* p_GameObject, uint32 p_State, Unit* p_Unit)
{
    ASSERT(p_GameObject);

    GET_SCRIPT(GameObjectScript, p_GameObject->GetScriptId(), tmpscript);
    tmpscript->OnLootStateChanged(p_GameObject, p_State, p_Unit);
}

/// Called when the game object state is changed.
/// @p_GameObject : Changed GameObject
/// @p_State      : GameObject state
void ScriptMgr::OnGameObjectStateChanged(GameObject* p_GameObject, uint32 p_State)
{
    ASSERT(p_GameObject);

    GET_SCRIPT(GameObjectScript, p_GameObject->GetScriptId(), tmpscript);
    tmpscript->OnGameObjectStateChanged(p_GameObject, p_State);
}

/// Called when server want to send elevator update, by default all GameObject type transport are elevator
/// @p_GameObject : GameObject instance
bool ScriptMgr::OnGameObjectElevatorCheck(const GameObject* p_GameObject) const
{
    ASSERT(p_GameObject);

    GET_SCRIPT_RET(GameObjectScript, p_GameObject->GetScriptId(), tmpscript, true);
    return tmpscript->OnGameObjectElevatorCheck(p_GameObject);
}

bool ScriptMgr::OnGameObjectSpellCasterUse(GameObject const* p_GameObject, Player* p_User) const
{
    ASSERT(p_GameObject);

    GET_SCRIPT_RET(GameObjectScript, p_GameObject->GetScriptId(), tmpscript, true);
    return tmpscript->OnGameObjectSpellCasterUse(p_GameObject, p_User);
}

/// Called when a GameObjectAI object is needed for the GameObject.
/// @p_GameObject : GameObject instance
GameObjectAI* ScriptMgr::GetGameObjectAI(GameObject* p_GameObject)
{
    ASSERT(p_GameObject);

    GET_SCRIPT_RET(GameObjectScript, p_GameObject->GetScriptId(), tmpscript, NULL);
    return tmpscript->GetAI(p_GameObject);
}

/// On update
/// @p_Object : Updated object instance
/// @p_Diff   : Time since last update
void ScriptMgr::OnGameObjectUpdate(GameObject* p_Object, uint32 p_Diff)
{
    ASSERT(p_Object);

    GET_SCRIPT(GameObjectScript, p_Object->GetScriptId(), tmpscript);
    tmpscript->OnUpdate(p_Object, p_Diff);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Called when a member is added to a group.
/// @p_Group : Group Instance
/// @p_GUID  : Added member GUID
void ScriptMgr::OnGroupAddMember(Group* p_Group, uint64 p_GUID)
{
    ASSERT(p_Group);
    FOREACH_SCRIPT(GroupScript)->OnAddMember(p_Group, p_GUID);
}

/// Called when a member is invited to join a group.
/// @p_Group : Group Instance
/// @p_GUID  : Invited member GUID
void ScriptMgr::OnGroupInviteMember(Group* p_Group, uint64 p_GUID)
{
    ASSERT(p_Group);
    FOREACH_SCRIPT(GroupScript)->OnInviteMember(p_Group, p_GUID);
}

/// Called when a member is removed from a group.
/// @p_Group      : Group Instance
/// @p_GUID       : Removed member GUID
/// @p_Method     : Remove method
/// @p_KickerGUID : Kicker GUID
/// @p_Reason     : Kick reason
void ScriptMgr::OnGroupRemoveMember(Group* p_Group, uint64 p_GUID, RemoveMethod p_Method, uint64 p_KickerGUID, const char* p_Reason)
{
    ASSERT(p_Group);
    FOREACH_SCRIPT(GroupScript)->OnRemoveMember(p_Group, p_GUID, p_Method, p_KickerGUID, p_Reason);
}

/// Called when the leader of a group is changed.
/// @p_Group         : Group Instance
/// @p_NewLeaderGUID : New group leader GUID
/// @p_OldLeaderGUID : Old group leader GUID
void ScriptMgr::OnGroupChangeLeader(Group* p_Group, uint64 p_NewLeaderGUID, uint64 p_OldLeaderGUID)
{
    ASSERT(p_Group);
    FOREACH_SCRIPT(GroupScript)->OnChangeLeader(p_Group, p_NewLeaderGUID, p_OldLeaderGUID);
}

/// Called when a group is disbanded.
/// @p_Group : Group Instance
void ScriptMgr::OnGroupDisband(Group* p_Group)
{
    ASSERT(p_Group);
    FOREACH_SCRIPT(GroupScript)->OnDisband(p_Group);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Called when a member is added to the guild.
/// @p_Guild  : Guild instance
/// @p_Player : Added player
/// @p_Rank   : Added player destination rank
void ScriptMgr::OnGuildAddMember(Guild* p_Guild, Player* p_Player, uint8 & p_Rank)
{
    FOREACH_SCRIPT(GuildScript)->OnAddMember(p_Guild, p_Player, p_Rank);
}

/// Called when a member is removed from the guild.
/// @p_Guild        : Guild instance
/// @p_Player       : Removed player
/// @p_IsDisbanding : Player is removed from a guild disbanding
/// @p_IsKicked     : Is that removed player kicked
void ScriptMgr::OnGuildRemoveMember(Guild* p_Guild, Player* p_Player, bool p_IsDisbanding, bool p_IsKicked)
{
    FOREACH_SCRIPT(GuildScript)->OnRemoveMember(p_Guild, p_Player, p_IsDisbanding, p_IsKicked);
}

/// Called when the guild MOTD (message of the day) changes.
/// @p_Guild   : Guild instance
/// @p_NewMotd : New message of the day
void ScriptMgr::OnGuildMOTDChanged(Guild* p_Guild, const std::string & p_NewMotd)
{
    FOREACH_SCRIPT(GuildScript)->OnMOTDChanged(p_Guild, p_NewMotd);
}

/// Called when the guild info is altered.
/// @p_Guild   : Guild instance
/// @p_NewInfo : New guild info
void ScriptMgr::OnGuildInfoChanged(Guild* p_Guild, const std::string & p_NewInfo)
{
    FOREACH_SCRIPT(GuildScript)->OnInfoChanged(p_Guild, p_NewInfo);
}

/// Called when a guild is created.
/// @p_Guild  : Guild instance
/// @p_Leader : Guild leader
/// @p_Name   : Guild Name
void ScriptMgr::OnGuildCreate(Guild* p_Guild, Player* p_Leader, const std::string & p_Name)
{
    FOREACH_SCRIPT(GuildScript)->OnCreate(p_Guild, p_Leader, p_Name);
}

/// Called when a guild is disbanded.
/// @p_Guild : Guild instance
void ScriptMgr::OnGuildDisband(Guild* p_Guild)
{
    FOREACH_SCRIPT(GuildScript)->OnDisband(p_Guild);
}

/// Called when a guild member withdraws money from a guild bank.
/// @p_Guild    : Guild instance
/// @p_Player   : Withdrawer player
/// @p_Amount   : Dest gold amount
/// @p_IsRepair : Is repair
void ScriptMgr::OnGuildMemberWitdrawMoney(Guild* p_Guild, Player* p_Player, uint64 & p_Amount, bool p_IsRepair)
{
    FOREACH_SCRIPT(GuildScript)->OnMemberWitdrawMoney(p_Guild, p_Player, p_Amount, p_IsRepair);
}

/// Called when a guild member deposits money in a guild bank.
/// @p_Guild  : Guild instance
/// @p_Player : Depositor player
/// @p_Amount : Dest gold amount
void ScriptMgr::OnGuildMemberDepositMoney(Guild* p_Guild, Player* p_Player, uint64 & p_Amount)
{
    FOREACH_SCRIPT(GuildScript)->OnMemberDepositMoney(p_Guild, p_Player, p_Amount);
}

/// Called when a guild member moves an item in a guild bank.
/// @p_Guild         : Guild instance
/// @p_Player        : Player item mover
/// @p_Item          : Item instance
/// @p_IsSrcBank     : Is from guild bank
/// @p_SrcContainer  : Source Bag
/// @p_SrcSlotID     : Source Bag slot ID
/// @p_IsDestBank    : Is to guild bank
/// @p_DestContainer : Destination Bag
/// @p_DestSlotID    : Destination Bag slot ID
void ScriptMgr::OnGuildItemMove(Guild* p_Guild, Player* p_Player, Item* p_Item, bool p_IsSrcBank, uint8 p_SrcContainer, uint8 p_SrcSlotID, bool p_IsDestBank, uint8 p_DestContainer, uint8 p_DestSlotID)
{
    FOREACH_SCRIPT(GuildScript)->OnItemMove(p_Guild, p_Player, p_Item, p_IsSrcBank, p_SrcContainer, p_SrcSlotID, p_IsDestBank, p_DestContainer, p_DestSlotID);
}

/// On Guild event
/// @p_Guild       : Guild instance
/// @p_EventType   : Event type
/// @p_PlayerGUID1 : Player GUID 1
/// @p_PlayerGUID2 : Player GUID 2
/// @p_NewRank     : New Rank (contextual)
void ScriptMgr::OnGuildEvent(Guild* p_Guild, uint8 p_EventType, uint32 p_PlayerGUID1, uint32 p_PlayerGUID2, uint8 p_NewRank)
{
    FOREACH_SCRIPT(GuildScript)->OnEvent(p_Guild, p_EventType, p_PlayerGUID1, p_PlayerGUID2, p_NewRank);
}

/// @p_Guild          : Guild instance
/// @p_EventType      : Event type
/// @p_TabID          : Source tab ID
/// @p_PlayerGUID     : Player GUID
/// @p_ItemOrMoney    : Item entry or gold amount
/// @p_ItemStackCount : Item stack count
/// @p_DestTabID      : Destination tab ID
void ScriptMgr::OnGuildBankEvent(Guild* p_Guild, uint8 p_EventType, uint8 p_TabID, uint32 p_PlayerGUID, uint64 p_ItemOrMoney, uint16 p_ItemStackCount, uint8 p_DestTabID)
{
    FOREACH_SCRIPT(GuildScript)->OnBankEvent(p_Guild, p_EventType, p_TabID, p_PlayerGUID, p_ItemOrMoney, p_ItemStackCount, p_DestTabID);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Called when a dummy spell effect is triggered on the item.
/// @p_Caster      : Caster unit instance
/// @p_SpellID     : Dummy effect origin spell ID
/// @p_EffectIndex : Dummy effect index
/// @p_Target      : Spell target
bool ScriptMgr::OnDummyEffect(Unit* p_Caster, uint32 p_SpellID, SpellEffIndex p_EffectIndex, Item* p_Target)
{
    ASSERT(p_Caster);
    ASSERT(p_Target);

    GET_SCRIPT_RET(ItemScript, p_Target->GetScriptId(), tmpscript, false);
    return tmpscript->OnDummyEffect(p_Caster, p_SpellID, p_EffectIndex, p_Target);
}

/// Called when a player accepts a quest from the item.
/// @p_Player : Player who accepted quest from this item
/// @p_Item   : Item quest owner instance
/// @p_Quest  : Accepted quest instance
bool ScriptMgr::OnQuestAccept(Player* p_Player, Item* p_Item, const Quest* p_Quest)
{
    ASSERT(p_Player);
    ASSERT(p_Item);
    ASSERT(p_Quest);

    GET_SCRIPT_RET(ItemScript, p_Item->GetScriptId(), tmpscript, false);
    p_Player->PlayerTalkClass->ClearMenus();

    return tmpscript->OnQuestAccept(p_Player, p_Item, p_Quest);
}

/// Called when a player uses the item.
/// @p_Player           : Player who use this item
/// @p_Item             : Used Item instance
/// @p_SpellCastTargets : Item spell action targets
bool ScriptMgr::OnItemUse(Player* p_Player, Item* p_Item, const SpellCastTargets& p_SpellCastTargets)
{
    ASSERT(p_Player);
    ASSERT(p_Item);

    GET_SCRIPT_RET(ItemScript, p_Item->GetScriptId(), tmpscript, false);
    return tmpscript->OnUse(p_Player, p_Item, p_SpellCastTargets);
}

/// Called when a player opnes the item
/// @p_Player : The Player who has used this item
/// @p_Item   : Used Item instance
bool ScriptMgr::OnItemOpen(Player* p_Player, Item* p_Item)
{
    ASSERT(p_Player);
    ASSERT(p_Item);
    
    GET_SCRIPT_RET(ItemScript, p_Item->GetScriptId(), tmpscript, false);
    return tmpscript->OnOpen(p_Player, p_Item);
}

/// Called when the item expires (is destroyed).
/// @p_Player       : Item destroyer player instance
/// @p_ItemTemplate : Destroyed item template
bool ScriptMgr::OnItemExpire(Player* p_Player, const ItemTemplate* p_ItemTemplate)
{
    ASSERT(p_Player);
    ASSERT(p_ItemTemplate);

    GET_SCRIPT_RET(ItemScript, p_ItemTemplate->ScriptId, tmpscript, false);
    return tmpscript->OnExpire(p_Player, p_ItemTemplate);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
    
#define SCR_MAP_BGN(M, V, I, E, C, T) \
    if (V->GetEntry() && V->GetEntry()->T()) \
    { \
        FOR_SCRIPTS(M, I, E) \
        { \
            MapEntry const* C = I->second->GetEntry(); \
            if (!C) \
                continue; \
            if (C->MapID == V->GetId()) \
            {

#define SCR_MAP_END \
                return; \
            } \
        } \
    }

/// Called when the map is created.
/// @p_Map : Created map instance
void ScriptMgr::OnCreateMap(Map* p_Map)
{
    ASSERT(p_Map);

    SCR_MAP_BGN(WorldMapScript, p_Map, l_It, end, entry, IsWorldMap);
        l_It->second->OnCreate(p_Map);
    SCR_MAP_END;

    SCR_MAP_BGN(InstanceMapScript, p_Map, l_It, end, entry, IsDungeon);
        l_It->second->OnCreate((InstanceMap*)p_Map);
    SCR_MAP_END;

    SCR_MAP_BGN(BattlegroundMapScript, p_Map, l_It, end, entry, IsBattleground);
        l_It->second->OnCreate((BattlegroundMap*)p_Map);
    SCR_MAP_END;
}

/// Called just before the map is destroyed.
/// @p_Map : Destroyed map instance
void ScriptMgr::OnDestroyMap(Map* p_Map)
{
    ASSERT(p_Map);

    SCR_MAP_BGN(WorldMapScript, p_Map, l_It, end, entry, IsWorldMap);
        l_It->second->OnDestroy(p_Map);
    SCR_MAP_END;

    SCR_MAP_BGN(InstanceMapScript, p_Map, l_It, end, entry, IsDungeon);
        l_It->second->OnDestroy((InstanceMap*)p_Map);
    SCR_MAP_END;

    SCR_MAP_BGN(BattlegroundMapScript, p_Map, l_It, end, entry, IsBattleground);
        l_It->second->OnDestroy((BattlegroundMap*)p_Map);
    SCR_MAP_END;
}

/// Called when a grid map is loaded.
/// @p_Map     : Context map
/// @p_GridMap : Grid map container
/// @p_GridX   : Loaded grid X offset
/// @p_GridY   : Loaded grid Y offset
void ScriptMgr::OnLoadGridMap(Map* p_Map, GridMap* p_GridMap, uint32 p_GridX, uint32 p_GridY)
{
    ASSERT(p_Map);
    ASSERT(p_GridMap);

    SCR_MAP_BGN(WorldMapScript, p_Map, l_It, end, entry, IsWorldMap);
        l_It->second->OnLoadGridMap(p_Map, p_GridMap, p_GridX, p_GridY);
    SCR_MAP_END;

    SCR_MAP_BGN(InstanceMapScript, p_Map, l_It, end, entry, IsDungeon);
        l_It->second->OnLoadGridMap((InstanceMap*)p_Map, p_GridMap, p_GridX, p_GridY);
    SCR_MAP_END;

    SCR_MAP_BGN(BattlegroundMapScript, p_Map, l_It, end, entry, IsBattleground);
        l_It->second->OnLoadGridMap((BattlegroundMap*)p_Map, p_GridMap, p_GridX, p_GridY);
    SCR_MAP_END;
}

/// Called when a grid map is unloaded.
/// @p_Map     : Context map
/// @p_GridMap : Grid map container
/// @p_GridX   : Unloaded grid X offset
/// @p_GridY   : Unloaded grid Y offset
void ScriptMgr::OnUnloadGridMap(Map* p_Map, GridMap* p_GridMap, uint32 p_GridX, uint32 p_GridY)
{
    ASSERT(p_Map);
    ASSERT(p_GridMap);

    SCR_MAP_BGN(WorldMapScript, p_Map, l_It, end, entry, IsWorldMap);
        l_It->second->OnUnloadGridMap(p_Map, p_GridMap, p_GridX, p_GridY);
    SCR_MAP_END;

    SCR_MAP_BGN(InstanceMapScript, p_Map, l_It, end, entry, IsDungeon);
        l_It->second->OnUnloadGridMap((InstanceMap*)p_Map, p_GridMap, p_GridX, p_GridY);
    SCR_MAP_END;

    SCR_MAP_BGN(BattlegroundMapScript, p_Map, l_It, end, entry, IsBattleground);
        l_It->second->OnUnloadGridMap((BattlegroundMap*)p_Map, p_GridMap, p_GridX, p_GridY);
    SCR_MAP_END;
}

/// Called when a player enters the map.
/// @p_Map    : Context map
/// @p_Player : Entered player instance
void ScriptMgr::OnPlayerEnterMap(Map* p_Map, Player* p_Player)
{
    ASSERT(p_Map);
    ASSERT(p_Player);

    SCR_MAP_BGN(WorldMapScript, p_Map, l_It, end, entry, IsWorldMap);
        l_It->second->OnPlayerEnter(p_Map, p_Player);
    SCR_MAP_END;

    SCR_MAP_BGN(InstanceMapScript, p_Map, l_It, end, entry, IsDungeon);
        l_It->second->OnPlayerEnter((InstanceMap*)p_Map, p_Player);
    SCR_MAP_END;

    SCR_MAP_BGN(BattlegroundMapScript, p_Map, l_It, end, entry, IsBattleground);
        l_It->second->OnPlayerEnter((BattlegroundMap*)p_Map, p_Player);
    SCR_MAP_END;
}

/// Called when a player leaves the map.
/// @p_Map    : Context map
/// @p_Player : Leaved player instance
void ScriptMgr::OnPlayerLeaveMap(Map* p_Map, Player* p_Player)
{
    ASSERT(p_Map);
    ASSERT(p_Player);

    SCR_MAP_BGN(WorldMapScript, p_Map, l_It, end, entry, IsWorldMap);
        l_It->second->OnPlayerLeave(p_Map, p_Player);
    SCR_MAP_END;

    SCR_MAP_BGN(InstanceMapScript, p_Map, l_It, end, entry, IsDungeon);
        l_It->second->OnPlayerLeave((InstanceMap*)p_Map, p_Player);
    SCR_MAP_END;

    SCR_MAP_BGN(BattlegroundMapScript, p_Map, l_It, end, entry, IsBattleground);
        l_It->second->OnPlayerLeave((BattlegroundMap*)p_Map, p_Player);
    SCR_MAP_END;
}

/// Called on every map update tick.
/// @p_Map  : Context map
/// @p_Diff : Time since last update
void ScriptMgr::OnMapUpdate(Map* p_Map, uint32 p_Diff)
{
    ASSERT(p_Map);

    SCR_MAP_BGN(WorldMapScript, p_Map, l_It, end, entry, IsWorldMap);
        l_It->second->OnUpdate(p_Map, p_Diff);
    SCR_MAP_END;

    SCR_MAP_BGN(InstanceMapScript, p_Map, l_It, end, entry, IsDungeon);
        l_It->second->OnUpdate((InstanceMap*)p_Map, p_Diff);
    SCR_MAP_END;

    SCR_MAP_BGN(BattlegroundMapScript, p_Map, l_It, end, entry, IsBattleground);
        l_It->second->OnUpdate((BattlegroundMap*)p_Map, p_Diff);
    SCR_MAP_END;
}

#undef SCR_MAP_BGN
#undef SCR_MAP_END

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Gets an InstanceScript object for this instance.
/// @p_Map : Context map
InstanceScript* ScriptMgr::CreateInstanceData(InstanceMap* p_Map)
{
    ASSERT(p_Map);

    GET_SCRIPT_RET(InstanceMapScript, p_Map->GetScriptId(), tmpscript, NULL);
    return tmpscript->GetInstanceScript(p_Map);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Called when reactive socket I/O is started (WorldSocketMgr).
void ScriptMgr::OnNetworkStart()
{
    FOREACH_SCRIPT(ServerScript)->OnNetworkStart();
}

/// Called when reactive I/O is stopped.
void ScriptMgr::OnNetworkStop()
{
    FOREACH_SCRIPT(ServerScript)->OnNetworkStop();
}

/// Called when a remote socket establishes a connection to the server. Do not store the socket object.
/// @p_Socket : Opened socket
void ScriptMgr::OnSocketOpen(WorldSocket* p_Socket)
{
    ASSERT(p_Socket);

    FOREACH_SCRIPT(ServerScript)->OnSocketOpen(p_Socket);
}

/// Called when a socket is closed. Do not store the socket object, and do not rely on the connection being open; it is not.
/// @p_Socket : Closed socket
/// @p_WasNew : Was new ?
void ScriptMgr::OnSocketClose(WorldSocket* p_Socket, bool p_WasNew)
{
    ASSERT(p_Socket);

    FOREACH_SCRIPT(ServerScript)->OnSocketClose(p_Socket, p_WasNew);
}

/// Called when a packet is sent to a client. The packet object is a copy of the original packet, so reading and modifying it is safe.
/// @p_Socket : Socket who send the packet
/// @p_Packet : Sent packet
void ScriptMgr::OnPacketReceive(WorldSocket* p_Socket, WorldPacket p_Packet, WorldSession* p_Session)
{
    ASSERT(p_Socket);

    FOREACH_SCRIPT(ServerScript)->OnPacketReceive(p_Socket, p_Packet, p_Session);
}

/// Called when a (valid) packet is received by a client. The packet object is a copy of the original packet, so reading and modifying it is safe.
/// @p_Socket : Socket who received the packet
/// @p_Packet : Received packet
void ScriptMgr::OnPacketSend(WorldSocket* p_Socket, WorldPacket p_Packet)
{
    ASSERT(p_Socket);

    FOREACH_SCRIPT(ServerScript)->OnPacketSend(p_Socket, p_Packet);
}

/// Called when an invalid (unknown opcode) packet is received by a client. The packet is a reference to the original packet; not a copy.
/// This allows you to actually handle unknown packets (for whatever purpose).
/// @p_Socket : Socket who received the packet
/// @p_Packet : Received packet
void ScriptMgr::OnUnknownPacketReceive(WorldSocket* p_Socket, WorldPacket p_Packet)
{
    ASSERT(p_Socket);

    FOREACH_SCRIPT(ServerScript)->OnUnknownPacketReceive(p_Socket, p_Packet);

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Called when the open/closed state of the world changes.
/// @p_Open : Open ?
void ScriptMgr::OnOpenStateChange(bool p_Open)
{
    FOREACH_SCRIPT(WorldScript)->OnOpenStateChange(p_Open);
}

/// Called after the world configuration is (re)loaded.
/// @p_Reload : Is the config reload
void ScriptMgr::OnConfigLoad(bool p_Reload)
{
    FOREACH_SCRIPT(WorldScript)->OnConfigLoad(p_Reload);
}

/// Called before the message of the day is changed.
/// @p_NewMotd : New server message of the day
void ScriptMgr::OnMotdChange(std::string& p_NewMotd)
{
    FOREACH_SCRIPT(WorldScript)->OnMotdChange(p_NewMotd);
}

/// Called when a world shutdown is initiated.
/// @p_Code : Server exit code
/// @p_Mask : Shutdown mask
void ScriptMgr::OnShutdownInitiate(ShutdownExitCode p_Code, ShutdownMask p_Mask)
{
    FOREACH_SCRIPT(WorldScript)->OnShutdownInitiate(p_Code, p_Mask);
}

/// Called when a world shutdown is cancelled.
void ScriptMgr::OnShutdownCancel()
{
    FOREACH_SCRIPT(WorldScript)->OnShutdownCancel();
}

/// Called on every world tick (don't execute too heavy code here).
/// @p_Diff : Time since last update
void ScriptMgr::OnWorldUpdate(uint32 p_Diff)
{
    FOREACH_SCRIPT(WorldScript)->OnUpdate(p_Diff);
}

/// Called when the world is started.
void ScriptMgr::OnStartup()
{
    FOREACH_SCRIPT(WorldScript)->OnStartup();
}

/// Called when the world is actually shut down.
void ScriptMgr::OnShutdown()
{
    FOREACH_SCRIPT(WorldScript)->OnShutdown();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Should return a pointer to a valid command table (ChatCommand array) to be used by ChatHandler.
std::vector<ChatCommand*> ScriptMgr::GetChatCommands()
{
    std::vector<ChatCommand*> l_Table;

    FOR_SCRIPTS_RET(CommandScript, l_It, end, l_Table)
        l_Table.push_back(l_It->second->GetCommands());

    return l_Table;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Called when the weather changes in the zone this script is associated with.
/// @p_Weather : Weather instance
/// @p_State   : New weather state
/// @p_Grade   : New weather grade
void ScriptMgr::OnWeatherChange(Weather* p_Weather, WeatherState p_State, float p_Grade)
{
    ASSERT(p_Weather);

    GET_SCRIPT(WeatherScript, p_Weather->GetScriptId(), tmpscript);
    tmpscript->OnChange(p_Weather, p_State, p_Grade);
}

/// On update
/// @p_Object : Updated object instance
/// @p_Diff   : Time since last update
void ScriptMgr::OnWeatherUpdate(Weather* p_Object, uint32 p_Diff)
{
    ASSERT(p_Object);

    GET_SCRIPT(WeatherScript, p_Object->GetScriptId(), tmpscript);
    tmpscript->OnUpdate(p_Object, p_Diff);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// On update
/// @p_Object : Updated object instance
/// @p_Diff   : Time since last update
void ScriptMgr::OnDynamicObjectUpdate(DynamicObject* p_Object, uint32 p_Diff)
{
    ASSERT(p_Object);

    FOR_SCRIPTS(DynamicObjectScript, l_It, end)
        l_It->second->OnUpdate(p_Object, p_Diff);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Called when an auction is added to an auction house.
/// @p_AuctionHouseObject : Auction House Object Instance
/// @p_Entry              : Auction to add
void ScriptMgr::OnAuctionAdd(AuctionHouseObject* p_AuctionHouseObject, AuctionEntry* p_Entry)
{
    ASSERT(p_AuctionHouseObject);
    ASSERT(p_Entry);

    FOREACH_SCRIPT(AuctionHouseScript)->OnAuctionAdd(p_AuctionHouseObject, p_Entry);
}

/// Called when an auction is removed from an auction house.
/// @p_AuctionHouseObject : Auction House Object Instance
/// @p_Entry              : Auction to remove
void ScriptMgr::OnAuctionRemove(AuctionHouseObject* p_AuctionHouseObject, AuctionEntry* p_Entry)
{
    ASSERT(p_AuctionHouseObject);
    ASSERT(p_Entry);

    FOREACH_SCRIPT(AuctionHouseScript)->OnAuctionRemove(p_AuctionHouseObject, p_Entry);
}

/// Called when an auction was successfully completed.
/// @p_AuctionHouseObject : Auction House Object Instance
/// @p_Entry              : Auction instance
void ScriptMgr::OnAuctionSuccessful(AuctionHouseObject* p_AuctionHouseObject, AuctionEntry* p_Entry)
{
    ASSERT(p_AuctionHouseObject);
    ASSERT(p_Entry);

    FOREACH_SCRIPT(AuctionHouseScript)->OnAuctionSuccessful(p_AuctionHouseObject, p_Entry);
}

/// Called when an auction expires.
/// @p_AuctionHouseObject : Auction House Object Instance
/// @p_Entry              : Auction who expired
void ScriptMgr::OnAuctionExpire(AuctionHouseObject* p_AuctionHouseObject, AuctionEntry* p_Entry)
{
    ASSERT(p_AuctionHouseObject);
    ASSERT(p_Entry);

    FOREACH_SCRIPT(AuctionHouseScript)->OnAuctionExpire(p_AuctionHouseObject, p_Entry);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Called after calculating honor.
/// @p_Honor      : Dest honor
/// @p_Level      : Player level
/// @p_Multiplier : Honor multiplier
void ScriptMgr::OnHonorCalculation(float & p_Honor, uint8 p_Level, float p_Multiplier)
{
    FOREACH_SCRIPT(FormulaScript)->OnHonorCalculation(p_Honor, p_Level, p_Multiplier);
}

/// Called after gray level calculation.
/// @p_GrayLevel   : Dest gray level
/// @p_PlayerLevel : Player level
void ScriptMgr::OnGrayLevelCalculation(uint8 & p_GrayLevel, uint8 p_PlayerLevel)
{
    FOREACH_SCRIPT(FormulaScript)->OnGrayLevelCalculation(p_GrayLevel, p_PlayerLevel);
}

/// Called after calculating experience color.
/// @p_Color       : Dest XP Color
/// @p_PlayerLevel : Player level
/// @p_MobLevel    : Killed mob level
void ScriptMgr::OnColorCodeCalculation(XPColorChar & p_Color, uint8 p_PlayerLevel, uint8 p_MobLevel)
{
    FOREACH_SCRIPT(FormulaScript)->OnColorCodeCalculation(p_Color, p_PlayerLevel, p_MobLevel);
}

/// Called after calculating zero difference.
/// @p_Diff        : Level difference
/// @p_PlayerLevel : Player level
void ScriptMgr::OnZeroDifferenceCalculation(uint8 & p_Diff, uint8 p_PlayerLevel)
{
    FOREACH_SCRIPT(FormulaScript)->OnZeroDifferenceCalculation(p_Diff, p_PlayerLevel);
}

/// Called after calculating base experience gain.
/// @p_Gain        : Dest XP Gain
/// @p_PlayerLevel : Rewarded player instance
/// @p_MobLevel    : Killed mob level
/// @p_Content     : Content expansion mob
void ScriptMgr::OnBaseGainCalculation(uint32 & p_Gain, uint8 p_PlayerLevel, uint8 p_MobLevel, ContentLevels p_Content)
{
    FOREACH_SCRIPT(FormulaScript)->OnBaseGainCalculation(p_Gain, p_PlayerLevel, p_MobLevel, p_Content);
}

/// Called after calculating experience gain.
/// @p_Gain   : Dest XP Gain
/// @p_Player : Player instance for XP computation
/// @p_Unit   : Killed unit
void ScriptMgr::OnGainCalculation(uint32 & p_Gain, Player* p_Player, Unit* p_Unit)
{
    ASSERT(p_Player);
    ASSERT(p_Unit);

    FOREACH_SCRIPT(FormulaScript)->OnGainCalculation(p_Gain, p_Player, p_Unit);
}

/// Called when calculating the experience rate for group experience.
/// @p_Rate   : Dest XP rate
/// @p_Count  : Group member count
/// @p_IsRaid : Is a raid group
void ScriptMgr::OnGroupRateCalculation(float & p_Rate, uint32 p_Count, bool p_IsRaid)
{
    FOREACH_SCRIPT(FormulaScript)->OnGroupRateCalculation(p_Rate, p_Count, p_IsRaid);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Called when an additional criteria is checked.
/// @p_ScriptID : Script ID
/// @p_Source   : Criteria owner player
/// @p_Target   : Target instance
bool ScriptMgr::OnCriteriaCheck(uint32 p_ScriptID, Player* p_Source, Unit* p_Target)
{
    ASSERT(p_Source);
    // target can be NULL.

    GET_SCRIPT_RET(AchievementCriteriaScript, p_ScriptID, tmpscript, false);
    return tmpscript->OnCheck(p_Source, p_Target);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Called when a single condition is checked for a player.
/// @p_Condition  : Condition instance
/// @p_SourceInfo : Condition  source
bool ScriptMgr::OnConditionCheck(Condition const* p_Condition, ConditionSourceInfo & p_SourceInfo)
{
    ASSERT(p_Condition);

    GET_SCRIPT_RET(ConditionScript, p_Condition->ScriptId, tmpscript, true);
    return tmpscript->OnConditionCheck(p_Condition, p_SourceInfo);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Called just before item is destroyed
/// @p_Item        : Item to be destroyed
/// @p_Player      : Player level
void ScriptMgr::OnItemDestroyed(Player* p_Player, Item* p_Item)
{
    FOREACH_SCRIPT(PlayerScript)->OnItemDestroyed(p_Player, p_Item);
}

/// Called when a player kills another player
/// @p_Killer : Killer instance
/// @p_Killed : Killed instance
void ScriptMgr::OnPVPKill(Player* p_Killer, Player* p_Killed)
{
    FOREACH_SCRIPT(PlayerScript)->OnPVPKill(p_Killer, p_Killed);
}

/// Called when a player kills a Unit
/// @p_Killer : Killer instance
/// @p_Killed : Killed instance
void ScriptMgr::OnKill(Player* p_Killer, Unit* p_Killed)
{
    FOREACH_SCRIPT(PlayerScript)->OnKill(p_Killer, p_Killed);
}

/// Called when a player kills a creature
/// @p_Killer : Killer instance
/// @p_Killed : Killed instance
void ScriptMgr::OnCreatureKill(Player* p_Killer, Creature* p_Killed)
{
    FOREACH_SCRIPT(PlayerScript)->OnCreatureKill(p_Killer, p_Killed);
}

/// Called when a player is killed by a creature
/// @p_Killer : Killer instance
/// @p_Killed : Killed instance
void ScriptMgr::OnPlayerKilledByCreature(Creature* p_Killer, Player* p_Killed)
{
    FOREACH_SCRIPT(PlayerScript)->OnPlayerKilledByCreature(p_Killer, p_Killed);
}

/// Called when power change is modify (SetPower)
/// @p_Player : Player instance
/// @p_Power  : Power type
/// @p_OldValue  : Old value
/// @p_NewValue  : New value
/// @p_Regen  : If it's a regen modification
/// @p_After : If it's after modification
void ScriptMgr::OnModifyPower(Player* p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen, bool p_After)
{
    FOREACH_SCRIPT(PlayerScript)->OnModifyPower(p_Player, p_Power, p_OldValue, p_NewValue, p_Regen, p_After);
}

/// Called when the player switch from indoors to outdoors or from outdoors to indoors
/// @p_Player : Player instance
/// @p_IsOutdoors : Bool setting whether player is indoors or outdoors
void ScriptMgr::OnSwitchOutdoorsState(Player* p_Player, bool p_IsOutdoors)
{
    FOREACH_SCRIPT(PlayerScript)->OnSwitchOutdoorsState(p_Player, p_IsOutdoors);
}

/// Called when specialisation is modify (SetSpecializationId)
/// @p_Player : Player instance
/// @p_NewSpec  : New Specialisation
void ScriptMgr::OnModifySpec(Player* p_Player, int32 p_NewSpec)
{
    FOREACH_SCRIPT(PlayerScript)->OnModifySpec(p_Player, p_NewSpec);
}

/// Called when a player kills another player
/// @p_Player : Player instance
/// @p_Value  : New value
void ScriptMgr::OnModifyHealth(Player* p_Player, int32 p_Value)
{
    FOREACH_SCRIPT(PlayerScript)->OnModifyHealth(p_Player, p_Value);
}

/// Called when a player's level changes (right before the level is applied)
/// @p_Player   : Player instance
/// @p_OldLevel : Old player Level
void ScriptMgr::OnPlayerLevelChanged(Player* p_Player, uint8 p_OldLevel)
{
    FOREACH_SCRIPT(PlayerScript)->OnLevelChanged(p_Player, p_OldLevel);
}

/// Called when a player's talent points are reset (right before the reset is done)
/// @p_Player : Player instance
/// @p_NoCost : Talent was reset without cost
void ScriptMgr::OnPlayerTalentsReset(Player* p_Player, bool p_NoCost)
{
    FOREACH_SCRIPT(PlayerScript)->OnTalentsReset(p_Player, p_NoCost);
}

/// Called when a player's money is modified (before the modification is done)
/// @p_Player : Player instance
/// @p_Amount : Modified money amount
void ScriptMgr::OnPlayerMoneyChanged(Player* p_Player, int64 & p_Amount)
{
    FOREACH_SCRIPT(PlayerScript)->OnMoneyChanged(p_Player, p_Amount);
}

/// Called when a player gains XP (before anything is given)
/// @p_Player : Player instance
/// @p_Amount : Modified XP amount
/// @p_Victim : XP Source
void ScriptMgr::OnGivePlayerXP(Player* p_Player, uint32 & p_Amount, Unit* p_Victim)
{
    FOREACH_SCRIPT(PlayerScript)->OnGiveXP(p_Player, p_Amount, p_Victim);
}

/// Called when a player's reputation changes (before it is actually changed)
/// @p_Player       : Player instance
/// @p_FactionID    : Reward faction ID
/// @p_Standing     : Standing
/// @p_Incremential : Is incremental
void ScriptMgr::OnPlayerReputationChange(Player* p_Player, uint32 p_FactionID, int32 & p_Standing, bool p_Incremential)
{
    FOREACH_SCRIPT(PlayerScript)->OnReputationChange(p_Player, p_FactionID, p_Standing, p_Incremential);
}

/// Called when a duel is requested
/// @p_Target     : Duel target
/// @p_Challenger : Duel challenger
void ScriptMgr::OnPlayerDuelRequest(Player* p_Target, Player* p_Challenger)
{
    FOREACH_SCRIPT(PlayerScript)->OnDuelRequest(p_Target, p_Challenger);
}

/// Called when a duel starts (after 3s countdown)
/// @p_Player1 : First player
/// @p_Player2 : Second player
void ScriptMgr::OnPlayerDuelStart(Player* p_Player1, Player* p_Player2)
{
    FOREACH_SCRIPT(PlayerScript)->OnDuelStart(p_Player1, p_Player2);
}

/// Called when a duel ends
/// @p_Winner         : Duel winner
/// @p_Looser         : Duel looser
/// @p_CompletionType : Duel Completion Type
void ScriptMgr::OnPlayerDuelEnd(Player* p_Winner, Player* p_Looser, DuelCompleteType p_CompletionType)
{
    FOREACH_SCRIPT(PlayerScript)->OnDuelEnd(p_Winner, p_Looser, p_CompletionType);
}

/// Called when the player get Teleport
/// @p_Player : Player
/// @p_SpellID : SpellID
void ScriptMgr::OnTeleport(Player* p_Player, const SpellInfo*p_SpellInfo)
{
    FOREACH_SCRIPT(PlayerScript)->OnTeleport(p_Player, p_SpellInfo);
}

/// The following methods are called when a player sends a chat message. (World)
/// @p_Player  : Player instance
/// @p_Type    : Message type
/// @p_Lang    : Message language (WoW)
/// @p_Message : Message content
void ScriptMgr::OnPlayerChat(Player* p_Player, uint32 p_Type, uint32 p_Lang, std::string & p_Message)
{
    FOREACH_SCRIPT(PlayerScript)->OnChat(p_Player, p_Type, p_Lang, p_Message);
}

/// The following methods are called when a player sends a chat message. (Whisper)
/// @p_Player   : Player instance
/// @p_Type     : Message type
/// @p_Lang     : Message language (WoW)
/// @p_Message  : Message content
/// @p_Receiver : Message receiver
void ScriptMgr::OnPlayerChat(Player* p_Player, uint32 p_Type, uint32 p_Lang, std::string & p_Message, Player* p_Receiver)
{
    FOREACH_SCRIPT(PlayerScript)->OnChat(p_Player, p_Type, p_Lang, p_Message, p_Receiver);
}

/// The following methods are called when a player sends a chat message. (Party)
/// @p_Player  : Player instance
/// @p_Type    : Message type
/// @p_Lang    : Message language (WoW)
/// @p_Message : Message content
/// @p_Group   : Message group target
void ScriptMgr::OnPlayerChat(Player* p_Player, uint32 p_Type, uint32 p_Lang, std::string & p_Message, Group* p_Group)
{
    FOREACH_SCRIPT(PlayerScript)->OnChat(p_Player, p_Type, p_Lang, p_Message, p_Group);
}

/// The following methods are called when a player sends a chat message. (Guild)
/// @p_Player  : Player instance
/// @p_Type    : Message type
/// @p_Lang    : Message language (WoW)
/// @p_Message : Message content
/// @p_Guild   : Message guild target
void ScriptMgr::OnPlayerChat(Player* p_Player, uint32 p_Type, uint32 p_Lang, std::string & p_Message, Guild* p_Guild)
{
    FOREACH_SCRIPT(PlayerScript)->OnChat(p_Player, p_Type, p_Lang, p_Message, p_Guild);
}

/// The following methods are called when a player sends a chat message. (Channel)
/// @p_Player  : Player instance
/// @p_Type    : Message type
/// @p_Lang    : Message language (WoW)
/// @p_Message : Message content
/// @p_Channel : Message channel target
void ScriptMgr::OnPlayerChat(Player* p_Player, uint32 p_Type, uint32 p_Lang, std::string & p_Message, Channel* p_Channel)
{
    FOREACH_SCRIPT(PlayerScript)->OnChat(p_Player, p_Type, p_Lang, p_Message, p_Channel);
}

/// Both of the below are called on emote opcodes.
/// @p_Player : Player instance
/// @p_Emote  : Emote ID
void ScriptMgr::OnPlayerEmote(Player* p_Player, uint32 p_Emote)
{
    FOREACH_SCRIPT(PlayerScript)->OnEmote(p_Player, p_Emote);
}

/// When player start a text emote
/// @p_Player     : Player instance
/// @p_TextEmote  : Text emote ID
/// @p_SoundIndex : Emote sound ID
/// @p_TargetGUID : Text emote target GUID
void ScriptMgr::OnPlayerTextEmote(Player* p_Player, uint32 p_TextEmote, uint32 p_SoundIndex, uint64 p_TargetGUID)
{
    FOREACH_SCRIPT(PlayerScript)->OnTextEmote(p_Player, p_TextEmote, p_SoundIndex, p_TargetGUID);
}

/// Called in Spell::Cast.
/// @p_Player    : Player instance
/// @p_Spell     : Casted spell
/// @p_SkipCheck : Skipped checks
void ScriptMgr::OnPlayerSpellCast(Player* p_Player, Spell* p_Spell, bool p_SkipCheck)
{
    FOREACH_SCRIPT(PlayerScript)->OnSpellCast(p_Player, p_Spell, p_SkipCheck);
}

/// When the player learn a spell
/// @p_Player  : Player instance
/// @p_SpellID : Learned spell ID
void ScriptMgr::OnPlayerSpellLearned(Player* p_Player, uint32 p_SpellID)
{
    FOREACH_SCRIPT(PlayerScript)->OnSpellLearned(p_Player, p_SpellID);
}

/// Called when a player logs in.
/// @p_Player : Player instance
void ScriptMgr::OnPlayerLogin(Player* p_Player)
{
    FOREACH_SCRIPT(PlayerScript)->OnLogin(p_Player);
}

/// Called when a player logs out.
/// @p_Player : Player instance
void ScriptMgr::OnPlayerLogout(Player* p_Player)
{
    FOREACH_SCRIPT(PlayerScript)->OnLogout(p_Player);
}

/// Called when a player is created.
/// @p_Player : Player instance
void ScriptMgr::OnPlayerCreate(Player* p_Player)
{
    FOREACH_SCRIPT(PlayerScript)->OnCreate(p_Player);
}

/// Called when a player is deleted.
/// @p_GUID : Player instance
void ScriptMgr::OnPlayerDelete(uint64 p_GUID)
{
    FOREACH_SCRIPT(PlayerScript)->OnDelete(p_GUID);
}

/// Called when a update() of a player is done
/// @p_Player : Player instance
/// @p_Diff : diff time
void ScriptMgr::OnPlayerUpdate(Player* p_Player, uint32 p_Diff)
{
    FOREACH_SCRIPT(PlayerScript)->OnUpdate(p_Player, p_Diff);
}

/// Called when a player is bound to an instance
/// @p_Player     : Player instance
/// @p_Difficulty : Instance Difficulty ID
/// @p_MapID      : Instance Map ID
/// @p_Permanent  : Is a permanent bind
void ScriptMgr::OnPlayerBindToInstance(Player* p_Player, Difficulty p_Difficulty, uint32 p_MapID, bool p_Permanent)
{
    FOREACH_SCRIPT(PlayerScript)->OnBindToInstance(p_Player, p_Difficulty, p_MapID, p_Permanent);
}

/// Called when a player switches to a new zone
/// @p_Player    : Player instance
/// @p_NewZoneID : New player zone ID
/// @p_OldZoneID : Old player zone ID
/// @p_NewAreaID : New player area ID
void ScriptMgr::OnPlayerUpdateZone(Player* p_Player, uint32 p_NewZoneID, uint32 p_OldZoneID, uint32 p_NewAreaID)
{
    FOREACH_SCRIPT(PlayerScript)->OnUpdateZone(p_Player, p_NewZoneID, p_OldZoneID, p_NewAreaID);
}

/// Called when a player updates his movement
/// @p_Player : Player instance
void ScriptMgr::OnPlayerUpdateMovement(Player* p_Player)
{
    FOREACH_SCRIPT(PlayerScript)->OnUpdateMovement(p_Player);
}

/// Called when a spline step is done
/// @p_Player   : Player instance
/// @p_MoveType : Movement type
/// @p_ID       : Movement ID
void ScriptMgr::OnPlayerMovementInform(Player* p_Player, uint32 p_MoveType, uint32 p_ID)
{
    FOREACH_SCRIPT(PlayerScript)->OnMovementInform(p_Player, p_MoveType, p_ID);
}

/// Called when player accepts some quest
/// @p_Player : Player instance
/// @p_Quest  : Accpeted quest
void ScriptMgr::OnQuestAccept(Player* p_Player, const Quest* p_Quest)
{
    FOREACH_SCRIPT(PlayerScript)->OnQuestAccept(p_Player, p_Quest);
}
/// Called when player rewards some quest
/// @p_Player : Player instance
/// @p_Quest  : Rewarded quest
void ScriptMgr::OnQuestReward(Player* p_Player, const Quest* p_Quest)
{
    FOREACH_SCRIPT(PlayerScript)->OnQuestReward(p_Player, p_Quest);
}

/// Called when a player validates some quest objective
/// @p_Player      : Player instance
/// @p_QuestID     : Quest ID
/// @p_ObjectiveID : Validated quest objective ID
void ScriptMgr::OnObjectiveValidate(Player* p_Player, uint32 p_QuestID, uint32 p_ObjectiveID)
{
    FOREACH_SCRIPT(PlayerScript)->OnObjectiveValidate(p_Player, p_QuestID, p_ObjectiveID);
}

/// Called when player completes some quest
/// @p_Player : Player instance
/// @p_Quest  : Completed quest
void ScriptMgr::OnQuestComplete(Player* p_Player, const Quest* p_Quest)
{
    FOREACH_SCRIPT(PlayerScript)->OnQuestComplete(p_Player, p_Quest);
}

/// Called when player has quest removed from questlog (active or rewarded)
/// @p_Player : Player instance
/// @p_Quest  : Removed quest
void ScriptMgr::OnQuestAbandon(Player* p_Player, const Quest* p_Quest)
{
    FOREACH_SCRIPT(PlayerScript)->OnQuestAbandon(p_Player, p_Quest);
}

void ScriptMgr::OnQuestCleared(Player* p_Player, Quest const* p_Quest)
{
    FOREACH_SCRIPT(PlayerScript)->OnQuestCleared(p_Player, p_Quest);
}

/// Called when a player shapeshift
/// @p_Player : Player instance
/// @p_Form   : New shapeshift from
void ScriptMgr::OnPlayerChangeShapeshift(Player* p_Player, ShapeshiftForm p_Form)
{
    FOREACH_SCRIPT(PlayerScript)->OnChangeShapeshift(p_Player, p_Form);
}

/// Called when a player changes his faction
/// @p_Player : Player instance
void ScriptMgr::OnPlayerFactionChanged(Player* p_Player)
{
    FOREACH_SCRIPT(PlayerScript)->OnFactionChanged(p_Player);
}

/// Called when a player loot an item
/// @p_Player : Player instance
/// @p_Item   : New looted item instance
void ScriptMgr::OnPlayerItemLooted(Player* p_Player, Item* p_Item)
{
    FOREACH_SCRIPT(PlayerScript)->OnItemLooted(p_Player, p_Item);
}

/// Called when a player enter in combat
/// @p_Player : Player instance
void ScriptMgr::OnPlayerEnterInCombat(Player* p_Player)
{
    FOREACH_SCRIPT(PlayerScript)->OnEnterInCombat(p_Player);
}

/// Called when a player leave combat status
/// @p_Player : Player instance
void ScriptMgr::OnPlayerLeaveCombat(Player* p_Player)
{
    FOREACH_SCRIPT(PlayerScript)->OnLeaveCombat(p_Player);
}

/// Called when a player receive a scene triggered event
/// @p_Player          : Player instance
/// @p_SceneInstanceID : Standalone scene instance ID
/// @p_Event           : Event string received from client
void ScriptMgr::OnSceneTriggerEvent(Player* p_Player, uint32 p_SceneInstanceID, std::string p_Event)
{
    FOREACH_SCRIPT(PlayerScript)->OnSceneTriggerEvent(p_Player, p_SceneInstanceID, p_Event);
}

/// Called when a player cancels a scene who takes camera controls
/// @p_Player          : Player instance
/// @p_SceneInstanceID : Standalone scene instance ID
void ScriptMgr::OnSceneCancel(Player* p_Player, uint32 p_SceneInstanceId)
{
    FOREACH_SCRIPT(PlayerScript)->OnSceneCancel(p_Player, p_SceneInstanceId);
}


/// Called when a player enter in bg
/// @p_Player   : Player instance
/// @p_MapID    : Map ID
void ScriptMgr::OnEnterBG(Player* p_Player, uint32 p_MapID)
{
    FOREACH_SCRIPT(PlayerScript)->OnEnterBG(p_Player, p_MapID);
}

/// Called when a leave a bg
/// @p_Player   : Player instance
/// @p_MapID    : Map ID
void ScriptMgr::OnLeaveBG(Player* p_Player, uint32 p_MapID)
{
    FOREACH_SCRIPT(PlayerScript)->OnLeaveBG(p_Player, p_MapID);
}

/// Called when a player finish a movement like a jump
/// @p_Player   : Player instance
/// @p_SpellID  : Spell ID
/// @p_TargetGUID : Target GUID
void ScriptMgr::OnFinishMovement(Player* p_Player, uint32 p_SpellID, uint64 const p_TargetGUID)
{
    FOREACH_SCRIPT(PlayerScript)->OnFinishMovement(p_Player, p_SpellID, p_TargetGUID);
}

/// Called when a player regen a power
/// @p_Player         : Player instance
/// @p_Power          : Power to be regenerate
/// @p_AddValue       : amount of power to regenerate
/// @p_PreventDefault : avoid default regeneration
void ScriptMgr::OnPlayerRegenPower(Player* p_Player, Powers const p_Power, float& p_AddValue, bool& p_PreventDefault)
{
    FOREACH_SCRIPT(PlayerScript)->OnRegenPower(p_Player, p_Power, p_AddValue, p_PreventDefault);
}

/// Called when a player take damage
/// @p_Player          : Player instance
/// @p_DamageEffectTyp : Damage type
/// @p_Damage          : Amount of damage taken
void ScriptMgr::OnPlayerTakeDamage(Player* p_Player, DamageEffectType p_DamageEffectType, uint32 p_Damage, SpellSchoolMask p_SchoolMask, CleanDamage const* p_CleanDamage)
{
    FOREACH_SCRIPT(PlayerScript)->OnTakeDamage(p_Player, p_DamageEffectType, p_Damage, p_SchoolMask, p_CleanDamage);
}

/// Called when player block attack
/// @p_Player : Player instance
/// @p_DamageInfo  : Damage Infos
void ScriptMgr::OnPlayerBlock(Player* p_Player, Unit* p_Attacker)
{
    FOREACH_SCRIPT(PlayerScript)->OnBlock(p_Player, p_Attacker);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Should return a fully valid Battleground object for the type ID.
/// @p_TypeID : Battleground Type ID
Battleground* ScriptMgr::CreateBattleground(BattlegroundTypeId /*p_TypeID*/)
{
    // TODO: Implement script-side battlegrounds.
    ASSERT(false);
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Should return a fully valid OutdoorPvP object for the type ID.
/// @p_Data : Outdoor PvP Data
OutdoorPvP* ScriptMgr::CreateOutdoorPvP(const OutdoorPvPData* p_Data)
{
    ASSERT(p_Data);

    GET_SCRIPT_RET(OutdoorPvPScript, p_Data->ScriptId, tmpscript, NULL);
    return tmpscript->GetOutdoorPvP();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Should return a fully valid list of SpellScript pointer.
/// @p_SpellID      : Spell ID
/// @p_ScriptVector : Scripts for spell ID
void ScriptMgr::CreateSpellScripts(uint32 p_SpellID, std::list<SpellScript*> & p_ScriptVector)
{
    SpellScriptsBounds l_Bounds = sObjectMgr->GetSpellScriptsBounds(p_SpellID);

    for (SpellScriptsContainer::iterator l_It = l_Bounds.first; l_It != l_Bounds.second; ++l_It)
    {
        SpellScriptLoader* l_TempScript = ScriptRegistry<SpellScriptLoader>::GetScriptById(l_It->second);

        if (!l_TempScript)
            continue;

        SpellScript* l_Script = l_TempScript->GetSpellScript();

        if (!l_Script)
            continue;

        l_Script->_Init(&l_TempScript->GetName(), p_SpellID);

        p_ScriptVector.push_back(l_Script);
    }
}

/// Should return a fully valid list of AuraScript pointer.
/// @p_SpellID      : Spell ID
/// @p_ScriptVector : Scripts for spell ID
void ScriptMgr::CreateAuraScripts(uint32 p_SpellID, std::list<AuraScript*> & p_ScriptVector)
{
    SpellScriptsBounds l_Bounds = sObjectMgr->GetSpellScriptsBounds(p_SpellID);

    for (SpellScriptsContainer::iterator l_It = l_Bounds.first; l_It != l_Bounds.second; ++l_It)
    {
        SpellScriptLoader* l_TempScript = ScriptRegistry<SpellScriptLoader>::GetScriptById(l_It->second);
        if (!l_TempScript)
            continue;

        AuraScript* l_AuraScript = l_TempScript->GetAuraScript();

        if (!l_AuraScript)
            continue;

        l_AuraScript->_Init(&l_TempScript->GetName(), p_SpellID);

        p_ScriptVector.push_back(l_AuraScript);
    }
}

/// Create new spell script loaders
/// @p_SpellID      : Spell ID
/// @p_ScriptVector :
void ScriptMgr::CreateSpellScriptLoaders(uint32 p_SpellID, std::vector<std::pair<SpellScriptLoader*, SpellScriptsContainer::iterator> >& p_ScriptVector)
{
    SpellScriptsBounds l_Bounds = sObjectMgr->GetSpellScriptsBounds(p_SpellID);
    p_ScriptVector.reserve(std::distance(l_Bounds.first, l_Bounds.second));

    for (SpellScriptsContainer::iterator l_It = l_Bounds.first; l_It != l_Bounds.second; ++l_It)
    {
        SpellScriptLoader* l_TempScript = ScriptRegistry<SpellScriptLoader>::GetScriptById(l_It->second);
        if (!l_TempScript)
            continue;

        p_ScriptVector.push_back(std::make_pair(l_TempScript, l_It));
    }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Called when a player boards the transport.
/// @p_Transport : Transport instance
/// @p_Player    : Added player passenger instance
void ScriptMgr::OnAddPassenger(Transport* p_Transport, Player* p_Player)
{
    ASSERT(p_Transport);
    ASSERT(p_Player);

    GET_SCRIPT(TransportScript, p_Transport->GetScriptId(), tmpscript);
    tmpscript->OnAddPassenger(p_Transport, p_Player);
}

/// Called when a creature boards the transport.
/// @p_Transport : Transport instance
/// @p_Creature  : Added creature passenger instance
void ScriptMgr::OnAddCreaturePassenger(Transport* p_Transport, Creature* p_Creature)
{
    ASSERT(p_Transport);
    ASSERT(p_Creature);

    GET_SCRIPT(TransportScript, p_Transport->GetScriptId(), tmpscript);
    tmpscript->OnAddCreaturePassenger(p_Transport, p_Creature);
}

/// Called when a player exits the transport.
/// @p_Transport : Transport instance
/// @p_Player    : Added player passenger instance
void ScriptMgr::OnRemovePassenger(Transport* p_Transport, Player* p_Player)
{
    ASSERT(p_Transport);
    ASSERT(p_Player);

    GET_SCRIPT(TransportScript, p_Transport->GetScriptId(), tmpscript);
    tmpscript->OnRemovePassenger(p_Transport, p_Player);
}

/// Called when a transport moves.
/// @p_Transport  : Transport instance
/// @p_WaypointID : Reached waypoint ID
/// @p_MapID      : Reached MapID
/// @p_X          : New position X offset
/// @p_Y          : New position Y offset
/// @p_Z          : New position Z offset
void ScriptMgr::OnRelocate(Transport* p_Transport, uint32 p_WaypointID, uint32 p_MapID, float p_X, float p_Y, float p_Z)
{
    GET_SCRIPT(TransportScript, p_Transport->GetScriptId(), tmpscript);
    tmpscript->OnRelocate(p_Transport, p_WaypointID, p_MapID, p_X, p_Y, p_Z);
}

/// Called on every Transport update tick.
/// @p_Transport : Context Transport
/// @p_Diff      : Time since last update
void ScriptMgr::OnTransportUpdate(Transport* p_Transport, uint32 p_Diff)
{
    ASSERT(p_Transport);

    GET_SCRIPT(TransportScript, p_Transport->GetScriptId(), tmpscript);
    tmpscript->OnUpdate(p_Transport, p_Diff);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Called after a vehicle is installed.
/// @p_Vehicle : Vehicle instance
void ScriptMgr::OnInstall(Vehicle* p_Vehicle)
{
    ASSERT(p_Vehicle);
    ASSERT(p_Vehicle->GetBase()->GetTypeId() == TYPEID_UNIT);

    GET_SCRIPT(VehicleScript, p_Vehicle->GetBase()->ToCreature()->GetScriptId(), tmpscript);
    tmpscript->OnInstall(p_Vehicle);
}

/// Called after a vehicle is uninstalled.
/// @p_Vehicle : Vehicle instance
void ScriptMgr::OnUninstall(Vehicle* p_Vehicle)
{
    ASSERT(p_Vehicle);
    ASSERT(p_Vehicle->GetBase()->GetTypeId() == TYPEID_UNIT);

    GET_SCRIPT(VehicleScript, p_Vehicle->GetBase()->ToCreature()->GetScriptId(), tmpscript);
    tmpscript->OnUninstall(p_Vehicle);
}

/// Called when a vehicle resets.
/// @p_Vehicle : Vehicle instance
void ScriptMgr::OnReset(Vehicle* p_Vehicle)
{
    ASSERT(p_Vehicle);
    ASSERT(p_Vehicle->GetBase()->GetTypeId() == TYPEID_UNIT);

    GET_SCRIPT(VehicleScript, p_Vehicle->GetBase()->ToCreature()->GetScriptId(), tmpscript);
    tmpscript->OnReset(p_Vehicle);
}

/// Called after an accessory is installed in a vehicle.
/// @p_Vehicle   : Vehicle instance
/// @p_Accessory : Accessory to install
void ScriptMgr::OnInstallAccessory(Vehicle* p_Vehicle, Creature* p_Accessory)
{
    ASSERT(p_Vehicle);
    ASSERT(p_Vehicle->GetBase()->GetTypeId() == TYPEID_UNIT);
    ASSERT(p_Accessory);

    GET_SCRIPT(VehicleScript, p_Vehicle->GetBase()->ToCreature()->GetScriptId(), tmpscript);
    tmpscript->OnInstallAccessory(p_Vehicle, p_Accessory);
}

/// Called after a passenger is added to a vehicle.
/// @p_Vehicle   : Vehicle instance
/// @p_Passanger : Passenger to add
/// @p_SeatID    : Passenger destination seat ID
void ScriptMgr::OnAddPassenger(Vehicle* p_Vehicle, Unit* p_Passanger, int8 p_SeatID)
{
    ASSERT(p_Vehicle);
    ASSERT(p_Vehicle->GetBase()->GetTypeId() == TYPEID_UNIT);
    ASSERT(p_Passanger);

    GET_SCRIPT(VehicleScript, p_Vehicle->GetBase()->ToCreature()->GetScriptId(), tmpscript);
    tmpscript->OnAddPassenger(p_Vehicle, p_Passanger, p_SeatID);
}

/// Called after a passenger is removed from a vehicle.
/// @p_Vehicle   : Vehicle instance
/// @p_Passanger : Passenger to remove
void ScriptMgr::OnRemovePassenger(Vehicle* p_Vehicle, Unit* p_Passanger)
{
    ASSERT(p_Vehicle);
    ASSERT(p_Vehicle->GetBase()->GetTypeId() == TYPEID_UNIT);
    ASSERT(p_Passanger);

    GET_SCRIPT(VehicleScript, p_Vehicle->GetBase()->ToCreature()->GetScriptId(), tmpscript);
    tmpscript->OnRemovePassenger(p_Vehicle, p_Passanger);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Register a player condition script
/// @p_ID     : Condition ID
/// @p_Script : Script instance
void ScriptMgr::RegisterPlayerConditionScript(uint32 p_ID, PlayerConditionScript* p_Script)
{
    if (!p_Script)
        return;

    m_PlayerConditionScripts.Remove(p_ID);
    m_PlayerConditionScripts.Insert(p_ID, p_Script);
}

/// Has player condition script
/// @p_ID: Player condition ID
bool ScriptMgr::HasPlayerConditionScript(uint32 p_ID)
{
    return m_PlayerConditionScripts.Find(p_ID) != nullptr;
}

/// Eval a player condition script
/// @p_ConditionID : Condition ID
/// @p_Condition   : Condition
/// @p_Player      : Player instance
bool ScriptMgr::EvalPlayerConditionScript(uint32 p_ConditionID, PlayerConditionEntry const* p_Condition, Player* p_Player)
{
    auto l_Script = m_PlayerConditionScripts.Find(p_ConditionID);

    if (!l_Script)
        return false;

    return l_Script->OnConditionCheck(p_ConditionID, p_Condition, p_Player);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ScriptMgr::RegisterBattlePayProductScript(std::string p_ScriptName, BattlePayProductScript* p_Script)
{
    if (m_BattlePayProductScripts.find(p_ScriptName) != m_BattlePayProductScripts.end())
        return;

    m_BattlePayProductScripts[p_ScriptName] = p_Script;
}

void ScriptMgr::OnBattlePayProductDelivery(WorldSession* p_Session, Battlepay::Product const& p_Product)
{
    auto l_Itr = m_BattlePayProductScripts.find(p_Product.ScriptName);
    if (l_Itr == m_BattlePayProductScripts.end())
        return;

    BattlePayProductScript* l_Script = l_Itr->second;
    l_Script->OnProductDelivery(p_Session, p_Product);
}

bool ScriptMgr::BattlePayCanBuy(WorldSession* p_Session, Battlepay::Product const& p_Product, std::string& p_Reason)
{
    auto l_Itr = m_BattlePayProductScripts.find(p_Product.ScriptName);
    if (l_Itr == m_BattlePayProductScripts.end())
        return true;

    BattlePayProductScript* l_Script = l_Itr->second;
    return l_Script->CanBuy(p_Session, p_Product, p_Reason);
}

//////////////////////////////////////////////////////////////////////////
/// EncounterScripts
void ScriptMgr::OnEncounterEnd(EncounterDatas const* p_EncounterDatas)
{
    FOREACH_SCRIPT(EncounterScript)->OnEncounterEnd(p_EncounterDatas);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Constructor
/// @p_Name : Script name
SpellScriptLoader::SpellScriptLoader(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<SpellScriptLoader>::AddScript(this);
}

/// Constructor
/// @p_Name : Script name
ServerScript::ServerScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<ServerScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script Name
WorldScript::WorldScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<WorldScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script Name
FormulaScript::FormulaScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<FormulaScript>::AddScript(this);
}

/// Constructor
/// @p_Name  : Script Name
/// @p_MapID : Map Script map ID
WorldMapScript::WorldMapScript(const char* p_Name, uint32 p_MapID)
    : ScriptObjectImpl(p_Name), MapScript<Map>(p_MapID)
{
    if (GetEntry() && !GetEntry()->IsWorldMap())
        sLog->outError(LOG_FILTER_TSCR, "WorldMapScript for map %u is invalid.", p_MapID);

    ScriptRegistry<WorldMapScript>::AddScript(this);
}

/// Constructor
/// @p_Name  : Script Name
/// @p_MapID : Map Script map ID
InstanceMapScript::InstanceMapScript(const char* p_Name, uint32 p_MapID)
    : ScriptObjectImpl(p_Name), MapScript<InstanceMap>(p_MapID)
{
    if (GetEntry() && !GetEntry()->IsDungeon())
        sLog->outError(LOG_FILTER_TSCR, "InstanceMapScript for map %u is invalid.", p_MapID);

    ScriptRegistry<InstanceMapScript>::AddScript(this);
}

/// Constructor
/// @p_Name  : Script Name
/// @p_MapID : Map Script map ID
BattlegroundMapScript::BattlegroundMapScript(const char* p_Name, uint32 p_MapID)
    : ScriptObjectImpl(p_Name), MapScript<BattlegroundMap>(p_MapID)
{
    if (GetEntry() && !GetEntry()->IsBattleground())
        sLog->outError(LOG_FILTER_TSCR, "BattlegroundMapScript for map %u is invalid.", p_MapID);

    ScriptRegistry<BattlegroundMapScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script Name
ItemScript::ItemScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<ItemScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script Name
CreatureScript::CreatureScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<CreatureScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script Name
GameObjectScript::GameObjectScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<GameObjectScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script p_Name
AreaTriggerScript::AreaTriggerScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<AreaTriggerScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script p_Name
BattlegroundScript::BattlegroundScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<BattlegroundScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script p_Name
OutdoorPvPScript::OutdoorPvPScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<OutdoorPvPScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script p_Name
CommandScript::CommandScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<CommandScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script p_Name
WeatherScript::WeatherScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<WeatherScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script p_Name
AuctionHouseScript::AuctionHouseScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<AuctionHouseScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script p_Name
ConditionScript::ConditionScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<ConditionScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script p_Name
VehicleScript::VehicleScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<VehicleScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script p_Name
DynamicObjectScript::DynamicObjectScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<DynamicObjectScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script p_Name
TransportScript::TransportScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<TransportScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script p_Name
AchievementCriteriaScript::AchievementCriteriaScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<AchievementCriteriaScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script p_Name
PlayerScript::PlayerScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<PlayerScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script p_Name
GuildScript::GuildScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<GuildScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script p_Name
GroupScript::GroupScript(const char* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<GroupScript>::AddScript(this);
}

/// Constructor
/// @p_Name : Script Name
AreaTriggerEntityScript::AreaTriggerEntityScript(char const* p_Name)
    : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<AreaTriggerEntityScript>::AddScript(this);
}

/// Constructor
/// @p_ID : Player condition ID
PlayerConditionScript::PlayerConditionScript(uint32 p_ID)
    : ScriptObjectImpl("PlayerConditionScript")
{
    sScriptMgr->RegisterPlayerConditionScript(p_ID, this);
}

BattlePayProductScript::BattlePayProductScript(std::string p_ScriptName)
    : ScriptObjectImpl(p_ScriptName.c_str())
{
    sScriptMgr->RegisterBattlePayProductScript(p_ScriptName, this);
}

EncounterScript::EncounterScript(char const* p_Name) : ScriptObjectImpl(p_Name)
{
    ScriptRegistry<EncounterScript>::AddScript(this);
}

/// Instantiate static members of ScriptRegistry.
template<class TScript> std::map<uint32, TScript*> ScriptRegistry<TScript>::ScriptPointerList;
template<class TScript> uint32 ScriptRegistry<TScript>::_scriptIdCounter = 0;

/// Specialize for each script type class like so:
template class ScriptRegistry<SpellScriptLoader>;
template class ScriptRegistry<ServerScript>;
template class ScriptRegistry<WorldScript>;
template class ScriptRegistry<FormulaScript>;
template class ScriptRegistry<WorldMapScript>;
template class ScriptRegistry<InstanceMapScript>;
template class ScriptRegistry<BattlegroundMapScript>;
template class ScriptRegistry<ItemScript>;
template class ScriptRegistry<CreatureScript>;
template class ScriptRegistry<GameObjectScript>;
template class ScriptRegistry<AreaTriggerScript>;
template class ScriptRegistry<BattlegroundScript>;
template class ScriptRegistry<OutdoorPvPScript>;
template class ScriptRegistry<CommandScript>;
template class ScriptRegistry<WeatherScript>;
template class ScriptRegistry<AuctionHouseScript>;
template class ScriptRegistry<ConditionScript>;
template class ScriptRegistry<VehicleScript>;
template class ScriptRegistry<DynamicObjectScript>;
template class ScriptRegistry<TransportScript>;
template class ScriptRegistry<AchievementCriteriaScript>;
template class ScriptRegistry<PlayerScript>;
template class ScriptRegistry<GuildScript>;
template class ScriptRegistry<GroupScript>;
template class ScriptRegistry<AreaTriggerEntityScript>;
template class ScriptRegistry<EncounterScript>;

/// Undefine utility macros.
#undef GET_SCRIPT_RET
#undef GET_SCRIPT
#undef FOREACH_SCRIPT
#undef FOR_SCRIPTS_RET
#undef FOR_SCRIPTS
#undef SCR_REG_LST
#undef SCR_REG_ITR
#undef SCR_REG_MAP
