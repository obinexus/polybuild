# PolyBuild - Modular Build System

PolyBuild is a domain-specific build system designed for polyglot projects, focusing on build-time logic and configuration management.

## Quick Start

1. Run the setup script:
   `powershell
   .\setup-modules.ps1
   `

2. Build the project:
   `cmd
   make
   `

3. Run PolyBuild:
   `cmd
   build\bin\polybuild.exe --help
   `

## Module Architecture

- **micro**: Microservice isolation and dependency management
 - **repl**: Interactive build configuration testing environment
 - **edge**: Edge deployment cache preparation and sync logic
 - **crypto**: Schema-driven cryptographic primitives and validation
 - **telemetry**: Build state tracking and checkpoint management


## Build Requirements

- CMake 3.16 or higher
- C11-compatible compiler (GCC, Clang, MSVC)
- Make utility (for simplified building)

## Development

The project follows a modular architecture with clear separation between:
- Core logic (src/core/)
- CLI interfaces (src/cli/)
- Build configuration (CMakeLists.txt, Makefile)

Each module is designed to solve specific build-time problems without runtime execution concerns.
