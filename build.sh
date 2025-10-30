#!/usr/bin/env bash
set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Default values
BUILD_TYPE="Release"
BUILD_DIR="build"
JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --build-dir)
            BUILD_DIR="$2"
            shift 2
            ;;
        --jobs|-j)
            JOBS="$2"
            shift 2
            ;;
        --clean)
            CLEAN=true
            shift
            ;;
        --help|-h)
            echo "Usage: $0 [OPTIONS]"
            echo ""
            echo "Options:"
            echo "  --debug          Build in Debug mode (default: Release)"
            echo "  --build-dir DIR  Specify build directory (default: build)"
            echo "  --jobs N, -j N   Number of parallel jobs (default: number of cores)"
            echo "  --clean          Clean build directory before building"
            echo "  --help, -h       Show this help message"
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

echo -e "${GREEN}=== Conway's Game of Life Build Script ===${NC}"
echo -e "Build Type: ${YELLOW}${BUILD_TYPE}${NC}"
echo -e "Build Directory: ${YELLOW}${BUILD_DIR}${NC}"
echo -e "Parallel Jobs: ${YELLOW}${JOBS}${NC}"
echo ""

# Clean if requested
if [[ "$CLEAN" == true ]]; then
    echo -e "${YELLOW}Cleaning build directory...${NC}"
    rm -rf "${BUILD_DIR}"
fi

# Step 1: Install Conan dependencies
echo -e "${GREEN}Step 1: Installing Conan dependencies...${NC}"
if ! conan install . \
    --output-folder="${BUILD_DIR}" \
    --build=missing \
    --settings=build_type="${BUILD_TYPE}"; then
    echo -e "${RED}Conan install failed!${NC}"
    exit 1
fi
echo ""

# Step 2: Configure CMake with Conan toolchain
echo -e "${GREEN}Step 2: Configuring CMake...${NC}"
if ! cmake -B "${BUILD_DIR}" \
    -DCMAKE_TOOLCHAIN_FILE="${BUILD_DIR}/build/${BUILD_TYPE}/generators/conan_toolchain.cmake" \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"; then
    echo -e "${RED}CMake configuration failed!${NC}"
    exit 1
fi
echo ""

# Step 3: Build
echo -e "${GREEN}Step 3: Building project...${NC}"
if ! cmake --build "${BUILD_DIR}" -j"${JOBS}"; then
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi
echo ""

# Success message
echo -e "${GREEN}=== Build completed successfully! ===${NC}"
echo ""
echo -e "Executables:"
echo -e "  Game:  ${YELLOW}${BUILD_DIR}/game_of_life${NC}"
echo -e "  Tests: ${YELLOW}${BUILD_DIR}/gol_unit_tests${NC}"
echo ""
echo -e "Run with:"
echo -e "  ${YELLOW}./${BUILD_DIR}/game_of_life <pattern-file> [--wrapped] [--random]${NC}"
echo -e "  ${YELLOW}./${BUILD_DIR}/gol_unit_tests${NC}"
