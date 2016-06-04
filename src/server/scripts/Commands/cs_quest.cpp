////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
Name: quest_commandscript
%Complete: 100
Comment: All quest related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Chat.h"

class quest_commandscript: public CommandScript
{
public:
    quest_commandscript() : CommandScript("quest_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand questCommandTable[] =
        {
            { "add",            SEC_ADMINISTRATOR,  false, &HandleQuestAdd,                    "", NULL },
            { "complete",       SEC_ADMINISTRATOR,  false, &HandleQuestComplete,               "", NULL },
            { "remove",         SEC_ADMINISTRATOR,  false, &HandleQuestRemove,                 "", NULL },
            { "reward",         SEC_ADMINISTRATOR,  false, &HandleQuestReward,                 "", NULL },
            { NULL,             SEC_PLAYER,         false, NULL,                               "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "quest",          SEC_ADMINISTRATOR,  false, NULL,                  "", questCommandTable },
            { NULL,             SEC_PLAYER,         false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    static bool HandleQuestAdd(ChatHandler* handler, const char* args)
    {
        Player* player = handler->getSelectedPlayer();
        if (!player)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // .addquest #entry'
        // number or [name] Shift-click form |color|Hquest:quest_id:quest_level|h[name]|h|r
        char* cId = handler->extractKeyFromLink((char*)args, "Hquest");
        if (!cId)
            return false;

        uint32 entry = atol(cId);

        Quest const* quest = sObjectMgr->GetQuestTemplate(entry);

        if (!quest)
        {
            handler->PSendSysMessage(LANG_COMMAND_QUEST_NOTFOUND, entry);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check item starting quest (it can work incorrectly if added without item in inventory)
        ItemTemplateContainer const* itc = sObjectMgr->GetItemTemplateStore();
        ItemTemplateContainer::const_iterator result = find_if(itc->begin(), itc->end(), Finder<uint32, ItemTemplate>(entry, &ItemTemplate::StartQuest));

        if (result != itc->end())
        {
            handler->PSendSysMessage(LANG_COMMAND_QUEST_STARTFROMITEM, entry, result->second.ItemId);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // ok, normal (creature/GO starting) quest
        if (player->CanAddQuest(quest, true))
        {
            player->AddQuest(quest, NULL);

            if (player->CanCompleteQuest(entry))
                player->CompleteQuest(entry);
        }

        return true;
    }

    static bool HandleQuestRemove(ChatHandler* handler, const char* args)
    {
        Player* player = handler->getSelectedPlayer();
        if (!player)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // .removequest #entry'
        // number or [name] Shift-click form |color|Hquest:quest_id:quest_level|h[name]|h|r
        char* cId = handler->extractKeyFromLink((char*)args, "Hquest");
        if (!cId)
            return false;

        uint32 entry = atol(cId);

        Quest const* quest = sObjectMgr->GetQuestTemplate(entry);

        if (!quest)
        {
            handler->PSendSysMessage(LANG_COMMAND_QUEST_NOTFOUND, entry);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // remove all quest entries for 'entry' from quest log
        for (uint8 slot = 0; slot < MAX_QUEST_LOG_SIZE; ++slot)
        {
            uint32 logQuest = player->GetQuestSlotQuestId(slot);
            if (logQuest == entry)
            {
                player->SetQuestSlot(slot, 0);

                // we ignore unequippable quest items in this case, its' still be equipped
                player->TakeQuestSourceItem(logQuest, false);
            }
        }

        player->RemoveActiveQuest(entry);
        player->RemoveRewardedQuest(entry);

        handler->SendSysMessage(LANG_COMMAND_QUEST_REMOVED);
        return true;
    }

    static bool HandleQuestComplete(ChatHandler* handler, const char* args)
    {
        Player* l_Player = handler->getSelectedPlayer();
        if (!l_Player)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // .quest complete #entry
        // number or [name] Shift-click form |color|Hquest:quest_id:quest_level|h[name]|h|r
        char* cId = handler->extractKeyFromLink((char*)args, "Hquest");
        if (!cId)
            return false;

        uint32 entry = atol(cId);

        Quest const* quest = sObjectMgr->GetQuestTemplate(entry);

        // If player doesn't have the quest
        if (!quest || l_Player->GetQuestStatus(entry) == QUEST_STATUS_NONE)
        {
            handler->PSendSysMessage(LANG_COMMAND_QUEST_NOTFOUND, entry);
            handler->SetSentErrorMessage(true);
            return false;
        }

        for (QuestObjective l_Objective : quest->QuestObjectives)
        {
            if (l_Objective.Type == QUEST_OBJECTIVE_TYPE_ITEM)
            {
                uint32 id = l_Objective.ObjectID;
                uint32 count = l_Objective.Amount;

                if (!id || !count)
                    continue;

                uint32 curItemCount = l_Player->GetItemCount(id, true);

                ItemPosCountVec dest;
                uint8 msg = l_Player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, id, count - curItemCount);
                if (msg == EQUIP_ERR_OK)
                {
                    Item* item = l_Player->StoreNewItem(dest, id, true);
                    l_Player->SendNewItem(item, count - curItemCount, true, false);
                }
            }
            else if (l_Objective.Type == QUEST_OBJECTIVE_TYPE_NPC)
            {
                int32 creature = l_Objective.ObjectID;
                uint32 creaturecount = l_Objective.Amount;

                if (CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(creature))
                {
                    for (uint16 z = 0; z < creaturecount; ++z)
                        l_Player->KilledMonster(cInfo, 0);
                }
            }
            else if (l_Objective.Type == QUEST_OBJECTIVE_TYPE_GO)
            {
                for (uint16 z = 0; z < l_Objective.Amount; ++z)
                    l_Player->CastedCreatureOrGO(l_Objective.ObjectID, 0, 0);
            }
            else if (l_Objective.Type == QUEST_OBJECTIVE_TYPE_SPELL)
            {
                /// @TODO
            }
            else if (l_Objective.Type == QUEST_OBJECTIVE_TYPE_CURRENCY)
            {
                if (!l_Objective.ObjectID || !l_Objective.Amount)
                    continue;

                l_Player->ModifyCurrency(l_Objective.ObjectID, l_Objective.Amount);
            }
            else if (l_Objective.Type == QUEST_OBJECTIVE_TYPE_FACTION_REP || l_Objective.Type == QUEST_OBJECTIVE_TYPE_FACTION_REP2)
            {
                if (l_Player->GetReputationMgr().GetReputation(l_Objective.ObjectID) < l_Objective.Amount)
                {
                    if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(l_Objective.ObjectID))
                        l_Player->GetReputationMgr().SetReputation(factionEntry, l_Objective.Amount);
                }
            }
            else if (l_Objective.Type == QUEST_OBJECTIVE_TYPE_MONEY)
            {
                l_Player->ModifyMoney(l_Objective.Amount);
            }
            else if (l_Objective.Type == QUEST_OBJECTIVE_TYPE_CRITERIA_TREE)
            {
                l_Player->QuestObjectiveSatisfy(l_Objective.ObjectID, l_Objective.Amount, l_Objective.Type);
            }
        }

        l_Player->CompleteQuest(entry);
        return true;
    }

    static bool HandleQuestReward(ChatHandler* handler, char const* args)
    {
        Player* player = handler->getSelectedPlayer();
        if (!player)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // .quest reward #entry
        // number or [name] Shift-click form |color|Hquest:quest_id:quest_level|h[name]|h|r
        char* cId = handler->extractKeyFromLink((char*)args, "Hquest");
        if (!cId)
            return false;

        uint32 entry = atol(cId);

        Quest const* quest = sObjectMgr->GetQuestTemplate(entry);

        // If player doesn't have the quest
        if (!quest || player->GetQuestStatus(entry) != QUEST_STATUS_COMPLETE)
        {
            handler->PSendSysMessage(LANG_COMMAND_QUEST_NOTFOUND, entry);
            handler->SetSentErrorMessage(true);
            return false;
        }

        player->RewardQuest(quest, 0, player);
        return true;
    }
};

#ifndef __clang_analyzer__
void AddSC_quest_commandscript()
{
    new quest_commandscript();
}
#endif
