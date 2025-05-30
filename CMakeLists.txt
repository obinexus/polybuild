cmake_minimum_required(VERSION 3.16)
project(PolyBuild VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_C_FLAGS "setup-modules.ps1{CMAKE_C_FLAGS} -Wall -Wextra -pedantic")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "setup-modules.ps1{CMAKE_C_FLAGS} -g -O0 -DDEBUG")
else()
    set(CMAKE_C_FLAGS "setup-modules.ps1{CMAKE_C_FLAGS} -O2 -DNDEBUG")
endif()

# Include directories
include_directories(setup-modules.ps1{CMAKE_SOURCE_DIR}/src/core)
include_directories(setup-modules.ps1{CMAKE_SOURCE_DIR}/src/cli)

# Core module source files
set(CORE_SOURCES
    src/core/micro/micro.c
     src/core/repl/repl.c
     src/core/edge/edge.c
     src/core/crypto/crypto.c
     src/core/telemetry/telemetry.c

)

# CLI source files
set(CLI_SOURCES
    src/cli/main.c
    src/cli/micro_cli.c
     src/cli/repl_cli.c
     src/cli/edge_cli.c
     src/cli/crypto_cli.c
     src/cli/telemetry_cli.c

)

# Create the main executable
add_executable(polybuild setup-modules.ps1{CORE_SOURCES} setup-modules.ps1{CLI_SOURCES})

# Set output directory
set_target_properties(polybuild PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY setup-modules.ps1{CMAKE_BINARY_DIR}/bin
)

# Install configuration
install(TARGETS polybuild DESTINATION bin)

# Add custom target for clean build
add_custom_target(clean-all
    COMMAND setup-modules.ps1{CMAKE_COMMAND} --build . --target clean
    COMMAND setup-modules.ps1{CMAKE_COMMAND} -E remove_directory setup-modules.ps1{CMAKE_BINARY_DIR}/bin
)

# Print build information
message(STATUS "PolyBuild Configuration Summary:")
message(STATUS "  Version: setup-modules.ps1{PROJECT_VERSION}")
message(STATUS "  Build Type: setup-modules.ps1{CMAKE_BUILD_TYPE}")
message(STATUS "  C Compiler: setup-modules.ps1{CMAKE_C_COMPILER}")
message(STATUS "  Install Prefix: setup-modules.ps1{CMAKE_INSTALL_PREFIX}")
