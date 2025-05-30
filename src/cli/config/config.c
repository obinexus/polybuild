/*
 * config.c - CLI Configuration Interface  
 * Version: 1.0.0 - PolyBuild Configuration Management
 * Author: Aegis Project Team - OBINexus Computing
 * 
 * CLI wrapper for IOC configuration system access
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "../../include/config/config_ioc.h"

// Forward declarations for CLI command handlers
static int handle_show_command(int argc, char* argv[]);
static int handle_list_command(int argc, char* argv[]);
static int handle_validate_command(int argc, char* argv[]);
static void print_config_help(void);
static void print_config_details(const char* module_name, const polybuild_config_t* config);

/**
 * Main CLI entry point for configuration management
 * Implements systematic configuration access via IOC
 */
int config_cli_main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("PolyBuild Configuration Management v1.0.0\n");
        printf("Usage: polybuild config <command> [options]\n\n");
        print_config_help();
        return 0;
    }
    
    const char* command = argv[1];
    
    // Handle help command explicitly
    if (strcmp(command, "help") == 0 || strcmp(command, "--help") == 0) {
        print_config_help();
        return 0;
    }
    
    // Initialize configuration system
    if (initialize_config_system() != 0) {
        fprintf(stderr, "Error: Failed to initialize configuration system\n");
        return 1;
    }
    
    // Dispatch to appropriate command handler
    int result = 0;
    if (strcmp(command, "show") == 0) {
        result = handle_show_command(argc - 1, argv + 1);
    } else if (strcmp(command, "list") == 0) {
        result = handle_list_command(argc - 1, argv + 1);
    } else if (strcmp(command, "validate") == 0) {
        result = handle_validate_command(argc - 1, argv + 1);
    } else {
        printf("Error: Unknown config command '%s'\n", command);
        print_config_help();
        result = 1;
    }
    
    // Cleanup configuration system
    cleanup_config_system();
    return result;
}

/**
 * Handle 'show' command for configuration display
 * Syntax: polybuild config show --module <module_name>
 */
static int handle_show_command(int argc, char* argv[]) {
    const char* module_name = NULL;
    bool verbose = false;
    
    static struct option long_options[] = {
        {"module", required_argument, 0, 'm'},
        {"verbose", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    int opt;
    int option_index = 0;
    
    while ((opt = getopt_long(argc, argv, "m:vh", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'm':
                module_name = optarg;
                break;
            case 'v':
                verbose = true;
                break;
            case 'h':
                printf("Usage: polybuild config show --module <module_name> [--verbose]\n");
                return 0;
            case '?':
                return 1;
        }
    }
    
    if (!module_name) {
        printf("Error: --module argument required\n");
        return 1;
    }
    
    const polybuild_config_t* config = get_config_for(module_name);
    if (!config) {
        printf("Error: Configuration not found for module: %s\n", module_name);
        return 1;
    }
    
    printf("Configuration for module: %s\n", module_name);
    print_config_details(module_name, config);
    
    return 0;
}

/**
 * Handle 'list' command for provider enumeration
 */
static int handle_list_command(int argc, char* argv[]) {
    printf("Listing all registered configuration providers\n\n");
    list_config_providers();
    return 0;
}

/**
 * Handle 'validate' command for configuration validation
 */
static int handle_validate_command(int argc, char* argv[]) {
    printf("Configuration validation not yet implemented\n");
    return 0;
}

/**
 * Print comprehensive help for configuration commands
 */
static void print_config_help(void) {
    printf("PolyBuild Configuration Management - IOC System Interface\n\n");
    
    printf("COMMANDS:\n");
    printf("  show      Display configuration for specific module\n");
    printf("  list      List all registered configuration providers\n");
    printf("  validate  Validate configuration files and schemas\n");
    printf("  help      Display this help information\n\n");
    
    printf("EXAMPLES:\n");
    printf("  # Show crypto module configuration\n");
    printf("  polybuild config show --module crypto\n\n");
    printf("  # List all configuration providers\n");
    printf("  polybuild config list\n\n");
}

/**
 * Print detailed configuration information
 */
static void print_config_details(const char* module_name, const polybuild_config_t* config) {
    printf("  Version: %d.%d\n", config->version_major, config->version_minor);
    printf("  Schema Path: %s\n", config->schema_path);
    printf("  Validation: %s\n", config->validation_enabled ? "enabled" : "disabled");
    printf("  Strict Mode: %s\n", config->strict_mode ? "enabled" : "disabled");
    printf("  Log Level: %d\n", config->log_level);
}
