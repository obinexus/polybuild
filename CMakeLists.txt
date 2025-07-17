cmake_minimum_required(VERSION 3.16)
project(PolyBuild VERSION 1.0.0 LANGUAGES C)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Set C standard for systematic compilation
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# For multi-config builds (e.g. Visual Studio)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
endforeach()

# Create an obj/ directory
set(CMAKE_OBJECT_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)

# Compiler flags for systematic error checking
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -DDEBUG")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -DNDEBUG")
endif()

# Core module source files with systematic organization
set(CORE_SOURCES
    src/core/dag/dag.c
    src/core/trie/trie.c
    src/core/integration/trie_dag.c
)

# Create the main library
add_library(polybuild SHARED ${CORE_SOURCES})
add_library(polybuild_static STATIC ${CORE_SOURCES})

# Set library properties
set_target_properties(polybuild_static PROPERTIES
    OUTPUT_NAME polybuild
)

# Create test executable
add_executable(polybuild_test tests/main.c)
target_link_libraries(polybuild_test polybuild)

# Installation configuration
install(TARGETS polybuild polybuild_static
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/polybuild
)

install(DIRECTORY include/polybuild DESTINATION include)
