////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include "Object.h"
#include "../SharedPtrs/SharedPtrs.h"


class Unit;
class Aura;
class SpellInfo;

enum DynamicObjectType
{
    DYNAMIC_OBJECT_PORTAL           = 0x0,      // unused
    DYNAMIC_OBJECT_AREA_SPELL       = 0x1,
    DYNAMIC_OBJECT_FARSIGHT_FOCUS   = 0x2,
    DYNAMIC_OBJECT_UNK              = 0x8
};

class DynamicObject : public WorldObject, public GridObject<DynamicObject>
{
    public:
        DynamicObject(bool isWorldObject);
        ~DynamicObject();

        void AddToWorld();
        void RemoveFromWorld();

        bool CreateDynamicObject(uint32 guidlow, Unit* caster, SpellInfo const* spell, Position const& pos, float radius, DynamicObjectType type);
        void Update(uint32 p_time);
        void Remove();
        void SetDuration(int32 newDuration);
        int32 GetDuration() const;
        void Delay(int32 delaytime);
        void SetAura(Aura* aura);
        void RemoveAura();
        void SetCasterViewpoint();
        void RemoveCasterViewpoint();
        Unit* GetCaster() const { return _caster; }
        SpellInfo const* GetSpellInfo() const { return _spell; }
        DynamicObjectType GetType() const { return _type; }
        void BindToCaster();
        void UnbindFromCaster();
        uint32 GetSpellId() const {  return GetUInt32Value(DYNAMICOBJECT_FIELD_SPELL_ID); }
        uint64 GetCasterGUID() const { return GetGuidValue(DYNAMICOBJECT_FIELD_CASTER); }
        float GetRadius() const { return GetFloatValue(DYNAMICOBJECT_FIELD_RADIUS); }

        void Say(int32 textId, uint32 language, uint64 targetGuid) { MonsterSay(textId, language, targetGuid); }
        void Yell(int32 textId, uint32 language, uint64 targetGuid) { MonsterYell(textId, language, targetGuid); }
        void TextEmote(int32 textId, uint64 targetGuid) { MonsterTextEmote(textId, targetGuid); }
        void Whisper(int32 textId, uint64 receiver) { MonsterWhisper(textId, receiver); }
        void YellToZone(int32 textId, uint32 language, uint64 targetGuid) { MonsterYellToZone(textId, language, targetGuid); }

    protected:
        Aura* _aura;
        Aura* _removedAura;
        Unit* _caster;
        int32 _duration; // for non-aura dynobjects
        SpellInfo const* _spell;
        DynamicObjectType _type;
        bool _isViewpoint;
};
#endif
