////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef SC_SCRIPTLOADER_H
#define SC_SCRIPTLOADER_H

void AddScripts();
void AddExampleScripts();
void AddSpellScripts();
#ifndef CROSS
void AddCommandScripts();
#endif /* not CROSS */
void AddWorldScripts();
#ifdef CROSS
void AddCommandScripts();
#endif /* CROSS */
void AddEasternKingdomsScripts();
void AddKalimdorScripts();
void AddOutlandScripts();
void AddNorthrendScripts();
void AddPandarieScripts();
void AddDraenorScripts();
void AddBattlegroundScripts();
void AddOutdoorPvPScripts();
void AddCustomScripts();
#ifndef CROSS
void AddBattlePayScripts();
#endif /* not CROSS */

#endif
