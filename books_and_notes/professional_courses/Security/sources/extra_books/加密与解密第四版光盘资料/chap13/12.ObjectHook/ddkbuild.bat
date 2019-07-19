@echo off
setlocal
::
:: $Header: /test/LocalBin/ddkbuild.bat,v 1.8 2005/05/02 02:54:11 markr Exp $
::
set version=3.12.35
@echo DDKBUILD VERSION %version% FREEWARE FROM HOLLIS TECHNOLOGY SOLUTIONS
@echo Comments? Suggestions? info@hollistech.com

set scriptDebug=off
if "%1" NEQ "-debug" goto nodebug
set scriptDebug=on
shift

:nodebug
@echo %scriptDebug%

set verbose=0
if "%1" NEQ "-verbose" goto noverbose
set verbose=1
shift

:noverbose
if %verbose% EQU 1 @echo check shell

set status=0

VERIFY OTHER 2>nul
setlocal ENABLEEXTENSIONS
if ERRORLEVEL 1 goto wrongplatform
::
:: some shells is different
::
set foo=dog
if %foo% EQU cat (
    set foo=rat 
) else (
    set foo=cat
)
if %foo% EQU dog goto :nt4ddkbuild   

if /I "%1" EQU "/?"    goto usage   
::
:: next test, some shells is different
::
setlocal enabledelayedexpansion
set VAR=before
if "%VAR%" == "before" (
     set VAR=after
     if "!VAR!" == "after" (
        :: @echo W2K or later system
        call :ddkbuild %*
      ) else (
        @echo nt4 or earlier system
        call :nt4ddkbuild %*
      )
)

set status=0
goto buildExit
::
:: ================
:: the latest and greatest ddkbuild
:: ================
::
:ddkbuild

if %verbose% EQU 1 @echo process args
if "%1" == "-debug" shift
if "%1" == "-verbose" shift

set w2kflag=
:: ================
:: NOTE: w2kflag appears to be somewhat obsolete,
:: at best it is either null or non-null.
:: buildbsc may use its value, but the setting of 
:: that value is inconsistent.
:: ================
::
set chk=checked
set fre=free
set xp64=
set xp2k=0
set wnet=
set wlh=
set ddk=UNKNOWN
set prefast=
set mode=
set setenv=setenv.bat
set wdf=0

if "%1" EQU ""     goto usage
if /I %1 EQU -h  goto usage
if /I %1 EQU -help goto usage
::
:: test for all known ddk variations
::
:: =========================
:: W2K DDK Support (Obsolete)
:: =========================
::
:: 64bit w2k
::
if /I "%1" EQU "-W2K64" (
    set w2kflag=W2K64
    if "%W2K64BASE%" EQU "" goto NoW2k64Base
    set BASEDIR=%W2K64BASE%
    set ddk=W2K
    shift
    goto buildswitchdone
) 
::
:: regular w2k
::
if /I "%1" EQU "-W2K" (
    set w2kflag=W2K
    shift
    if "%W2KBASE%" EQU "" goto NoW2kBase
    set BASEDIR=%W2KBASE%
    set ddk=W2K
    goto buildswitchdone
)
::
:: =========================
:: XP DDK Support (Obsolete)
:: =========================
::
:: regular xp
::
if /I "%1" EQU "-XP" (
    set w2kflag=W2K
    shift
    if "%XPBASE%" EQU "" goto NoXPBase    
    set BASEDIR=%XPBASE%
    set chk=chk
    set fre=fre
    set ddk=XP
    goto buildswitchdone
) 
::
:: 64bit xp
::
if /I "%1" EQU "-XP64" (
    set w2kflag=W2K
    shift
    if "%XPBASE%" EQU "" goto NoXPBase    
    set BASEDIR=%XPBASE%
    set chk=chk
    set fre=fre
    set xp64=64
    set ddk=XP
    goto buildswitchdone
) 
::
:: w2k build/xp ddk
::
if /I "%1" EQU "-XPW2K" (
    set w2kflag=W2K
    set xp2k=1
    shift
    if "%XPBASE%" EQU "" goto NoXPBase    
    set BASEDIR=%XPBASE%
    set chk=checked
    set fre=free
    set ddk=XP
    goto buildswitchdone
)
::
:: =========================
:: NET DDK Support
:: =========================
::
:: .net ddk .net build
::
if /I "%1" EQU "-WNET" (
    set w2kflag=W2K
    shift
    if "%WNETBASE%" EQU "" goto NoWNBase    
    set BASEDIR=%WNETBASE%
    set chk=chk
    set fre=fre
    set wnet=wnet
    set ddk=NET
    goto buildswitchdone
)
::
:: .net ddk w2k build
::
if /I "%1" EQU "-WNETW2K" (
    set w2kflag=NET
    shift
    if "%WNETBASE%" EQU "" goto NoWNBase    
    set BASEDIR=%WNETBASE%
    set chk=chk
    set fre=free
    set wnet=w2k
    set ddk=NET
    goto buildswitchdone
)
::
:: .net ddk xp build
::
if /I "%1" EQU "-WNETXP" (
    set w2kflag=NET
    shift
    if "%WNETBASE%" EQU "" goto NoWNBase    
    set BASEDIR=%WNETBASE%
    set chk=chk
    set fre=fre
    set wnet=wxp
    set ddk=NET
    goto buildswitchdone
)
::
:: .net ddk IA64 build
::
if /I "%1" EQU "-WNET64" (
    set w2kflag=NET
    shift
    if "%WNETBASE%" EQU "" goto NoWNBase    
    set BASEDIR=%WNETBASE%
    set chk=chk
    set fre=fre
    set xp64=64
    set wnet=wnet
    set ddk=NET
    goto buildswitchdone
)
::
:: .net ddk AMD64 build
::
if /I "%1" EQU "-WNETA64" (
    set w2kflag=NET
    shift
    if "%WNETBASE%" EQU "" goto NoWNBase    
    set BASEDIR=%WNETBASE%
    set chk=chk
    set fre=fre
    set xp64=AMD64
    set wnet=wnet
    set ddk=NET
    goto buildswitchdone
)
::
:: ============================
:: LONGHORN DDK SUPPORT (BETA)
:: ============================
::
:: wlh ddk wlh build
::
if /I "%1" EQU "-WLH" (
    set w2kflag=W2K
    shift
    if "%WLHBASE%" EQU "" goto NoWLHBase    
    set BASEDIR=%WLHBASE%
    set chk=chk
    set fre=fre
    set wnet=wlh
    set ddk=WLH
    goto buildswitchdone
)
::
:: wlh ddk IA64 net build
::
if /I "%1" EQU "-WLH64" (
    set w2kflag=NET
    shift
    if "%WLHBASE%" EQU "" goto NoWLHBase    
    set BASEDIR=%WLHBASE%
    set chk=chk
    set fre=fre
    set xp64=64
    set wnet=wlh
    set ddk=WLH
    goto buildswitchdone
)
::
:: wlh ddk AMD64 net build
::
if /I "%1" EQU "-WLHA64" (
    set w2kflag=NET
    shift
    if "%WLHBASE%" EQU "" goto NoWLHBase    
    set BASEDIR=%WLHBASE%
    set chk=chk
    set fre=fre
    set xp64=AMD64
    set wnet=wlh
    set ddk=WLH
    goto buildswitchdone
)
::
:: wlh ddk w2k build
::
if /I "%1" EQU "-WLHW2K" (
    set w2kflag=NET
    shift
    if "%WLHBASE%" EQU "" goto NoWLHBase    
    set BASEDIR=%WLHBASE%
    set chk=chk
    set fre=free
    set wnet=w2k
    set ddk=WLH
    goto buildswitchdone
)
::
:: wlh ddk xp build
::
if /I "%1" EQU "-WLHXP" (
    set w2kflag=NET
    shift
    if "%WLHBASE%" EQU "" goto NoWLHBase    
    set BASEDIR=%WLHBASE%
    set chk=chk
    set fre=fre
    set wnet=wxp
    set ddk=WLH
    goto buildswitchdone
)
::
:: wlh ddk xp IA64 build
::
if /I "%1" EQU "-WLHXP64" (
    set w2kflag=NET
    shift
    if "%WLHBASE%" EQU "" goto NoWLHBase    
    set BASEDIR=%WLHBASE%
    set chk=chk
    set fre=fre
    set wnet=wxp
    set xp64=64
    set ddk=WLH
    goto buildswitchdone
)

:: wlh ddk .net build
if /I "%1" EQU "-WLHNET" (
    set w2kflag=W2K
    shift
    if "%WLHBASE%" EQU "" goto NoWLHBase    
    set BASEDIR=%WNETBASE%
    set chk=chk
    set fre=fre
    set wnet=wnet
    set ddk=WLH
    goto buildswitchdone
)
::
:: wlh ddk IA64 net build
::
if /I "%1" EQU "-WLHNET64" (
    set w2kflag=NET
    shift
    if "%WLHBASE%" EQU "" goto NoWLHBase    
    set BASEDIR=%WLHBASE%
    set chk=chk
    set fre=fre
    set xp64=64
    set wnet=wnet
    set ddk=WLH
    goto buildswitchdone
)
::
:: wlh ddk AMD64 net build
::
if /I "%1" EQU "-WLHNETA64" (
    set w2kflag=NET
    shift
    if "%WLHBASE%" EQU "" goto NoWLHBase    
    set BASEDIR=%WLHBASE%
    set chk=chk
    set fre=fre
    set xp64=AMD64
    set wnet=wnet
    set ddk=WLH
    goto buildswitchdone
)
::
:: ============================
:: WDF Build Support - (BETA)
:: ============================
::  needs WDF_ROOT and
::  WDF_DDK set in environment.
::  ONLY supports WNET XP and NET builds.
::
if /I "%1" EQU "-WDF" (
    set w2kflag=NET
    shift
    if "%WDF_ROOT%" EQU "" goto NoWDFroot
    if "%WDF_DDK%" EQU  "" goto NoWDFddk
    set BASEDIR=%WDF_DDK%
    set chk=chk
    set fre=fre
    set wnet=wnet
    set ddk=NET
    set wdf=1    
    goto buildswitchdone
)
if /I "%1" EQU "-WDFXP" (
    set w2kflag=NET
    shift
    if "%WDF_ROOT%" EQU "" goto NoWDFroot
    if "%WDF_DDK%" EQU  "" goto NoWDFddk
    set BASEDIR=%WDF_DDK%
    set chk=chk
    set fre=fre
    set wnet=wxp
    set ddk=NET
    set wdf=1  
    goto buildswitchdone
)

:buildswitchdone
::
:: =====================
:: arg sanity tests
:: =====================
::
if %verbose% EQU 1 @echo sanity tests

if "%w2kflag%" EQU "" goto BadTarget

if "%BASEDIR%" EQU "" goto ErrNoBASEDIR
set path=%BASEDIR%\bin;%path%
if "%scriptDebug%" EQU "on" @echo PATH: %PATH%
::
:: =====================
:: check drvfast switch
:: =====================
::
if /I "%1" EQU "-drvfast" (
    if %verbose% EQU 1 @echo drvfast support
    set prefast=prefast
    if EXIST %basedir%\bin\drvfast.cmd (
        set setenv=drvfast.cmd
    ) else if EXIST %basedir%\bin\drvfast.bat (
        set setenv=drvfast.bat
    )     
    shift
)    
::
:: =====================
:: check prefast switch
:: =====================
::
if /I "%1" EQU "-prefast" (
    if %verbose% EQU 1 @echo prefast support 
    set prefast=prefast
    shift
)
::
:: =====================
:: check CUV switch
:: =====================
::
if /I "%1" EQU "-cuv" (
    if %verbose% EQU 1 @echo CUV support
    set VERIFIER_DDK_EXTENSIONS=1
    shift
)    
::
:: END of position independent switches
::
if /I "%1" EQU "free" (

    set mode=%fre%

) else if /I "%1" EQU "checked" (

    set mode=%chk%

)

if "%mode%" EQU "" goto ErrBadMode
shift

if "%1" EQU "" goto ErrNoDir
if not exist %1 goto ErrNoDir
set buildDir=%1
shift

set batfile=%BASEDIR%\bin\%setenv%
if "%xp2k%" EQU "1" set batfile=%BASEDIR%\bin\w2k\set2k.bat
::
:: =====================
:: invoke the DDK's setenv script
:: =====================
::
if %verbose% EQU 1 @echo run ddk setenv
pushd .
if "%w2kflag%" NEQ "" (
    if %verbose% EQU 1 @echo check for .net sp1 ddk
    for /F "usebackq"  %%x IN (`findstr /M "_BscMake" %batfile%`) do (
        if %verbose% EQU 1 @echo call %batfile% %BASEDIR% %mode% %xp64% %wnet% bscmake
        call %batfile% %BASEDIR% %mode% %xp64% %wnet% bscmake
        goto wnetddkdone
    )
    if %verbose% EQU 1 @echo call %batfile% %BASEDIR% %mode% %xp64% %wnet%
    call %batfile% %BASEDIR% %mode% %xp64% %wnet%
    
) else (

    call %batfile% %BASEDIR% %mode% 
)

:wnetddkdone    
popd
::
:: ======================
:: latest WDF beta has its own extra setenv script, so run it if it exists
:: =======================
::   

if %verbose% EQU 1 @echo WDF_ROOT "%WDF_ROOT%" wdf "%wdf%" 
if %wdf% EQU 1 (    
    if %verbose% EQU 1 @echo check for wdf environment scripts
    if EXIST %WDF_ROOT%\set_wdf_env.cmd (
        if %verbose% EQU 1 @echo run wdf set env
        call %WDF_ROOT%\set_wdf_env.cmd
    )
)

@echo %scriptDebug%
::
:: =====================
:: fixup the multiprocessor flag
:: so that Visual Studio doesn't get confused
:: =====================
::
set mpFlag=-M
if "%BUILD_ALT_DIR%" EQU "" goto NT4
set W2kEXT=%BUILD_ALT_DIR%
set mpFlag=-MI
:NT4
if "%NUMBER_OF_PROCESSORS%"=="" set mpFlag=
if "%NUMBER_OF_PROCESSORS%" EQU "1" set mpFlag=
set extraArgs=%~1
::
:: =====================
:: add any locally generated extra args to build command
:: =====================
::
cd /d %2
set bflags=-e
set bscFlags=""

if "%extraArgs%" NEQ "" (

    if "%extraArgs%"  EQU  "/a" (
        set bscFlags=/n
        set bflags=-cfe 

    ) else (
        set bscFlags=/n
        set bflags=%extraArgs% -e
    )
)
shift
::
:: ===================
:: add any remaining commandline arguments to extraArgs
:: ===================
::
set moreExtraArgs=%1 %2 %3 %4 %5 %6 %7 %8 %9

if EXIST build%W2kEXT%.err  erase build%W2kEXT%.err
if EXIST build%W2kEXT%.wrn  erase build%W2kEXT%.wrn
if EXIST build%W2kEXT%.log  erase build%W2kEXT%.log

@echo.
@echo DDKBUILD using %ddk% DDK in directory %buildDir% 
@echo  for %mode% version (basedir %BASEDIR% extension %W2kEXT%)
@echo  commandline: "%prefast% build %bflags% %mpFlag% %moreExtraArgs%" 
@echo.
if %verbose% EQU 1 @echo Using path %path%
pushd .
pushd %buildDir%
%prefast% build  %bflags% %mpFlag% %moreExtraArgs%
set status=%ERRORLEVEL%
if %status%==1 set status=0
popd
popd

@echo %scriptDebug%
::
:: ===================
:: assume that the onscreen errors are complete!
:: ===================
::
@echo =============== build warnings ======================
if exist build%W2kEXT%.log (
    findstr "warning.*[CLU][0-9]*" build%W2kEXT%.log
)

if exist build%W2kEXT%.err (
    set status=3
    @echo.
    @echo warnings found in build!
    @echo.
)

if exist build%W2kEXT%.err (
    set status=4
    @echo.
    @echo errors found in build!
    @echo.
)   

@echo. 
@echo. 
@echo build complete status %status%
::
:: ===================
:: BSCMAKE support
:: ===================
::
@echo building browse information files

@echo %scriptDebug%

if EXIST buildbrowse.cmd goto doBrowsescript
set sbrlist=sbrList.txt
if not EXIST sbrList%CPU%.txt goto sbrDefault
set sbrlist=sbrList%CPU%.txt

:sbrDefault
if not EXIST %sbrlist% goto postBuildEvents
if %bscFlags% == "" goto noBscFlags
bscmake %bscFlags% @%sbrlist%
goto buildExit
::
:: ===================
:: error handlers
:: ===================
::
:noBscFlags
bscmake @%sbrlist%
goto buildExit

:doBrowsescript
call buildBrowse %mode% %w2kflag%
goto buildExit

:ErrBadMode
@echo error: first param must be "checked" or "free"
goto usage

:ErrNoBASEDIR
@echo error: BASEDIR environment variable not set, reinstall DDK!
goto usage

:NoW2kBase
@echo error: W2KBASE environment variable not set!
goto usage

:NoW2k64Base
@echo error: W2K64BASE environment variable not set!
goto usage

:NoXPBase
@echo error: XPBASE environment variable not set!
goto usage

:NoWNBase
@echo error: WNETBASE environment variable not set!
goto usage

:NoWLHBase
@echo error: WLHBASE environment variable not set!
goto usage

:ErrnoDir
@echo Error: invalid build directory specified
goto usage

:BadTarget
@echo Error: invalid TARGET specified
goto usage

:NoWDFroot
@echo Error: WDF_ROOT environment variable not set!
goto usage

:NoWDFddk
@echo Error: WDF_DDK environment variable not set!
goto usage

:usage
start http:\\www.hollistech.com\Resources\ddkbuild\ddkbuildhelp3_12.htm

@echo usage: ddkbuild [-debug^|-verbose] "TARGET" [-prefast^|-drvfast^|-cuv] "checked | free" "directory-to-build" [flags] 
@echo.
@echo       -debug      turns on script echoing for debug purposes
@echo.
@echo       -verbose    announce various script activities to standard out
@echo.
@echo        TARGET     can be any of the following combinations of DDK and target platform:
@echo.
@echo        -W2K       indicates development system uses W2KBASE environment variable
@echo                    to locate the win2000 ddk, otherwise BASEDIR is used (optional.)
@echo.
@echo        -W2K64     indicates development syatem uses W2K64BASE environment variable
@echo                    to locate the win2000 64 ddk, otherwise BASEDIR is used (optional.)
@echo.
@echo        -XP        indicates development system uses XPBASE environment variable
@echo                    to locate the XP ddk, otherwise BASEDIR is used (optional.)
@echo.
@echo        -XP64      indicates development system uses XPBASE environment variable
@echo                    to locate the XP ddk and builds IA64 binaries (optional.)
@echo.
@echo        -XPW2K     indicates development system uses the XPBASE environment variable
@echo                    to locate the XP ddk and builds W2K binaries (optional.)
@echo.
@echo        -WNET      indicates development system uses WNETBASE environment variable
@echo                    to locate the .Net ddk and builds .net binaries (optional.)
@echo.
@echo        -WNETW2K   indicates development system uses the WNETBASE environment variable
@echo                    to locate the .Net ddk and builds W2K binaries (optional.)
@echo.
@echo        -WNETXP    indicates development system uses WNETBASE environment variable
@echo                    to locate the .Net ddk and builds xp binaries (optional.)
@echo.
@echo        -WNET64    indicates development system uses WNETBASE environment variable
@echo                    to locate the .Net ddk and builds 64bit binaries (optional.)
@echo.
@echo        -WNETA64   indicates development system uses WNETBASE environment variable
@echo                    to locate the .Net ddk and builds AMD 64bit binaries (optional.)
@echo.
@echo        -WLH       indicates development system uses the WHLBASE environment variable
@echo                    to locate the Longhorn ddk and builds Longhorn binaries (optional.)
@echo.
@echo        -WLH64     indicates development system uses the WHLBASE environment variable
@echo                    to locate the Longhorn ddk and builds IA64 Longhorn binaries (optional.)
@echo.
@echo        -WLHA64    indicates development system uses the WHLBASE environment variable
@echo                    to locate the Longhorn ddk and builds AMD64 Longhorn binaries (optional.)
@echo.
:: @echo        -WLHW2K    indicates development system uses the WHLBASE environment variable
:: @echo                    to locate the Longhorn ddk and builds W2K binaries (optional.)
:: @echo.
:: @echo        -WLHXP     indicates development system uses the WHLBASE environment variable
:: @echo                    to locate the Longhorn ddk and builds XP binaries (optional.)
:: @echo.
:: @echo        -WLHXP64   indicates development system uses the WHLBASE environment variable
:: @echo                    to locate the Longhorn ddk and builds XP IA64 binaries (optional.)
:: @echo.
@echo        -WLHNET    indicates development system uses the WHLBASE environment variable
@echo                    to locate the Longhorn ddk and builds .net binaries (optional.)
@echo.
:: @echo        -WLHNET64  indicates development system uses the WHLBASE environment variable
:: @echo                    to locate the Longhorn ddk and builds IA64 .bet binaries (optional.)
:: @echo.
:: @echo        -WLHNETA64 indicates development system uses the WHLBASE environment variable
:: @echo                    to locate the Longhorn ddk and builds AMD64 .net binaries (optional.)
:: @echo.
@echo       -WDF    indicates development system uses the WDF_DDK environment variable
@echo                to locate the WDF supported DDK and builds a W2K3 binary (optional).
@echo.
@echo       -WDFXP  indicates development system uses the WDF_DDK environment variable
@echo                to locate the WDF supported DDK and builds an XP binary (optional). 
@echo.
@echo       -prefast   run prefast rather than a normal build
@echo.
@echo       -drvfast   run the 'driver fast' version of prefast
@echo.
@echo       -cuv    use code usage verifier when building driver
@echo.
@echo        checked    indicates a checked build.
@echo.
@echo        free       indicates a free build (must choose one or the other of free or checked.)
@echo.
@echo        directory  path to build directory, try . (cwd).
@echo.
@echo        flags      any random flags or arguments you think should be passed to build (note that the
@echo                    visual studio /a for clean build is translated to the equivalent build flag.)
@echo                    Note also that multiple arguments can be specified by using quotes to contain
@echo                    the set of arguments, as in "-Z foo blortz w2k xp"
@echo.        
@echo         ex: ddkbuild -XP checked . 
@echo.
@echo         NOTE: windows .net DDK versions supported must be build 3663 or later 
@echo.

set status=2
goto buildExit

:: ======================
:: post build events
:: currently supports prefast view
:: ======================
:postBuildEvents
    if "%prefast%" NEQ "" (
        prefast view
    )
goto buildExit

:: ======================
:: bad shell error handlers
:: ======================

:wrongplatform
@echo Sorry: NT4/W2K/XP/.net only! 
set status=5
goto buildExit

:nt4ddkbuild

@echo Sorry ddkbuild supports windows2000 or later platforms only
set status=6
goto buildExit

:buildExit
exit /b %status%