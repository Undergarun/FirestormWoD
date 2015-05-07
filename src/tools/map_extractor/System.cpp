#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <deque>
#include <list>
#include <cstdlib>
#include <cstring>

#ifdef _WIN32
#include "direct.h"
#else
#include <sys/stat.h>
#include <unistd.h>
#define ERROR_PATH_NOT_FOUND ERROR_FILE_NOT_FOUND
#endif

#include "dbcfile.h"

#include "adt.h"
#include "wdt.h"
#include <fcntl.h>

#if defined( __GNUC__ )
    #define _open   open
    #define _close close
    #ifndef O_BINARY
        #define O_BINARY 0
    #endif
#else
    #include <io.h>
#endif

#ifdef O_LARGEFILE
    #define OPEN_FLAGS (O_RDONLY | O_BINARY | O_LARGEFILE)
#else
    #define OPEN_FLAGS (O_RDONLY | O_BINARY)
#endif

typedef struct
{
    char FolderName[64];
    uint32 ID;
} Map;

Map *gMaps;
uint16 *gAreasBits;
uint16 *gLiquidType;
uint32 gMaxAreaID = 0;

// This option allow limit minimum height to some value (Allow save some memory)
bool  CONF_allow_height_limit = true;
float CONF_use_minHeight = -500.0f;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static char const* MAP_MAGIC         = "MAPS";
static char const* MAP_VERSION_MAGIC = "v1.3";
static char const* MAP_AREA_MAGIC    = "AREA";
static char const* MAP_HEIGHT_MAGIC  = "MHGT";
static char const* MAP_LIQUID_MAGIC  = "MLIQ";

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct MapFileHeader
{
    uint32 MapMagic;
    uint32 VersionMagic;
    uint32 BuildMagic;
    uint32 AreaMapOffset;
    uint32 AreaMapSize;
    uint32 HeightMapOffset;
    uint32 HeightMapSize;
    uint32 LiquidMapOffset;
    uint32 LiquidMapSize;
    uint32 HolesOffset;
    uint32 HolesSize;
};
struct MapAreaHeader
{
    uint32 FourCC;
    uint16 Flags;
    uint16 GridArea;
};
struct MapHeightHeader
{
    uint32 FourCC;
    uint32 Flags;
    float  GridHeight;
    float  GridMaxHeight;
};
struct MapLiquidHeader
{
    uint32 FourCC;
    uint16 Flags;
    uint16 LiquidType;
    uint8  OffsetX;
    uint8  OffsetY;
    uint8  Width;
    uint8  Height;
    float  LiquidLevel;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define MAP_AREA_NO_AREA      0x0001

#define MAP_HEIGHT_NO_HEIGHT  0x0001
#define MAP_HEIGHT_AS_INT16   0x0002
#define MAP_HEIGHT_AS_INT8    0x0004

#define MAP_LIQUID_TYPE_NO_WATER    0x00
#define MAP_LIQUID_TYPE_WATER       0x01
#define MAP_LIQUID_TYPE_OCEAN       0x02
#define MAP_LIQUID_TYPE_MAGMA       0x04
#define MAP_LIQUID_TYPE_SLIME       0x08

#define MAP_LIQUID_TYPE_DARK_WATER  0x10
#define MAP_LIQUID_TYPE_WMO_WATER   0x20

#define MAP_LIQUID_NO_TYPE    0x0001
#define MAP_LIQUID_NO_HEIGHT  0x0002

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

float gMapV8[ADT_GRID_SIZE][ADT_GRID_SIZE];
float gMapV9[ADT_GRID_SIZE + 1][ADT_GRID_SIZE + 1];
float gMapLiquidHeight[ADT_GRID_SIZE + 1][ADT_GRID_SIZE + 1];

uint16 gMapLiquidEntry[ADT_CELLS_PER_GRID][ADT_CELLS_PER_GRID];
uint16 gMapHoles[ADT_CELLS_PER_GRID][ADT_CELLS_PER_GRID];
uint16 gMapAreaFlags[ADT_CELLS_PER_GRID][ADT_CELLS_PER_GRID];

uint8 gMapLiquidFlags[ADT_CELLS_PER_GRID][ADT_CELLS_PER_GRID];

bool  gMapLiquidShow[ADT_GRID_SIZE][ADT_GRID_SIZE];

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Fill map headers
void FillMapHeaders(MapFileHeader & p_MapHeader, MapAreaHeader & l_AreaHeader, MapHeightHeader & p_HeightHeader, MapLiquidHeader & p_LiquidHeader, bool p_HasHoles)
{
    bool l_FullAreaData = false;
    uint32 l_FirstAreaFlag = gMapAreaFlags[0][0];

    for (int l_Y = 0; l_Y < ADT_CELLS_PER_GRID; l_Y++)
    {
        for (int l_X = 0; l_X < ADT_CELLS_PER_GRID; l_X++)
        {
            if (gMapAreaFlags[l_Y][l_X] != l_FirstAreaFlag)
            {
                l_FullAreaData = true;
                break;
            }
        }
    }

    p_MapHeader.AreaMapOffset   = sizeof(p_MapHeader);
    p_MapHeader.AreaMapSize     = sizeof(MapAreaHeader);

    l_AreaHeader.FourCC = *(uint32 const*)MAP_AREA_MAGIC;
    l_AreaHeader.Flags  = 0;

    if (l_FullAreaData)
    {
        l_AreaHeader.GridArea = 0;
        p_MapHeader.AreaMapSize += sizeof(gMapAreaFlags);
    }
    else
    {
        l_AreaHeader.Flags |= MAP_AREA_NO_AREA;
        l_AreaHeader.GridArea = (uint16)l_FirstAreaFlag;
    }

    //////////////////////////////////////////////////////////////////////////
    // Try pack height data
    //////////////////////////////////////////////////////////////////////////

    float l_MaxHeight = -20000;
    float l_MinHeight = 20000;

    for (int l_Y = 0; l_Y < ADT_GRID_SIZE; l_Y++)
    {
        for (int l_X = 0; l_X < ADT_GRID_SIZE; l_X++)
        {
            float l_Height = gMapV8[l_Y][l_X];

            if (l_MaxHeight < l_Height) 
                l_MaxHeight = l_Height;

            if (l_MinHeight > l_Height) 
                l_MinHeight = l_Height;
        }
    }
    for (int l_Y = 0; l_Y <= ADT_GRID_SIZE; l_Y++)
    {
        for (int l_X = 0; l_X <= ADT_GRID_SIZE; l_X++)
        {
            float l_Height = gMapV9[l_Y][l_X];

            if (l_MaxHeight < l_Height) 
                l_MaxHeight = l_Height;

            if (l_MinHeight > l_Height) 
                l_MinHeight = l_Height;
        }
    }

    /// Check for allow limit minimum height (not store height in deep ochean - allow save some memory)
    if (CONF_allow_height_limit && l_MinHeight < CONF_use_minHeight)
    {
        for (int l_Y = 0; l_Y < ADT_GRID_SIZE; l_Y++)
            for (int l_X = 0; l_X < ADT_GRID_SIZE; l_X++)
                if (gMapV8[l_Y][l_X] < CONF_use_minHeight)
                    gMapV8[l_Y][l_X] = CONF_use_minHeight;

        for (int l_Y = 0; l_Y <= ADT_GRID_SIZE; l_Y++)
            for (int l_X = 0; l_X <= ADT_GRID_SIZE; l_X++)
                if (gMapV9[l_Y][l_X] < CONF_use_minHeight)
                    gMapV9[l_Y][l_X] = CONF_use_minHeight;

        if (l_MinHeight < CONF_use_minHeight)
            l_MinHeight = CONF_use_minHeight;
        if (l_MaxHeight < CONF_use_minHeight)
            l_MaxHeight = CONF_use_minHeight;
    }

    p_MapHeader.HeightMapOffset = p_MapHeader.AreaMapOffset + p_MapHeader.AreaMapSize;
    p_MapHeader.HeightMapSize   = sizeof(MapHeightHeader);

    p_HeightHeader.FourCC           = *(uint32 const*)MAP_HEIGHT_MAGIC;
    p_HeightHeader.Flags            = 0;
    p_HeightHeader.GridHeight       = l_MinHeight;
    p_HeightHeader.GridMaxHeight    = l_MaxHeight;

    if (l_MaxHeight == l_MinHeight)
        p_HeightHeader.Flags |= MAP_HEIGHT_NO_HEIGHT;

    if (!(p_HeightHeader.Flags & MAP_HEIGHT_NO_HEIGHT))
        p_MapHeader.HeightMapSize += sizeof(gMapV9) + sizeof(gMapV8);

    //////////////////////////////////////////////////////////////////////////
    // Pack liquid data
    //////////////////////////////////////////////////////////////////////////

    uint8 l_FirstLiquidFlag = gMapLiquidFlags[0][0];
    bool l_FullType = false;

    for (int l_Y = 0; l_Y < ADT_CELLS_PER_GRID; l_Y++)
    {
        for (int l_X = 0; l_X < ADT_CELLS_PER_GRID; l_X++)
        {
            if (gMapLiquidFlags[l_Y][l_X] != l_FirstLiquidFlag)
            {
                l_FullType  = true;
                l_Y         = ADT_CELLS_PER_GRID;

                break;
            }
        }
    }

    /// no water data (if all grid have 0 liquid type)
    if (l_FirstLiquidFlag == 0 && !l_FullType)
    {
        // No liquid data
        p_MapHeader.LiquidMapOffset = 0;
        p_MapHeader.LiquidMapSize   = 0;
    }
    else
    {
        int l_MinX = 255,   l_MinY = 255;
        int l_MaxX = 0,     l_MaxY = 0;

        l_MaxHeight = -20000;
        l_MinHeight = 20000;

        for (int l_Y = 0; l_Y < ADT_GRID_SIZE; l_Y++)
        {
            for (int l_X = 0; l_X < ADT_GRID_SIZE; l_X++)
            {
                if (gMapLiquidShow[l_Y][l_X])
                {
                    if (l_MinX > l_X) 
                        l_MinX = l_X;

                    if (l_MaxX < l_X) 
                        l_MaxX = l_X;

                    if (l_MinY > l_Y) 
                        l_MinY = l_Y;

                    if (l_MaxY < l_Y) 
                        l_MaxY = l_Y;

                    float l_Height = gMapLiquidHeight[l_Y][l_X];

                    if (l_MaxHeight < l_Height) 
                        l_MaxHeight = l_Height;

                    if (l_MinHeight > l_Height) 
                        l_MinHeight = l_Height;
                }
                else
                    gMapLiquidHeight[l_Y][l_X] = CONF_use_minHeight;
            }
        }

        p_MapHeader.LiquidMapOffset = p_MapHeader.HeightMapOffset + p_MapHeader.HeightMapSize;
        p_MapHeader.LiquidMapSize   = sizeof(MapLiquidHeader);
        p_LiquidHeader.FourCC       = *(uint32 const*)MAP_LIQUID_MAGIC;
        p_LiquidHeader.Flags        = 0;
        p_LiquidHeader.LiquidType   = 0;
        p_LiquidHeader.OffsetX      = l_MinX;
        p_LiquidHeader.OffsetY      = l_MinY;
        p_LiquidHeader.Width        = l_MaxX - l_MinX + 1 + 1;
        p_LiquidHeader.Height       = l_MaxY - l_MinY + 1 + 1;
        p_LiquidHeader.LiquidLevel  = l_MinHeight;

        if (l_MaxHeight == l_MinHeight)
            p_LiquidHeader.Flags |= MAP_LIQUID_NO_HEIGHT;

        if (!l_FullType)
            p_LiquidHeader.Flags |= MAP_LIQUID_NO_TYPE;

        if (p_LiquidHeader.Flags & MAP_LIQUID_NO_TYPE)
            p_LiquidHeader.LiquidType = l_FirstLiquidFlag;
        else
            p_MapHeader.LiquidMapSize += sizeof(gMapLiquidEntry)+sizeof(gMapLiquidFlags);

        if (!(p_LiquidHeader.Flags & MAP_LIQUID_NO_HEIGHT))
            p_MapHeader.LiquidMapSize += sizeof(float)*p_LiquidHeader.Width*p_LiquidHeader.Height;
    }

    if (p_MapHeader.LiquidMapOffset)
        p_MapHeader.HolesOffset = p_MapHeader.LiquidMapOffset + p_MapHeader.LiquidMapSize;
    else
        p_MapHeader.HolesOffset = p_MapHeader.HeightMapOffset + p_MapHeader.HeightMapSize;

    if (p_HasHoles)
        p_MapHeader.HolesSize = sizeof(gMapHoles);
    else
        p_MapHeader.HolesSize = 0;
}
/// Write map file
bool WriteMapFile(char * p_FileName, MapFileHeader & p_MapHeader, MapAreaHeader & p_AreaHeader, MapHeightHeader & l_HeightHeader, MapLiquidHeader & l_LiquidHeader, bool p_HasHoles)
{
    FILE * l_File = fopen(p_FileName, "wb");

    if (!l_File)
    {
        printf("Can't create the output file '%s'\n", p_FileName);
        return false;
    }

    fwrite(&p_MapHeader, sizeof(p_MapHeader), 1, l_File);

    /// Store area data
    fwrite(&p_AreaHeader, sizeof(p_AreaHeader), 1, l_File);

    if (!(p_AreaHeader.Flags&MAP_AREA_NO_AREA))
        fwrite(gMapAreaFlags, sizeof(gMapAreaFlags), 1, l_File);

    /// Store height data
    fwrite(&l_HeightHeader, sizeof(l_HeightHeader), 1, l_File);

    if (!(l_HeightHeader.Flags & MAP_HEIGHT_NO_HEIGHT))
    {
        fwrite(gMapV9, sizeof(gMapV9), 1, l_File);
        fwrite(gMapV8, sizeof(gMapV8), 1, l_File);
    }

    /// Store liquid data if need
    if (p_MapHeader.LiquidMapOffset)
    {
        fwrite(&l_LiquidHeader, sizeof(l_LiquidHeader), 1, l_File);

        if (!(l_LiquidHeader.Flags & MAP_LIQUID_NO_TYPE))
        {
            fwrite(gMapLiquidEntry, sizeof(gMapLiquidEntry), 1, l_File);
            fwrite(gMapLiquidFlags, sizeof(gMapLiquidFlags), 1, l_File);
        }

        if (!(l_LiquidHeader.Flags & MAP_LIQUID_NO_HEIGHT))
        {
            for (int l_Y = 0; l_Y < l_LiquidHeader.Height; l_Y++)
                fwrite(&gMapLiquidHeight[l_Y + l_LiquidHeader.OffsetY][l_LiquidHeader.OffsetX], sizeof(float), l_LiquidHeader.Width, l_File);
        }
    }

    /// store hole data
    if (p_HasHoles)
        fwrite(gMapHoles, p_MapHeader.HolesSize, 1, l_File);

    fclose(l_File);

    return true;
}
/// Convert an single ADT
bool ConvertADT(char *filename, char *filename2, int /*cell_y*/, int /*cell_x*/, uint32 p_ClientBuild)
{
    ChunkedFile l_ADT;

    if (!l_ADT.LoadFile(filename))
        return false;

    MapFileHeader l_MapHeader;

    // Prepare map header
    l_MapHeader.MapMagic        = *(uint32 const*)MAP_MAGIC;
    l_MapHeader.VersionMagic    = *(uint32 const*)MAP_VERSION_MAGIC;
    l_MapHeader.BuildMagic      = p_ClientBuild;

    memset(gMapAreaFlags,   0xFF,   sizeof(gMapAreaFlags));
    memset(gMapV9,          0,      sizeof(gMapV9));
    memset(gMapV8,          0,      sizeof(gMapV8));
    memset(gMapLiquidShow,     0,      sizeof(gMapLiquidShow));
    memset(gMapLiquidFlags, 0,      sizeof(gMapLiquidFlags));
    memset(gMapLiquidEntry, 0,      sizeof(gMapLiquidEntry));

    memset(gMapHoles, 0, sizeof(gMapHoles));

    bool hasHoles = false;

    for (std::multimap<std::string, FileChunk*>::const_iterator itr = l_ADT.Chunks.lower_bound("MCNK"); itr != l_ADT.Chunks.upper_bound("MCNK"); ++itr)
    {
        McnkADT * l_MCNK = itr->second->As<McnkADT>();

        /// Area data
        if (l_MCNK->areaid <= gMaxAreaID && gAreasBits[l_MCNK->areaid] != 0xFFFF)
            gMapAreaFlags[l_MCNK->iy][l_MCNK->ix] = gAreasBits[l_MCNK->areaid];

        /// Height
        /// Height values for triangles stored in order:
        /// 1     2     3     4     5     6     7     8     9
        ///    10    11    12    13    14    15    16    17
        /// 18    19    20    21    22    23    24    25    26
        ///    27    28    29    30    31    32    33    34
        /// . . . . . . . .
        /// For better get height values merge it to V9 and V8 map
        /// V9 height map:
        /// 1     2     3     4     5     6     7     8     9
        /// 18    19    20    21    22    23    24    25    26
        /// . . . . . . . .
        /// V8 height map:
        ///    10    11    12    13    14    15    16    17
        ///    27    28    29    30    31    32    33    34
        /// . . . . . . . .

        // Set map height as grid height
        for (int y = 0; y <= ADT_CELL_SIZE; y++)
        {
            int cy = l_MCNK->iy * ADT_CELL_SIZE + y;
            for (int x = 0; x <= ADT_CELL_SIZE; x++)
            {
                int cx = l_MCNK->ix * ADT_CELL_SIZE + x;
                gMapV9[cy][cx] = l_MCNK->ypos;
            }
        }

        for (int y = 0; y < ADT_CELL_SIZE; y++)
        {
            int cy = l_MCNK->iy * ADT_CELL_SIZE + y;
            for (int x = 0; x < ADT_CELL_SIZE; x++)
            {
                int cx = l_MCNK->ix * ADT_CELL_SIZE + x;
                gMapV8[cy][cx] = l_MCNK->ypos;
            }
        }

        // Get custom height
        if (FileChunk* chunk = itr->second->GetSubChunk("MCVT"))
        {
            adt_MCVT* mcvt = chunk->As<adt_MCVT>();
            // get V9 height map
            for (int y = 0; y <= ADT_CELL_SIZE; y++)
            {
                int cy = l_MCNK->iy * ADT_CELL_SIZE + y;
                for (int x = 0; x <= ADT_CELL_SIZE; x++)
                {
                    int cx = l_MCNK->ix * ADT_CELL_SIZE + x;
                    gMapV9[cy][cx] += mcvt->height_map[y*(ADT_CELL_SIZE * 2 + 1) + x];
                }
            }
            // get V8 height map
            for (int y = 0; y < ADT_CELL_SIZE; y++)
            {
                int cy = l_MCNK->iy * ADT_CELL_SIZE + y;
                for (int x = 0; x < ADT_CELL_SIZE; x++)
                {
                    int cx = l_MCNK->ix * ADT_CELL_SIZE + x;
                    gMapV8[cy][cx] += mcvt->height_map[y*(ADT_CELL_SIZE * 2 + 1) + ADT_CELL_SIZE + 1 + x];
                }
            }
        }

        // Liquid data
        if (l_MCNK->sizeMCLQ > 8)
        {
            if (FileChunk* chunk = itr->second->GetSubChunk("MCLQ"))
            {
                adt_MCLQ* liquid = chunk->As<adt_MCLQ>();
                int count = 0;
                for (int y = 0; y < ADT_CELL_SIZE; ++y)
                {
                    int cy = l_MCNK->iy * ADT_CELL_SIZE + y;
                    for (int x = 0; x < ADT_CELL_SIZE; ++x)
                    {
                        int cx = l_MCNK->ix * ADT_CELL_SIZE + x;
                        if (liquid->flags[y][x] != 0x0F)
                        {
                            gMapLiquidShow[cy][cx] = true;
                            if (liquid->flags[y][x] & (1 << 7))
                                gMapLiquidFlags[l_MCNK->iy][l_MCNK->ix] |= MAP_LIQUID_TYPE_DARK_WATER;
                            ++count;
                        }
                    }
                }

                uint32 c_flag = l_MCNK->flags;
                if (c_flag & (1 << 2))
                {
                    gMapLiquidEntry[l_MCNK->iy][l_MCNK->ix] = 1;
                    gMapLiquidFlags[l_MCNK->iy][l_MCNK->ix] |= MAP_LIQUID_TYPE_WATER;            // water
                }
                if (c_flag & (1 << 3))
                {
                    gMapLiquidEntry[l_MCNK->iy][l_MCNK->ix] = 2;
                    gMapLiquidFlags[l_MCNK->iy][l_MCNK->ix] |= MAP_LIQUID_TYPE_OCEAN;            // ocean
                }
                if (c_flag & (1 << 4))
                {
                    gMapLiquidEntry[l_MCNK->iy][l_MCNK->ix] = 3;
                    gMapLiquidFlags[l_MCNK->iy][l_MCNK->ix] |= MAP_LIQUID_TYPE_MAGMA;            // magma/slime
                }

                if (!count && gMapLiquidFlags[l_MCNK->iy][l_MCNK->ix])
                    fprintf(stderr, "Wrong liquid detect in MCLQ chunk");

                for (int y = 0; y <= ADT_CELL_SIZE; ++y)
                {
                    int cy = l_MCNK->iy * ADT_CELL_SIZE + y;
                    for (int x = 0; x <= ADT_CELL_SIZE; ++x)
                    {
                        int cx = l_MCNK->ix * ADT_CELL_SIZE + x;
                        gMapLiquidHeight[cy][cx] = liquid->liquid[y][x].height;
                    }
                }
            }
        }

        // Hole data
        if (!(l_MCNK->flags & 0x10000))
        {
            if (uint16 hole = l_MCNK->holes)
            {
                gMapHoles[l_MCNK->iy][l_MCNK->ix] = l_MCNK->holes;
                hasHoles = true;
            }
        }
    }

    // Get liquid map for grid (in WOTLK used MH2O chunk)
    if (FileChunk* chunk = l_ADT.GetChunk("MH2O"))
    {
        adt_MH2O* h2o = chunk->As<adt_MH2O>();
        for (int i = 0; i < ADT_CELLS_PER_GRID; i++)
        {
            for (int j = 0; j < ADT_CELLS_PER_GRID; j++)
            {
                adt_liquid_header *h = h2o->getLiquidData(i, j);
                if (!h)
                    continue;

                int count = 0;
                uint64 show = h2o->getLiquidShowMap(h);
                for (int y = 0; y < h->height; y++)
                {
                    int cy = i * ADT_CELL_SIZE + y + h->yOffset;
                    for (int x = 0; x < h->width; x++)
                    {
                        int cx = j * ADT_CELL_SIZE + x + h->xOffset;
                        if (show & 1)
                        {
                            gMapLiquidShow[cy][cx] = true;
                            ++count;
                        }
                        show >>= 1;
                    }
                }

                gMapLiquidEntry[i][j] = h->liquidType;
                switch (gLiquidType[h->liquidType])
                {
                    case LIQUID_TYPE_WATER: gMapLiquidFlags[i][j] |= MAP_LIQUID_TYPE_WATER; break;
                    case LIQUID_TYPE_OCEAN: gMapLiquidFlags[i][j] |= MAP_LIQUID_TYPE_OCEAN; break;
                    case LIQUID_TYPE_MAGMA: gMapLiquidFlags[i][j] |= MAP_LIQUID_TYPE_MAGMA; break;
                    case LIQUID_TYPE_SLIME: gMapLiquidFlags[i][j] |= MAP_LIQUID_TYPE_SLIME; break;
                    default:
                        printf("\nCan't find Liquid type %u for map %s\nchunk %d,%d\n", h->liquidType, filename, i, j);
                        break;
                }
                // Dark water detect
                if (gLiquidType[h->liquidType] == LIQUID_TYPE_OCEAN)
                {
                    uint8* lm = h2o->getLiquidLightMap(h);
                    if (!lm)
                        gMapLiquidFlags[i][j] |= MAP_LIQUID_TYPE_DARK_WATER;
                }

                if (!count && gMapLiquidFlags[i][j])
                    printf("Wrong liquid detect in MH2O chunk");

                float* height = h2o->getLiquidHeightMap(h);
                int pos = 0;
                for (int y = 0; y <= h->height; y++)
                {
                    int cy = i * ADT_CELL_SIZE + y + h->yOffset;
                    for (int x = 0; x <= h->width; x++)
                    {
                        int cx = j * ADT_CELL_SIZE + x + h->xOffset;

                        if (height)
                            gMapLiquidHeight[cy][cx] = height[pos];
                        else
                            gMapLiquidHeight[cy][cx] = h->heightLevel1;

                        pos++;
                    }
                }
            }
        }
    }

    MapAreaHeader   l_AreaHeader;
    MapHeightHeader l_HeightHeader;
    MapLiquidHeader l_LiquidHeader;

    FillMapHeaders(l_MapHeader, l_AreaHeader, l_HeightHeader, l_LiquidHeader, hasHoles);

    return WriteMapFile(filename2, l_MapHeader, l_AreaHeader, l_HeightHeader, l_LiquidHeader, hasHoles);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Read map DBC
uint32 ReadMapDBC()
{
    printf("Read Map.dbc file... ");

    DBCFile l_DBC("ServerData/dbc/Map.dbc");

    if (!l_DBC.open())
    {
        printf("Fatal error: Invalid Map.dbc file format!\n");
        exit(1);
    }

    size_t l_MapCount = l_DBC.GetRecordCount();

    gMaps = new Map[l_MapCount];

    for (uint32 l_I = 0; l_I < l_MapCount; ++l_I)
    {
        gMaps[l_I].ID = l_DBC.GetRecord(l_I).GetUInt(0);
        strcpy(gMaps[l_I].FolderName, l_DBC.GetRecord(l_I).GetString(1));
    }

    printf("Done! (%u maps loaded)\n", uint32(l_MapCount));

    return l_MapCount;
}
/// Read AreaTable DBC
void ReadAreaTableDBC()
{
    printf("Read AreaTable.dbc file...");

    DBCFile l_DBC("ServerData/dbc/AreaTable.dbc");

    if (!l_DBC.open())
    {
        printf("Fatal error: Invalid AreaTable.dbc file format!\n");
        exit(1);
    }

    size_t l_AreaCount = l_DBC.GetRecordCount();

    gMaxAreaID = l_DBC.GetMaxId();
    gAreasBits = new uint16[gMaxAreaID + 1];

    for (uint32 l_I = 0; l_I < l_AreaCount; ++l_I)
        gAreasBits[l_DBC.GetRecord(l_I).GetUInt(0)] = l_DBC.GetRecord(l_I).GetUInt(3);

    printf("Done! (%u areas loaded)\n", uint32(l_AreaCount));
}
/// Read LiquidType DBC
void ReadLiquidTypeTableDBC()
{
    printf("Read LiquidType.dbc file...");

    DBCFile l_DBC("ServerData/dbc/LiquidType.dbc");

    if (!l_DBC.open())
    {
        printf("Fatal error: Invalid LiquidType.dbc file format!\n");
        exit(1);
    }

    size_t l_LiquidTypeCount = l_DBC.GetRecordCount();
    size_t l_LiquidTypeMaxID = l_DBC.GetMaxId();

    gLiquidType = new uint16[l_LiquidTypeMaxID + 1];

    memset(gLiquidType, 0xff, (l_LiquidTypeMaxID + 1) * sizeof(uint16));

    for (uint32 l_I = 0; l_I < l_LiquidTypeCount; ++l_I)
        gLiquidType[l_DBC.GetRecord(l_I).GetUInt(0)] = l_DBC.GetRecord(l_I).GetUInt(3);

    printf("Done! (%u LiqTypes loaded)\n", (uint32)l_LiquidTypeCount);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Extract maps
void ExtractMaps(uint32 p_ClientBuild)
{
    char l_MapADTFileName[1024];
    char l_MapOutputFileName[1024];
    char l_MapWDTFileName[1024];

    printf("Extracting maps...\n");

    uint32 l_MapCount = ReadMapDBC();

    ReadAreaTableDBC();
    ReadLiquidTypeTableDBC();

    printf("Convert map files\n");

    for (uint32 l_I = 0; l_I < l_MapCount; ++l_I)
    {
        printf("Extract %s (%d/%u)                  \n", gMaps[l_I].FolderName, l_I+1, l_MapCount);

        /// Loadup map grid data
        sprintf(l_MapWDTFileName, "Temp\\World\\Maps\\%s\\%s.wdt", gMaps[l_I].FolderName, gMaps[l_I].FolderName);

        ChunkedFile l_MapWDT;

        if (!l_MapWDT.LoadFile(l_MapWDTFileName, true))
            continue;

        FileChunk* l_MapChunk = l_MapWDT.GetChunk("MAIN");

        for (uint32 l_Y = 0; l_Y < WDT_MAP_SIZE; ++l_Y)
        {
            for (uint32 l_X = 0; l_X < WDT_MAP_SIZE; ++l_X)
            {
                if (!(l_MapChunk->As<MainWDT>()->adt_list[l_Y][l_X].flag & 0x1))
                    continue;

                sprintf(l_MapADTFileName,       "Temp\\World\\Maps\\%s\\%s_%u_%u.adt",  gMaps[l_I].FolderName, gMaps[l_I].FolderName, l_X, l_Y);
                sprintf(l_MapOutputFileName,    "ServerData\\maps\\%04u%02u%02u.map",   gMaps[l_I].ID, l_Y, l_X);

                ConvertADT(l_MapADTFileName, l_MapOutputFileName, l_Y, l_X, p_ClientBuild);
            }

            /// Draw progress bar
            printf("Processing........................%d%%\r", (100 * (l_Y+1)) / WDT_MAP_SIZE);
        }
    }

    printf("\n");

    delete[] gAreasBits;
    delete[] gMaps;
    delete[] gLiquidType;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Entry point
int main(int p_Argc, char * p_Argv[])
{
    uint32 build = 0;

    ExtractMaps(build);

    return 0;
}
