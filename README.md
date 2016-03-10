## Overview
World of Warcraft Warlords of Draenor (6.X) emulator based on TrinityCore project, JadeCore (5.0.5), Garrosh (5.4.X)

[![Build Status](https://drone.firestorm-servers.com/api/badges/MilleniumStudio/wod/status.svg)](https://drone.firestorm-servers.com/MilleniumStudio/wod)

## Requirements

+ Platform: Linux, Windows or Mac
+ Processor with SSE2 support
+ ACE ≥ 6.2.0
+ MySQL ≥ 5.1.0
+ CMake ≥ 2.8.11.2 / 2.8.9 (Windows / Linux)
+ OpenSSL ≥ 1.0.0
+ GCC ≥ 4.7.2 (Linux only)
+ MS Visual Studio ≥ 12 (2013) (Windows only)
+ Xcode 5 (Mac only)


## Install

Detailed installation guides are available in the wiki for
[Windows](http://collab.kpsn.org/display/tc/Win),
[Linux](http://collab.kpsn.org/display/tc/Linux) and
[Mac OSX](http://collab.kpsn.org/display/tc/Mac).

In order to complete the installation of the core, you need some other dependencies, they are:
- LibCurl, under linux its the package libcurl4-openssl-dev, under windows you may have to compile it your self,
- EasyJson headers, for that you only need to execute in your git repo :
```
git submodule init && git submodule update --remote
```

- The MS framework that can be found on : https://github.com/MilleniumStudio/ms_framework, you need to generate the solution with cmake, compile it with visual studio or whatever favorite compiler. Then generate the INSTALL project in visual studio or make install with gcc for exemple.

## Commit naming convention
- When fixing bug, by all means include the issue number.
- When completing feature if you have some task identifier, include that.
- If the identifier included goes with subject, it often makes sense to just reuse it. (**Use Core/XXX: "Commit name" format**)
- If the project is large, mention which component is affected.
- **Describe the change in functionality** , **never** the code (that can be seen from the diff).
