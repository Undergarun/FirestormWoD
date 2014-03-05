#ifndef WDTFILE_H
#define WDTFILE_H

#include "mpqfile.h"
#include "wmo.h"
#include <string>
#include "stdlib.h"

class ADTFile;

class WDTFile
{
public:
    WDTFile(char* file_name, char* file_name1);
    ~WDTFile(void);
    bool init(char* map_id, unsigned int mapID);

    string* gWmoInstansName;
    int gnWMO;

    ADTFile* GetMap(int x, int z);

private:
    MPQFile WDT;
    string filename;
	

    struct adtData{
        uint32 flag;
        uint32 data1;
    } adt_list[64][64];	
};

#endif
