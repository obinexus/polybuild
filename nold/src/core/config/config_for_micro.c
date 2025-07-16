/*
 * config_for_micro.c - Configuration provider for micro module
 * Version: 1.0.0 - PolyBuild Configuration Management
 * Author: Aegis Project Team - OBINexus Computing
 */

#include <stdio.h>
#include <string.h>
#include <polybuild/core/config_ioc.h>

// Static configuration for micro module with systematic validation
static polybuild_config_t micro_config = {
    .version_major = 1,
    .version_minor = 0,
    .schema_path = "schemas/micro/micro-v1.json",
    .validation_enabled = true,
    .strict_mode = true,
    .log_level = LOG_LEVEL_INFO
};

/**
 * Provide configuration for micro module
 * Implements systematic validation and policy enforcement
 */
const polybuild_config_t* provide_micro_config(void) {
    printf("Providing configuration for micro module\n");
    printf("  Schema path: %s\n", micro_config.schema_path);
    printf("  Validation: %s\n", micro_config.validation_enabled ? "enabled" : "disabled");
    printf("  Strict mode: %s\n", micro_config.strict_mode ? "enabled" : "disabled");
    
    // Validate schema file accessibility
    FILE* schema_file = fopen(micro_config.schema_path, "r");
    if (!schema_file) {
        printf("Warning: Schema file not accessible: %s\n", micro_config.schema_path);
        // Fallback to default schema path
        strcpy(micro_config.schema_path, "schemas/default/polybuild-v1.json");
    } else {
        fclose(schema_file);
        printf("Schema file validated: %s\n", micro_config.schema_path);
    }
    
    return &micro_config;
}

/**
 * Validate micro-specific configuration parameters
 */
int validate_micro_config(const polybuild_config_t* config) {
    if (!config) {
        fprintf(stderr, "Error: Configuration is NULL for micro module\n");
        return -1;
    }
    
    printf("Validating micro module configuration\n");
    
    // Version validation
    if (config->version_major < 1) {
        fprintf(stderr, "Error: Invalid version for micro module: %d.%d\n", 
                config->version_major, config->version_minor);
        return -1;
    }
    
    // Schema path validation
    if (strlen(config->schema_path) == 0) {
        fprintf(stderr, "Error: Empty schema path for micro module\n");
        return -1;
    }
    
    printf("Configuration validation successful for micro module\n");
    return 0;
}
