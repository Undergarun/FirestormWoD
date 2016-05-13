# Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

if (USE_COREPCH)
  set(shared_STAT_PCH_HDR PrecompiledHeaders/sharedPCH.h)
  set(shared_STAT_PCH_SRC PrecompiledHeaders/sharedPCH.cpp)
endif()

file(GLOB_RECURSE sources_Configuration Configuration/*.cpp Configuration/*.h)
file(GLOB_RECURSE sources_Cryptography Cryptography/*.cpp Cryptography/*.h)
file(GLOB_RECURSE sources_Database Database/*.cpp Database/*.h Database/*.hpp)
file(GLOB_RECURSE sources_DataStores DataStores/*.cpp DataStores/*.h)
file(GLOB_RECURSE sources_Dynamic Dynamic/*.cpp Dynamic/*.h)
file(GLOB_RECURSE sources_Logging Logging/*.cpp Logging/*.h Logging/*.hpp)
file(GLOB_RECURSE sources_Packets Packets/*.cpp Packets/*.h)
file(GLOB_RECURSE sources_Threading Threading/*.cpp Threading/*.h Threading/*.hpp)
file(GLOB_RECURSE sources_Utilities Utilities/*.cpp Utilities/*.h Utilities/*.hpp)
file(GLOB_RECURSE sources_Reporting Reporting/*.cpp Reporting/*.hpp Reporting/*.hxx)

file(GLOB sources_localdir *.cpp *.h)

#
# Build shared sourcelist
#

if( USE_COREPCH AND MSVC )
  set(shared_STAT_SRCS
    PrecompiledHeaders/sharedPCH.cpp
    PrecompiledHeaders/sharedPCH.h
  )
endif()

set(shared_STAT_SRCS
  ${shared_STAT_SRCS}
  ${sources_Configuration}
  ${sources_Cryptography}
  ${sources_Database}
  ${sources_DataStores}
  ${sources_Dynamic}
  ${sources_Logging}
  ${sources_Packets}
  ${sources_Threading}
  ${sources_Utilities}
  ${sources_Reporting}
  ${sources_localdir}
  Debugging/Errors.h
)

MACRO( auto_group ProjectDirectory ProjectSources )
	SET( DirectorySources "${ProjectSources}" )
	FOREACH( Source ${DirectorySources} )
		if ("${Source}" MATCHES "(/)+")
			STRING( REGEX REPLACE "${ProjectDirectory}" "" RelativePath "${Source}" )
			STRING( REGEX REPLACE "[\\\\/][^\\\\/]*$" "" RelativePath "${RelativePath}" )
			STRING( REGEX REPLACE "^[\\\\/]" "" RelativePath "${RelativePath}" )
			STRING( REGEX REPLACE "/" "\\\\\\\\" RelativePath "${RelativePath}" )
			SOURCE_GROUP( "${RelativePath}" FILES ${Source} )
		else()
			SOURCE_GROUP( "" FILES ${Source} )
		endif()
	ENDFOREACH( Source )
ENDMACRO( auto_group )

auto_group("${CMAKE_CURRENT_SOURCE_DIR}" "${shared_STAT_SRCS}")

set(shared_STAT_SRCS
  ${shared_STAT_SRCS}
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
  ${CMAKE_CURRENT_SOURCE_DIR}/Configuration
  ${CMAKE_CURRENT_SOURCE_DIR}/Cryptography
  ${CMAKE_CURRENT_SOURCE_DIR}/Database
  ${CMAKE_CURRENT_SOURCE_DIR}/DataStores
  ${CMAKE_CURRENT_SOURCE_DIR}/Debugging
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamic
  ${CMAKE_CURRENT_SOURCE_DIR}/Logging
  ${CMAKE_CURRENT_SOURCE_DIR}/Packets
  ${CMAKE_CURRENT_SOURCE_DIR}/Threading
  ${CMAKE_CURRENT_SOURCE_DIR}/Utilities
  ${CMAKE_CURRENT_SOURCE_DIR}/Reporting
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Object
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ${CURL_INCLUDE_DIR}
  ${EASYJSON_INCLUDE_DIR}
)

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
