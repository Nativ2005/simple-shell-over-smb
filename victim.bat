@echo off
setlocal enabledelayedexpansion


set "FILE_A=c:\Users\NativT\Documents\A.txt"
set "FILE_B=c:\Users\NativT\Documents\B.txt"
set "FILE_C=c:\Users\NativT\Documents\C.txt"
set "CURR_DIR=%cd%"
set "MYPC=%COMPUTERNAME%"


:loopC
if exist "%FILE_C%" (
    set /p allowed=<"%FILE_C%"
) else (
    set "allowed="
)
if /i "!allowed!" NEQ "!MYPC!" (
    timeout /t 1 /nobreak >nul
    goto loopC
)
:loop
if not exist "%FILE_A%" (
    timeout /t 1 /nobreak >nul
    goto loop
)
set /p cmd=<"%FILE_A%"

echo %cmd% | findstr /i "^cd" >nul
if not errorlevel 1 (
    set "arg=!cmd:~3!"
    if "!arg!"=="" (
        > "%FILE_B%" echo Current directory: !CURR_DIR!
	del "%FILE_A%"
        goto loop
    )

    if "!arg!"==".." (
        for %%i in ("!CURR_DIR!\..") do set "CURR_DIR=%%~fi"
        > "%FILE_B%" echo Changed directory to !CURR_DIR!
	del "%FILE_A%"
        goto loop
    )

    if exist "!arg!\" (
        for %%i in ("!arg!") do set "CURR_DIR=%%~fi"
        > "%FILE_B%" echo Changed directory to !CURR_DIR!
	del "%FILE_A%"
    ) else (
        > "%FILE_B%" echo Directory not found: !arg!
	del "%FILE_A%"
    )
    goto loop
)

if /i "!cmd!"=="exit" (
    del "%FILE_A%"
    goto loopC
)

pushd "!CURR_DIR!"
cmd /c "!cmd!" > "%FILE_B%" 2>&1
del "%FILE_A%"
goto loop
:end
