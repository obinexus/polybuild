#ifndef POLYBUILD__H
#define POLYBUILD__H

#include <stdint.h>
#include <stdbool.h>

/**
 * Build state tracking and checkpoint management
 * 
 * This module implements build-time logic for the telemetry domain.
 * It operates strictly during the build phase and does not handle runtime execution.
 */

// Module initialization and cleanup
int telemetry_init(void);
void telemetry_cleanup(void);

// Core module functionality
int telemetry_execute(int argc, char* argv[]);
bool telemetry_validate_config(const char* config_path);

// Module information
const char* telemetry_get_version(void);
const char* telemetry_get_description(void);

#endif // POLYBUILD__H
