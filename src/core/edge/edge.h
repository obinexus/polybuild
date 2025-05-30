#ifndef POLYBUILD__H
#define POLYBUILD__H

#include <stdint.h>
#include <stdbool.h>

/**
 * Edge deployment cache preparation and sync logic
 * 
 * This module implements build-time logic for the edge domain.
 * It operates strictly during the build phase and does not handle runtime execution.
 */

// Module initialization and cleanup
int edge_init(void);
void edge_cleanup(void);

// Core module functionality
int edge_execute(int argc, char* argv[]);
bool edge_validate_config(const char* config_path);

// Module information
const char* edge_get_version(void);
const char* edge_get_description(void);

#endif // POLYBUILD__H
