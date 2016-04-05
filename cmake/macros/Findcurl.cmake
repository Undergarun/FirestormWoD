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
if(PLATFORM EQUAL 64)
# dynamic change for cURL 
set(MSVC_EXPECTED_VERSION 19.0.0.0) # MSVC 2015 update 2
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER MSVC_EXPECTED_VERSION)

FIND_LIBRARY(CURL_LIBRARIES
  NAMES
    libcurl curl curl_a libcurl_a
  PATHS
    ${CURL_ROOT_DIR}/lib/MSVC_2015/x64/
)
message(STATUS "MSVC:  CURL MSVC 2015 x64")

else()

FIND_LIBRARY(CURL_LIBRARIES
  NAMES
    libcurl curl curl_a libcurl_a
  PATHS
    ${CURL_ROOT_DIR}/lib/MSVC_2013/x64/
)

message(STATUS "MSVC: CURL MSVC 2013 x64")
endif()

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

else()

# dynamic change for cURL 
set(MSVC_EXPECTED_VERSION 19.0.0.0) # MSVC 2015 update 2
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER MSVC_EXPECTED_VERSION)

FIND_LIBRARY(CURL_LIBRARIES
  NAMES
    libcurl curl curl_a libcurl_a
  PATHS
    ${CURL_ROOT_DIR}/lib/MSVC_2015/x32/
)
message(STATUS "MSVC:  CURL MSVC 2015 x32")

else()

FIND_LIBRARY(CURL_LIBRARIES
  NAMES
    libcurl curl curl_a libcurl_a
  PATHS
    ${CURL_ROOT_DIR}/lib/MSVC_2013/x32/
)

message(STATUS "MSVC: CURL MSVC 2013 x32")
endif()

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
endif()