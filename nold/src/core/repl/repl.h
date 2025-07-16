#ifndef POLYBUILD__H
#define POLYBUILD__H

#include <stdint.h>
#include <stdbool.h>

/**
 * Interactive build configuration testing environment
 * 
 * This module implements build-time logic for the repl domain.
 * It operates strictly during the build phase and does not handle runtime execution.
 */

// Module initialization and cleanup
int repl_init(void);
void repl_cleanup(void);

// Core module functionality
int repl_execute(int argc, char* argv[]);
bool repl_validate_config(const char* config_path);

// Module information
const char* repl_get_version(void);
const char* repl_get_description(void);

#endif // POLYBUILD__H
