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
"C:\Program Files\Microsoft Visual Studio\2022\Community\Msbuild\Current\Bin\MSBuild.exe" ".\Twilight Escapers.sln" /property:Configuration=Release