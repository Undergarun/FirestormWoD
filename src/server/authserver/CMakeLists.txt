# Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

########### authserver ###############

file(GLOB_RECURSE sources_authentication Authentication/*.cpp Authentication/*.h)
file(GLOB_RECURSE sources_realms Realms/*.cpp Realms/*.h)
file(GLOB_RECURSE sources_server Server/*.cpp Server/*.h)
file(GLOB_RECURSE sources_bnet Bnet2/*.cpp Bnet2/*.hpp)
file(GLOB sources_localdir *.cpp *.h)

if (USE_COREPCH)
  set(authserver_PCH_HDR PrecompiledHeaders/authPCH.h)
  set(authserver_PCH_SRC PrecompiledHeaders/authPCH.cpp)
endif()

set(authserver_SRCS
  ${authserver_SRCS}
  ${sources_authentication}
  ${sources_realms}
  ${sources_server}
  ${sources_bnet}
  ${sources_localdir}
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

auto_group("${CMAKE_CURRENT_SOURCE_DIR}" "${authserver_SRCS}")

set(authserver_SRCS
  ${authserver_SRCS}
)

if( WIN32 )
set(authserver_SRCS
    ${authserver_SRCS}
    ${sources_Debugging}
    authserver.rc
)
endif()

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/g3dlite/include
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Packets
  ${CMAKE_SOURCE_DIR}/src/server/shared/Cryptography
  ${CMAKE_SOURCE_DIR}/src/server/shared/Cryptography/Authentication
  ${CMAKE_SOURCE_DIR}/src/server/shared/Logging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${CMAKE_SOURCE_DIR}/src/server/shared/Reporting
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Authentication
  ${CMAKE_CURRENT_SOURCE_DIR}/Realms
  ${CMAKE_CURRENT_SOURCE_DIR}/Server
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ${EASYJSON_INCLUDE_DIR}
  ${CURL_INCLUDE_DIR}
  ${MSFRAMEWORK_INCLUDE_DIR}
)

add_executable(authserver
  ${authserver_SRCS}
  ${authserver_PCH_SRC}
)
set_property(TARGET authserver PROPERTY FOLDER "server")

add_dependencies(authserver revision.h)

if( UNIX AND NOT NOJEM AND NOT APPLE )
  set(authserver_LINK_FLAGS "-pthread")
endif()

if( NOT WIN32 )
   set_target_properties(authserver PROPERTIES
    LINK_FLAGS "${authserver_LINK_FLAGS}" COMPILE_DEFINITIONS _TRINITY_REALM_CONFIG="${CONF_DIR}/authserver.conf"
   )
endif()

target_link_libraries(authserver
  shared
  ${MYSQL_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${OPENSSL_EXTRA_LIBRARIES}
  ${OSX_LIBS}
  ${CURL_LIBRARIES}
  ${MSFRAMEWORK_LIBRARIES}
)

if( WIN32 )
  if ( MSVC )
    add_custom_command(TARGET authserver
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/authserver.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
    )
  elseif ( MINGW )
    add_custom_command(TARGET authserver
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/authserver.conf.dist ${CMAKE_BINARY_DIR}/bin/
    )
  endif()
endif()

if( UNIX )
  install(TARGETS authserver DESTINATION bin)
  install(FILES  authserver.conf.dist DESTINATION ${CONF_DIR})
elseif( WIN32 )
  install(TARGETS authserver DESTINATION "${CMAKE_INSTALL_PREFIX}")
  install(FILES authserver.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()

# Generate precompiled header
if (USE_COREPCH)
  add_cxx_pch(authserver ${authserver_PCH_HDR} ${authserver_PCH_SRC})
endif()
