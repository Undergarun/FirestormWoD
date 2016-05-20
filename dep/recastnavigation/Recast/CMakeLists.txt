#
#  MILLENIUM-STUDIO
#  Copyright 2016 Millenium-studio SARL
#  All Rights Reserved.
#

set(Recast_STAT_SRCS
    Source/Recast.cpp
    Source/RecastAlloc.cpp
    Source/RecastArea.cpp
    Source/RecastContour.cpp
    Source/RecastFilter.cpp
	Source/RecastLayers.cpp
    Source/RecastMesh.cpp
    Source/RecastMeshDetail.cpp
    Source/RecastRasterization.cpp
    Source/RecastRegion.cpp
)

include_directories(Include)

if(WIN32)
  include_directories(
    ${CMAKE_SOURCE_DIR}/dep/zlib
  )
endif()

add_library(Recast STATIC ${Recast_STAT_SRCS})

target_link_libraries(Recast ${ZLIB_LIBRARIES})

set_property(TARGET Recast PROPERTY FOLDER "dep")
