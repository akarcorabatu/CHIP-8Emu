cmake_minimum_required(VERSION 3.10)
project(CHIP8EmuC)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set(SOURCES
src/main.c
src/chip8.c
)

add_executable(chip8emu ${SOURCES})

if(UNIX)
    target_link_libraries(chip8emu ${SDL2_LIBRARIES} m)
else()
    target_link_libraries(chip8emu ${SDL2_LIBRARIES})
endif()