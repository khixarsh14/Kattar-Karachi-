cmake_minimum_required(VERSION 3.10)
project(KattarKarachi)

set(SOURCES
    src/main.cpp
    src/game.cpp
    src/player.cpp
    src/map.cpp
    src/obstacle.cpp
    src/Pothole.cpp
    src/pushcart.cpp
    src/trashpile.cpp
    src/task.cpp
    src/TaskManager.cpp
    src/scoremanager.cpp
    src/UImanager.cpp
)

add_executable(game ${SOURCES})

target_include_directories(KattarKarachi PRIVATE include)

target_link_libraries(KattarKarachi
    raylib
    opengl32
    gdi32
    winmm
)