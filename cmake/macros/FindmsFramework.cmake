################################################################################
##
##  MILLENIUM-STUDIO
##  Copyright 2015 Millenium-studio SARL
##  All Rights Reserved.
##
################################################################################

SET(MSFRAMEWORK_ROOT_HINTS "C:/msFramework/" "/usr/local/lib/")
SET(MSFRAMEWORK_ROOT_PATHS "C:/msFramework/" "/usr/local/lib/")

FIND_PATH(MSFRAMEWORK_ROOT_DIR
  NAMES
    include/Stream/Base64.hpp
    include/DesignPatterns/Singleton.hpp
    include/Generic/Casting.hpp
    include/Threading/ScopedLock.hpp
    include/Reporting/Reporter.hpp
    include/Reporting/Reports.hpp
    include/Reporting/ReportWorker.hpp
    include/Reporting/Reports.hxx
  HINTS
    ${MSFRAMEWORK_ROOT_HINTS}
  PATHS
    ${MSFRAMEWORK_ROOT_PATHS}
)

MARK_AS_ADVANCED(MSFRAMEWORK_ROOT_DIR)
# Re-use the previous path:
SET(MSFRAMEWORK_INCLUDE_DIR ${MSFRAMEWORK_ROOT_DIR}/include)

    FIND_LIBRARY(MSREPORTING_LIBRARIES_RELEASE
      NAMES
          msReporting
      PATHS
        ${MSFRAMEWORK_ROOT_DIR}/lib/
    )
    FIND_LIBRARY(MSSTREAM_LIBRARIES_RELEASE
      NAMES
            msStream
      PATHS
        ${MSFRAMEWORK_ROOT_DIR}/lib/
    )
    FIND_LIBRARY(MSREPORTING_LIBRARIES_DEBUG
      NAMES
          msReporting-dev
      PATHS
        ${MSFRAMEWORK_ROOT_DIR}/lib/
    )
    FIND_LIBRARY(MSSTREAM_LIBRARIES_DEBUG
      NAMES
            msStream-dev
      PATHS
        ${MSFRAMEWORK_ROOT_DIR}/lib/
    )

  MARK_AS_ADVANCED(MSREPORTING_LIBRARIES MSSTREAM_LIBRARIES)
  SET(MSFRAMEWORK_LIBRARIES
      optimized ${MSREPORTING_LIBRARIES_RELEASE}
      optimized ${MSSTREAM_LIBRARIES_RELEASE}
      debug ${MSREPORTING_LIBRARIES_DEBUG}
      debug ${MSSTREAM_LIBRARIES_DEBUG}
  )
  FIND_LIBRARY(MSFRAMEWORK_LIBRARIES NAMES msFramework)
  MARK_AS_ADVANCED(MSFRAMEWORK_LIBRARIES)

if(MSFRAMEWORK_INCLUDE_DIR)
  if (MSFRAMEWORK_LIBRARIES)
      SET(MSFRAMEWORK_FOUND 1)
      MESSAGE(STATUS "Found msFramework headers: ${MSFRAMEWORK_INCLUDE_DIR}")
      MESSAGE(STATUS "Found msFramework librairies: ${MSFRAMEWORK_LIBRARIES}")
    else(MSFRAMEWORK_LIBRARIES)
      MESSAGE(FATAL_ERROR "Could not find msFramework libs! Please install msFramework libraries and headers")
    endif(MSFRAMEWORK_LIBRARIES)
else( MSFRAMEWORK_INCLUDE_DIR)
    MESSAGE(FATAL_ERROR "Could not find msFramework headers! Please install msFramework libraries and headers")
endif(MSFRAMEWORK_INCLUDE_DIR)

MARK_AS_ADVANCED(MSFRAMEWORK_FOUND MSFRAMEWORK_LIBRARIES MSFRAMEWORK_INCLUDE_DIR)
