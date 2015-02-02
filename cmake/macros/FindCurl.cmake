# - Try to find the cUrl library.
# Once done this will define
#
#  CURL_ROOT_DIR - Set this variable to the root installation of cUrl
#
# Read-Only variables:
#  CURL_FOUND - system has the cUrl library
#  CURL_INCLUDE_DIR - the cUrl include directory
#  CURL_LIBRARIES - The libraries needed to use cUrl

# http://curl.haxx.se/

SET(_CURL_ROOT_HINTS "${CMAKE_SOURCE_DIR}/dep/curl/")

SET(_CURL_ROOT_PATHS "${CMAKE_SOURCE_DIR}/dep/curl/")

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${_CURL_ROOT_HINTS}/lib/ ) 

FIND_PATH(CURL_ROOT_DIR
  NAMES
    include/curl/curl.h
  HINTS
    ${_CURL_ROOT_HINTS}
  PATHS
    ${_CURL_ROOT_PATHS}
)
MARK_AS_ADVANCED(CURL_ROOT_DIR)

# Re-use the previous path:
FIND_PATH(CURL_INCLUDE_DIR curl/curl.h
  ${CURL_ROOT_DIR}/include
)

FIND_LIBRARY(CURL_LIBRARIES
  NAMES
    libcurl curl curl_a libcurl_a
  PATHS
    ${CURL_ROOT_DIR}/lib
)

MARK_AS_ADVANCED(CURL_LIBRARIES)

if ( CURL_LIBRARIES )
    if ( CURL_INCLUDE_DIR )
        SET( CURL_FOUND 1 )
        MESSAGE(STATUS "Found CURL library: ${CURL_LIBRARIES}")
        MESSAGE(STATUS "Found CURL headers: ${CURL_INCLUDE_DIR}")
    else ( CURL_INCLUDE_DIR )
        MESSAGE(FATAL_ERROR "Could not find CURL headers! Please install CURL libraries and headers")
    endif ( CURL_INCLUDE_DIR )
endif ( CURL_LIBRARIES )

MARK_AS_ADVANCED(CURL_FOUND CURL_LIBRARIES CURL_INCLUDE_DIR )

