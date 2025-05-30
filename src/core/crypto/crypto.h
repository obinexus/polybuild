#ifndef POLYBUILD__H
#define POLYBUILD__H

#include <stdint.h>
#include <stdbool.h>

/**
 * Schema-driven cryptographic primitives and validation
 * 
 * This module implements build-time logic for the crypto domain.
 * It operates strictly during the build phase and does not handle runtime execution.
 */

// Module initialization and cleanup
int crypto_init(void);
void crypto_cleanup(void);

// Core module functionality
int crypto_execute(int argc, char* argv[]);
bool crypto_validate_config(const char* config_path);

// Module information
const char* crypto_get_version(void);
const char* crypto_get_description(void);

#endif // POLYBUILD__H
