# Set build-directive (used in core to tell which buildtype we used)
add_definitions(-D_BUILD_DIRECTIVE='"$(CONFIGURATION)"')

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
message(STATUS "Clang: Enabled c++14 support")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
message(STATUS "Clang: Enabled c++11 support")

if(WITH_WARNINGS)
  set(WARNING_FLAGS "-W -Wall -Wextra -Winit-self -Winvalid-pch -Wfatal-errors -Wno-mismatched-tags -Wno-unknown-pragmas")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNING_FLAGS} -fcolor-diagnostics")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} -Woverloaded-virtual")
  message(STATUS "Clang: All warnings enabled")
else()
  add_definitions(--no-warnings)
  message(STATUS "Clang: All warnings disabled")
endif()

if(WITH_COREDEBUG)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
  message(STATUS "Clang: Debug-flags set (-g3)")
endif()

# -Wno-narrowing needed to suppress a warning in g3d
# -Wno-deprecated-register is needed to suppress 185 gsoap warnings on Unix systems.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing -Wno-deprecated-register")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG=1")
