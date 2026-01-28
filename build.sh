#!/bin/bash

echo "Building Temperature Monitor..."

mkdir -p build
cd build

echo "Configuring with CMake..."
cmake ..

echo "Building..."
make

echo "Done!"
echo "Executables created in build directory:"
echo "  - temperature_monitor"
echo "  - device_simulator"

cd ..
