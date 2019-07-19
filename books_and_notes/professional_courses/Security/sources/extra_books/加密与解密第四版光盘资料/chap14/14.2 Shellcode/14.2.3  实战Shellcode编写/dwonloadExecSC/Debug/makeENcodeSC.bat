@echo off

    if exist "ENcodeSC.obj" del "ENcodeSC.obj"
    if exist "ENcodeSC.exe" del "ENcodeSC.exe"

    \masm32\bin\ml /c /coff "ENcodeSC.asm"
    if errorlevel 1 goto errasm

    \masm32\bin\PoLink /SUBSYSTEM:CONSOLE "ENcodeSC.obj"
    if errorlevel 1 goto errlink
    dir "ENcodeSC.*"
    goto TheEnd

  :errlink
    echo _
    echo Link error
    goto TheEnd

  :errasm
    echo _
    echo Assembly Error
    goto TheEnd
    
  :TheEnd

pause
