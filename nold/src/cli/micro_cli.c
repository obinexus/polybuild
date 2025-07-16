#include "../core/micro/micro.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * CLI wrapper for micro module
 * Handles command-line argument parsing and delegates to core module
 */

int micro_cli_main(int argc, char* argv[]) {
    // Parse command-line arguments specific to micro
    // TODO: Implement argument parsing logic
    
    return micro_execute(argc, argv);
}
