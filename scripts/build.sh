#!/bin/bash

# CodeTime Build Script

echo "Building CodeTime Compiler..."

# Check if we're in the right directory
if [ ! -f "Makefile" ]; then
    echo "Error: Makefile not found. Please run this script from the codetime directory."
    exit 1
fi

# Clean previous build
echo "Cleaning previous build..."
make clean

# Build the compiler
echo "Building compiler..."
make

if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "Compiler binary: bin/codetime"
    echo ""
    echo "You can now use the compiler with:"
    echo "  ./bin/codetime build <file.cdt>"
    echo "  ./bin/codetime run <file.cdt>"
    echo "  ./bin/codetime check <file.cdt>"
else
    echo "Build failed!"
    exit 1
fi
