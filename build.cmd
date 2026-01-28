@echo off
echo Building Temperature Monitor...

if not exist build mkdir build
cd build

echo Configuring with CMake...
cmake -G "MinGW Makefiles" ..

echo Building...
cmake --build .

echo Done!
cd ..
pause