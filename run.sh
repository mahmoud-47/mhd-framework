#!/bin/bash

# Exit on any error
set -e

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure the project with CMake
cmake ..

# Build the project
make

# Run the application
./application
