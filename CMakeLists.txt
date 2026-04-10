# =================================================================
# KATTAR KARACHI - BUILD CONFIGURATION
# This file tells CMake how to compile the game and link folders.
# =================================================================

cmake_minimum_required(VERSION 3.10)
project(KattarKarachi)

# 1. DIRECTORIES: Tell the compiler where our header (.h) files are
target_include_directories(KattarKarachi PRIVATE include)

# 2. SOURCE FILES: List all the .cpp files that need to be compiled
# (Add new .cpp files here as you create them)
set(SOURCES 
    src/main.cpp 
    src/Game.cpp 
    src/Player.cpp
    # Add other .cpp files here...
)

# 3. EXECUTABLE: Create the final playable game file
add_executable(KattarKarachi ${SOURCES})

# 4. LIBRARIES: If we add SFML or SDL later, we link them here
# target_link_libraries(KattarKarachi ...)