#
#  MILLENIUM-STUDIO
#  Copyright 2016 Millenium-studio SARL
#  All Rights Reserved.
#

if( USE_COREPCH )
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

file(GLOB_RECURSE sources_Cryptography Cryptography/*.cpp Cryptography/*.h)
file(GLOB_RECURSE sources_DataStores DataStores/*.cpp DataStores/*.h)
file(GLOB_RECURSE sources_Dynamic Dynamic/*.cpp Dynamic/*.h)
file(GLOB_RECURSE sources_Packets Packets/*.cpp Packets/*.h)

if(WIN32)
  file(GLOB_RECURSE sources_Service Service/*.cpp Service/*.h)
endif(WIN32)
 
file(GLOB sources_localdir *.cpp *.h)

#
# Build shared sourcelist
#
if (USE_COREPCH)
  set(shared_STAT_PCH_HDR PrecompiledHeaders/sharedPCH.h)
  set(shared_STAT_PCH_SRC PrecompiledHeaders/sharedPCH.cpp)
endif()

set(shared_STAT_SRCS
  ${shared_STAT_SRCS}
  ${sources_Cryptography}
  ${sources_DataStores}
  ${sources_Dynamic}
  ${sources_Packets}
  ${sources_Service}
  ${sources_localdir}
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour/
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour/Include
  ${CMAKE_SOURCE_DIR}/dep/SFMT
  ${CMAKE_SOURCE_DIR}/dep/sockets/include
  ${CMAKE_SOURCE_DIR}/dep/utf8cpp
  ${CMAKE_SOURCE_DIR}/dep/g3dlite/include
  ${CMAKE_SOURCE_DIR}/src/server
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src/common/
  ${CMAKE_SOURCE_DIR}/src/common/Configuration
  ${CMAKE_SOURCE_DIR}/src/common/Cryptography
  ${CMAKE_SOURCE_DIR}/src/server/database/
  ${CMAKE_SOURCE_DIR}/src/server/database/Database
  ${CMAKE_CURRENT_SOURCE_DIR}/DataStores
  ${CMAKE_CURRENT_SOURCE_DIR}/Debugging
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamic
  ${CMAKE_SOURCE_DIR}/src/common/Logging
  ${CMAKE_CURRENT_SOURCE_DIR}/Packets
  ${CMAKE_SOURCE_DIR}/src/common/Threading
  ${CMAKE_SOURCE_DIR}/src/common/Utilities
  ${CMAKE_CURRENT_SOURCE_DIR}/Reporting
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Object
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ${CURL_INCLUDE_DIR}
  ${EASYJSON_INCLUDE_DIR}
)

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_library(shared STATIC
  ${shared_STAT_SRCS}
  ${shared_STAT_PCH_SRC}
)
set_property(TARGET shared PROPERTY FOLDER "server")

target_link_libraries(shared
  ${ACE_LIBRARY}
  ${CURL_LIBRARY}
)

add_dependencies(shared revision_data.h)

# Generate precompiled header
if (USE_COREPCH)
  add_cxx_pch(shared ${shared_STAT_PCH_HDR} ${shared_STAT_PCH_SRC})
endif ()
