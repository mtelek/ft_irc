#!/bin/bash

# Check for argument
if [ -z "$1" ]; then
    echo "Usage: organize <directory>"
    exit 1
fi

# Get absolute path
TARGET_DIR=$(realpath "$1")

# Create folders if they don't exist
mkdir -p "$TARGET_DIR/src"
mkdir -p "$TARGET_DIR/inc"

# Move files
find "$TARGET_DIR" -maxdepth 1 -name "*.cpp" -exec mv {} "$TARGET_DIR/src/" \;
find "$TARGET_DIR" -maxdepth 1 -name "*.hpp" -exec mv {} "$TARGET_DIR/inc/" \;

echo "Organized files in $TARGET_DIR"

