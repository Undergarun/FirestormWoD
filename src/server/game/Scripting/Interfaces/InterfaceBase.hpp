////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef SCRIPTING_INTERFACES_INTERFACEBASE_HPP_INCLUDED
#define SCRIPTING_INTERFACES_INTERFACEBASE_HPP_INCLUDED

#include "Common.h"
#include <ace/Atomic_Op.h>

#include "Player.h"
#include "SharedDefines.h"
#include "World.h"
#include "Weather.h"

#ifndef CROSS
class AuctionHouseObject;
#endif /* not CROSS */
class AuraScript;
class Battleground;
class BattlegroundMap;
class Channel;
class ChatCommand;
class Creature;
class CreatureAI;
class DynamicObject;
class GameObject;
class GameObjectAI;
class Guild;
class GridMap;
class Group;
class InstanceMap;
class InstanceScript;
class Item;
class Map;
class OutdoorPvP;
class Player;
class Quest;
class Spell;
class SpellScript;
class SpellCastTargets;
class Transport;
class Unit;
class Vehicle;
class WorldPacket;
class WorldSocket;
class WorldObject;
class ScriptMgr;

struct AchievementCriteriaData;
struct AuctionEntry;
struct ConditionSourceInfo;
struct Condition;
struct ItemTemplate;
struct OutdoorPvPData;
struct EncounterDatas;

#define VISIBLE_RANGE       166.0f                          //MAX visible range (size of grid)

// Generic scripting text function.
void DoScriptText(int32 textEntry, WorldObject* pSource, Unit* target = nullptr);

/// Standard procedure when adding new script type classes:
///
/// First of all, define the actual class, and have it inherit from ScriptObject, like so:
///
/// class MyScriptType : public ScriptObject
/// {
///     uint32 m_SomeId;
///
///     private:
///         void RegisterSelf();
///
///     protected:
///         MyScriptType(const char* p_Name, uint32 p_SomeId)
///             : ScriptObject(p_Name), m_SomeId(p_SomeId)
///         {
///             ScriptRegistry<MyScriptType>::AddScript(this);
///         }
///
///     public:
///         /// If a virtual function in your script type class is not necessarily
///         /// required to be overridden, just declare it virtual with an empty
///         /// body. If, on the other hand, it's logical only to override it (i.e.
///         /// if it's the only method in the class), make it pure virtual, by adding
///         /// = 0 to it.
///         virtual void OnSomeEvent(uint32 p_SomeArg1, std::string & p_SomeArg2) { }
///
///         // This is a pure virtual function:
///         virtual void OnAnotherEvent(uint32 p_SomeArg) = 0;
/// }
///
/// Next, you need to add a specialization for ScriptRegistry. Put this in the bottom of
/// ScriptMgr.cpp:
///
/// template class ScriptRegistry<MyScriptType>;
///
/// Now, add a cleanup routine in ScriptMgr::~ScriptMgr:
///
/// SCR_CLEAR(MyScriptType);
///
/// Now your script type is good to go with the script system. What you need to do now
/// is add functions to ScriptMgr that can be called from the core to actually trigger
/// certain events. For example, in ScriptMgr.h:
///
/// void OnSomeEvent(uint32 p_SomeArg1, std::string & p_SomeArg);
/// void OnAnotherEvent(uint32 p_SomeArg);
///
/// In ScriptMgr.cpp:
///
/// void ScriptMgr::OnSomeEvent(uint32 p_SomeArg1, std::string & p_SomeArg2)
/// {
///     FOREACH_SCRIPT(MyScriptType)->OnSomeEvent(p_SomeArg1, p_SomeArg2);
/// }
///
/// void ScriptMgr::OnAnotherEvent(uint32 p_SomeArg)
/// {
///     FOREACH_SCRIPT(MyScriptType)->OnAnotherEvent(p_SomeArg);
/// }
///
/// Now you simply call these two functions from anywhere in the core to trigger the
/// event on all registered scripts of that type.

/// World Object script interface
/// @t_DatabaseBound : It indicates whether or not this script type must be assigned in the database.
class ScriptObject
{
    friend class ScriptMgr;

    public:
        /// Do not override this in scripts; it should be overridden by the various script type classes.
        /// It indicates whether or not this script type must be assigned in the database.
        virtual bool IsDatabaseBound() const = 0;
        /// Return script name
        const std::string & GetName()
        {
            return m_Name;
        }

    protected:
        /// Constructor
        /// @p_Name : Script Name
        ScriptObject(const char * p_Name)
            : m_Name(p_Name)
        {

        }
        /// Destructor
        virtual ~ScriptObject()
        {

        }

    private:
        /// Script Name
        const std::string m_Name;

};

template<bool t_DatabaseBound> class ScriptObjectImpl : public ScriptObject
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        ScriptObjectImpl(const char * p_Name)
            : ScriptObject(p_Name)
        {

        }
        /// Destructor
        virtual ~ScriptObjectImpl()
        {

        }

    public:
        /// Do not override this in scripts; it should be overridden by the various script type classes.
        /// It indicates whether or not this script type must be assigned in the database.
        virtual bool IsDatabaseBound() const
        {
            return t_DatabaseBound;
        }

};

/// Update script base interface
template<class TObject> class UpdatableScript
{
    protected:
        /// Constructor
        UpdatableScript()
        {

        }

    public:
        /// On update
        /// @p_Object : Updated object instance
        /// @p_Diff   : Time since last update
        virtual void OnUpdate(TObject * p_Object, uint32 p_Diff)
        {
            UNUSED(p_Object);
            UNUSED(p_Diff);
        }

};

#endif  ///< SCRIPTING_INTERFACES_INTERFACEBASE_HPP_INCLUDED
