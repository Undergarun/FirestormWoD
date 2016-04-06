////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
# ifndef _PACKET_STRUCTURES_H_
#define _PACKET_STRUCTURES_H_

#include "Common.h"

template<typename T> struct Optional
{
	T Data;
	bool HaveData;
};

struct ServerSpec
{
	uint32 ID;
	uint32 Type;
	uint32 Server;
};

#define REPORT_TYPE uint32
#define READ_ITEM_FAILURE uint32
#define BATTLEPETRESULT uint32
#define JAM_PET_BATTLE_EFFECT_TARGET_EX uint32
#define LOOT_LIST_ITEM_TYPE uint32
#define LOOT_ITEM_UI_TYPE uint32
#define RESET_FAILED_REASON uint32
#define LFG_TELEPORT_RESULT uint32
#define SPELLWEIGHT_TOKEN_TYPES uint32
#define TAXIREPLY uint32
#define TAXISTATUS uint32
#define CLIENT_SUSPEND_REASON uint32
#define JAM_ACCOUNTOBJECT_SET_CHEATS uint32
#define RECRUIT_A_FRIEND_RESULT uint32
#define JAM_RAF_EMAIL_ENABLED_RESPONSE uint32
#define BATTLENET_CHALLENGE_RESULT uint32
#define TRADE_STATUS uint32
#define TRANSFER_ABORT uint32
#define DIFFICULTY_COOLDOWN_REASON uint32
#define SET_PLAYER_DIFFICULTY_RESULTS uint32
#define JAM_PRODUCT_CHOICE_RESULT uint32
#define JamAddBattlenetFriendError uint32
#define ARENA_ERROR_TYPE uint32
#define PETITION_ERROR uint32
#define ATTACKSWINGERR uint32
#define IMMIGRATION_STATE uint32
#define TOAST_TYPE uint32
#define UNDELETE_CHARACTER_RESULT uint32
#define UPDATE_COLLISION_HEIGHT_REASON uint32
#define JamCliBattlePetSetFlagControlType uint32
#define JAM_CLI_COMPLAINT_TYPE uint32
#define PROGRESSIVE_EVENT_CHEAT uint32
#define TUTORIAL_ACTION uint32

#include "_PacketStructures.h"

#endif _PACKET_STRUCTURES_H_