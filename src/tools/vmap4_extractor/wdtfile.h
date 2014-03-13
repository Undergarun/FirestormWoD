/*
 * Copyright (C) 2005-2013 MaNGOS <http://www.getmangos.com/>
 * Copyright (C) 2008-2013 Trinity <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
 
#ifndef WDTFILE_H
#define WDTFILE_H

#include "mpqfile.h"
#include "wmo.h"
#include <string>
#include "stdlib.h"

class ADTFile;

class wdt_MAIN{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
public:
    uint32 size;

    bool   prepareLoadedData();
};

class WDTFile
{
private:
    MPQFile WDT;
    string filename;
public:
    WDTFile(char* file_name, char* file_name1);
    ~WDTFile(void);
    bool init(char* map_id, unsigned int mapID);

    struct adtData{
        uint32 flag;
        uint32 data1;
    } adt_list[64][64];

    string* gWmoInstansName;
    int gnWMO;

    ADTFile* GetMap_obj0(int x, int z);
    ADTFile* GetMap_obj1(int x, int z);
};

#endif
