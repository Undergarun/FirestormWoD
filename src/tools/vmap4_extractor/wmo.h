////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef WMO_H
#define WMO_H
#define TILESIZE (533.33333f)
#define CHUNKSIZE ((TILESIZE) / 16.0f)

#include <string>
#include <set>
#include "vec3d.h"
#include "mpqfile.h"

class WMOInstance;
class WMOManager;
class MPQFile;

/* for whatever reason a certain company just can't stick to one coordinate system... */
static inline Vec3D fixCoords(const Vec3D &v){ return Vec3D(v.z, v.x, v.y); }

class WMORoot
{
private:
    std::string filename;
public:
    unsigned int col;
    uint32 nTextures, nGroups, nP, nLights, nModels, nDoodads, nDoodadSets, RootWMOID, liquidType;
    float bbcorn1[3];
    float bbcorn2[3];

    WMORoot(std::string& filename);

    bool open();
    bool ConvertToVMAPRootWmo(FILE* output);
};

struct WMOLiquidHeader
{
    int xverts, yverts, xtiles, ytiles;
    float pos_x;
    float pos_y;
    float pos_z;
    short type;
};

#pragma pack(push, 1)

struct WMOLiquidVert
{
    uint16 unk1;
    uint16 unk2;
    float height;
};

#pragma pack(pop)

class WMOGroup
{
private:
    std::string filename;
public:
    // MOGP

    char* MOPY;
    uint16* MOVI;
    uint16* MoviEx;
    float* MOVT;
    uint16* MOBA;
    int* MobaEx;
    WMOLiquidHeader* hlq;
    WMOLiquidVert* LiquEx;
    char* LiquBytes;
    int groupName, descGroupName;
    int mogpFlags;
    float bbcorn1[3];
    float bbcorn2[3];
    uint16 moprIdx;
    uint16 moprNItems;
    uint16 nBatchA;
    uint16 nBatchB;
    uint32 nBatchC, fogIdx, liquidType, groupWMOID;

    int mopy_size, moba_size;
    int LiquEx_size;
    unsigned int nVertices; // number when loaded
    int nTriangles; // number when loaded
    uint32 liquflags;

    WMOGroup(std::string const& filename);
    ~WMOGroup();

    bool open();
    int ConvertToVMAPGroupWmo(FILE* output, WMORoot* rootWMO, bool preciseVectorData);
};

class WMOInstance
{
    static std::set<int> ids;
public:
    std::string MapName;
    int currx;
    int curry;
    WMOGroup* wmo;
    int doodadset;
    Vec3D pos;
    Vec3D pos2, pos3, rot;
    uint32 indx, id, d2, d3;

    WMOInstance(MPQFile&f , char const* WmoInstName, uint32 mapID, uint32 tileX, uint32 tileY, FILE* pDirfile);

    static void reset();
};

#endif
