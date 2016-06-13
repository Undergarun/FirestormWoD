////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Guid.h"

Guid128 Guid64To128(const uint64 & p_64Guid)
{
    if (p_64Guid == 0)
        return Guid128(0, 0);

    Guid128Type l_Type  = GUID_TYPE_NONE;
    uint64 l_Low        = GUID_LOPART(p_64Guid);
    uint32 l_Mid        = GUID_ENPART(p_64Guid);
    uint32 l_RealmID    = 1;

    switch (GUID_HIPART(p_64Guid))
    {
        case HIGHGUID_ITEM:               l_Type = GUID_TYPE_ITEM;              break;
        case HIGHGUID_PLAYER:             l_Type = GUID_TYPE_PLAYER;            break;
        case HIGHGUID_GAMEOBJECT:         l_Type = GUID_TYPE_GAMEOBJECT;        break;
        case HIGHGUID_TRANSPORT:          l_Type = GUID_TYPE_TRANSPORT;         break;
        case HIGHGUID_UNIT:               l_Type = GUID_TYPE_UNIT;              break;
        case HIGHGUID_PET:                l_Type = GUID_TYPE_PET;               break;
        case HIGHGUID_VEHICLE:            l_Type = GUID_TYPE_VEHICLE;           break;
        case HIGHGUID_DYNAMICOBJECT:      l_Type = GUID_TYPE_DYNAMIC_OBJECT;    break;
        case HIGHGUID_CORPSE:             l_Type = GUID_TYPE_CORPSE;            break;
        case HIGHGUID_AREATRIGGER:        l_Type = GUID_TYPE_AREATRIGGER;       break;
        case HIGHGUID_TYPE_BATTLEGROUND:  l_Type = GUID_TYPE_BATTLEGROUND;      break;
        case HIGHGUID_MO_TRANSPORT:       l_Type = GUID_TYPE_MO_TRANSPORT;      break;
        case HIGHGUID_GROUP:              l_Type = GUID_TYPE_GROUP;             break;
        case HIGHGUID_GUILD:              l_Type = GUID_TYPE_GUILD;             break;
        case HIGHGUID_INSTANCE_SAVE:      l_Type = GUID_TYPE_INSTANCE_SAVE;     break;
        case HIGHGUID_LOOT:               l_Type = GUID_TYPE_LOOT;              break;
        case HIGHGUID_SCENEOBJECT:        l_Type = GUID_TYPE_SCENE_OBJECT;      break;
        case HIGHGUID_BNET_ACCOUNT:       l_Type = GUID_TYPE_BNET_ACC;          break;
        case HIGHGUID_WOW_ACCOUNT:        l_Type = GUID_TYPE_WOW_ACC;           break;
        case HIGHGUID_VIGNETTE:           l_Type = GUID_TYPE_VIGNETTE;          break;
        case HIGHGUID_BATTLE_PET:         l_Type = GUID_TYPE_BATTLE_PET;        break;
    }

    if (l_Type == GUID_TYPE_BNET_ACC || l_Type == GUID_TYPE_WOW_ACC)
        l_RealmID = 0;

    return Guid128(l_Type, l_RealmID, l_Mid, l_Low);
}

uint64 Guid128To64(const Guid128 & p_128Guid)
{
    uint32 l_Type;
    uint32 l_Low = p_128Guid.GetLow();
    uint32 l_Mid = p_128Guid.GetMid();

    switch (p_128Guid.GetType())
    {
        case GUID_TYPE_ITEM:               l_Type = HIGHGUID_ITEM;              break;
        case GUID_TYPE_PLAYER:             l_Type = HIGHGUID_PLAYER;            break;
        case GUID_TYPE_GAMEOBJECT:         l_Type = HIGHGUID_GAMEOBJECT;        break;
        case GUID_TYPE_TRANSPORT:          l_Type = HIGHGUID_TRANSPORT;         break;
        case GUID_TYPE_UNIT:               l_Type = HIGHGUID_UNIT;              break;
        case GUID_TYPE_PET:                l_Type = HIGHGUID_PET;               break;
        case GUID_TYPE_VEHICLE:            l_Type = HIGHGUID_VEHICLE;           break;
        case GUID_TYPE_DYNAMIC_OBJECT:     l_Type = HIGHGUID_DYNAMICOBJECT;     break;
        case GUID_TYPE_CORPSE:             l_Type = HIGHGUID_CORPSE;            break;
        case GUID_TYPE_AREATRIGGER:        l_Type = HIGHGUID_AREATRIGGER;       break;
        case GUID_TYPE_BATTLEGROUND:       l_Type = HIGHGUID_TYPE_BATTLEGROUND; break;
        case GUID_TYPE_MO_TRANSPORT:       l_Type = HIGHGUID_MO_TRANSPORT;      break;
        case GUID_TYPE_GROUP:              l_Type = HIGHGUID_GROUP;             break;
        case GUID_TYPE_GUILD:              l_Type = HIGHGUID_GUILD;             break;
        case GUID_TYPE_INSTANCE_SAVE:      l_Type = HIGHGUID_INSTANCE_SAVE;     break;
        case GUID_TYPE_LOOT:               l_Type = HIGHGUID_LOOT;              break;
        case GUID_TYPE_SCENE_OBJECT:       l_Type = HIGHGUID_SCENEOBJECT;       break;
        case GUID_TYPE_BNET_ACC:           l_Type = HIGHGUID_BNET_ACCOUNT;      break;
        case GUID_TYPE_WOW_ACC:            l_Type = HIGHGUID_WOW_ACCOUNT;       break;
        case GUID_TYPE_VIGNETTE:           l_Type = HIGHGUID_VIGNETTE;          break;
        case GUID_TYPE_BATTLE_PET:         l_Type = HIGHGUID_BATTLE_PET;        break;

        default:
            l_Type = 0;
            break;
    }

    return MAKE_NEW_GUID(l_Low, l_Mid, l_Type);
}
