#!/bin/bash

# CodeTime Uninstallation Script

INSTALL_PREFIX=${1:-/usr/local}

echo "Uninstalling CodeTime Compiler from $INSTALL_PREFIX..."

# Check if the binary exists
if [ ! -f "$INSTALL_PREFIX/bin/codetime" ]; then
    echo "CodeTime is not installed at $INSTALL_PREFIX/bin/codetime"
    exit 1
fi

# Remove the binary
echo "Removing binary..."
rm -f "$INSTALL_PREFIX/bin/codetime"

if [ $? -eq 0 ]; then
    echo "Uninstallation successful!"
    echo "CodeTime has been removed from $INSTALL_PREFIX/bin/codetime"
else
    echo "Uninstallation failed!"
    exit 1
fi
