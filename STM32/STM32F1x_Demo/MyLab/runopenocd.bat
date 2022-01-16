@ echo off

tasklist|findstr /i "openocd.exe" > nul
if ERRORLEVEL 1 (
    echo openocd is closed, opne it.
    ::start cmd /k "openocd -f "C:/openocd/scripts/interface/stlink-v2.cfg" -f "C:/openocd/scripts/target/stm32f1x.cfg""
    start cmd /c openocd -f "C:/openocd/scripts/interface/stlink-v2.cfg" -f "C:/openocd/scripts/target/stm32f1x.cfg"
    goto end
) 
echo openocd has been open, close and restart.
rem taskkill /f /im openocd.exe
::delay 200ms
rem choice /t 1 /d y /n > nul
rem start cmd /k "openocd -f "C:/openocd/scripts/interface/stlink-v2.cfg" -f "C:/openocd/scripts/target/stm32f1x.cfg""

:end

::openocd.exe -f "C:/openocd/scripts/interface/stlink-v2.cfg" -f "C:/openocd/scripts/target/stm32f1x.cfg"
