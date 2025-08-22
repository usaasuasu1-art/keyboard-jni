#!/bin/bash

# ImGui Android Soft Keyboard Build Script
# Make sure you have Android NDK installed and ANDROID_NDK environment variable set

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Building ImGui Android Soft Keyboard...${NC}"

# Check if ANDROID_NDK is set
if [ -z "$ANDROID_NDK" ]; then
    echo -e "${RED}Error: ANDROID_NDK environment variable is not set${NC}"
    echo "Please set it to your Android NDK path:"
    echo "export ANDROID_NDK=/path/to/your/android-ndk"
    exit 1
fi

# Check if NDK exists
if [ ! -d "$ANDROID_NDK" ]; then
    echo -e "${RED}Error: ANDROID_NDK directory does not exist: $ANDROID_NDK${NC}"
    exit 1
fi

echo -e "${YELLOW}Using Android NDK: $ANDROID_NDK${NC}"

# Create build directory
BUILD_DIR="build"
if [ -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}Cleaning existing build directory...${NC}"
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure with CMake
echo -e "${YELLOW}Configuring with CMake...${NC}"
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK/build/cmake/android.toolchain.cmake" \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM=android-21 \
    -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo -e "${RED}CMake configuration failed${NC}"
    exit 1
fi

# Build the project
echo -e "${YELLOW}Building project...${NC}"
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed${NC}"
    exit 1
fi

# Check if library was created
LIB_PATH="libs/arm64-v8a/libBossUnknownX.so"
if [ -f "$LIB_PATH" ]; then
    echo -e "${GREEN}Build successful!${NC}"
    echo -e "${GREEN}Library created at: $LIB_PATH${NC}"
    
    # Show library info
    echo -e "${YELLOW}Library information:${NC}"
    file "$LIB_PATH"
    ls -lh "$LIB_PATH"
else
    echo -e "${RED}Error: Library not found at expected path${NC}"
    exit 1
fi

echo -e "${GREEN}Build completed successfully!${NC}"
echo -e "${YELLOW}You can now use the library in your Android project.${NC}"