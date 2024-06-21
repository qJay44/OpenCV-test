@echo off
cls

Rem Build
if not exist Build mkdir Build
cd Build
cmake -S .. -B . -G"Visual Studio 17 2022" -D CMAKE_BUILD_TYPE=Release
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.com" MyProject.sln /Rebuild "Release|x64" /Out log.txt

Rem Copy dll
echo n | copy /-y "C:\Users\gerku\Documents\opencv\install\opencv\x64\vc17\bin\opencv_world4100.dll" Release\

Rem Compile commands
if not exist cc mkdir cc
cd cc
cmake -S ..\.. -B . -G"MinGW Makefiles" -D CMAKE_EXPORT_COMPILE_COMMANDS=ON
xcopy /y compile_commands.json ..\compile_commands.json

Rem Lauch
cd ..\Release
MyProject.exe exampleImage.png
cd ..\..

