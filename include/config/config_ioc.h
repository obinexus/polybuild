/*
 * config_ioc.h - IOC Configuration System Interface
 * Version: 1.0.0 - PolyBuild Configuration Management
 * Author: Aegis Project Team - OBINexus Computing
 */

#ifndef POLYBUILD_CONFIG_IOC_H
#define POLYBUILD_CONFIG_IOC_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_CONFIG_MODULES 16
#define MAX_MODULE_NAME_LENGTH 32
#define MAX_SCHEMA_PATH_LENGTH 512

// Log level enumeration for systematic logging
typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_ERROR = 3
} log_level_t;

// Core configuration structure for systematic module management
typedef struct {
    uint32_t version_major;
    uint32_t version_minor;
    char schema_path[MAX_SCHEMA_PATH_LENGTH];
    bool validation_enabled;
    bool strict_mode;
    log_level_t log_level;
} polybuild_config_t;

// Configuration provider function pointer type
typedef const polybuild_config_t* (*config_provider_t)(void);

// Module configuration registry entry
typedef struct {
    char module_name[MAX_MODULE_NAME_LENGTH];
    int registration_id;
    bool is_active;
    config_provider_t provider;
} module_config_entry_t;

// IOC configuration management operations
int register_config_provider(const char* module_name, config_provider_t provider);
const polybuild_config_t* get_config_for(const char* module_name);
int initialize_config_system(void);
void cleanup_config_system(void);

// Configuration utility operations
const polybuild_config_t* get_default_config(void);
void list_config_providers(void);

// Module-specific configuration providers (forward declarations)
const polybuild_config_t* provide_crypto_config(void);
const polybuild_config_t* provide_micro_config(void);
const polybuild_config_t* provide_telemetry_config(void);
const polybuild_config_t* provide_repl_config(void);

// Configuration validation operations
int validate_crypto_config(const polybuild_config_t* config);
int validate_micro_config(const polybuild_config_t* config);
int validate_telemetry_config(const polybuild_config_t* config);
int validate_repl_config(const polybuild_config_t* config);

#endif // POLYBUILD_CONFIG_IOC_H
