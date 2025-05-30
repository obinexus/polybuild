/*
 * config_for_repl.c - Configuration provider for repl module
 * Version: 1.0.0 - PolyBuild Configuration Management
 * Author: Aegis Project Team - OBINexus Computing
 */

#include <stdio.h>
#include <string.h>
#include "../../include/config/config_ioc.h"

// Static configuration for repl module with systematic validation
static polybuild_config_t repl_config = {
    .version_major = 1,
    .version_minor = 0,
    .schema_path = "schemas/repl/repl-v1.json",
    .validation_enabled = true,
    .strict_mode = true,
    .log_level = LOG_LEVEL_INFO
};

/**
 * Provide configuration for repl module
 * Implements systematic validation and policy enforcement
 */
const polybuild_config_t* provide_repl_config(void) {
    printf("Providing configuration for repl module\n");
    printf("  Schema path: %s\n", repl_config.schema_path);
    printf("  Validation: %s\n", repl_config.validation_enabled ? "enabled" : "disabled");
    printf("  Strict mode: %s\n", repl_config.strict_mode ? "enabled" : "disabled");
    
    // Validate schema file accessibility
    FILE* schema_file = fopen(repl_config.schema_path, "r");
    if (!schema_file) {
        printf("Warning: Schema file not accessible: %s\n", repl_config.schema_path);
        // Fallback to default schema path
        strcpy(repl_config.schema_path, "schemas/default/polybuild-v1.json");
    } else {
        fclose(schema_file);
        printf("Schema file validated: %s\n", repl_config.schema_path);
    }
    
    return &repl_config;
}

/**
 * Validate repl-specific configuration parameters
 */
int validate_repl_config(const polybuild_config_t* config) {
    if (!config) {
        fprintf(stderr, "Error: Configuration is NULL for repl module\n");
        return -1;
    }
    
    printf("Validating repl module configuration\n");
    
    // Version validation
    if (config->version_major < 1) {
        fprintf(stderr, "Error: Invalid version for repl module: %d.%d\n", 
                config->version_major, config->version_minor);
        return -1;
    }
    
    // Schema path validation
    if (strlen(config->schema_path) == 0) {
        fprintf(stderr, "Error: Empty schema path for repl module\n");
        return -1;
    }
    
    printf("Configuration validation successful for repl module\n");
    return 0;
}
