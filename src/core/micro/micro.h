#ifndef POLYBUILD__H
#define POLYBUILD__H

#include <stdint.h>
#include <stdbool.h>

/**
 * Microservice isolation and dependency management
 * 
 * This module implements build-time logic for the micro domain.
 * It operates strictly during the build phase and does not handle runtime execution.
 */

// Module initialization and cleanup
int micro_init(void);
void micro_cleanup(void);

// Core module functionality
int micro_execute(int argc, char* argv[]);
bool micro_validate_config(const char* config_path);

// Module information
const char* micro_get_version(void);
const char* micro_get_description(void);

#endif // POLYBUILD__H
