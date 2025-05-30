cmake_minimum_required(VERSION 3.16)
project(PolyBuild VERSION 1.0.0 LANGUAGES C)

# Set C standard for systematic compilation
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler flags for systematic error checking
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -DDEBUG")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -DNDEBUG")
endif()

# Library consumption standardization - Public header exposure
include_directories(${CMAKE_SOURCE_DIR}/include)
target_include_directories(polybuild PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Core module source files with systematic organization
set(CORE_SOURCES
    src/core/crypto/crypto.c
     src/core/micro/micro.c
     src/core/edge/edge.c
     src/core/telemetry/telemetry.c
     src/core/repl/repl.c

)

# IOC Configuration system sources
set(CONFIG_CORE_SOURCES
    src/core/config/config_for_crypto.c
     src/core/config/config_for_edge.c
     src/core/config/config_for_micro.c
     src/core/config/config_for_repl.c
     src/core/config/config_for_telemetry.c
     src/core/config/config_ioc.c

)

# CLI Configuration sources  
set(CONFIG_CLI_SOURCES
    src/cli/config/config.c

)

# CLI interface sources
set(CLI_SOURCES
    src/cli/main.c
    src/cli/crypto_cli.c
     src/cli/micro_cli.c
     src/cli/edge_cli.c
     src/cli/telemetry_cli.c
     src/cli/repl_cli.c

)

# Create the main executable with systematic linking
add_executable(polybuild 
    ${CORE_SOURCES} 
    ${CONFIG_CORE_SOURCES} 
    ${CONFIG_CLI_SOURCES} 
    ${CLI_SOURCES}
)

# Create static library for external consumption
add_library(polybuild_static STATIC
    ${CORE_SOURCES}
    ${CONFIG_CORE_SOURCES}
)

# Library consumption setup
set_target_properties(polybuild_static PROPERTIES
    OUTPUT_NAME polybuild
    PUBLIC_HEADER "include/polybuild/core/config_ioc.h;include/polybuild/cli/config.h"
)

# External library dependencies
find_package(OpenSSL)
if(OPENSSL_FOUND)
    target_link_libraries(polybuild ${OPENSSL_LIBRARIES})
    target_link_libraries(polybuild_static ${OPENSSL_LIBRARIES})
    target_include_directories(polybuild PRIVATE ${OPENSSL_INCLUDE_DIR})
    target_include_directories(polybuild_static PRIVATE ${OPENSSL_INCLUDE_DIR})
    add_definitions(-DHAVE_OPENSSL)
endif()

# Set output directories for systematic organization
set_target_properties(polybuild PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(polybuild_static PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# Installation configuration for library consumption
install(TARGETS polybuild polybuild_static
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib  
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/polybuild
)

install(DIRECTORY include/polybuild DESTINATION include)

# Configuration validation targets
add_custom_target(config-validate
    COMMAND ${CMAKE_BINARY_DIR}/bin/polybuild config validate --all
    DEPENDS polybuild
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Validating PolyBuild IOC configuration system"
)

add_custom_target(schema-validate
    COMMAND ${CMAKE_BINARY_DIR}/bin/polybuild config show --module crypto
    COMMAND ${CMAKE_BINARY_DIR}/bin/polybuild config show --module micro  
    COMMAND ${CMAKE_BINARY_DIR}/bin/polybuild config show --module telemetry
    DEPENDS polybuild
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Validating all module configurations"
)

# Testing framework integration
enable_testing()
add_test(NAME ioc_system_test 
         COMMAND polybuild config list
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_test(NAME crypto_config_test 
         COMMAND polybuild config show --module crypto
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_test(NAME schema_validation_test
         COMMAND polybuild config validate --all  
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Build information for systematic validation
message(STATUS "PolyBuild Library Standardization Summary:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  OpenSSL Found: ${OPENSSL_FOUND}")
message(STATUS "  Core Modules: crypto, micro, edge, telemetry, repl")
message(STATUS "  IOC Configuration: Enabled")
message(STATUS "  Library Consumption: Standardized")
