cmake_minimum_required(VERSION 3.16)
project(PolyBuild VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -DDEBUG")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -DNDEBUG")
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src/core)
include_directories(${CMAKE_SOURCE_DIR}/src/cli)
include_directories(${CMAKE_SOURCE_DIR}/src/core/crypto)

# Core module source files
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

# CLI source files
set(CLI_SOURCES
    src/cli/main.c
    src/cli/crypto_cli.c
    src/cli/micro_cli.c
    src/cli/edge_cli.c
    src/cli/telemetry_cli.c
    src/cli/repl_cli.c
)

# Create the main executable
add_executable(polybuild ${CORE_SOURCES} ${CLI_SOURCES})

# Link OpenSSL for cryptographic primitives (optional, for full implementation)
find_package(OpenSSL)
if(OPENSSL_FOUND)
    target_link_libraries(polybuild ${OPENSSL_LIBRARIES})
    target_include_directories(polybuild PRIVATE ${OPENSSL_INCLUDE_DIR})
    add_definitions(-DHAVE_OPENSSL)
endif()

# Set output directory
set_target_properties(polybuild PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Install configuration
install(TARGETS polybuild DESTINATION bin)

# Add custom target for clean build
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} --build . --target clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin
)

# Print build information
message(STATUS "PolyBuild Configuration Summary:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C Compiler: ${CMAKE_COMPILER}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  OpenSSL Found: ${OPENSSL_FOUND}")

# Add crypto-specific target for validation
add_custom_target(crypto-validate
    COMMAND ${CMAKE_BINARY_DIR}/bin/polybuild crypto validate --config schemas/crypto-v1.json
    DEPENDS polybuild
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Validating crypto module configuration"
)

# Add test target
enable_testing()
add_test(NAME crypto_help_test 
         COMMAND polybuild crypto --help
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_test(NAME crypto_validate_test 
         COMMAND polybuild crypto validate --config ${CMAKE_SOURCE_DIR}/schemas/crypto-v1.json
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
