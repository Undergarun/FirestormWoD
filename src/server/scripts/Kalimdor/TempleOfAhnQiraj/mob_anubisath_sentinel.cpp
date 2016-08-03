////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: mob_anubisath_sentinel
SD%Complete: 95
SDComment: Shadow storm is not properly implemented in core it should only target ppl outside of melee range.
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "WorldPacket.h"

#include "Item.h"
#include "Player.h"
#include "Spell.h"

#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"

#define SPELL_MENDING_BUFF      2147

#define SPELL_KNOCK_BUFF        21737
#define SPELL_KNOCK             25778
#define SPELL_MANAB_BUFF        812
#define SPELL_MANAB             25779

#define SPELL_REFLECTAF_BUFF    13022
#define SPELL_REFLECTSFr_BUFF   19595
#define SPELL_THORNS_BUFF       25777

#define SPELL_THUNDER_BUFF      2834
#define SPELL_THUNDER           8732

#define SPELL_MSTRIKE_BUFF      9347
#define SPELL_MSTRIKE           24573

#define SPELL_STORM_BUFF        2148
#define SPELL_STORM             26546

class mob_anubisath_sentinel : public CreatureScript
{
public:
    mob_anubisath_sentinel() : CreatureScript("mob_anubisath_sentinel") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new aqsentinelAI(creature);
    }

    struct aqsentinelAI : public ScriptedAI
    {
        uint32 ability;
        int abselected;

        void selectAbility(int asel)
        {
            switch (asel)
            {
            case 0: ability = SPELL_MENDING_BUFF; break;
            case 1: ability = SPELL_KNOCK_BUFF; break;
            case 2: ability = SPELL_MANAB_BUFF; break;
            case 3: ability = SPELL_REFLECTAF_BUFF; break;
            case 4: ability = SPELL_REFLECTSFr_BUFF; break;
            case 5: ability = SPELL_THORNS_BUFF; break;
            case 6: ability = SPELL_THUNDER_BUFF; break;
            case 7: ability = SPELL_MSTRIKE_BUFF; break;
            case 8: ability = SPELL_STORM_BUFF; break;
            }
        }

        aqsentinelAI(Creature* creature) : ScriptedAI(creature)
        {
            ClearBuddyList();
            abselected = 0;                                     // just initialization of variable
        }

        uint64 NearbyGUID[3];

        void ClearBuddyList()
        {
            NearbyGUID[0] = NearbyGUID[1] = NearbyGUID[2] = 0;
        }

        void AddBuddyToList(uint64 CreatureGUID)
        {
            if (CreatureGUID == me->GetGUID())
                return;

            for (int i = 0; i < 3; ++i)
            {
                if (NearbyGUID[i] == CreatureGUID)
                    return;
                if (!NearbyGUID[i])
                {
                    NearbyGUID[i] = CreatureGUID;
                    return;
                }
            }
        }

        void GiveBuddyMyList(Creature* c)
        {
            aqsentinelAI* cai = CAST_AI(aqsentinelAI, (c)->AI());
            if (cai)
            {
                for (int i = 0; i < 3; ++i)
                    if (NearbyGUID[i] && NearbyGUID[i] != c->GetGUID())
                        cai->AddBuddyToList(NearbyGUID[i]);
                cai->AddBuddyToList(me->GetGUID());
            }
        }

        void SendMyListToBuddies()
        {
            for (int i = 0; i < 3; ++i)
                if (Creature* pNearby = Unit::GetCreature(*me, NearbyGUID[i]))
                    GiveBuddyMyList(pNearby);
        }

        void CallBuddiesToAttack(Unit* who)
        {
            for (int i = 0; i < 3; ++i)
            {
                Creature* c = Unit::GetCreature(*me, NearbyGUID[i]);
                if (c)
                {
                    if (!c->isInCombat())
                    {
                        c->SetNoCallAssistance(true);
                        if (c->AI())
                            c->AI()->AttackStart(who);
                    }
                }
            }
        }

        void AddSentinelsNear(Unit* /*nears*/)
        {
            std::list<Creature*> assistList;
            me->GetCreatureListWithEntryInGrid(assistList, 15264, 70.0f);

            if (assistList.empty())
                return;

            for (std::list<Creature*>::const_iterator iter = assistList.begin(); iter != assistList.end(); ++iter)
                AddBuddyToList((*iter)->GetGUID());
        }

        int pickAbilityRandom(bool *chosenAbilities)
        {
            for (int t = 0; t < 2; ++t)
            {
                for (int i = !t ? (rand() % 9) : 0; i < 9; ++i)
                {
                    if (!chosenAbilities[i])
                    {
                        chosenAbilities[i] = true;
                        return i;
                    }
                }
            }
            return 0;                                           // should never happen
        }

        void GetOtherSentinels(Unit* who)
        {
            bool *chosenAbilities = new bool[9];
            memset(chosenAbilities, 0, 9 * sizeof(bool));
            selectAbility(pickAbilityRandom(chosenAbilities));

            ClearBuddyList();
            AddSentinelsNear(me);
            int bli;
            for (bli = 0; bli < 3; ++bli)
            {
                if (!NearbyGUID[bli])
                    break;

                Creature* pNearby = Unit::GetCreature(*me, NearbyGUID[bli]);
                if (!pNearby)
                    break;

                AddSentinelsNear(pNearby);

                aqsentinelAI* l_AI = CAST_AI(aqsentinelAI, pNearby->AI());
                if (l_AI != nullptr)
                {
                    l_AI->gatherOthersWhenAggro = false;
                    l_AI->selectAbility(pickAbilityRandom(chosenAbilities));
                }
            }

            SendMyListToBuddies();
            CallBuddiesToAttack(who);

            delete[] chosenAbilities;
        }

        bool gatherOthersWhenAggro;

        void Reset()
        {
            if (!me->isDead())
            {
                for (int i = 0; i < 3; ++i)
                {
                    if (!NearbyGUID[i])
                        continue;
                    if (Creature* pNearby = Unit::GetCreature(*me, NearbyGUID[i]))
                    {
                        if (pNearby->isDead())
                            pNearby->Respawn();
                    }
                }
            }
            ClearBuddyList();
            gatherOthersWhenAggro = true;
        }

        void GainSentinelAbility(uint32 id)
        {
            me->AddAura(id, me);
        }

        void EnterCombat(Unit* who)
        {
            if (gatherOthersWhenAggro)
                GetOtherSentinels(who);

            GainSentinelAbility(ability);
            DoZoneInCombat();
        }

        void JustDied(Unit* /*killer*/)
        {
            for (int l_Ni = 0; l_Ni < 3; ++l_Ni)
            {
                Creature* l_Sent = Unit::GetCreature(*me, NearbyGUID[l_Ni]);
                if (!l_Sent)
                    continue;

                if (l_Sent->isDead())
                    continue;

                l_Sent->ModifyHealth(int32(l_Sent->CountPctFromMaxHealth(50)));

                aqsentinelAI* l_AI = CAST_AI(aqsentinelAI, l_Sent->AI());
                if (l_AI != nullptr)
                    l_AI->GainSentinelAbility(ability);
            }
        }
    };
};

#ifndef __clang_analyzer__
void AddSC_mob_anubisath_sentinel()
{
    new mob_anubisath_sentinel();
}
#endif
