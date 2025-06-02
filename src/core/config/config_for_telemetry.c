/*
 * config_for_telemetry.c - Configuration provider for telemetry module
 * Version: 1.0.0 - PolyBuild Configuration Management
 * Author: Aegis Project Team - OBINexus Computing
 */

#include <stdio.h>
#include <string.h>
#include <polybuild/core/config_ioc.h>

// Static configuration for telemetry module with systematic validation
static polybuild_config_t telemetry_config = {
    .version_major = 1,
    .version_minor = 0,
    .schema_path = "schemas/telemetry/telemetry-v1.json",
    .validation_enabled = true,
    .strict_mode = true,
    .log_level = LOG_LEVEL_INFO
};

/**
 * Provide configuration for telemetry module
 * Implements systematic validation and policy enforcement
 */
const polybuild_config_t* provide_telemetry_config(void) {
    printf("Providing configuration for telemetry module\n");
    printf("  Schema path: %s\n", telemetry_config.schema_path);
    printf("  Validation: %s\n", telemetry_config.validation_enabled ? "enabled" : "disabled");
    printf("  Strict mode: %s\n", telemetry_config.strict_mode ? "enabled" : "disabled");
    
    // Validate schema file accessibility
    FILE* schema_file = fopen(telemetry_config.schema_path, "r");
    if (!schema_file) {
        printf("Warning: Schema file not accessible: %s\n", telemetry_config.schema_path);
        // Fallback to default schema path
        strcpy(telemetry_config.schema_path, "schemas/default/polybuild-v1.json");
    } else {
        fclose(schema_file);
        printf("Schema file validated: %s\n", telemetry_config.schema_path);
    }
    
    return &telemetry_config;
}

/**
 * Validate telemetry-specific configuration parameters
 */
int validate_telemetry_config(const polybuild_config_t* config) {
    if (!config) {
        fprintf(stderr, "Error: Configuration is NULL for telemetry module\n");
        return -1;
    }
    
    printf("Validating telemetry module configuration\n");
    
    // Version validation
    if (config->version_major < 1) {
        fprintf(stderr, "Error: Invalid version for telemetry module: %d.%d\n", 
                config->version_major, config->version_minor);
        return -1;
    }
    
    // Schema path validation
    if (strlen(config->schema_path) == 0) {
        fprintf(stderr, "Error: Empty schema path for telemetry module\n");
        return -1;
    }
    
    printf("Configuration validation successful for telemetry module\n");
    return 0;
}
