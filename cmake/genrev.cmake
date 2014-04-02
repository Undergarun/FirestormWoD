# Copyright (C) 2008-2012 Trinity <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

include(${CMAKE_SOURCE_DIR}/cmake/macros/EnsureVersion.cmake)

# Last minute check - ensure that we have a proper revision
# If everything above fails (means the user has erased the git revision control directory or removed the origin/HEAD tag)
if(NOT rev_info)
  # No valid ways available to find/set the revision/hash, so let's force some defaults
  message(STATUS "WARNING - Missing repository tags - you may need to pull tags with git fetch -t")
  message(STATUS "WARNING - Continuing anyway - note that the versionstring will be set to 0000-00-00 00:00:00 (Archived)")
  set(rev_date "0000-00-00 00:00:00 +0000")
  set(rev_hash "Archived")
else()
  # Extract information required to build a proper versionstring
  string(REGEX REPLACE init-|[0-9]+-g "" rev_hash ${rev_info})
endif()

# Its not set during initial run
if(NOT BUILDDIR)
  set(BUILDDIR ${CMAKE_BINARY_DIR})
endif()

# Create the actual revision.h file from the above params
if(NOT "${rev_hash_cached}" MATCHES "${rev_hash}")
  configure_file(
    "${CMAKE_SOURCE_DIR}/revision.h.in.cmake"
    "${BUILDDIR}/revision.h"
    @ONLY
  )
  set(rev_hash_cached "${rev_hash}" CACHE INTERNAL "Cached commit-hash")
endif()
