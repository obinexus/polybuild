#include "../core/repl/repl.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * CLI wrapper for repl module
 * Handles command-line argument parsing and delegates to core module
 */

int repl_cli_main(int argc, char* argv[]) {
    // Parse command-line arguments specific to repl
    // TODO: Implement argument parsing logic
    
    return repl_execute(argc, argv);
}
