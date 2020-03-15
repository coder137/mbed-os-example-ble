@echo off

REM call compile.bat

echo "Running Flash script"
call mbed.py compile --target DISCO_L475VG_IOT01A --toolchain GCC_ARM --flash

echo "Starting Terminal"
call mbed.py sterm
