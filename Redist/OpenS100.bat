@echo off
chcp 65001
rmdir /s /q "OpenS100"
mkdir "OpenS100"
mkdir "OpenS100\bin"
mkdir "OpenS100\ProgramData"
mkdir "OpenS100\TEMP"

xcopy /q /e /y ..\Release_x64\*.dll "OpenS100\bin\"
xcopy /q /e /y "..\Release_x64\OpenS100.exe" "OpenS100\bin\"
xcopy /q /e /y ..\vcpkg\installed\x64-windows\bin\*.dll "OpenS100\bin\"
xcopy /q /e /y ..\extlibs\Lua\bin\x64\*.dll "OpenS100\bin\"

xcopy /q /e /y "..\ProgramData\*" "OpenS100\ProgramData\"

xcopy /q /e /y "..\TEMP\*" "OpenS100\TEMP\"