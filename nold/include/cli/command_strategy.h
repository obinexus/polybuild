/*
 * command_strategy.h - PolyBuild Command Strategy Interface
 * Version: 1.0.0 - Aegis Project Architecture
 * Author: Aegis Project Team - OBINexus Computing
 * Collaborating Engineer: Nnamdi Michael Okpala
 * 
 * Systematic command abstraction for unified behavior across PolyBuild domains
 */

#ifndef POLYBUILD_COMMAND_STRATEGY_H
#define POLYBUILD_COMMAND_STRATEGY_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Maximum limits for systematic resource management
#define MAX_COMMAND_NAME_LENGTH 32
#define MAX_DESCRIPTION_LENGTH 256
#define MAX_SCHEMA_PATH_LENGTH 512
#define MAX_ARGUMENTS 64
#define MAX_OPTIONS 32
#define MAX_COMMANDS 16

// Command execution context for systematic state management
typedef enum {
    COMMAND_CONTEXT_CLI = 0,        // Interactive CLI execution
    COMMAND_CONTEXT_LIBRARY = 1,    // Library consumption
    COMMAND_CONTEXT_BUILD = 2,      // Build system integration
    COMMAND_CONTEXT_TEST = 3        // Testing framework
} command_context_t;

// Command execution environment for systematic configuration resolution
typedef enum {
    COMMAND_ENV_DEVELOPMENT = 0,    // Development with relaxed validation
    COMMAND_ENV_STAGING = 1,        // Staging with enhanced validation
    COMMAND_ENV_PRODUCTION = 2,     // Production with strict validation
    COMMAND_ENV_CI = 3              // CI/CD with systematic automation
} command_environment_t;

// Systematic command result structure for unified error handling
typedef struct {
    int exit_code;                  // Standard exit code (0 = success)
    bool has_output;                // Output data availability flag
    char output_buffer[1024];       // Structured output buffer
    char error_message[512];        // Systematic error description
    uint64_t execution_time_ms;     // Performance tracking
    size_t operations_performed;    // Operation count for auditing
} command_result_t;

// Command option definition for systematic argument parsing
typedef struct {
    char short_name;                // Single character option (-h)
    char long_name[32];             // Long form option (--help)
    char description[128];          // Help text description
    bool requires_argument;         // Argument requirement flag
    bool is_required;               // Option requirement flag
    char default_value[64];         // Default value if not specified
} command_option_t;

// Command validation context for schema-driven behavior
typedef struct {
    char schema_path[MAX_SCHEMA_PATH_LENGTH];
    bool strict_validation;         // Strict vs permissive validation
    bool audit_enabled;             // Audit logging requirement
    command_environment_t env;      // Execution environment context
    const char* config_override;    // Configuration override path
} command_validation_context_t;

// Forward declaration for command strategy interface
typedef struct command_strategy command_strategy_t;

// Command execution function pointer with systematic context
typedef command_result_t (*command_execute_fn)(
    const command_strategy_t* strategy,
    int argc, 
    char* argv[],
    const command_validation_context_t* context
);

// Command validation function pointer for schema enforcement
typedef bool (*command_validate_fn)(
    const command_strategy_t* strategy,
    const command_validation_context_t* context
);

// Command help generation function pointer for systematic documentation
typedef void (*command_help_fn)(
    const command_strategy_t* strategy,
    bool verbose
);

// Command cleanup function pointer for systematic resource management
typedef void (*command_cleanup_fn)(
    const command_strategy_t* strategy
);

// Core Command Strategy Interface - Systematic abstraction for all domains
struct command_strategy {
    // Command identification and metadata
    char name[MAX_COMMAND_NAME_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    char version[16];
    char domain[32];                // Domain classification (crypto, micro, etc.)
    
    // Command behavior configuration
    bool requires_config;           // Configuration dependency requirement
    bool supports_dry_run;          // Dry run execution support
    bool is_build_time_only;        // Build-time vs runtime classification
    uint32_t min_arguments;         // Minimum argument requirement
    uint32_t max_arguments;         // Maximum argument allowance
    
    // Command options for systematic CLI parsing
    command_option_t options[MAX_OPTIONS];
    size_t option_count;
    
    // Strategy implementation functions
    command_execute_fn execute;     // Primary command execution
    command_validate_fn validate;   // Configuration validation
    command_help_fn show_help;      // Help documentation generation
    command_cleanup_fn cleanup;     // Resource cleanup
    
    // Domain-specific extension point
    void* domain_data;              // Domain-specific configuration
    size_t domain_data_size;        // Domain data size for validation
    
    // Performance and audit metadata
    uint64_t total_executions;      // Execution counter for monitoring
    uint64_t total_execution_time;  // Cumulative execution time
    uint64_t last_execution_time;   // Most recent execution timestamp
};

// Command registry for systematic discovery and dispatch
typedef struct {
    command_strategy_t commands[MAX_COMMANDS];
    size_t command_count;
    bool is_initialized;
    char registry_version[16];
} command_registry_t;

// Systematic Command Strategy Management API

/**
 * Initialize the command strategy registry with systematic validation
 */
int initialize_command_registry(command_registry_t* registry);

/**
 * Register a command strategy with systematic collision detection
 */
int register_command_strategy(
    command_registry_t* registry,
    const command_strategy_t* strategy
);

/**
 * Discover command by name with systematic lookup
 */
const command_strategy_t* find_command_strategy(
    const command_registry_t* registry,
    const char* command_name
);

/**
 * Execute command with systematic validation and context management
 */
command_result_t execute_command_strategy(
    const command_strategy_t* strategy,
    int argc,
    char* argv[],
    command_context_t context,
    command_environment_t environment
);

/**
 * Validate command configuration with schema enforcement
 */
bool validate_command_configuration(
    const command_strategy_t* strategy,
    const char* config_path,
    command_environment_t environment
);

/**
 * Generate systematic help documentation for command
 */
void generate_command_help(
    const command_strategy_t* strategy,
    bool verbose,
    bool include_examples
);

/**
 * List all registered commands with systematic formatting
 */
void list_available_commands(
    const command_registry_t* registry,
    bool include_descriptions
);

/**
 * Parse command line arguments with systematic option handling
 */
int parse_command_arguments(
    const command_strategy_t* strategy,
    int argc,
    char* argv[],
    char parsed_options[][64],
    size_t* option_count
);

/**
 * Create validation context with systematic environment resolution
 */
command_validation_context_t create_validation_context(
    const char* schema_path,
    command_environment_t environment,
    bool strict_mode
);

/**
 * Cleanup command registry with systematic resource management
 */
void cleanup_command_registry(command_registry_t* registry);

// Systematic utility functions for command implementation

/**
 * Create command result with systematic error handling
 */
command_result_t create_command_result(
    int exit_code,
    const char* output,
    const char* error_message
);

/**
 * Log command execution with systematic audit trail
 */
void log_command_execution(
    const command_strategy_t* strategy,
    const command_result_t* result,
    const command_validation_context_t* context
);

/**
 * Validate command prerequisites with systematic checking
 */
bool validate_command_prerequisites(
    const command_strategy_t* strategy,
    const command_validation_context_t* context
);

// Domain-specific command strategy factory functions
command_strategy_t create_crypto_command_strategy(void);
command_strategy_t create_micro_command_strategy(void);
command_strategy_t create_edge_command_strategy(void);
command_strategy_t create_telemetry_command_strategy(void);
command_strategy_t create_repl_command_strategy(void);
command_strategy_t create_config_command_strategy(void);

#endif // POLYBUILD_COMMAND_STRATEGY_HxY

