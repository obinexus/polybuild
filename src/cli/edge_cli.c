#include "../core/edge/edge.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * CLI wrapper for edge module
 * Handles command-line argument parsing and delegates to core module
 */

int edge_cli_main(int argc, char* argv[]) {
    // Parse command-line arguments specific to edge
    // TODO: Implement argument parsing logic
    
    return edge_execute(argc, argv);
}
