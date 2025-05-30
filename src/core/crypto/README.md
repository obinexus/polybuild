# PolyBuild - Modular Build System

PolyBuild is a domain-specific build system designed for polyglot projects, focusing on build-time logic and configuration management within the OBINexus project architecture.

## Quick Start

1. Run the setup script:
   ```powershell
   .\setup-modules.ps1
   ```

2. Build the project:
   ```cmd
   make
   ```

3. Run PolyBuild:
   ```cmd
   build\bin\polybuild.exe --help
   ```

## Module Architecture

PolyBuild implements a systematic modular architecture with clear separation of concerns:

- **crypto**: Schema-driven cryptographic primitives and validation
- **micro**: Microservice isolation and dependency management  
- **edge**: Edge deployment cache preparation and sync logic
- **telemetry**: Build state tracking and checkpoint management
- **repl**: Interactive build configuration testing environment

## Using the Crypto Module

The crypto module provides systematic cryptographic operations integrated into the PolyBuild ecosystem. It implements schema-driven validation and supports multiple language bindings for polyglot environments.

### Core Cryptographic Operations

#### Primitive Registration
Register cryptographic primitives with systematic validation:

```bash
# Register SHA512 primitive with custom configuration
polybuild crypto register --primitive SHA512 --config schemas/sha512.json

# Register PBKDF2 with policy enforcement
polybuild crypto register --primitive PBKDF2_HMAC_SHA512 --config schemas/pbkdf2.json
```

#### Configuration Validation
Validate crypto configurations against policy constraints:

```bash
# Validate default crypto schema
polybuild crypto validate --config schemas/crypto-v1.json

# Verbose validation with detailed output
polybuild crypto validate --config schemas/crypto-v1.json --verbose
```

#### Hash Generation
Generate cryptographic hashes with algorithm selection:

```bash
# Generate SHA512 hash from string input
polybuild crypto hash --algorithm SHA512 --input "build_artifact_data"

# Generate hash from file content
polybuild crypto hash --algorithm SHA512 --file build/output.bin --format hex

# Use PBKDF2 for key derivation
polybuild crypto hash --algorithm PBKDF2_HMAC_SHA512 --input "password" --format hex
```

### Schema Configuration Reference

The crypto module uses JSON schema files for systematic configuration management. Schema files define:

- **Algorithm Allowlists**: Approved cryptographic algorithms with version constraints
- **Parameter Validation**: Minimum security requirements and bounds checking
- **Policy Enforcement**: Deprecation warnings and security level requirements
- **Binding Configuration**: Language-specific integration parameters

#### Schema File Structure

```json
{
  "schema_version": "1.0.0",
  "policy_enforcement": {
    "enabled": true,
    "strict_validation": true,
    "minimum_security_level": "high"
  },
  "supported_algorithms": {
    "hash_functions": [...],
    "key_derivation_functions": [...], 
    "encryption_algorithms": [...]
  }
}
```

### Security Policy Implementation

The crypto module enforces systematic security policies:

#### Algorithm Requirements
- **SHA512**: Minimum approved hash function
- **PBKDF2**: Minimum 600,000 iterations required
- **AES256-GCM**: Authenticated encryption mandatory
- **Deprecated Algorithms**: MD5, SHA1, DES automatically rejected

#### Parameter Validation
- Key lengths validated against minimum security requirements
- Iteration counts enforced for key derivation functions
- Salt uniqueness and entropy requirements validated
- Hardware acceleration preferences configured per algorithm

### Language Binding Integration

The crypto module supports systematic integration across polyglot environments:

#### Supported Languages
- **C**: Native implementation with OpenSSL integration
- **Python**: Binding via `cryptography` and `hashlib` modules
- **Node.js**: Integration with native `crypto` module
- **Java**: JCE provider integration for enterprise environments

#### Binding Architecture
```c
// Register language-specific crypto operations
int register_language_binding(const char* language, const binding_ops_t* operations);

// Execute crypto operations through language bindings
int execute_binding_operation(const char* language, const char* operation, 
                             const void* input, void* output);
```

### Build Integration Testing

Validate crypto module integration with systematic testing:

```bash
# Run crypto-specific validation target
make crypto-validate

# Execute unit tests for crypto operations
make test

# Test CLI functionality
build\bin\polybuild.exe crypto --help
```

### Configuration File Locations

- **Main Schema**: `schemas/crypto-v1.json`
- **Algorithm Configs**: `schemas/{algorithm_name}.json`
- **Build Configuration**: `CMakeLists.txt` crypto target definitions
- **Policy Files**: `policies/crypto-security.json`

## Build Requirements

- CMake 3.16 or higher
- C11-compatible compiler (GCC, Clang, MSVC)
- Make utility (for simplified building)
- OpenSSL 1.1.1+ (optional, for production crypto operations)
- pkg-config (for dependency resolution)

## Development Methodology

The OBINexus project follows systematic waterfall methodology with clear phase gates:

### Phase Structure
- **Requirements Analysis**: Schema definition and policy specification
- **Design**: Module architecture and interface definition
- **Implementation**: Systematic module development with testing
- **Integration**: Cross-language binding validation
- **Testing**: Comprehensive validation including security testing
- **Deployment**: Build system integration and documentation

### Code Organization
The project maintains systematic separation between:
- **Core Logic** (`src/core/`): Domain-specific implementation
- **CLI Interfaces** (`src/cli/`): User interaction and command processing
- **Build Configuration** (`CMakeLists.txt`, `Makefile`): Build system integration
- **Documentation** (`docs/`): Technical specifications and usage guides

Each module implements build-time logic exclusively, maintaining clear boundaries between build-phase and runtime operations.

## Testing Strategy

### Unit Testing
- Comprehensive test coverage for all crypto primitives
- Mock implementations for build-time validation
- Parameter boundary testing and error condition handling

### Integration Testing  
- Cross-language binding validation
- Schema parsing and validation testing
- CLI interface functionality verification

### Security Testing
- Algorithm allowlist enforcement validation
- Parameter constraint verification
- Deprecation policy compliance testing

## Contributing to OBINexus Project

Development follows systematic engineering practices:

1. **Requirements Documentation**: Clear specification of functionality
2. **Design Review**: Architecture validation before implementation
3. **Implementation**: Systematic development with testing
4. **Code Review**: Technical validation and standards compliance
5. **Integration Testing**: Cross-module functionality validation
6. **Documentation**: Technical documentation and usage examples

Each contribution maintains the project's focus on build-time logic, security policy enforcement, and systematic polyglot integration.

## Project Status

**Current Phase**: Implementation and Build System Finalization (Phase 2)
**Next Milestone**: Full cryptographic implementation and cross-language testing (Phase 3)
**Target Architecture**: Production-ready build system with comprehensive security validation

The crypto module represents the first complete implementation of the PolyBuild modular architecture, demonstrating systematic integration of security-focused build-time operations across polyglot environments.
