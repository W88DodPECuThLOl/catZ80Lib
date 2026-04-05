@echo off
setlocal enabledelayedexpansion

set TARGET=libCatZ80.lib

set BASE_DIR=%~dp0
set SRC_DIR=%BASE_DIR%src
set OBJ_DIR=%BASE_DIR%obj

REM sdcc --version
REM SDCC : mcs51/z80/z180/r2k/r2ka/r3ka/sm83/tlcs90/ez80_z80/z80n/r800/ds390/pic16/pic14/TININative/ds400/hc08/s08/stm8/pdk13/pdk14/pdk15/mos6502/mos65c02/f8 TD- 4.5.1 #15295 (MINGW32)
REM published under GNU General Public License (GPL)
set C=sdcc
set C_FLAGS=-mz80 --asm=asxxxx --std-c23 --disable-warning 110
set C_OPT_FLAGS=--opt-code-speed --allow-undocumented-instructions

REM sdasz80
REM sdas Assembler V02.00 + NoICE + SDCC mods  (Zilog Z80 / Hitachi HD64180 / ZX-Next / eZ80 / R800)
set ASM=sdasz80
set ASM_FLAGS=-l -s

REM sdar -V
REM GNU ar (sdbinutils derived from GNU Binutils) 2.38
REM Copyright (C) 2022 Free Software Foundation, Inc.
set AR=sdar
set AR_FLAGS=

echo.
echo [96mMakeTarget  %TARGET%[0m
echo [96mSourceFiles %SRC_DIR%[0m
echo.

:main
	pushd %~dp0
    call :clean_up

    echo.
    echo =========================================================
    @echo [96massemble[0m
    echo =========================================================
    echo.
    for /r "%SRC_DIR%" %%s in (*.asm) do (
        call :assemble %%s
        if %errorlevel% neq 0 (
            echo [41mAssemble error "%%s"[0m
            pause
            exit /b 1
        )
    )

    echo.
    echo =========================================================
    @echo [96mcompile[0m
    echo =========================================================
    echo.
    for /r "%SRC_DIR%" %%s in (*.c) do (
        call :compile %%s
        if %errorlevel% neq 0 (
            echo [41mCompile error "%%s"[0m
            pause
            exit /b 1
        )
    )

    echo.
    echo =========================================================
    @echo [96mArchive[0m
    echo =========================================================
    echo.
    for /r "%OBJ_DIR%" %%s in (*.rel) do (
        call :archive %%s
        if %errorlevel% neq 0 (
            echo [41mArchive error "%%s"[0m
            pause
            exit /b 1
        )
    )

    echo.
    echo [92mSuccess "%TARGET%"[0m
    echo.

    popd
    ENDLOCAL
exit /b

:clean_up
    if exist "%OBJ_DIR%" (
        del "%OBJ_DIR%\*.o" 2> nul
        del "%OBJ_DIR%\*.rel" 2> nul
        del "%OBJ_DIR%\*.lst" 2> nul
        del "%OBJ_DIR%\*.sym" 2> nul
        del "%OBJ_DIR%\*.asm" 2> nul
        del "%OBJ_DIR%\*.ihx" 2> nul
        del "%OBJ_DIR%\*.lk" 2> nul
        del "%OBJ_DIR%\*.map" 2> nul
    ) else (
        mkdir "%OBJ_DIR%"
    )
    if exist "%TARGET%" (
        del "%TARGET%" 2> nul
    )
exit /b

:assemble
    @echo Assemble "%~nx1"
    %ASM% %ASM_FLAGS% -o "%OBJ_DIR%\%~n1.rel" "%1"
exit /b %errorlevel%

:compile
    @echo Compile "%~nx1"
    %C% %C_FLAGS% %C_OPT_FLAGS% -c "%1" -o "%OBJ_DIR%\%~n1.rel"
exit /b %errorlevel%

:archive
    @echo archive "%~nx1"
    %AR% -r %TARGET% "%1" 2> nul
exit /b %errorlevel%
