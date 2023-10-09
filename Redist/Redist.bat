@echo off
chcp 65001
rmdir /s /q "OpenS100"

xcopy /q /e /y "..\Release_x64\*" "OpenS100\bin\"
xcopy /q /e /y "..\ProgramData\*" "OpenS100\ProgramData\"
xcopy /q /e /y "..\extlibs\Lua\bin\x64\*" "OpenS100\bin\"