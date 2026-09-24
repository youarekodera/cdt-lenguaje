#!/bin/bash

# CodeTime Installation Script

INSTALL_PREFIX=${1:-/usr/local}

echo "Installing CodeTime Compiler to $INSTALL_PREFIX..."

# Check if we're in the right directory
if [ ! -f "Makefile" ]; then
    echo "Error: Makefile not found. Please run this script from the codetime directory."
    exit 1
fi

# Build the compiler first
echo "Building compiler..."
make

if [ $? -ne 0 ]; then
    echo "Build failed! Cannot install."
    exit 1
fi

# Install using make
echo "Installing..."
make install PREFIX=$INSTALL_PREFIX

if [ $? -eq 0 ]; then
    echo "Installation successful!"
    echo "CodeTime is now installed to $INSTALL_PREFIX/bin/codetime"
    echo ""
    echo "Make sure $INSTALL_PREFIX/bin is in your PATH."
else
    echo "Installation failed!"
    exit 1
fi
