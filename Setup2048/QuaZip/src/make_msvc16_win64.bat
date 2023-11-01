@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
set build=build_msvc16_win64
if not exist %build% mkdir %build%
cd %build%
cmake -G "Visual Studio 16 2019"  -DCMAKE_PREFIX_PATH="C:\\Qt\\5.15.1\\msvc2019_64;D:\\usr" ..
pause
