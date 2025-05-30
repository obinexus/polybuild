#include "../core/crypto/crypto.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * CLI wrapper for crypto module
 * Handles command-line argument parsing and delegates to core module
 */

int crypto_cli_main(int argc, char* argv[]) {
    // Parse command-line arguments specific to crypto
    // TODO: Implement argument parsing logic
    
    return crypto_execute(argc, argv);
}
