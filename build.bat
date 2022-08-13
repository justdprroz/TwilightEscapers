@echo off
rem Generate directories

if not exist build\ mkdir build
if not exist tmp\ mkdir tmp

rem Move dlls and assets to build directory
xcopy assets\ build\assets\ /E /Y

rem Go to directories
cd tmp

rem Generate cmake files
cmake ..

rem Build projects
"MSBuild.exe" ".\Twilight Escapers.sln" /property:Configuration=Release

rem Return to project root
cd ..