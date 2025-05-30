/*
 * config_for_edge.c - Configuration provider for edge module
 * Version: 1.0.0 - PolyBuild Configuration Management
 * Author: Aegis Project Team - OBINexus Computing
 */

#include <stdio.h>
#include <string.h>
#include <polybuild/core/config_ioc.h>

// Static configuration for edge module with systematic validation
static polybuild_config_t edge_config = {
    .version_major = 1,
    .version_minor = 0,
    .schema_path = "schemas/edge/edge-v1.json",
    .validation_enabled = true,
    .strict_mode = true,
    .log_level = LOG_LEVEL_INFO
};

/**
 * Provide configuration for edge module
 * Implements systematic validation and policy enforcement
 */
const polybuild_config_t* provide_edge_config(void) {
    printf("Providing configuration for edge module\n");
    printf("  Schema path: %s\n", edge_config.schema_path);
    printf("  Validation: %s\n", edge_config.validation_enabled ? "enabled" : "disabled");
    printf("  Strict mode: %s\n", edge_config.strict_mode ? "enabled" : "disabled");
    
    // Validate schema file accessibility
    FILE* schema_file = fopen(edge_config.schema_path, "r");
    if (!schema_file) {
        printf("Warning: Schema file not accessible: %s\n", edge_config.schema_path);
        // Fallback to default schema path
        strcpy(edge_config.schema_path, "schemas/default/polybuild-v1.json");
    } else {
        fclose(schema_file);
        printf("Schema file validated: %s\n", edge_config.schema_path);
    }
    
    return &edge_config;
}

/**
 * Validate edge-specific configuration parameters
 */
int validate_edge_config(const polybuild_config_t* config) {
    if (!config) {
        fprintf(stderr, "Error: Configuration is NULL for edge module\n");
        return -1;
    }
    
    printf("Validating edge module configuration\n");
    
    // Version validation
    if (config->version_major < 1) {
        fprintf(stderr, "Error: Invalid version for edge module: %d.%d\n", 
                config->version_major, config->version_minor);
        return -1;
    }
    
    // Schema path validation
    if (strlen(config->schema_path) == 0) {
        fprintf(stderr, "Error: Empty schema path for edge module\n");
        return -1;
    }
    
    printf("Configuration validation successful for edge module\n");
    return 0;
}
