/*
 * command_strategy.c - PolyBuild Command Strategy Implementation
 * Version: 1.0.0 - Aegis Project Architecture
 * Author: Aegis Project Team - OBINexus Computing
 * Collaborating Engineer: Nnamdi Michael Okpala
 * 
 * Systematic implementation of unified command behavior across PolyBuild domains
 */

#include "command_strategy.h"
#include "../config/config_ioc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>

// Global command registry for systematic command management
static command_registry_t global_registry = {0};

/**
 * Initialize the command strategy registry with systematic validation
 */
int initialize_command_registry(command_registry_t* registry) {
    if (!registry) {
        fprintf(stderr, "Error: Registry pointer is NULL\n");
        return -1;
    }
    
    memset(registry, 0, sizeof(command_registry_t));
    strcpy(registry->registry_version, "1.0.0");
    registry->is_initialized = true;
    
    printf("Command registry initialized v%s\n", registry->registry_version);
    
    // Register all core domain commands with systematic validation
    register_command_strategy(registry, &(command_strategy_t)create_crypto_command_strategy());
    register_command_strategy(registry, &(command_strategy_t)create_micro_command_strategy());
    register_command_strategy(registry, &(command_strategy_t)create_edge_command_strategy());
    register_command_strategy(registry, &(command_strategy_t)create_telemetry_command_strategy());
    register_command_strategy(registry, &(command_strategy_t)create_repl_command_strategy());
    register_command_strategy(registry, &(command_strategy_t)create_config_command_strategy());
    
    printf("Registered %zu domain commands in registry\n", registry->command_count);
    return 0;
}

/**
 * Register a command strategy with systematic collision detection
 */
int register_command_strategy(command_registry_t* registry, const command_strategy_t* strategy) {
    if (!registry || !strategy) {
        fprintf(stderr, "Error: Invalid registry or strategy pointer\n");
        return -1;
    }
    
    if (registry->command_count >= MAX_COMMANDS) {
        fprintf(stderr, "Error: Registry capacity exceeded (%d commands)\n", MAX_COMMANDS);
        return -1;
    }
    
    // Check for name collision with systematic validation
    for (size_t i = 0; i < registry->command_count; i++) {
        if (strcmp(registry->commands[i].name, strategy->name) == 0) {
            fprintf(stderr, "Error: Command '%s' already registered\n", strategy->name);
            return -1;
        }
    }
    
    // Validate command strategy completeness
    if (!strategy->execute || strlen(strategy->name) == 0) {
        fprintf(stderr, "Error: Invalid command strategy - missing required fields\n");
        return -1;
    }
    
    // Register command with systematic initialization
    memcpy(&registry->commands[registry->command_count], strategy, sizeof(command_strategy_t));
    registry->commands[registry->command_count].total_executions = 0;
    registry->commands[registry->command_count].total_execution_time = 0;
    registry->commands[registry->command_count].last_execution_time = 0;
    
    printf("Registered command strategy: %s (%s domain)\n", 
           strategy->name, strategy->domain);
    
    registry->command_count++;
    return (int)(registry->command_count - 1);
}

/**
 * Discover command by name with systematic lookup
 */
const command_strategy_t* find_command_strategy(const command_registry_t* registry, const char* command_name) {
    if (!registry || !command_name || !registry->is_initialized) {
        return NULL;
    }
    
    for (size_t i = 0; i < registry->command_count; i++) {
        if (strcmp(registry->commands[i].name, command_name) == 0) {
            return &registry->commands[i];
        }
    }
    
    return NULL;
}

/**
 * Execute command with systematic validation and context management
 */
command_result_t execute_command_strategy(
    const command_strategy_t* strategy,
    int argc,
    char* argv[],
    command_context_t context,
    command_environment_t environment) {
    
    command_result_t result = {0};
    uint64_t start_time = (uint64_t)time(NULL) * 1000; // Milliseconds
    
    if (!strategy || !strategy->execute) {
        result.exit_code = -1;
        strcpy(result.error_message, "Invalid command strategy or missing execute function");
        return result;
    }
    
    // Create validation context with systematic environment resolution
    command_validation_context_t validation_context = create_validation_context(
        "", // Schema path resolved from configuration
        environment,
        (environment == COMMAND_ENV_PRODUCTION) // Strict mode for production
    );
    
    // Resolve configuration path based on command requirements
    if (strategy->requires_config) {
        const polybuild_config_t* config = get_config_for(strategy->domain);
        if (config) {
            strcpy(validation_context.schema_path, config->schema_path);
            validation_context.strict_validation = config->strict_mode;
            validation_context.audit_enabled = (config->log_level <= LOG_LEVEL_INFO);
        } else {
            printf("Warning: No configuration found for %s domain, using defaults\n", strategy->domain);
        }
    }
    
    // Perform systematic pre-execution validation
    if (strategy->validate && !strategy->validate(strategy, &validation_context)) {
        result.exit_code = -1;
        strcpy(result.error_message, "Command validation failed");
        return result;
    }
    
    // Validate argument count constraints
    if (argc < (int)strategy->min_arguments || argc > (int)strategy->max_arguments) {
        result.exit_code = -1;
        snprintf(result.error_message, sizeof(result.error_message),
                "Invalid argument count: expected %u-%u, got %d",
                strategy->min_arguments, strategy->max_arguments, argc);
        return result;
    }
    
    printf("Executing %s command in %s context with %s environment\n",
           strategy->name,
           (context == COMMAND_CONTEXT_CLI) ? "CLI" :
           (context == COMMAND_CONTEXT_LIBRARY) ? "LIBRARY" :
           (context == COMMAND_CONTEXT_BUILD) ? "BUILD" : "TEST",
           (environment == COMMAND_ENV_DEVELOPMENT) ? "DEVELOPMENT" :
           (environment == COMMAND_ENV_STAGING) ? "STAGING" :
           (environment == COMMAND_ENV_PRODUCTION) ? "PRODUCTION" : "CI");
    
    // Execute command with systematic error handling
    result = strategy->execute(strategy, argc, argv, &validation_context);
    
    // Calculate execution time for performance monitoring
    uint64_t end_time = (uint64_t)time(NULL) * 1000;
    result.execution_time_ms = end_time - start_time;
    
    // Update strategy execution statistics (cast away const for statistics)
    command_strategy_t* mutable_strategy = (command_strategy_t*)strategy;
    mutable_strategy->total_executions++;
    mutable_strategy->total_execution_time += result.execution_time_ms;
    mutable_strategy->last_execution_time = end_time;
    
    // Log execution with systematic audit trail
    if (validation_context.audit_enabled) {
        log_command_execution(strategy, &result, &validation_context);
    }
    
    return result;
}

/**
 * Create validation context with systematic environment resolution
 */
command_validation_context_t create_validation_context(
    const char* schema_path,
    command_environment_t environment,
    bool strict_mode) {
    
    command_validation_context_t context = {0};
    
    if (schema_path && strlen(schema_path) > 0) {
        strncpy(context.schema_path, schema_path, MAX_SCHEMA_PATH_LENGTH - 1);
    } else {
        strcpy(context.schema_path, "schemas/default/polybuild-v1.json");
    }
    
    context.env = environment;
    context.strict_validation = strict_mode;
    context.audit_enabled = (environment == COMMAND_ENV_PRODUCTION || environment == COMMAND_ENV_CI);
    context.config_override = NULL;
    
    return context;
}

/**
 * Generate systematic help documentation for command
 */
void generate_command_help(const command_strategy_t* strategy, bool verbose, bool include_examples) {
    if (!strategy) {
        return;
    }
    
    printf("COMMAND: %s\n", strategy->name);
    printf("DOMAIN:  %s\n", strategy->domain);
    printf("VERSION: %s\n\n", strategy->version);
    printf("DESCRIPTION:\n  %s\n\n", strategy->description);
    
    if (strategy->option_count > 0) {
        printf("OPTIONS:\n");
        for (size_t i = 0; i < strategy->option_count; i++) {
            const command_option_t* opt = &strategy->options[i];
            printf("  -%c, --%s", opt->short_name, opt->long_name);
            if (opt->requires_argument) {
                printf(" <value>");
            }
            printf("\n      %s", opt->description);
            if (strlen(opt->default_value) > 0) {
                printf(" (default: %s)", opt->default_value);
            }
            if (opt->is_required) {
                printf(" [REQUIRED]");
            }
            printf("\n\n");
        }
    }
    
    if (verbose) {
        printf("TECHNICAL DETAILS:\n");
        printf("  Configuration Required: %s\n", strategy->requires_config ? "Yes" : "No");
        printf("  Dry Run Support: %s\n", strategy->supports_dry_run ? "Yes" : "No");
        printf("  Build Time Only: %s\n", strategy->is_build_time_only ? "Yes" : "No");
        printf("  Argument Range: %u-%u\n\n", strategy->min_arguments, strategy->max_arguments);
        
        printf("EXECUTION STATISTICS:\n");
        printf("  Total Executions: %lu\n", strategy->total_executions);
        if (strategy->total_executions > 0) {
            printf("  Average Time: %lu ms\n", strategy->total_execution_time / strategy->total_executions);
        }
        printf("\n");
    }
    
    if (include_examples && strategy->show_help) {
        printf("EXAMPLES:\n");
        strategy->show_help(strategy, false);
        printf("\n");
    }
}

/**
 * List all registered commands with systematic formatting
 */
void list_available_commands(const command_registry_t* registry, bool include_descriptions) {
    if (!registry || !registry->is_initialized) {
        printf("Command registry not initialized\n");
        return;
    }
    
    printf("PolyBuild Available Commands (Registry v%s):\n\n", registry->registry_version);
    
    // Group commands by domain for systematic organization
    const char* domains[] = {"crypto", "micro", "edge", "telemetry", "repl", "config"};
    size_t domain_count = sizeof(domains) / sizeof(domains[0]);
    
    for (size_t d = 0; d < domain_count; d++) {
        bool domain_has_commands = false;
        
        // Check if domain has commands
        for (size_t i = 0; i < registry->command_count; i++) {
            if (strcmp(registry->commands[i].domain, domains[d]) == 0) {
                domain_has_commands = true;
                break;
            }
        }
        
        if (!domain_has_commands) continue;
        
        printf("%s DOMAIN:\n", domains[d]);
        for (size_t i = 0; i < registry->command_count; i++) {
            const command_strategy_t* cmd = &registry->commands[i];
            if (strcmp(cmd->domain, domains[d]) == 0) {
                printf("  %-12s", cmd->name);
                if (include_descriptions) {
                    printf(" - %s", cmd->description);
                }
                printf("\n");
            }
        }
        printf("\n");
    }
    
    printf("Total Commands: %zu\n", registry->command_count);
    printf("Use 'polybuild <command> --help' for command-specific help\n");
}

/**
 * Log command execution with systematic audit trail
 */
void log_command_execution(
    const command_strategy_t* strategy,
    const command_result_t* result,
    const command_validation_context_t* context) {
    
    if (!strategy || !result || !context) {
        return;
    }
    
    printf("[AUDIT] Command: %s | Domain: %s | Exit: %d | Time: %lu ms | Env: %d | Operations: %zu\n",
           strategy->name,
           strategy->domain,
           result->exit_code,
           result->execution_time_ms,
           context->env,
           result->operations_performed);
    
    if (result->exit_code != 0 && strlen(result->error_message) > 0) {
        printf("[AUDIT] Error: %s\n", result->error_message);
    }
}

/**
 * Create command result with systematic error handling
 */
command_result_t create_command_result(int exit_code, const char* output, const char* error_message) {
    command_result_t result = {0};
    result.exit_code = exit_code;
    result.has_output = (output != NULL && strlen(output) > 0);
    result.operations_performed = 1;
    
    if (output) {
        strncpy(result.output_buffer, output, sizeof(result.output_buffer) - 1);
    }
    
    if (error_message) {
        strncpy(result.error_message, error_message, sizeof(result.error_message) - 1);
    }
    
    return result;
}

/**
 * Get global command registry for CLI integration
 */
command_registry_t* get_global_command_registry(void) {
    if (!global_registry.is_initialized) {
        initialize_command_registry(&global_registry);
    }
    return &global_registry;
}

/**
 * Cleanup command registry with systematic resource management
 */
void cleanup_command_registry(command_registry_t* registry) {
    if (!registry) {
        return;
    }
    
    // Execute cleanup functions for all registered commands
    for (size_t i = 0; i < registry->command_count; i++) {
        if (registry->commands[i].cleanup) {
            registry->commands[i].cleanup(&registry->commands[i]);
        }
    }
    
    printf("Command registry cleanup completed (%zu commands)\n", registry->command_count);
    memset(registry, 0, sizeof(command_registry_t));
}
