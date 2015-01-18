#ifndef INSTANCE_BLOODMAUL_H
# define INSTANCE_BLOODMAUL_H

# include "ScriptUtils.h"

namespace MS
{
    namespace Instances
    {
        namespace Bloodmaul
        {
            enum class BossEntries : uint32
            {
                SlaveWatcherCrushto = 74787,
            };

            enum class BossIds : uint32 
            {
                SlaveWatcherCrushto,
                ForgemasterGogduh
            };

            enum class MobEntries : uint32
            {
                MinesBat = 75374,
                BloodmaulEnforcer = 84978,
                BloodmaulOverseer = 75426,
                BloodmaulSlaver = 75191,
                BloodmaulOverseer2 = 75193,
                BloodmaulGeomancer = 75198,
                BloodmaulEnforcer2 = 84978,
                BloodmaulWarder = 75210,
                BloodmaulOgreMage = 75272,
                EarthCrushStalker = 83650,
                CapturedMiner1 = 74355,
                CapturedMiner2 = 74356,
                CapturedMiner3 = 74357,
                HordeMinerSpawn = 75585,
                NeutralMinerSpawn = 75586,
                AllianceMinerSpawn = 75584,
                SlaveWatcherCrushto = 74787,

                OgreWheelStalker = 75499,
                Slagna = 75406,
                MoltenEarthElemental = 75209,
                FirePillar = 75327,
                SearingEmber = 75360,
                LavaExplosionStalker = 75846,
                MagmaLord = 75211,
            };

            enum class Data : uint32
            {
                RaiseTheMiners,
                RaiseTheMinersChangeTarget,
                SpawnSlagna,
            };

            enum class Talks : uint32
            {
                WarderAttack = 0,
                CapturedMinerReleased = 0,
            };

            namespace SlaverWatcherCrushto
            {
                enum class Texts : int32
                {
                    BloodmaulOgreMagesDied1 = 0,
                    BloodmaulOgreMagesDied2 = 1,
                    Aggro = 2,
                    RaiseTheMiners = 3,
                    Slay1 = 4,
                    CrushingLeap = 5,
                    Slay3 = 6,
                    JustDied = 7,
                    Slay2 = 8,
                    EarthCrush = 9,
                };
            }
        }
    }
}

#endif /* !INSTANCE_BLOODMAUL_H */