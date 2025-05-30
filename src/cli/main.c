#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include all module headers
#include "../core/micro/micro.h"
 #include "../core/repl/repl.h"
 #include "../core/edge/edge.h"
 #include "../core/crypto/crypto.h"
 #include "../core/telemetry/telemetry.h"


typedef struct {
    const char* name;
    const char* description;
    int (*execute)(int argc, char* argv[]);
} command_t;

// Command registry
static command_t commands[] = {
    {"micro", "Microservice isolation and dependency management", micro_execute},
     {"repl", "Interactive build configuration testing environment", repl_execute},
     {"edge", "Edge deployment cache preparation and sync logic", edge_execute},
     {"crypto", "Schema-driven cryptographic primitives and validation", crypto_execute},
     {"telemetry", "Build state tracking and checkpoint management", telemetry_execute},

    {NULL, NULL, NULL}
};

void print_usage(const char* program_name) {
    printf("PolyBuild - Modular Build System for Polyglot Projects\n\n");
    printf("Usage: %s <command> [options]\n\n", program_name);
    printf("Available commands:\n");
    
    for (int i = 0; commands[i].name != NULL; i++) {
        printf("  %-12s %s\n", commands[i].name, commands[i].description);
    }
    
    printf("\nFor command-specific help, use: %s <command> --help\n", program_name);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    const char* command_name = argv[1];
    
    // Find and execute command
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(commands[i].name, command_name) == 0) {
            // Initialize all modules
            micro_init();
             repl_init();
             edge_init();
             crypto_init();
             telemetry_init();

            
            // Execute specific command
            int result = commands[i].execute(argc - 1, argv + 1);
            
            // Cleanup all modules
            micro_cleanup();
             repl_cleanup();
             edge_cleanup();
             crypto_cleanup();
             telemetry_cleanup();

            
            return result;
        }
    }
    
    printf("Unknown command: %s\n", command_name);
    print_usage(argv[0]);
    return 1;
}
