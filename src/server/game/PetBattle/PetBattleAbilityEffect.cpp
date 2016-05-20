////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "PetBattleAbilityEffect.h"
#include "PetBattle.h"
#include "ScriptMgr.h"

#define MAX_PETBATTLE_EFFECT_TYPES 235

static const uint32 FailFlags = PETBATTLE_EVENT_FLAG_MISS | PETBATTLE_EVENT_FLAG_BLOCKED | PETBATTLE_EVENT_FLAG_DODGE | PETBATTLE_EVENT_FLAG_REFLECT | PETBATTLE_EVENT_FLAG_IMMUNE;

struct PetBattleAbilityEffectHandler
{
    bool (PetBattleAbilityEffect::*Handle)();
    PetBattleAbilityImplicitTarget ImplicitTarget;
};

static PetBattleAbilityEffectHandler Handlers[MAX_PETBATTLE_EFFECT_TYPES] =
{
    /* EFFECT 0   */{0,                                                         PETBATTLE_TARGET_NONE},
    /* EFFECT 1   */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 2   */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 3   */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 4   */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 5   */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 6   */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 7   */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 8   */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 9   */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 10  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 11  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 12  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 13  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 14  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 15  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 16  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 17  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 18  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 19  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 20  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 21  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 22  */{&PetBattleAbilityEffect::HandleDummy,                      PETBATTLE_TARGET_CASTER}, // Dummy
    /* EFFECT 23  */{&PetBattleAbilityEffect::HandleHeal,                       PETBATTLE_TARGET_CASTER},
    /* EFFECT 24  */{&PetBattleAbilityEffect::HandleDamage,                     PETBATTLE_TARGET_TARGET},
    /* EFFECT 25  */{&PetBattleAbilityEffect::HandleCatch,                      PETBATTLE_TARGET_TARGET},
    /* EFFECT 26  */{&PetBattleAbilityEffect::HandlePositiveAura,               PETBATTLE_TARGET_CASTER},
    /* EFFECT 27  */{&PetBattleAbilityEffect::HandleRampingDamage,              PETBATTLE_TARGET_TARGET},
    /* EFFECT 28  */{&PetBattleAbilityEffect::HandleCancellableAura,            PETBATTLE_TARGET_CASTER},
    /* EFFECT 29  */{&PetBattleAbilityEffect::HandleStateDamage,                PETBATTLE_TARGET_TARGET},
    /* EFFECT 30  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 31  */{&PetBattleAbilityEffect::HandleSetState,                   PETBATTLE_TARGET_CASTER},
    /* EFFECT 32  */{&PetBattleAbilityEffect::HandleHealPercentDealt,           PETBATTLE_TARGET_CASTER},
    /* EFFECT 33  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 34  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 35  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 36  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 37  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 38  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 39  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 40  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 41  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 42  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 43  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 44  */{&PetBattleAbilityEffect::HandleHealLastHitTaken,           PETBATTLE_TARGET_CASTER},
    /* EFFECT 45  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 46  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 47  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 48  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 49  */{&PetBattleAbilityEffect::HandleRemoveAura,                 PETBATTLE_TARGET_CASTER},
    /* EFFECT 50  */{&PetBattleAbilityEffect::HandleNegativeAura,               PETBATTLE_TARGET_TARGET},
    /* EFFECT 51  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 52  */{&PetBattleAbilityEffect::HandleControlAura,                PETBATTLE_TARGET_TARGET},
    /* EFFECT 53  */{&PetBattleAbilityEffect::HandleHealPercent,                PETBATTLE_TARGET_CASTER},
    /* EFFECT 54  */{&PetBattleAbilityEffect::HandlePeriodicTrigger,            PETBATTLE_TARGET_TARGET},
    /* EFFECT 55  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 56  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 57  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 58  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 59  */{&PetBattleAbilityEffect::HandleWitchingDamage,             PETBATTLE_TARGET_TARGET},
    /* EFFECT 60  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 61  */{&PetBattleAbilityEffect::HandleHealCasterPercentNotState,  PETBATTLE_TARGET_CASTER_TEAM_UNCONDITIONAL},
    /* EFFECT 62  */{&PetBattleAbilityEffect::HandleDamagePercent,              PETBATTLE_TARGET_TARGET},
    /* EFFECT 63  */{&PetBattleAbilityEffect::HandlePeriodicPositiveTrigger,    PETBATTLE_TARGET_CASTER},
    /* EFFECT 64  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 65  */{&PetBattleAbilityEffect::HandleDamageStateBonus,           PETBATTLE_TARGET_TARGET},
    /* EFFECT 66  */{&PetBattleAbilityEffect::HandleDamageRuthless,             PETBATTLE_TARGET_TARGET},
    /* EFFECT 67  */{&PetBattleAbilityEffect::HandleEqualizeLife,               PETBATTLE_TARGET_TARGET},
    /* EFFECT 68  */{&PetBattleAbilityEffect::HandleDamageCasterPercent,        PETBATTLE_TARGET_CASTER},
    /* EFFECT 69  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 70  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 71  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 72  */{&PetBattleAbilityEffect::HandleHealToggleAura,             PETBATTLE_TARGET_CASTER},
    /* EFFECT 73  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 74  */{&PetBattleAbilityEffect::HandleInitialization,             PETBATTLE_TARGET_CASTER},
    /* EFFECT 75  */{&PetBattleAbilityEffect::HandleDamageToggleAura,           PETBATTLE_TARGET_TARGET},
    /* EFFECT 76  */{&PetBattleAbilityEffect::HandleDamageToggleAura,           PETBATTLE_TARGET_TARGET},
    /* EFFECT 77  */{&PetBattleAbilityEffect::HandleDamageToggleAura,           PETBATTLE_TARGET_TARGET},
    /* EFFECT 78  */{&PetBattleAbilityEffect::HandleHealStateToggleAura,        PETBATTLE_TARGET_CASTER},
    /* EFFECT 79  */{&PetBattleAbilityEffect::HandleModState,                   PETBATTLE_TARGET_CASTER},
    /* EFFECT 80  */{&PetBattleAbilityEffect::HandleWeatherAura,                PETBATTLE_TARGET_ALL},
    /* EFFECT 81  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 82  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 83  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 84  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 85  */{&PetBattleAbilityEffect::HandleResetState,                 PETBATTLE_TARGET_ALL},
    /* EFFECT 86  */{&PetBattleAbilityEffect::HandleDamageAuraToggleAura,       PETBATTLE_TARGET_TARGET},
    /* EFFECT 87  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 88  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 89  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 90  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 91  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 92  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 93  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 94  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 95  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 96  */{&PetBattleAbilityEffect::HandleDamageHitState,             PETBATTLE_TARGET_TARGET},
    /* EFFECT 97  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 98  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 99  */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 100 */{&PetBattleAbilityEffect::HandleHealPetType,                PETBATTLE_TARGET_CASTER},
    /* EFFECT 101 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 102 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 103 */{&PetBattleAbilityEffect::HandleExtraAttackIfMoreFaster,    PETBATTLE_TARGET_TARGET},
    /* EFFECT 104 */{&PetBattleAbilityEffect::HandleHealState,                  PETBATTLE_TARGET_CASTER},
    /* EFFECT 105 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 106 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 107 */{&PetBattleAbilityEffect::HandleSwap,                       PETBATTLE_TARGET_TARGET},
    /* EFFECT 108 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 109 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 110 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 111 */{&PetBattleAbilityEffect::HandleResurect,                   PETBATTLE_TARGET_CASTER},
    /* EFFECT 112 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 113 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 114 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 115 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 116 */{&PetBattleAbilityEffect::HandleUpdatePriority,             PETBATTLE_TARGET_CASTER},
    /* EFFECT 117 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 118 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 119 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 120 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 121 */{&PetBattleAbilityEffect::HandleHealthConsume,              PETBATTLE_TARGET_CASTER},
    /* EFFECT 122 */{0,                                                         PETBATTLE_TARGET_NONE}, // Clone
    /* EFFECT 123 */{0,                                                         PETBATTLE_TARGET_NONE}, // Clone
    /* EFFECT 124 */{0,                                                         PETBATTLE_TARGET_NONE}, // Clone
    /* EFFECT 125 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 126 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 127 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 128 */{&PetBattleAbilityEffect::HandleSetHealthPercent,           PETBATTLE_TARGET_TARGET},
    /* EFFECT 129 */{&PetBattleAbilityEffect::HandleLockActiveAbility,          PETBATTLE_TARGET_TARGET},
    /* EFFECT 130 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 131 */{&PetBattleAbilityEffect::HandleAuraIfCasterFaster,         PETBATTLE_TARGET_TARGET},
    /* EFFECT 132 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 133 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 134 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 135 */{&PetBattleAbilityEffect::HandleKill,                       PETBATTLE_TARGET_CASTER},
    /* EFFECT 136 */{&PetBattleAbilityEffect::HandleCleansing,                  PETBATTLE_TARGET_CASTER},
    /* EFFECT 137 */{&PetBattleAbilityEffect::HandleAuraState,                  PETBATTLE_TARGET_CASTER},
    /* EFFECT 138 */{&PetBattleAbilityEffect::HandleChangeCasterState,          PETBATTLE_TARGET_TARGET},
    /* EFFECT 139 */{&PetBattleAbilityEffect::HandleSwapLow,                    PETBATTLE_TARGET_TARGET},
    /* EFFECT 140 */{&PetBattleAbilityEffect::HandleResetAuraDuration,          PETBATTLE_TARGET_TARGET},
    /* EFFECT 141 */{&PetBattleAbilityEffect::HandleSplittedDamage,             PETBATTLE_TARGET_TARGET_TEAM},
    /* EFFECT 142 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 143 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 144 */{0,                                                         PETBATTLE_TARGET_NONE}, // Dummy select dead target
    /* EFFECT 145 */{&PetBattleAbilityEffect::HandleSwapHigh,                   PETBATTLE_TARGET_TARGET},
    /* EFFECT 146 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 147 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 148 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 149 */{&PetBattleAbilityEffect::HandleDamageNonLetal,             PETBATTLE_TARGET_TARGET},
    /* EFFECT 150 */{&PetBattleAbilityEffect::HandleRedirectionAura,            PETBATTLE_TARGET_CASTER},
    /* EFFECT 151 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 152 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 153 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 154 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 155 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 156 */{&PetBattleAbilityEffect::HandleCheckState,                 PETBATTLE_TARGET_TARGET},
    /* EFFECT 157 */{&PetBattleAbilityEffect::HandleCheckState,                 PETBATTLE_TARGET_CASTER},
    /* EFFECT 158 */{&PetBattleAbilityEffect::HandleStopChainFailure,           PETBATTLE_TARGET_CASTER},
    /* EFFECT 159 */{0,                                                         PETBATTLE_TARGET_NONE}, // Dummy select next target?
    /* EFFECT 160 */{&PetBattleAbilityEffect::HandleExtraAttackIfLessFaster,    PETBATTLE_TARGET_TARGET},
    /* EFFECT 161 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 162 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 163 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 164 */{&PetBattleAbilityEffect::HandleExtraAttack,                PETBATTLE_TARGET_TARGET},
    /* EFFECT 165 */{&PetBattleAbilityEffect::HandleTryRevive,                  PETBATTLE_TARGET_CASTER},
    /* EFFECT 166 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 167 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 168 */{&PetBattleAbilityEffect::HandleNegativeAura,               PETBATTLE_TARGET_TARGET},
    /* EFFECT 169 */{&PetBattleAbilityEffect::HandleStateDamage,                PETBATTLE_TARGET_HEAD},
    /* EFFECT 170 */{&PetBattleAbilityEffect::HandleWeatherDamage,              PETBATTLE_TARGET_TARGET},
    /* EFFECT 171 */{&PetBattleAbilityEffect::HandleHealOnSpecificWeather,      PETBATTLE_TARGET_CASTER},
    /* EFFECT 172 */{&PetBattleAbilityEffect::HandleAuraCondAccuracyState,      PETBATTLE_TARGET_TARGET},
    /* EFFECT 173 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 174 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 175 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 176 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 177 */{&PetBattleAbilityEffect::HandleCheckState,                 PETBATTLE_TARGET_TARGET},
    /* EFFECT 178 */{&PetBattleAbilityEffect::HandlePowerlessAura,              PETBATTLE_TARGET_TARGET},
    /* EFFECT 179 */{&PetBattleAbilityEffect::HandleDamage,                     PETBATTLE_TARGET_TARGET},
    /* EFFECT 180 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 181 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 182 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 183 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 184 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 185 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 186 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 187 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 188 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 189 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 190 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 191 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 192 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 193 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 194 */{&PetBattleAbilityEffect::HandleCheckFailure,               PETBATTLE_TARGET_CASTER},
    /* EFFECT 195 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 196 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 197 */{&PetBattleAbilityEffect::HandleDamagePercentTaken,         PETBATTLE_TARGET_TARGET},
    /* EFFECT 198 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 199 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 200 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 201 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 202 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 203 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 204 */{&PetBattleAbilityEffect::HandleDamagePercentTaken,         PETBATTLE_TARGET_TARGET},
    /* EFFECT 205 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 206 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 207 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 208 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 209 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 210 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 211 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 212 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 213 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 214 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 215 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 216 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 217 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 218 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 219 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 220 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 221 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 222 */{&PetBattleAbilityEffect::HandleDamageRange,                PETBATTLE_TARGET_TARGET},
    /* EFFECT 223 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 224 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 225 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 226 */{&PetBattleAbilityEffect::HandleDamageWithBonus,            PETBATTLE_TARGET_NONE},
    /* EFFECT 227 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 228 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 229 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 230 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 231 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 232 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 233 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE},
    /* EFFECT 234 */{&PetBattleAbilityEffect::HandleNull,                       PETBATTLE_TARGET_NONE}
};

uint32 PetBattleAbilityEffect::GetActiveOpponent()
{
    return PetBattleInstance->Teams[!PetBattleInstance->Pets[Caster]->TeamID]->ActivePetID;
}

uint32 PetBattleAbilityEffect::GetPetType()
{
    if (BattlePetAbilityTurnEntry const* abilityTurn = sBattlePetAbilityTurnStore.LookupEntry(EffectInfo->abilityTurnId))
        if (BattlePetAbilityEntry const* ability = sBattlePetAbilityStore.LookupEntry(abilityTurn->abilityId))
            return ability->petType;

    return BATTLEPET_PETTYPE_HUMANOID;
}

bool PetBattleAbilityEffect::Damage(uint32 l_Target, int32 l_Damage, bool p_CantBeAvoidBlockedDodged)
{
    if (!(Flags & FailFlags) && l_Damage <= 0)
        Flags |= PETBATTLE_EVENT_FLAG_MISS;

    if (!(Flags & FailFlags) && !p_CantBeAvoidBlockedDodged)
    {
        for (uint32 l_PetID = 0; l_PetID < (MAX_PETBATTLE_SLOTS * MAX_PETBATTLE_TEAM); ++l_PetID)
        {
            if (!PetBattleInstance->Pets[l_PetID])
                continue;

            if (GetState(l_PetID, BATTLEPET_STATE_Mechanic_IsWall))
            {
                if (PetBattleInstance->Pets[l_PetID]->TeamID == PetBattleInstance->Pets[Caster]->TeamID
                    && !GetState(l_PetID, BATTLEPET_STATE_Special_BlockedFriendlyMode))
                    continue;

                int32 l_BlockedAttackCount = GetState(l_PetID, BATTLEPET_STATE_Special_BlockedAttackCount);

                if (l_BlockedAttackCount > 0)
                {
                    l_BlockedAttackCount -= 1;

                    SetState(l_PetID, BATTLEPET_STATE_Special_BlockedAttackCount, l_BlockedAttackCount);

                    Flags |= PETBATTLE_EVENT_FLAG_BLOCKED;

                    if (!l_BlockedAttackCount)
                        if (PetBattleAura* l_Aura = GetAura(l_PetID, GetState(l_PetID, BATTLEPET_STATE_Special_ObjectRedirectionAuraID)))
                            l_Aura->Expire(PetBattleInstance);

                    break;
                }
            }
        }
    }

    if (Flags & FailFlags)
    {
        l_Damage = 0;

        if (ReportFailAsImmune)
            Flags = (Flags & ~FailFlags) | PETBATTLE_EVENT_FLAG_IMMUNE;
    }

    /// Passive: aquatic
    if (GetState(l_Target, BATTLEPET_STATE_Passive_Aquatic) && (Flags & PETBATTLE_EVENT_FLAG_PERIODIC))
        l_Damage /= 2;

    /// Passive: magic
    if (GetState(l_Target, BATTLEPET_STATE_Passive_Magic))
        if (l_Damage * 100 / GetMaxHealth(l_Target) >= 35)
            l_Damage = CalculatePct(GetMaxHealth(l_Target), 35);

    int32 l_Health = PetBattleInstance->Pets[l_Target]->Health;
    l_Health -= l_Damage;

    if (l_Health <= 0 && PetBattleInstance->Pets[l_Target]->Health <= 0)
        return l_Damage > 0;

    SetHealth(l_Target, l_Health);

    if (l_Damage > 0)
    {
        PetBattleInstance->SetPetState(Caster, Caster, EffectInfo->id, BATTLEPET_STATE_Condition_DidDamageThisRound, 1);
        PetBattleInstance->SetPetState(Caster, l_Target, EffectInfo->id, BATTLEPET_STATE_Condition_WasDamagedThisTurn, 1);
        PetBattleInstance->SetPetState(Caster, l_Target, EffectInfo->id, BATTLEPET_STATE_Last_HitTaken, l_Damage);
        PetBattleInstance->SetPetState(Caster, Caster, EffectInfo->id, BATTLEPET_STATE_Last_HitDealt, l_Damage);
    }

    if (!IsTriggered)
    {
        for (PetBattleAuraList::iterator l_It = PetBattleInstance->PetAuras.begin(); l_It != PetBattleInstance->PetAuras.end(); ++l_It)
        {
            if ((*l_It)->Expired)
                continue;

            /// Can't self-proc, avoid deadloop
            if (AbilityID == (*l_It)->AbilityID)
                continue;

            if ((*l_It)->CasterPetID == Caster)
                PetBattleInstance->Cast((*l_It)->CasterPetID, (*l_It)->AbilityID, 0, PETBATTLE_ABILITY_TURN0_PROC_ON_DAMAGE_DEALT, PETBATTLE_CAST_TRIGGER_ALL);
            else if ((*l_It)->CasterPetID == l_Target)
                PetBattleInstance->Cast((*l_It)->CasterPetID, (*l_It)->AbilityID, 0, PETBATTLE_ABILITY_TURN0_PROC_ON_DAMAGE_TAKEN, PETBATTLE_CAST_TRIGGER_ALL);
        }
    }

    return l_Damage > 0;
}

bool PetBattleAbilityEffect::Heal(uint32 p_Target, int32 p_Heal)
{
    int32 l_Health = PetBattleInstance->Pets[p_Target]->Health;
    int32 l_MaxHealth = PetBattleInstance->Pets[p_Target]->GetMaxHealth();

    if (l_Health + p_Heal > l_MaxHealth)
        p_Heal = l_MaxHealth - l_Health;

    if (p_Heal <= 0)
    {
        p_Heal = 0;
        Flags |= PETBATTLE_EVENT_FLAG_MISS;
    }

    if (Flags & FailFlags)
    {
        p_Heal = 0;

        if (ReportFailAsImmune)
            Flags = (Flags & ~FailFlags) | PETBATTLE_EVENT_FLAG_IMMUNE;
    }

    l_Health += p_Heal;

    SetHealth(p_Target, l_Health);

    if (!IsTriggered)
    {
        for (PetBattleAuraList::iterator l_It = PetBattleInstance->PetAuras.begin(); l_It != PetBattleInstance->PetAuras.end(); ++l_It)
        {
            if ((*l_It)->Expired)
                continue;

            if ((*l_It)->CasterPetID == Caster)
                PetBattleInstance->Cast((*l_It)->CasterPetID, (*l_It)->AbilityID, 0, PETBATTLE_ABILITY_TURN0_PROC_ON_HEAL_DEALT, PETBATTLE_CAST_TRIGGER_ALL);
            else if ((*l_It)->CasterPetID == p_Target)
                PetBattleInstance->Cast((*l_It)->CasterPetID, (*l_It)->AbilityID, 0, PETBATTLE_ABILITY_TURN0_PROC_ON_HEAL_TAKEN, PETBATTLE_CAST_TRIGGER_ALL);
        }
    }

    return p_Heal > 0;
}

/// Modify pet battle state increment or decrement state by ModValue
/// @p_Target   : Target Pet ID
/// @p_StateID  : ID of the state to modify
/// @p_ModValue : Value to add or sub
/// @p_Apply    : Add or sub p_ModValue
void PetBattleAbilityEffect::ModState(uint32 p_Target, uint32 p_StateID, int32 p_ModValue, bool p_Apply)
{
    SetState(p_Target, p_StateID, PetBattleInstance->Pets[p_Target]->States[p_StateID] + (p_Apply ? p_ModValue : -p_ModValue));
}

int32 PetBattleAbilityEffect::CalculateDamage(int32 p_Damage)
{
    /// Modifiers
    p_Damage += GetState(Caster, BATTLEPET_STATE_Add_FlatDamageDealt);
    p_Damage += GetState(Target, BATTLEPET_STATE_Add_FlatDamageTaken);

    /// Power
    int32 l_ModPercent = CalculatePct(GetState(Caster, BATTLEPET_STATE_Stat_Power), 5);

    /// Mod Dealt / Taken
    l_ModPercent += GetState(Caster, BATTLEPET_STATE_Mod_DamageDealtPercent);
    l_ModPercent += GetState(Target, BATTLEPET_STATE_Mod_DamageTakenPercent);

    /// Pet type damage mod
    uint32 l_AbilityPetType = GetPetType();
    uint32 l_TargetPetType = BATTLEPET_PETTYPE_HUMANOID;

    if (BattlePetSpeciesEntry const* targetSpeciesInfo = sBattlePetSpeciesStore.LookupEntry(PetBattleInstance->Pets[Target]->Species))
        l_TargetPetType = targetSpeciesInfo->type;

    int32 l_ModPetTypeDamagePercent = -100;

    if (GtBattlePetTypeDamageModEntry const* l_DamageModInfo = sGtBattlePetTypeDamageModStore.LookupEntry(l_TargetPetType * NUM_BATTLEPET_PETTYPES + l_AbilityPetType))
        l_ModPetTypeDamagePercent += l_DamageModInfo->mod * 100;

    if (GetState(Caster, BATTLEPET_STATE_Mod_PetType_ID) == l_AbilityPetType)
        l_ModPetTypeDamagePercent += GetState(Caster, BATTLEPET_STATE_Mod_PetTypeDamageDealtPercent);

    if (GetState(Target, BATTLEPET_STATE_Mod_PetType_ID) == l_AbilityPetType)
        l_ModPetTypeDamagePercent += GetState(Target, BATTLEPET_STATE_Mod_PetTypeDamageTakenPercent);

    if (l_ModPetTypeDamagePercent > 0)
        Flags |= PETBATTLE_EVENT_FLAG_STRONG;
    else if (l_ModPetTypeDamagePercent < 0)
        Flags |= PETBATTLE_EVENT_FLAG_WEAK;

    // Critical
    if (roll_chance_i(GetState(Target, BATTLEPET_STATE_Stat_CritChance)))
    {
        l_ModPercent += 100;
        Flags |= PETBATTLE_EVENT_FLAG_CRITICAL;
    }

    // Passive: beast
    if (GetState(Caster, BATTLEPET_STATE_Passive_Beast))
        if (GetHealth(Caster) * 100 / GetMaxHealth(Caster) < 50)
            l_ModPetTypeDamagePercent += 25;

    // Calculate result
    p_Damage += CalculatePct(p_Damage, l_ModPercent + l_ModPetTypeDamagePercent);

    // Passive: magic
    if (GetState(Target, BATTLEPET_STATE_Passive_Magic))
        if (p_Damage * 100 / GetMaxHealth(Target) >= 35)
            p_Damage = CalculatePct(GetMaxHealth(Target), 35);

    return p_Damage;
}

int32 PetBattleAbilityEffect::CalculateHeal(int32 heal)
{
    // Add power
    int32 l_ModPercent = CalculatePct(GetState(Caster, BATTLEPET_STATE_Stat_Power), 5);

    // Modifiers Dealt / Taken
    l_ModPercent += GetState(Caster, BATTLEPET_STATE_Mod_HealingDealtPercent);
    l_ModPercent += GetState(Target, BATTLEPET_STATE_Mod_HealingTakenPercent);

    return heal + CalculatePct(heal, l_ModPercent);
}

int32 PetBattleAbilityEffect::CalculateHit(int32 p_Accuracy)
{
    // Add stat
    p_Accuracy += GetState(Caster, BATTLEPET_STATE_Stat_Accuracy);
    p_Accuracy -= GetState(Target, BATTLEPET_STATE_Stat_Dodge);

    // Calculate
    if (p_Accuracy < 100 && !roll_chance_i(p_Accuracy))
        Flags |= PETBATTLE_EVENT_FLAG_MISS;

    if (Caster != Target && GetState(Target, BATTLEPET_STATE_untargettable))
        Flags |= PETBATTLE_EVENT_FLAG_MISS;

    return p_Accuracy;
}

bool PetBattleAbilityEffect::SetState(uint32 p_Target, uint32 p_State, int32 p_Value)
{
    /// Passive : Critter
    if (GetState(p_Target, BATTLEPET_STATE_Passive_Critter))
    {
        switch (p_State)
        {
            case BATTLEPET_STATE_Mechanic_IsStunned:
            case BATTLEPET_STATE_Mechanic_IsWebbed:
            case BATTLEPET_STATE_turnLock:
                Flags |= PETBATTLE_EVENT_FLAG_IMMUNE;
                break;

            default:
                break;
        }
    }

    if (!(Flags & FailFlags))
    {
        PetBattleInstance->Pets[p_Target]->States[p_State] = p_Value;
    }

    BattlePetStateEntry const* l_StateInfo = sBattlePetStateStore.LookupEntry(p_State);

    if (l_StateInfo && l_StateInfo->flags)
    {
        PetBattleEvent l_Event(PETBATTLE_EVENT_SET_STATE, Caster, Flags, EffectInfo->id, PetBattleInstance->RoundTurn++, 0, 1);

        l_Event.UpdateState(p_Target, p_State, PetBattleInstance->Pets[p_Target]->States[p_State]);

        PetBattleInstance->RoundEvents.push_back(l_Event);
    }

    return !(Flags & FailFlags);
}

int32 PetBattleAbilityEffect::GetState(uint32 p_Target, uint32 p_State)
{
    return PetBattleInstance->Pets[p_Target]->States[p_State];
}

int32 PetBattleAbilityEffect::GetHealth(uint32 p_Target)
{
    return PetBattleInstance->Pets[p_Target]->Health;
}

int32 PetBattleAbilityEffect::GetSpeed(uint32 p_Target)
{
    return PetBattleInstance->Pets[p_Target]->GetSpeed();
}

int32 PetBattleAbilityEffect::GetMaxHealth(uint32 p_Target)
{
    return PetBattleInstance->Pets[p_Target]->GetMaxHealth();
}

bool PetBattleAbilityEffect::SetHealth(uint32 p_Target, int32 p_Value)
{
    int32 l_MaxHealth = GetMaxHealth(p_Target);

    if (p_Value > l_MaxHealth)
        p_Value = l_MaxHealth;

    if (!(Flags & FailFlags) && p_Value == PetBattleInstance->Pets[p_Target]->Health)
        Flags |= PETBATTLE_EVENT_FLAG_MISS;

    if (!(Flags & FailFlags))
    {
        if (p_Value <= 0 && !GetAura(p_Target, 284))    ///< Buff : Suvival http://www.wowhead.com/petability=283/survival
        {
            Flags |= PETBATTLE_EVENT_FLAG_UNK_KILL;

            if (!GetState(p_Target, BATTLEPET_STATE_Is_Dead))
                PetBattleInstance->SetPetState(Caster, p_Target, EffectInfo->id, BATTLEPET_STATE_Internal_HealthBeforeInstakill, PetBattleInstance->Pets[p_Target]->Health);
        }
        else if (p_Value <= 0 && GetAura(p_Target, 284))    ///< Buff : Suvival http://www.wowhead.com/petability=283/survival
            p_Value = 1;

        PetBattleInstance->Pets[p_Target]->Health = p_Value;
    }

    PetBattleEvent l_Event(PETBATTLE_EVENT_SET_HEALTH, Caster, Flags, EffectInfo->id, PetBattleInstance->RoundTurn++, 0, 1);
    l_Event.UpdateHealth(p_Target, PetBattleInstance->Pets[p_Target]->Health);

    PetBattleInstance->RoundEvents.push_back(l_Event);

    if (!(Flags & FailFlags) && PetBattleInstance->Pets[p_Target]->Health <= 0 && !GetState(p_Target, BATTLEPET_STATE_Is_Dead) && !GetState(p_Target, BATTLEPET_STATE_unkillable))
        Kill(p_Target);

    return !(Flags & FailFlags);
}

void PetBattleAbilityEffect::Trigger(uint32 p_Target, uint32 p_Ability)
{
    PetBattleEvent l_Event(PETBATTLE_EVENT_SET_HEALTH, Caster, Flags, EffectInfo->id, PetBattleInstance->RoundTurn++, 0, 1);

    l_Event.Trigger(p_Target, p_Ability);

    PetBattleInstance->RoundEvents.push_back(l_Event);

    PetBattleInstance->Cast(Caster, p_Ability, 0, PETBATTLE_ABILITY_TURN0_PROC_ON_NONE, PETBATTLE_CAST_TRIGGER_NONE);
}

PetBattleAura* PetBattleAbilityEffect::GetAura(uint32 p_Target, uint32 p_Ability)
{
    for (PetBattleAuraList::iterator l_AuraIT = PetBattleInstance->PetAuras.begin(); l_AuraIT != PetBattleInstance->PetAuras.end(); ++l_AuraIT)
    {
        PetBattleAura * l_Aura = *l_AuraIT;

        if (!l_Aura->Expired && l_Aura->TargetPetID == p_Target && l_Aura->AbilityID == p_Ability)
            return l_Aura;
    }

    return NULL;
}

void PetBattleAbilityEffect::Kill(uint32 p_Target)
{
    if (Flags & FailFlags)
    {
        SetState(p_Target, BATTLEPET_STATE_Is_Dead, 1);
        return;
    }

    PetBattleInstance->Kill(Caster, p_Target, EffectInfo->id, false, Flags);

    /// Passive: mechanical
    if (GetState(p_Target, BATTLEPET_STATE_Passive_Mechanical))
    {
        Trigger(p_Target, 723);
        SetState(p_Target, BATTLEPET_STATE_Passive_Mechanical, 0);
        return;
    }

    /// Passive: undead
    if (GetState(p_Target, BATTLEPET_STATE_Passive_Undead))
    {
        Trigger(p_Target, 242);
        SetState(p_Target, BATTLEPET_STATE_Passive_Undead, 0);
        return;
    }

    SetState(p_Target, BATTLEPET_STATE_Special_ConsumedCorpse, 1);

    PetBattleInstance->RoundDeadPets.push_back(p_Target);
    PetBattleInstance->Kill(Caster, p_Target, EffectInfo->id);

    StopChain = true;
}

bool PetBattleAbilityEffect::AddTarget(uint32 target)
{
    if (!PetBattleInstance->Pets[target])
        return false;

    if (!PetBattleInstance->Pets[target]->IsAlive())
        return false;

    Targets.push_back(target);
    return true;
}

bool PetBattleAbilityEffect::AddTarget(PetBattleAbilityImplicitTarget /*target*/)
{
    PetBattleTeam* l_Team;
    switch (Handlers[EffectInfo->effect].ImplicitTarget)
    {
        case PETBATTLE_TARGET_CASTER:
            return AddTarget(Caster);
        case PETBATTLE_TARGET_TARGET:
            return AddTarget(GetActiveOpponent());
        case PETBATTLE_TARGET_CASTER_TEAM:
            l_Team = PetBattleInstance->Teams[PetBattleInstance->Pets[Caster]->TeamID];
            for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < l_Team->TeamPetCount; ++l_CurrentPetSlot)
                AddTarget(l_Team->TeamPets[l_CurrentPetSlot]->ID);
            return true;
        case PETBATTLE_TARGET_CASTER_TEAM_0:
            l_Team = PetBattleInstance->Teams[PetBattleInstance->Pets[Caster]->TeamID];
            if (l_Team->TeamPetCount <= 1)
                return AddTarget(l_Team->TeamPets[0]->ID);
            return false;
        case PETBATTLE_TARGET_CASTER_TEAM_1:
            l_Team = PetBattleInstance->Teams[PetBattleInstance->Pets[Caster]->TeamID];
            if (l_Team->TeamPetCount <= 2)
                return AddTarget(l_Team->TeamPets[1]->ID);
            return false;
        case PETBATTLE_TARGET_CASTER_TEAM_2:
            l_Team = PetBattleInstance->Teams[PetBattleInstance->Pets[Caster]->TeamID];
            if (l_Team->TeamPetCount <= 3)
                return AddTarget(l_Team->TeamPets[2]->ID);
            return false;
        case PETBATTLE_TARGET_TARGET_TEAM:
            l_Team = PetBattleInstance->Teams[!PetBattleInstance->Pets[Caster]->TeamID];
            for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < l_Team->TeamPetCount; ++l_CurrentPetSlot)
                return AddTarget(l_Team->TeamPets[l_CurrentPetSlot]->ID);
            return true;
        case PETBATTLE_TARGET_TARGET_TEAM_0:
            l_Team = PetBattleInstance->Teams[!PetBattleInstance->Pets[Caster]->TeamID];
            if (l_Team->TeamPetCount <= 1)
                return AddTarget(l_Team->TeamPets[PETBATTLE_TEAM_1]->ID);
            return false;
        case PETBATTLE_TARGET_TARGET_TEAM_1:
            l_Team = PetBattleInstance->Teams[!PetBattleInstance->Pets[Caster]->TeamID];
            if (l_Team->TeamPetCount <= 2)
                return AddTarget(l_Team->TeamPets[1]->ID);
            return false;
        case PETBATTLE_TARGET_TARGET_TEAM_2:
            l_Team = PetBattleInstance->Teams[!PetBattleInstance->Pets[Caster]->TeamID];
            if (l_Team->TeamPetCount <= 3)
                return AddTarget(l_Team->TeamPets[2]->ID);
            return false;
        case PETBATTLE_TARGET_ALL:
            for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
                AddTarget(l_CurrentPetSlot);
            return true;
        case PETBATTLE_TARGET_CASTER_TEAM_UNCONDITIONAL:
            l_Team = PetBattleInstance->Teams[PetBattleInstance->Pets[Caster]->TeamID];
            for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < l_Team->TeamPetCount; ++l_CurrentPetSlot)
                if (PetBattleInstance->Pets[l_CurrentPetSlot])
                    Targets.push_back(l_Team->TeamPets[l_CurrentPetSlot]->ID);
            return true;
        case PETBATTLE_TARGET_HEAD:
            return AddTarget(Caster) | AddTarget(GetActiveOpponent());
        default:
            return false;
    }
}

bool PetBattleAbilityEffect::AddAura(uint32 target, int32 duration, int32 maxAllowed)
{
    return PetBattleInstance->AddAura(Caster, target, EffectInfo->triggerAbility, duration, maxAllowed, EffectInfo->id, Flags);
}

void PetBattleAbilityEffect::SelectTargets()
{
    Targets.clear();
    AddTarget(Handlers[EffectInfo->effect].ImplicitTarget);
}

bool PetBattleAbilityEffect::Execute()
{
    if (!Handlers[EffectInfo->effect].Handle)
        return false;

    if (Targets.empty())
        return false;

    StopChain = false;
    bool l_Result = true;

    for (size_t l_I = 0; l_I < Targets.size(); ++l_I)
    {
        Target = Targets[l_I];
        Flags = 0;

        if (PetBattleInstance->Pets[Caster]->TeamID != PetBattleInstance->Pets[Target]->TeamID && PetBattleInstance->Pets[Target]->States[BATTLEPET_STATE_untargettable])
            Flags |= PETBATTLE_EVENT_FLAG_DODGE;

        bool l_iResult = (this->*Handlers[EffectInfo->effect].Handle)();
        l_Result &= l_iResult;

        if (!l_iResult)
            continue;

        BattlePetEffectPropertiesEntry const* l_PropertyEntry = sBattlePetEffectPropertiesStore.LookupEntry(EffectInfo->effect);

        if (!l_PropertyEntry)
            continue;

        //if ((l_PropertyEntry->flags >> 2) == BATTLEPET_EFFECT_CATEGORY_DEAL)
        //{
        //    if (l_PropertyEntry->flags & BATTLEPET_EFFECT_FLAG_POSITIVE)
        //        Heal(Target, Value);

        //    if (l_PropertyEntry->flags & BATTLEPET_EFFECT_FLAG_NEGATIVE)
        //        Damage(Target, Value);
        //}
    }

    return l_Result;
}

bool PetBattleAbilityEffect::HandleDummy()
{
    return true;
}

bool PetBattleAbilityEffect::HandleDamage()
{
    CalculateHit(EffectInfo->prop[1]);

    if (EffectInfo->prop[2])
        Flags |= PETBATTLE_EVENT_FLAG_PERIODIC;

    return Damage(Target, CalculateDamage(EffectInfo->prop[0]), EffectInfo->prop[2]);
}

bool PetBattleAbilityEffect::HandleWitchingDamage()
{
    CalculateHit(EffectInfo->prop[1]);

    /// Witching
    int32 l_Damage        = EffectInfo->prop[0];
    int32 l_CasterHealPct = (GetHealth(Caster) * 100) / GetMaxHealth(Caster);

    if (EffectInfo->prop[2] && l_CasterHealPct < EffectInfo->prop[2])
        l_Damage *= 2;
    else if (GetHealth(Target) > GetHealth(Caster))                     ///< http://fr.wowhead.com/petability=253/repliquer
        l_Damage *= 2;

    return Damage(Target, CalculateDamage(l_Damage));
}

bool PetBattleAbilityEffect::HandleStateDamage()
{
    /// In some case, proc only if caster state prop[2] is on
    if (EffectInfo->prop[2] != 0 && !GetState(Caster, EffectInfo->prop[2]))
        return false;

    if (EffectInfo->prop[4])
        Flags |= PETBATTLE_EVENT_FLAG_PERIODIC;

    CalculateHit(EffectInfo->prop[1]);
    int32 l_Damage = CalculateDamage(EffectInfo->prop[0]);

    /// Double base damage is the state in Prop[3] is on
    if (EffectInfo->prop[3] != 0 && GetState(Target, EffectInfo->prop[3]) == 1)
        l_Damage *= 2;

    return Damage(Target, l_Damage);
}

bool PetBattleAbilityEffect::HandleSetState()
{
    return SetState(Target, EffectInfo->prop[0], EffectInfo->prop[1]);
}

bool PetBattleAbilityEffect::HandleStopChainFailure()
{
    if (AbilityTurn->ChainFailure)
        StopChain = true;

    return true;
}

bool PetBattleAbilityEffect::HandleHealPercentDealt()
{
    // Chain Failure
    if (EffectInfo->prop[2] && AbilityTurn->ChainFailure)
        return false;

    CalculateHit(EffectInfo->prop[1]);

    // Recovery
    int32 heal = CalculatePct(GetState(Caster, BATTLEPET_STATE_Last_HitDealt), EffectInfo->prop[0]);
    int32 l_ModPercent = 0;

    // Modifiers Dealt / Taken
    l_ModPercent += GetState(Caster, BATTLEPET_STATE_Mod_HealingDealtPercent);
    l_ModPercent += GetState(Target, BATTLEPET_STATE_Mod_HealingTakenPercent);

    return Heal(Target, heal + CalculatePct(heal, l_ModPercent));
}

bool PetBattleAbilityEffect::HandleHeal()
{
    CalculateHit(EffectInfo->prop[1]);

    return Heal(Caster, CalculateHeal(EffectInfo->prop[0]));
}

bool PetBattleAbilityEffect::HandleCatch()
{
    int32 chance = EffectInfo->prop[0] + (GetState(Target, BATTLEPET_STATE_Internal_CaptureBoost) * EffectInfo->prop[1]);

    bool success = roll_chance_i(chance);

    PetBattleEvent event(PETBATTLE_EVENT_CATCH, Caster, success ? 0 : PETBATTLE_EVENT_FLAG_MISS, EffectInfo->id, PetBattleInstance->RoundTurn++, 0, 1);
    event.UpdateSpeed(Target, success ? 1 : 0);
    PetBattleInstance->RoundEvents.push_back(event);
    PetBattleInstance->RoundPetSpeedUpdate.push_back(std::pair<uint32, uint32>(Target, 0));

    if (success)
        PetBattleInstance->Catch(Caster, Target, EffectInfo->id);
    else
        ModState(Target, BATTLEPET_STATE_Internal_CaptureBoost, 1);

    return success;
}

bool PetBattleAbilityEffect::HandlePeriodicTrigger()
{
    // Chain Failure
    if (EffectInfo->prop[0] && AbilityTurn->ChainFailure)
        return false;

    CalculateHit(EffectInfo->prop[1]);

    return AddAura(Target, EffectInfo->prop[2], EffectInfo->prop[3]);
}

bool PetBattleAbilityEffect::HandleRampingDamage()
{
    CalculateHit(EffectInfo->prop[1]);

    int32 l_BaseDamage              = EffectInfo->prop[0];
    int32 l_DamageIncrementPerUse   = EffectInfo->prop[2];
    int32 l_MaxDamage               = EffectInfo->prop[3];

    if (GetState(Caster, BATTLEPET_STATE_Ramping_DamageID) != AbilityID)
    {
        SetState(Caster, BATTLEPET_STATE_Ramping_DamageID, AbilityID);
        SetState(Caster, BATTLEPET_STATE_Ramping_DamageUses, 0);
    }

    int32 l_CurrentUse  = GetState(Caster, BATTLEPET_STATE_Ramping_DamageUses);
    int32 l_BonusDamage = l_DamageIncrementPerUse * l_CurrentUse;

    if ((l_BaseDamage + l_BonusDamage) >= l_MaxDamage)
    {
        l_BonusDamage = l_MaxDamage - l_BaseDamage;

        if (EffectInfo->prop[4]) ///< StateToTriggerMaxPoints
            SetState(Target, EffectInfo->prop[4], 1);
    }

    ModState(Caster, BATTLEPET_STATE_Ramping_DamageUses, 1);

    /// Damage
    int32 damage = CalculateDamage(l_BaseDamage + l_BonusDamage);

    return Damage(Target, damage);
}

bool PetBattleAbilityEffect::HandleHealLastHitTaken()
{
    CalculateHit(EffectInfo->prop[1]);

    // Recovery
    int32 heal = CalculatePct(GetState(Caster, BATTLEPET_STATE_Last_HitTaken), EffectInfo->prop[0]);
    int32 l_ModPercent = 0;

    // Modifiers Dealt / Taken
    l_ModPercent += GetState(Caster, BATTLEPET_STATE_Mod_HealingDealtPercent);
    l_ModPercent += GetState(Target, BATTLEPET_STATE_Mod_HealingTakenPercent);

    return Heal(Target, heal + CalculatePct(heal, l_ModPercent));
}

bool PetBattleAbilityEffect::HandleRemoveAura()
{
    /// Data for this effect are unknown so we assume 100% unaura
    ///CalculateHit(EffectInfo->prop[0]);
    ///
    ///if (!(Flags & FailFlags))
    ///{
    ///    for (PetBattleAuraList::iterator l_It = PetBattleInstance->PetAuras.begin(); l_It != PetBattleInstance->PetAuras.end(); l_It++)
    ///    {
    ///        if (!(*l_It)->Expired && (*l_It)->TargetPetID == Target && (*l_It)->AbilityID == AbilityID)
    ///        {
    ///            /// Make aura expired
    ///            (*l_It)->Expire(PetBattleInstance);
    ///        }
    ///    }
    ///}
    ///
    ///return !(Flags & FailFlags);

    CalculateHit(100);
    return true;
}

bool PetBattleAbilityEffect::HandleModState()
{
    assert(EffectInfo->prop[0] < NUM_BATTLEPET_STATES);

    /// Min check
    if (GetState(Caster, EffectInfo->prop[0]) < EffectInfo->prop[2])
        return false;

    /// Max check
    if (GetState(Caster, EffectInfo->prop[0]) > EffectInfo->prop[3])
        return false;

    ModState(Target, EffectInfo->prop[0], EffectInfo->prop[1]);

    return true;
}

bool PetBattleAbilityEffect::HandlePositiveAura()
{
    CalculateHit(EffectInfo->prop[1]);

    return AddAura(Target, EffectInfo->prop[2], 0);
}

bool PetBattleAbilityEffect::HandleHealPetType()
{
    /// Handle only first effect of this type
    /// Don't know how to handle it properly
    /// http://wowhead.com/petability=533
    /// http://wowhead.com/petability=922
    if (EffectInfo->effectIndex != 2)
        return true;

    CalculateHit(EffectInfo->prop[1]);

    int32 l_ModPercent = CalculatePct(GetState(Caster, BATTLEPET_STATE_Stat_Power), 5);
    l_ModPercent += GetState(Caster, BATTLEPET_STATE_Mod_HealingDealtPercent);

    BattlePetSpeciesEntry const* l_BattlePetSpeciesEntry = sBattlePetSpeciesStore.LookupEntry(PetBattleInstance->Pets[Caster]->Species);
    int32 l_CasterType = 0;
    if (l_BattlePetSpeciesEntry)
        l_CasterType = l_BattlePetSpeciesEntry->type;

    uint32 l_Team = PetBattleInstance->Pets[Caster]->TeamID;

    for (uint8 l_I = 0; l_I < (MAX_PETBATTLE_SLOTS * MAX_PETBATTLE_TEAM); ++l_I)
    {
        std::shared_ptr<BattlePetInstance> l_CurrentPet = PetBattleInstance->Pets[l_I];

        if (l_CurrentPet && l_CurrentPet->TeamID == l_Team)
        {
            l_BattlePetSpeciesEntry = sBattlePetSpeciesStore.LookupEntry(l_CurrentPet->Species);
            if (l_BattlePetSpeciesEntry && l_BattlePetSpeciesEntry->type == l_CasterType)
            {
                int32 l_Heal = (EffectInfo->prop[0] / 2) + CalculatePct((EffectInfo->prop[0] / 2), l_ModPercent + GetState(l_CurrentPet->ID, BATTLEPET_STATE_Mod_HealingTakenPercent));
                Heal(l_CurrentPet->ID, l_Heal);
            }
        }
    }

    return Heal(Target, CalculateHeal(EffectInfo->prop[0]));
}

bool PetBattleAbilityEffect::HandleExtraAttackIfMoreFaster()
{
    if (PetBattleInstance->RoundFirstTeamCasting != PetBattleInstance->Pets[Caster]->TeamID)
        return false;

    CalculateHit(EffectInfo->prop[1]);

    return Damage(Target, CalculateDamage(EffectInfo->prop[0]));
}

bool PetBattleAbilityEffect::HandleHealState()
{
    if (EffectInfo->prop[3] && !GetState(Target, EffectInfo->prop[3]))
        return false;

    CalculateHit(EffectInfo->prop[1]);

    return Heal(Target, CalculateHeal(EffectInfo->prop[0]));
}

bool PetBattleAbilityEffect::HandleSplittedDamage()
{
    CalculateHit(EffectInfo->prop[1]);
    int32 damage = CalculateDamage(EffectInfo->prop[0] / Targets.size());
    return Damage(Target, damage);
}

bool PetBattleAbilityEffect::HandleDamageNonLetal()
{
    CalculateHit(EffectInfo->prop[1]);

    int32 damage = CalculateDamage(EffectInfo->prop[0]);

    if (damage >= GetHealth(Target))
        damage = GetHealth(Target) - 1;

    return Damage(Target, damage);
}

bool PetBattleAbilityEffect::HandleExtraAttackIfLessFaster()
{
    if (PetBattleInstance->RoundFirstTeamCasting == PetBattleInstance->Pets[Caster]->TeamID)
        return false;

    CalculateHit(EffectInfo->prop[1]);

    return Damage(Target, CalculateDamage(EffectInfo->prop[0]));
}

bool PetBattleAbilityEffect::HandleHealOnSpecificWeather()
{
    int32 l_RequiredState = EffectInfo->prop[2];

    if (l_RequiredState && !GetState(Caster, l_RequiredState))
        return false;

    CalculateHit(EffectInfo->prop[1]);

    int32 l_HealBase = EffectInfo->prop[0];
    int32 l_HealBonusOnStateID = EffectInfo->prop[3];

    if (l_HealBonusOnStateID && GetState(Caster, l_HealBonusOnStateID))
        l_HealBase *= 2;

    return Heal(Target, CalculateHeal(l_HealBase));
}

bool PetBattleAbilityEffect::HandleWeatherAura()
{
    // Chain Failure
    if (EffectInfo->prop[3] && AbilityTurn->ChainFailure)
        return false;

    CalculateHit(EffectInfo->prop[1]);

    if (!(Flags & FailFlags) && PetBattleInstance->WeatherAbilityId)
    {
        for (PetBattleAuraList::iterator l_It = PetBattleInstance->PetAuras.begin(); l_It != PetBattleInstance->PetAuras.end(); ++l_It)
        {
            if (!(*l_It)->Expired && (*l_It)->AbilityID == PetBattleInstance->WeatherAbilityId)
            {
                /// Make aura expired
                (*l_It)->Expire(PetBattleInstance);
            }
        }
    }

    return AddAura(Target, EffectInfo->prop[2], 0);
}

bool PetBattleAbilityEffect::HandleAuraCondAccuracyState()
{
    // Chain Failure
    if (EffectInfo->prop[4] && AbilityTurn->ChainFailure)
        return false;

    // Accuracy
    int32 accuracy = EffectInfo->prop[5];
    if ((!EffectInfo->prop[1] || GetState(Caster, EffectInfo->prop[1]) != EffectInfo->prop[0]) && (!EffectInfo->prop[3] || GetState(Target, EffectInfo->prop[3]) != EffectInfo->prop[0]))
        accuracy = 100;

    CalculateHit(accuracy);

    return AddAura(Target, EffectInfo->prop[2], 0);
}

bool PetBattleAbilityEffect::HandleCheckState()
{
    if (GetState(Target, EffectInfo->prop[0]) != EffectInfo->prop[1])
        StopChain = true;

    return true;
}

bool PetBattleAbilityEffect::HandleCancellableAura()
{
    // Accuracy
    CalculateHit(EffectInfo->prop[1]);

    return AddAura(Target, EffectInfo->prop[2], 0);
}

bool PetBattleAbilityEffect::HandleWeatherDamage()
{
    // weatherState
    if (!GetState(Target, EffectInfo->prop[2]))
        return false;

    // Accuracy
    CalculateHit(EffectInfo->prop[1]);

    // Periodic
    if (EffectInfo->prop[4])
        Flags |= PETBATTLE_EVENT_FLAG_PERIODIC;

    // Damage
    return Damage(Target, CalculateDamage(EffectInfo->prop[0]));
}

bool PetBattleAbilityEffect::HandleNegativeAura()
{
    // Chain Failure
    if (EffectInfo->prop[0] && AbilityTurn->ChainFailure)
        return false;

    // Accuracy
    CalculateHit(EffectInfo->prop[1]);

    return AddAura(Target, EffectInfo->prop[2], EffectInfo->prop[3]);
}

bool PetBattleAbilityEffect::HandlePeriodicPositiveTrigger()
{
    CalculateHit(EffectInfo->prop[1]);

    return AddAura(Target, EffectInfo->prop[2], 0);
}

bool PetBattleAbilityEffect::HandleControlAura()
{
    // Chain Failure
    if (EffectInfo->prop[0] && AbilityTurn->ChainFailure)
        return false;

    CalculateHit(EffectInfo->prop[1]);

    return AddAura(Target, EffectInfo->prop[2], 0);
}

bool PetBattleAbilityEffect::HandlePowerlessAura()
{
    if (EffectInfo->prop[5])
        ReportFailAsImmune = true;

    // Chain Failure
    if (EffectInfo->prop[4] && AbilityTurn->ChainFailure)
    {
        if (ReportFailAsImmune)
            Flags |= PETBATTLE_EVENT_FLAG_IMMUNE;
        else
            return false;
    }

    // State
    if (EffectInfo->prop[3] && GetState(Target, EffectInfo->prop[3]) == EffectInfo->prop[0])
    {
        if (ReportFailAsImmune)
            Flags |= PETBATTLE_EVENT_FLAG_IMMUNE;
        else
            return false;
    }

    CalculateHit(EffectInfo->prop[1]);

    return AddAura(Target, EffectInfo->prop[2], 0);
}

bool PetBattleAbilityEffect::HandleHealPercent()
{
    CalculateHit(EffectInfo->prop[1]);

    /// No sure if all ability should not use spell power on this effect
    /// Passive humanoid http://wowhead.com/petability=726
    if (AbilityID == 726)
    {
        int32 l_ModPercent = 1;
        int32 l_Heal = CalculatePct(GetMaxHealth(Target), EffectInfo->prop[0]);

        /// Modifiers Dealt / Taken
        l_ModPercent += GetState(Caster, BATTLEPET_STATE_Mod_HealingDealtPercent);
        l_ModPercent += GetState(Target, BATTLEPET_STATE_Mod_HealingTakenPercent);

        return l_Heal + CalculatePct(l_Heal, l_ModPercent);
    }

    // Recovery
    int32 heal = CalculateHeal(CalculatePct(GetMaxHealth(Target), EffectInfo->prop[0]));

    return Heal(Target, heal);
}

bool PetBattleAbilityEffect::HandleExtraAttack()
{
    CalculateHit(EffectInfo->prop[1]);

    if (EffectInfo->prop[2])
        Flags |= PETBATTLE_EVENT_FLAG_PERIODIC;

    int32 damage = CalculateDamage(EffectInfo->prop[0]);

    bool result = !(Flags & FailFlags);
    if (result)
        Damage(Target, damage);

    return result;
}

bool PetBattleAbilityEffect::HandleHealCasterPercentNotState()
{
    if (EffectInfo->prop[2] && !GetState(Caster, EffectInfo->prop[2]))
        return false;

    if (EffectInfo->prop[3] && !GetState(Target, EffectInfo->prop[3]))
        return false;

    // It's caster who need to be heal
    Target = Caster;

    CalculateHit(EffectInfo->prop[1]);

    int32 heal = CalculateHeal(CalculatePct(GetMaxHealth(Target), EffectInfo->prop[0]));
    return Heal(Target, heal);
}

bool PetBattleAbilityEffect::HandleDamagePercent()
{
    CalculateHit(EffectInfo->prop[1]);

    if (EffectInfo->prop[2])
        Flags |= PETBATTLE_EVENT_FLAG_PERIODIC;

    int32 damage = CalculateDamage(CalculatePct(GetMaxHealth(Target), EffectInfo->prop[0]));
    return Damage(Target, damage);
}

bool PetBattleAbilityEffect::HandleDamageStateBonus()
{
    CalculateHit(EffectInfo->prop[1]);

    int32 damage = EffectInfo->prop[0];
    if (EffectInfo->prop[3] && GetState(Target, EffectInfo->prop[3]) == EffectInfo->prop[4])
        damage += EffectInfo->prop[2];

    return Damage(Target, CalculateDamage(damage));
}

bool PetBattleAbilityEffect::HandleDamageRuthless()
{
    CalculateHit(EffectInfo->prop[1]);

    int32 damage = EffectInfo->prop[0];
    int32 l_TargetHealthPct = (GetHealth(Target) * 100) / GetMaxHealth(Target);

    if (l_TargetHealthPct < 25)
        damage += CalculatePct(damage, EffectInfo->prop[2]);

    return Damage(Target, CalculateDamage(damage));
}

bool PetBattleAbilityEffect::HandleEqualizeLife()
{
    CalculateHit(EffectInfo->prop[0]);
    if (Flags & FailFlags)
        return SetHealth(Caster, GetHealth(Caster));

    int32 life = (GetHealth(Caster) + GetHealth(Target)) / 2;
    SetHealth(Caster, life);
    SetHealth(Target, life);
    return true;
}

bool PetBattleAbilityEffect::HandleDamageCasterPercent()
{
    if (EffectInfo->prop[2])
        Target = GetActiveOpponent();

    CalculateHit(EffectInfo->prop[1]);

    int32 damage = CalculateDamage(CalculatePct(GetMaxHealth(Caster), EffectInfo->prop[0]));
    return Damage(Target, damage, true);
}

bool PetBattleAbilityEffect::HandleHealToggleAura()
{
    CalculateHit(EffectInfo->prop[1]);
    if (Flags & FailFlags)
        return SetHealth(Target, GetHealth(Target));

    if (PetBattleAura* aura = GetAura(Target, EffectInfo->triggerAbility))
    {
        aura->Expire(PetBattleInstance);

        // Heal
        return Heal(Target, CalculateHeal(EffectInfo->prop[0]));
    }
    else
    {
        // No aura, add it
        return AddAura(Target, EffectInfo->prop[2], 0);
    }
}

bool PetBattleAbilityEffect::HandleInitialization()
{
    SetState(Target, BATTLEPET_STATE_Internal_InitialHealth, GetHealth(Target));
    SetState(Target, BATTLEPET_STATE_Internal_InitialLevel, PetBattleInstance->Pets[Target]->Level);
    SetState(Target, BATTLEPET_STATE_Stat_CritChance, 5);

    uint32 petType = BATTLEPET_PETTYPE_HUMANOID;
    if (BattlePetSpeciesEntry const* targetSpeciesInfo = sBattlePetSpeciesStore.LookupEntry(PetBattleInstance->Pets[Target]->Species))
        petType = targetSpeciesInfo->type;

    static const uint32 petTypePassiveState[NUM_BATTLEPET_PETTYPES] = {
        /*BATTLEPET_PETTYPE_HUMANOID*/      BATTLEPET_STATE_Passive_Humanoid,
        /*BATTLEPET_PETTYPE_DRAGONKIN*/     BATTLEPET_STATE_Passive_Dragon,
        /*BATTLEPET_PETTYPE_FLYING*/        BATTLEPET_STATE_Passive_Flying,
        /*BATTLEPET_PETTYPE_UNDEAD*/        BATTLEPET_STATE_Passive_Undead,
        /*BATTLEPET_PETTYPE_CRITTER*/       BATTLEPET_STATE_Passive_Critter,
        /*BATTLEPET_PETTYPE_MAGIC*/         BATTLEPET_STATE_Passive_Magic,
        /*BATTLEPET_PETTYPE_ELEMENTAL*/     BATTLEPET_STATE_Passive_Elemental,
        /*BATTLEPET_PETTYPE_BEAST*/         BATTLEPET_STATE_Passive_Beast,
        /*BATTLEPET_PETTYPE_AQUATIC*/       BATTLEPET_STATE_Passive_Aquatic,
        /*BATTLEPET_PETTYPE_MECHANICAL*/    BATTLEPET_STATE_Passive_Mechanical,
    };

    SetState(Target, petTypePassiveState[petType], 1);

    return true;
}

bool PetBattleAbilityEffect::HandleResurect()
{
    SetHealth(Target, CalculatePct(GetMaxHealth(Target), EffectInfo->prop[0]));
    SetState(Target, BATTLEPET_STATE_Is_Dead, 0);
    return true;
}

bool PetBattleAbilityEffect::HandleKill()
{
    /// @TODO Figure out what to do with prop[1] & prop[3]
    CalculateHit(EffectInfo->prop[0]);

    int32 l_ImmuneStateCondition = EffectInfo->prop[2];

    if (!l_ImmuneStateCondition || !GetState(GetActiveOpponent(), l_ImmuneStateCondition))
        Kill(GetActiveOpponent());

    if (!l_ImmuneStateCondition || !GetState(Caster, l_ImmuneStateCondition))
        Kill(Caster);

    return !(Flags & FailFlags);
}

bool PetBattleAbilityEffect::HandleDamageToggleAura()
{
    CalculateHit(EffectInfo->prop[1]);
    if (Flags & FailFlags)
        return SetHealth(Target, GetHealth(Target));

    if (PetBattleAura* aura = GetAura(Caster, EffectInfo->triggerAbility))
    {
        aura->Expire(PetBattleInstance);

        // Damage
        return Damage(Target, CalculateDamage(EffectInfo->prop[0]));
    }
    else
    {
        // No aura, add it
        return AddAura(Caster, EffectInfo->prop[2], 0);
    }
}

bool PetBattleAbilityEffect::HandleHealStateToggleAura()
{
    if (PetBattleAura* aura = GetAura(Caster, EffectInfo->triggerAbility))
    {
        aura->Expire(PetBattleInstance);

        // Heal
        int32 stateValue = std::min(GetState(Caster, EffectInfo->prop[1]), EffectInfo->prop[2]);
        int32 heal = EffectInfo->prop[0] + EffectInfo->prop[3] * stateValue;
        return Heal(Target, CalculateHeal(heal));
    }
    else
    {
        // No aura, add it
        return AddAura(Caster, 0, 0);
    }
}

bool PetBattleAbilityEffect::HandleResetState()
{
    return SetState(Target, EffectInfo->prop[0], 0);
}

bool PetBattleAbilityEffect::HandleDamageHitState()
{
    if ((!EffectInfo->prop[2] || !GetState(Caster, EffectInfo->prop[2]))
        && (!EffectInfo->prop[3] || !GetState(Target, EffectInfo->prop[3])))
        CalculateHit(EffectInfo->prop[1]);
    else
        CalculateHit(200);

    return Damage(Target, CalculateDamage(EffectInfo->prop[0]));
}

bool PetBattleAbilityEffect::HandleDamageAuraToggleAura()
{
    // Aura is applied independently of damages and cannot miss
    if (PetBattleAura* aura = GetAura(Target, EffectInfo->prop[3]))
    {
        aura->Expire(PetBattleInstance);
        AddAura(Target, EffectInfo->prop[4], 0);
    }
    else
        PetBattleInstance->AddAura(Caster, Target, EffectInfo->prop[3], EffectInfo->prop[2], 0, EffectInfo->id, Flags);

    CalculateHit(EffectInfo->prop[1]);

    return Damage(Target, CalculateDamage(EffectInfo->prop[0]));
}

bool PetBattleAbilityEffect::HandleHealthConsume()
{
    int32 l_HealthConsumePct = EffectInfo->prop[0];

    /// Hotfix for http://wowhead.com/petability=758
    if (AbilityID == 758)
        l_HealthConsumePct = 50;

    return SetHealth(Target, GetHealth(Target) - CalculatePct(GetMaxHealth(Target), l_HealthConsumePct));
}

bool PetBattleAbilityEffect::HandleSwap()
{
    CalculateHit(EffectInfo->prop[0]);
    if (Flags & FailFlags)
        return false;

    std::vector<uint32> availablesPets = PetBattleInstance->Teams[PetBattleInstance->Pets[Target]->TeamID]->GetAvailablesPets();
    std::vector<uint32>::iterator iter = std::find(availablesPets.begin(), availablesPets.end(), Target);
    if (iter != availablesPets.end())
        availablesPets.erase(iter);

    if (availablesPets.empty())
        return false;

    PetBattleInstance->SwapPet(PetBattleInstance->Pets[Target]->TeamID, availablesPets[rand() % availablesPets.size()]);
    return true;
}

bool PetBattleAbilityEffect::HandleUpdatePriority()
{
    PetBattleInstance->RoundFirstTeamCasting = PetBattleInstance->GetFirstAttackingTeam();
    return true;
}

bool PetBattleAbilityEffect::HandleSetHealthPercent()
{
    return SetHealth(Target, CalculatePct(GetMaxHealth(Target), EffectInfo->prop[0]));
}

bool PetBattleAbilityEffect::HandleLockActiveAbility()
{
    std::shared_ptr<BattlePetInstance> target = PetBattleInstance->Pets[Target];
    uint32 activeAbilityId = PetBattleInstance->Teams[target->TeamID]->ActiveAbilityId;

    uint32 abilitySlot;
    for (abilitySlot = 0; abilitySlot < MAX_PETBATTLE_ABILITIES; ++abilitySlot)
        if (target->Abilities[abilitySlot] == activeAbilityId)
            break;

    if (abilitySlot == MAX_PETBATTLE_ABILITIES)
        return false;

    target->Cooldowns[abilitySlot] = EffectInfo->prop[0];
    return true;
}

bool PetBattleAbilityEffect::HandleAuraIfCasterFaster()
{
    if (EffectInfo->prop[0] && AbilityTurn->ChainFailure)
        return false;

    if (PetBattleInstance->RoundFirstTeamCasting != PetBattleInstance->Pets[Caster]->TeamID)
        return false;

    CalculateHit(EffectInfo->prop[1]);

    return AddAura(Target, EffectInfo->prop[2], 0);
}

bool PetBattleAbilityEffect::HandleCleansing()
{
    //TODO: implement prop 0: DontMiss

    for (PetBattleAuraList::iterator auraIter = PetBattleInstance->PetAuras.begin(); auraIter != PetBattleInstance->PetAuras.end(); ++auraIter)
    {
        PetBattleAura* aura = *auraIter;
        if (!aura->Expired && aura->TargetPetID == Target)
            aura->Expire(PetBattleInstance);
    }

    return true;
}

bool PetBattleAbilityEffect::HandleAuraState()
{
    if (EffectInfo->prop[5] && AbilityTurn->ChainFailure)
        return false;

    if (EffectInfo->prop[0] && GetState(Caster, EffectInfo->prop[0]) != EffectInfo->prop[1])
        return false;

    if (EffectInfo->prop[3] && GetState(Caster, EffectInfo->prop[3]) != EffectInfo->prop[4])
        return false;

    return AddAura(Target, EffectInfo->prop[2], 0);
}

bool PetBattleAbilityEffect::HandleChangeCasterState()
{
    if (EffectInfo->prop[4] && GetState(Target, EffectInfo->prop[4]) == EffectInfo->prop[5])
        return false;

    int32 value = GetState(Caster, EffectInfo->prop[0]);
    if (value < EffectInfo->prop[2] || value > EffectInfo->prop[3])
        return false;

    ModState(Caster, EffectInfo->prop[0], EffectInfo->prop[1]);

    return true;
}

struct PetBattleAbilityEffect_OrderByHealth
{
    PetBattleAbilityEffect* Effect;

    bool operator() (uint32 i, uint32 j)
    {
        return Effect->GetHealth(i) < Effect->GetHealth(j);
    }
};

bool PetBattleAbilityEffect::HandleSwapLow()
{
    CalculateHit(EffectInfo->prop[0]);
    if (Flags & FailFlags)
        return false;

    std::vector<uint32> availablesPets = PetBattleInstance->Teams[PetBattleInstance->Pets[Target]->TeamID]->GetAvailablesPets();
    std::vector<uint32>::iterator iter = std::find(availablesPets.begin(), availablesPets.end(), Target);
    if (iter != availablesPets.end())
        availablesPets.erase(iter);

    if (availablesPets.empty())
        return false;

    PetBattleAbilityEffect_OrderByHealth predicate;
    predicate.Effect = this;
    std::sort(availablesPets.begin(), availablesPets.end(), predicate);

    PetBattleInstance->SwapPet(PetBattleInstance->Pets[Target]->TeamID, availablesPets.front());
    return true;
}

bool PetBattleAbilityEffect::HandleResetAuraDuration()
{
    for (PetBattleAuraList::iterator auraIter = PetBattleInstance->PetAuras.begin(); auraIter != PetBattleInstance->PetAuras.end(); ++auraIter)
    {
        PetBattleAura* aura = *auraIter;
        if (!aura->Expired && aura->TargetPetID == Target && aura->AbilityID == EffectInfo->triggerAbility)
            aura->Duration = EffectInfo->prop[0];
    }
    return true;
}

bool PetBattleAbilityEffect::HandleSwapHigh()
{
    CalculateHit(EffectInfo->prop[0]);
    if (Flags & FailFlags)
        return false;

    std::vector<uint32> availablesPets = PetBattleInstance->Teams[PetBattleInstance->Pets[Target]->TeamID]->GetAvailablesPets();
    std::vector<uint32>::iterator iter = std::find(availablesPets.begin(), availablesPets.end(), Target);
    if (iter != availablesPets.end())
        availablesPets.erase(iter);

    if (availablesPets.empty())
        return false;

    PetBattleAbilityEffect_OrderByHealth predicate;
    predicate.Effect = this;
    std::sort(availablesPets.begin(), availablesPets.end(), predicate);

    PetBattleInstance->SwapPet(PetBattleInstance->Pets[Target]->TeamID, availablesPets.back());
    return true;
}

bool PetBattleAbilityEffect::HandleRedirectionAura()
{
    if (EffectInfo->prop[0] && AbilityTurn->ChainFailure)
        return false;

    CalculateHit(EffectInfo->prop[1]);

    // remove all wall auras
    if (uint32 wallAuraId = GetState(Target, BATTLEPET_STATE_Special_ObjectRedirectionAuraID))
        if (PetBattleAura* aura = GetAura(Target, wallAuraId))
            aura->Expire(PetBattleInstance);

    return AddAura(Target, 0, 0);
}

bool PetBattleAbilityEffect::HandleCheckFailure()
{
    if (!AbilityTurn->ChainFailure)
        StopChain = true;

    return true;
}

bool PetBattleAbilityEffect::HandleTryRevive()
{
    if (GetHealth(Target) <= 0)
        return false;

    if (EffectInfo->prop[1] && GetState(Caster, EffectInfo->prop[1]))
        Flags |= PETBATTLE_EVENT_FLAG_IMMUNE;

    if (EffectInfo->prop[2] && GetState(Target, EffectInfo->prop[2]))
        Flags |= PETBATTLE_EVENT_FLAG_IMMUNE;

    CalculateHit(EffectInfo->prop[0]);

    return SetState(Target, BATTLEPET_STATE_Is_Dead, 0);
}

bool PetBattleAbilityEffect::HandleDamagePercentTaken()
{
    CalculateHit(EffectInfo->prop[1]);

    int32 damage = CalculateDamage(CalculatePct(GetState(Caster, BATTLEPET_STATE_Last_HitTaken), EffectInfo->prop[0]));
    return Damage(Target, damage);
}

bool PetBattleAbilityEffect::HandleDamageRange()
{
    CalculateHit(EffectInfo->prop[1]);

    int32 l_BaseDamage = urand(EffectInfo->prop[0], EffectInfo->prop[2]);

    return Damage(Target, CalculateDamage(l_BaseDamage));
}

bool PetBattleAbilityEffect::HandleDamageWithBonus()
{
    CalculateHit(EffectInfo->prop[1]);

    uint32 l_Damage = CalculateDamage(EffectInfo->prop[0]);

    if (EffectInfo->prop[3] && GetState(Caster, EffectInfo->prop[3]))
        l_Damage += CalculateDamage(EffectInfo->prop[2]);

    return Damage(Target, l_Damage);
}
