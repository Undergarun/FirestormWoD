# - Try to find the EASYJSON library.
# Once done this will define
#
#  EASYJSON_ROOT_DIR - Set this variable to the root installation of EASYJSON
#
# Read-Only variables:
#  EASYJSON_FOUND - system has the EASYJSON library
#  EASYJSON_INCLUDE_DIR - the EASYJSON include directory

# http://EASYJSON.haxx.se/

SET(_EASYJSON_ROOT_HINTS "${CMAKE_SOURCE_DIR}/dep/EasyJSon/")
SET(_EASYJSON_ROOT_PATHS "${CMAKE_SOURCE_DIR}/dep/EasyJSon/")

FIND_PATH(EASYJSON_ROOT_DIR
  NAMES
    EasyJSon.hpp
  HINTS
    ${_EASYJSON_ROOT_HINTS}
  PATHS
    ${_EASYJSON_ROOT_PATHS}
)
MARK_AS_ADVANCED(EASYJSON_ROOT_DIR)

# Re-use the previous path:
FIND_PATH(EASYJSON_INCLUDE_DIR EasyJSon.hpp
  ${EASYJSON_ROOT_DIR}/
)

if ( EASYJSON_INCLUDE_DIR )
    SET( EASYJSON_FOUND 1 )
    MESSAGE(STATUS "Found EASYJSON headers: ${EASYJSON_INCLUDE_DIR}")
else ( EASYJSON_INCLUDE_DIR )
    MESSAGE(FATAL_ERROR "Could not find EASYJSON headers! Please install EASYJSON libraries and headers")
endif ( EASYJSON_INCLUDE_DIR )

MARK_AS_ADVANCED(EASYJSON_FOUND EASYJSON_INCLUDE_DIR)
