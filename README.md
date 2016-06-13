## Overview
World of Warcraft Warlords of Draenor (6.X) emulator based on TrinityCore project, JadeCore (5.0.5), Garrosh (5.4.X)

### BEWARE ! 
Never commit into interrealm only crash fix, wod branch is the prod branch !

[![Build Status](https://drone.firestorm-servers.com/api/badges/MilleniumStudio/wod/status.svg)](https://drone.firestorm-servers.com/MilleniumStudio/wod)

## Requirements

+ Platform: Linux, Windows or Mac
+ Git ≥ 1.7+
+ ACE ≥ 6.2.0
+ MySQL ≥ 5.5.0+
+ CMake ≥ 3.2+
+ OpenSSL ≥ 1.0.0
+ GCC ≥ 4.7.2 (Linux only)
+ MS Visual Studio ≥ 12 (2013) (Windows only)
+ Xcode 7 (Mac only)


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

- If you have the message `Git was NOT FOUND on your system` please uninstall and update your git version (you can also change the path for the binary on yout PATH)
- **NEW** Windows user you need the mysql server installed, download the last version (5.7+) here : [MySQL Community Server](http://dev.mysql.com/downloads/mysql/) if you need the x86 build just change the include path on cmake
- **NEW** If you use MSVC 2013 you *MUST* have the update 3+

## Commit naming convention
- When fixing bug, by all means include the issue number.
- When completing feature if you have some task identifier, include that.
- If the identifier included goes with subject, it often makes sense to just reuse it. (**Use Core/XXX: "Commit name" format**)
- If the project is large, mention which component is affected.
- **Describe the change in functionality** , **never** the code (that can be seen from the diff).

####How to Cppcheck
run ```cppcheck --enable=all --xml-version=2 > PathToFile/cppcheck.xml --inconclusive path PathToFile/wod/src/ --verbose```

and run ```./cppcheck-htmlreport --file=PathToFile/cppcheck.xml --source-dir=PathToFile/wod/src --report-dir=PathToFile/wod/cppcheck/``` to get the html out