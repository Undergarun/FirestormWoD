## Overview
World of Warcraft Mists of Pandaria emulator based on TrinityCore project,  & JadeCore (5.0.5)

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


## Reporting issues

Issues can be reported via the [Redmine](http://195.154.172.112/redmine/projects/pandashan-interne).

Please take the time to review existing issues before submitting your own to
prevent duplicates.

##Commit naming convention
- When fixing bug, by all means include the issue number.
- When completing feature if you have some task identifier, include that.
- If the identifier included goes with subject, it often makes sense to just reuse it. (**Use Core/XXX: "Commit name" format**)
- If the project is large, mention which component is affected.
- Describe the change in functionality, **never** the code (that can be seen from the diff).
