/*
 * config_ioc.c - Inversion of Control Configuration Engine
 * Version: 1.0.0 - PolyBuild Configuration Management
 * Author: Aegis Project Team - OBINexus Computing
 * 
 * Systematic configuration injection and management across PolyBuild modules
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../../include/config/config_ioc.h"

// Configuration registry for systematic module management
static module_config_entry_t config_registry[MAX_CONFIG_MODULES];
static int registered_module_count = 0;

/**
 * Register configuration provider for specific module
 * Enables systematic dependency injection across build system
 */
int register_config_provider(const char* module_name, config_provider_t provider) {
    if (!module_name || !provider || registered_module_count >= MAX_CONFIG_MODULES) {
        fprintf(stderr, "Error: Cannot register config provider for %s\n", 
                module_name ? module_name : "unknown");
        return -1;
    }
    
    // Check for duplicate registrations
    for (int i = 0; i < registered_module_count; i++) {
        if (strcmp(config_registry[i].module_name, module_name) == 0) {
            printf("Warning: Updating existing config provider for %s\n", module_name);
            config_registry[i].provider = provider;
            return i;
        }
    }
    
    // Register new configuration provider
    module_config_entry_t* entry = &config_registry[registered_module_count];
    strncpy(entry->module_name, module_name, sizeof(entry->module_name) - 1);
    entry->module_name[sizeof(entry->module_name) - 1] = '\0';
    entry->provider = provider;
    entry->is_active = true;
    entry->registration_id = registered_module_count;
    
    printf("Registered configuration provider for module: %s (ID: %d)\n", 
           module_name, registered_module_count);
    
    return registered_module_count++;
}

/**
 * Retrieve configuration for specified module with systematic fallback
 * Implements priority-based configuration resolution
 */
const polybuild_config_t* get_config_for(const char* module_name) {
    if (!module_name) {
        fprintf(stderr, "Error: Module name is NULL\n");
        return NULL;
    }
    
    // Search registered configuration providers
    for (int i = 0; i < registered_module_count; i++) {
        if (strcmp(config_registry[i].module_name, module_name) == 0 && 
            config_registry[i].is_active) {
            
            printf("Retrieving configuration for module: %s\n", module_name);
            return config_registry[i].provider();
        }
    }
    
    printf("Warning: No configuration provider found for module: %s\n", module_name);
    return get_default_config();
}

/**
 * Initialize IOC configuration system with systematic module registration
 */
int initialize_config_system(void) {
    printf("Initializing PolyBuild IOC configuration system\n");
    
    // Register configuration providers for core modules
    extern const polybuild_config_t* provide_crypto_config(void);
    extern const polybuild_config_t* provide_micro_config(void);
    extern const polybuild_config_t* provide_telemetry_config(void);
    extern const polybuild_config_t* provide_repl_config(void);
    
    register_config_provider("crypto", provide_crypto_config);
    register_config_provider("micro", provide_micro_config);
    register_config_provider("telemetry", provide_telemetry_config);
    register_config_provider("repl", provide_repl_config);
    
    printf("IOC configuration system initialized with %d modules\n", registered_module_count);
    return 0;
}

/**
 * Cleanup configuration system resources
 */
void cleanup_config_system(void) {
    printf("Cleaning up IOC configuration system\n");
    
    for (int i = 0; i < registered_module_count; i++) {
        config_registry[i].is_active = false;
    }
    
    registered_module_count = 0;
    printf("Configuration system cleanup completed\n");
}

/**
 * Provide default configuration with systematic fallback values
 */
const polybuild_config_t* get_default_config(void) {
    static polybuild_config_t default_config = {
        .version_major = 1,
        .version_minor = 0,
        .schema_path = "schemas/default/polybuild-v1.json",
        .validation_enabled = true,
        .strict_mode = false,
        .log_level = LOG_LEVEL_INFO
    };
    
    return &default_config;
}

/**
 * List all registered configuration providers for debugging
 */
void list_config_providers(void) {
    printf("Registered Configuration Providers:\n");
    
    if (registered_module_count == 0) {
        printf("  No providers registered\n");
        return;
    }
    
    for (int i = 0; i < registered_module_count; i++) {
        const module_config_entry_t* entry = &config_registry[i];
        printf("  [%d] %-12s %s\n", 
               entry->registration_id,
               entry->module_name,
               entry->is_active ? "ACTIVE" : "INACTIVE");
    }
}
