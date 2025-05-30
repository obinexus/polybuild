/*
 * config_for_crypto.c - Configuration provider for crypto module
 * Version: 1.0.0 - PolyBuild Configuration Management
 * Author: Aegis Project Team - OBINexus Computing
 */

#include <stdio.h>
#include <string.h>
#include <polybuild/core/config_ioc.h>

// Static configuration for crypto module with systematic validation
static polybuild_config_t crypto_config = {
    .version_major = 1,
    .version_minor = 0,
    .schema_path = "schemas/crypto/crypto-v1.json",
    .validation_enabled = true,
    .strict_mode = true,
    .log_level = LOG_LEVEL_INFO
};

/**
 * Provide configuration for crypto module
 * Implements systematic validation and policy enforcement
 */
const polybuild_config_t* provide_crypto_config(void) {
    printf("Providing configuration for crypto module\n");
    printf("  Schema path: %s\n", crypto_config.schema_path);
    printf("  Validation: %s\n", crypto_config.validation_enabled ? "enabled" : "disabled");
    printf("  Strict mode: %s\n", crypto_config.strict_mode ? "enabled" : "disabled");
    
    // Validate schema file accessibility
    FILE* schema_file = fopen(crypto_config.schema_path, "r");
    if (!schema_file) {
        printf("Warning: Schema file not accessible: %s\n", crypto_config.schema_path);
        // Fallback to default schema path
        strcpy(crypto_config.schema_path, "schemas/default/polybuild-v1.json");
    } else {
        fclose(schema_file);
        printf("Schema file validated: %s\n", crypto_config.schema_path);
    }
    
    return &crypto_config;
}

/**
 * Validate crypto-specific configuration parameters
 */
int validate_crypto_config(const polybuild_config_t* config) {
    if (!config) {
        fprintf(stderr, "Error: Configuration is NULL for crypto module\n");
        return -1;
    }
    
    printf("Validating crypto module configuration\n");
    
    // Version validation
    if (config->version_major < 1) {
        fprintf(stderr, "Error: Invalid version for crypto module: %d.%d\n", 
                config->version_major, config->version_minor);
        return -1;
    }
    
    // Schema path validation
    if (strlen(config->schema_path) == 0) {
        fprintf(stderr, "Error: Empty schema path for crypto module\n");
        return -1;
    }
    
    printf("Configuration validation successful for crypto module\n");
    return 0;
}
