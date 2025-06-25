cmake_minimum_required(VERSION 3.15)

project(chip_emulator)

set(CMAKE_C_STANDARD 99)
# set(LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
#
# find_library(LIBRARY_NAME
# NAMES
# PATHS ${LIBRARY_PATH}
# REQUIRED
# )
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall")

file(GLOB_RECURSE SOURCE src/*.c src/*.h)
file(GLOB_RECURSE LIB lib/*.c lib/*.h)
file(GLOB_RECURSE HEADER include/*.c include/*.h)

add_executable(chip_emulator
               src/main.c
               ${SOURCE}
               ${LIB}
               ${HEADER})

target_include_directories(chip_emulator PUBLIC src lib include)
# target_link_libraries(chip_emulator ${LIBRARY_NAME})
