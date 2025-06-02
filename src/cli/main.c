/*
 * main.c - Enhanced PolyBuild CLI with IOC Configuration Integration
 * Version: 1.0.0 - PolyBuild Modular Build System
 * Author: Aegis Project Team - OBINexus Computing
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include all module headers with systematic organization
#include <polybuild/core/crypto.h>
#include "../core/micro/micro.h"
#include "../core/edge/edge.h"
#include "../core/telemetry/telemetry.h"
#include "../core/repl/repl.h"

// Include IOC configuration system
#include <polybuild/core/config_ioc.h>

// Forward declarations for CLI interface functions
extern int crypto_cli_main(int argc, char* argv[]);
extern int micro_cli_main(int argc, char* argv[]);
extern int edge_cli_main(int argc, char* argv[]);
extern int telemetry_cli_main(int argc, char* argv[]);
extern int repl_cli_main(int argc, char* argv[]);
extern int config_cli_main(int argc, char* argv[]);

// Command registry structure for systematic dispatch
typedef struct {
    const char* name;
    const char* description;
    int (*execute)(int argc, char* argv[]);
    bool requires_config;
} command_t;

// Enhanced command registry with configuration integration
static command_t commands[] = {
    {"crypto", "Schema-driven cryptographic primitives and validation", crypto_cli_main, true},
    {"micro", "Microservice isolation and dependency management", micro_cli_main, true},
    {"edge", "Edge deployment cache preparation and sync logic", edge_cli_main, true},
    {"telemetry", "Build state tracking and checkpoint management", telemetry_cli_main, true},
    {"repl", "Interactive build configuration testing environment", repl_cli_main, true},
    {"config", "Configuration management and IOC system interface", config_cli_main, false},
    {NULL, NULL, NULL, false}
};

