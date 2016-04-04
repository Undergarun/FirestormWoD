# Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
# Copyright (C) 2012-2013 Pandashan <http://www.pandashan.com/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# CMake policies (can not be handled elsewhere)
cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0005 OLD)
if(POLICY CMP0053)
    cmake_policy(SET CMP0053 OLD)
endif()

# Set projectname (must be done AFTER setting configurationtypes)
project(WoDCore)

# Set RPATH-handing (CMake parameters)
set(CMAKE_SKIP_BUILD_RPATH 0)
set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)
if ( CMAKE_INSTALL_PREFIX MATCHES "/usr/local" )
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/firestorm")
else()
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH 1)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# set macro-directory
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/macros")

# build in Release-mode by default if not explicitly set
if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE "Release")
endif()

include(CheckCXXSourceRuns)
include(CheckIncludeFiles)

# set default buildoptions and print them
include(cmake/options.cmake)

# turn off PCH totally if enabled (hidden setting, mainly for devs)
if( NOPCH )
  set(USE_COREPCH 0)
  set(USE_SCRIPTPCH 0)
endif()

include(CheckPlatform)

# basic packagesearching and setup (further support will be needed, this is a preliminary release!)
find_package(PCHSupport)
find_package(ACE REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(curl REQUIRED)
find_package(EasyJSon REQUIRED)
# FIND_PACKAGE(msFramework REQUIRED)
find_package(MySQL REQUIRED)


if( UNIX )
  find_package(Readline)
  find_package(ZLIB)
  find_package(BZip2)
endif()

if( NOT DISABLEGITCHECK )
  find_package(Git)
endif()

# Find revision ID and hash of the sourcetree
include(cmake/genrev.cmake)

# print out the results before continuing
include(cmake/showoptions.cmake)

# add dependencies
add_subdirectory(dep)

# add core sources
add_subdirectory(src)
