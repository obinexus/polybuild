cmake_minimum_required(VERSION 3.16)
project(PolyBuild VERSION 1.0.0 LANGUAGES C)

# Project metadata for systematic identification
set(POLYBUILD_DESCRIPTION "Modular Build System for Polyglot Projects")
set(POLYBUILD_VENDOR "Aegis Project - OBINexus Computing")
set(POLYBUILD_COPYRIGHT "Copyright (c) 2025 OBINexus Computing")

# Set C standard for systematic compilation across platforms
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Systematic compiler configuration
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -DDEBUG")
    add_definitions(-DPOLYBUILD_DEBUG=1)
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -DNDEBUG")
    add_definitions(-DPOLYBUILD_RELEASE=1)
endif()

# Library consumption standardization - Public header exposure
include_directories(${CMAKE_SOURCE_DIR}/include)

# Platform-specific configurations
if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# External dependency resolution
find_package(OpenSSL)
if(OPENSSL_FOUND)
    add_definitions(-DHAVE_OPENSSL)
    message(STATUS "OpenSSL found: ${OPENSSL_VERSION}")
else()
    message(WARNING "OpenSSL not found - using mock cryptographic implementations")
endif()

# Core module source files with systematic organization
set(CORE_SOURCES
    src/core/crypto/crypto.c
    src/core/crypto/primitives/sha512.c
    src/core/crypto/primitives/pbkdf2.c
    src/core/crypto/primitives/aes_gcm.c
    src/core/crypto/validation/validator.c
    src/core/crypto/schemas/schema_parser.c
    src/core/crypto/integration/binding_interface.c
    src/core/micro/micro.c
    src/core/edge/edge.c
    src/core/telemetry/telemetry.c
    src/core/repl/repl.c
)

# IOC Configuration system sources
set(CONFIG_CORE_SOURCES
    src/core/config/config_ioc.c
    src/core/config/config_for_crypto.c
    src/core/config/config_for_micro.c
    src/core/config/config_for_edge.c
    src/core/config/config_for_telemetry.c
    src/core/config/config_for_repl.c
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

# Public headers for library consumption
set(PUBLIC_HEADERS
    include/polybuild/core/config_ioc.h
    include/polybuild/core/crypto.h
    include/polybuild/cli/config.h
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

# Create shared library for dynamic linking
add_library(polybuild_shared SHARED
    ${CORE_SOURCES}
    ${CONFIG_CORE_SOURCES}
)

# Library properties for systematic identification
set_target_properties(polybuild_static PROPERTIES
    OUTPUT_NAME polybuild
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
    ARCHIVE_OUTPUT_NAME polybuild
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

set_target_properties(polybuild_shared PROPERTIES
    OUTPUT_NAME polybuild
    VERSION ${PROJECT_VERSION} 
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

# Include directories for library consumption
target_include_directories(polybuild PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(polybuild_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(polybuild_shared PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# External library linking
if(OPENSSL_FOUND)
    target_link_libraries(polybuild ${OPENSSL_LIBRARIES})
    target_link_libraries(polybuild_static ${OPENSSL_LIBRARIES})
    target_link_libraries(polybuild_shared ${OPENSSL_LIBRARIES})
    
    target_include_directories(polybuild PRIVATE ${OPENSSL_INCLUDE_DIR})
    target_include_directories(polybuild_static PRIVATE ${OPENSSL_INCLUDE_DIR})
    target_include_directories(polybuild_shared PRIVATE ${OPENSSL_INCLUDE_DIR})
endif()

# Platform-specific linking requirements
if(WIN32)
    target_link_libraries(polybuild ws2_32)
    target_link_libraries(polybuild_static ws2_32)
    target_link_libraries(polybuild_shared ws2_32)
elseif(UNIX)
    target_link_libraries(polybuild m)
    target_link_libraries(polybuild_static m)
    target_link_libraries(polybuild_shared m)
endif()

# Set output directories for systematic organization
set_target_properties(polybuild PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(polybuild_static PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

set_target_properties(polybuild_shared PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Configuration validation targets for systematic testing
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
    COMMAND ${CMAKE_BINARY_DIR}/bin/polybuild config show --module repl
    DEPENDS polybuild
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Validating all module configurations"
)

add_custom_target(library-test
    COMMAND ${CMAKE_COMMAND} -E echo "Testing library consumption pattern"
    COMMAND ${CMAKE_COMMAND} -E echo "Static library: ${CMAKE_BINARY_DIR}/lib/"
    COMMAND ${CMAKE_COMMAND} -E echo "Headers: include/polybuild/"
    DEPENDS polybuild_static polybuild_shared
    COMMENT "Validating library consumption readiness"
)

# Comprehensive testing framework integration
enable_testing()

# Core functionality tests
add_test(NAME ioc_system_test 
         COMMAND polybuild config list
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
         
add_test(NAME help_system_test
         COMMAND polybuild --help
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Module configuration tests
add_test(NAME crypto_config_test 
         COMMAND polybuild config show --module crypto
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
         
add_test(NAME micro_config_test
         COMMAND polybuild config show --module micro
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
         
add_test(NAME telemetry_config_test
         COMMAND polybuild config show --module telemetry
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Schema validation tests
add_test(NAME schema_validation_test
         COMMAND polybuild config validate --all  
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Crypto module functionality tests
add_test(NAME crypto_help_test
         COMMAND polybuild crypto --help
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
         
add_test(NAME crypto_validate_test
         COMMAND polybuild crypto validate
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Performance validation tests
add_test(NAME performance_config_test
         COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/test-performance.cmake
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

# Installation configuration for library consumption
install(TARGETS polybuild polybuild_static polybuild_shared
    EXPORT PolyBuildTargets
    RUNTIME DESTINATION bin
        COMPONENT Runtime
    LIBRARY DESTINATION lib
        COMPONENT Runtime
    ARCHIVE DESTINATION lib
        COMPONENT Development
    PUBLIC_HEADER DESTINATION include/polybuild
        COMPONENT Development
)

# Install all public headers with systematic organization
install(DIRECTORY include/polybuild 
    DESTINATION include
    COMPONENT Development
    FILES_MATCHING PATTERN "*.h"
)

# Install schemas for runtime validation
install(DIRECTORY schemas/
    DESTINATION share/polybuild/schemas
    COMPONENT Runtime
    FILES_MATCHING PATTERN "*.json"
)

# Generate and install CMake configuration files for library consumption
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/PolyBuildConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

export(EXPORT PolyBuildTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/PolyBuildTargets.cmake"
    NAMESPACE PolyBuild::
)

configure_file(cmake/PolyBuildConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/PolyBuildConfig.cmake"
    @ONLY
)

install(EXPORT PolyBuildTargets
    FILE PolyBuildTargets.cmake
    NAMESPACE PolyBuild::
    DESTINATION lib/cmake/PolyBuild
    COMPONENT Development
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/PolyBuildConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/PolyBuildConfigVersion.cmake"
    DESTINATION lib/cmake/PolyBuild
    COMPONENT Development
)

# Generate pkg-config file for traditional library consumption
configure_file(cmake/polybuild.pc.in 
    "${CMAKE_CURRENT_BINARY_DIR}/polybuild.pc" 
    @ONLY
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/polybuild.pc"
    DESTINATION lib/pkgconfig
    COMPONENT Development
)

# Create uninstall target for systematic cleanup
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY
    )

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    )
endif()

# Development targets for systematic workflow
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} --build . --target clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/lib
    COMMENT "Comprehensive cleanup of build artifacts"
)

add_custom_target(rebuild-all
    COMMAND ${CMAKE_COMMAND} --build . --target clean-all
    COMMAND ${CMAKE_COMMAND} --build . --target all
    COMMENT "Complete rebuild of PolyBuild system"
)

# Systematic build information and validation
message(STATUS "PolyBuild Library Standardization Configuration:")
message(STATUS "  Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "  Description: ${POLYBUILD_DESCRIPTION}")
message(STATUS "  Vendor: ${POLYBUILD_VENDOR}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "  C Standard: C${CMAKE_C_STANDARD}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  OpenSSL Found: ${OPENSSL_FOUND}")
message(STATUS "  Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "")
message(STATUS "Library Consumption:")
message(STATUS "  Static Library: libpolybuild.a (Unix) / polybuild.lib (Windows)")
message(STATUS "  Shared Library: libpolybuild.so (Unix) / polybuild.dll (Windows)")
message(STATUS "  Headers: include/polybuild/")
message(STATUS "  Schemas: share/polybuild/schemas/")
message(STATUS "")
message(STATUS "Integration:")
message(STATUS "  CMake: find_package(PolyBuild)")
message(STATUS "  pkg-config: pkg-config --cflags --libs polybuild")
message(STATUS "  Direct: -I/usr/local/include -lpolybuild")
message(STATUS "")
message(STATUS "IOC Configuration: Enabled")
message(STATUS "Core Modules: crypto, micro, edge, telemetry, repl")
message(STATUS "CLI Interface: Integrated")
message(STATUS "Testing Framework: Comprehensive")

# Validation summary
if(OPENSSL_FOUND)
    message(STATUS "? OpenSSL integration available")
else()
    message(STATUS "??  OpenSSL not found - using mock implementations")
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/schemas/crypto/crypto-v1.json")
    message(STATUS "? Schema system configured")
else()
    message(STATUS "??  Schema files require initialization")
endif()

message(STATUS "")
message(STATUS "Build ready for systematic Phase 3 progression")
