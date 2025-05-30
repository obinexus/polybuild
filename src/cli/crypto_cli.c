/*
 * crypto_cli.c - CLI wrapper for crypto module
 * Version: 1.0.0 - OBINexus Crypto Module
 * Author: Aegis Project Team
 * 
 * Handles command-line argument parsing and delegates to core crypto module
 * Implements systematic CLI interface for cryptographic operations
 */

#include "../core/crypto/crypto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

// CLI command structure for systematic dispatch
typedef struct {
    const char* name;
    const char* description;
    int (*handler)(int argc, char* argv[]);
} crypto_command_t;

// Forward declarations for command handlers
static int handle_register_command(int argc, char* argv[]);
static int handle_validate_command(int argc, char* argv[]);
static int handle_hash_command(int argc, char* argv[]);
static int print_crypto_help(void);
static void print_command_usage(const char* command);

// Command registry for systematic operation dispatch
static crypto_command_t crypto_commands[] = {
    {"register", "Register cryptographic primitive with configuration", handle_register_command},
    {"validate", "Validate cryptographic configuration schema", handle_validate_command},
    {"hash", "Generate cryptographic hash using specified algorithm", handle_hash_command},
    {"help", "Display crypto module help information", NULL},
    {NULL, NULL, NULL}
};

/**
 * Main CLI entry point for crypto module
 * Implements systematic argument parsing and command dispatch
 */
int crypto_cli_main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("PolyBuild Crypto Module v%s\n", crypto_get_version());
        printf("Usage: polybuild crypto <command> [options]\n\n");
        return print_crypto_help();
    }
    
    const char* command = argv[1];
    
    // Handle help command explicitly
    if (strcmp(command, "help") == 0 || strcmp(command, "--help") == 0) {
        return print_crypto_help();
    }
    
    // Dispatch to appropriate command handler
    for (int i = 0; crypto_commands[i].name != NULL; i++) {
        if (strcmp(crypto_commands[i].name, command) == 0) {
            if (crypto_commands[i].handler) {
                return crypto_commands[i].handler(argc - 1, argv + 1);
            }
        }
    }
    
    printf("Error: Unknown crypto command '%s'\n", command);
    printf("Use 'polybuild crypto help' for available commands\n");
    return 1;
}

/**
 * Handle 'register' command for primitive registration
 * Syntax: polybuild crypto register --primitive <name> --config <path>
 */
static int handle_register_command(int argc, char* argv[]) {
    const char* primitive_name = NULL;
    const char* config_path = "config.json";
    
    // Long options structure for systematic parsing
    static struct option long_options[] = {
        {"primitive", required_argument, 0, 'p'},
        {"config", required_argument, 0, 'c'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    int opt;
    int option_index = 0;
    
    while ((opt = getopt_long(argc, argv, "p:c:h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'p':
                primitive_name = optarg;
                break;
            case 'c':
                config_path = optarg;
                break;
            case 'h':
                print_command_usage("register");
                return 0;
            case '?':
                print_command_usage("register");
                return 1;
        }
    }
    
    if (!primitive_name) {
        printf("Error: --primitive argument required\n");
        print_command_usage("register");
        return 1;
    }
    
    printf("Registering cryptographic primitive...\n");
    printf("  Primitive: %s\n", primitive_name);
    printf("  Configuration: %s\n", config_path);
    
    int result = crypto_register_primitive(primitive_name, config_path);
    
    if (result == CRYPTO_SUCCESS) {
        printf("V Primitive registration completed successfully\n");
        return 0;
    } else {
        printf("? Primitive registration failed (error code: %d)\n", result);
        return 1;
    }
}

/**
 * Handle 'validate' command for schema validation
 * Syntax: polybuild crypto validate --config <schema_path>
 */
static int handle_validate_command(int argc, char* argv[]) {
    const char* config_path = "schemas/crypto-v1.json";
    bool verbose = false;
    
    static struct option long_options[] = {
        {"config", required_argument, 0, 'c'},
        {"schema", required_argument, 0, 's'},  // Alternative name
        {"verbose", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    int opt;
    int option_index = 0;
    
    while ((opt = getopt_long(argc, argv, "c:s:vh", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'c':
            case 's':
                config_path = optarg;
                break;
            case 'v':
                verbose = true;
                break;
            case 'h':
                print_command_usage("validate");
                return 0;
            case '?':
                print_command_usage("validate");
                return 1;
        }
    }
    
    printf("Validating cryptographic configuration...\n");
    if (verbose) {
        printf("  Configuration file: %s\n", config_path);
        printf("  Validation mode: Full schema validation\n");
    }
    
    int result = crypto_validate_schema(config_path);
    
    if (result == CRYPTO_SUCCESS) {
        printf("V Configuration validation completed successfully\n");
        return 0;
    } else {
        printf("? Configuration validation failed (error code: %d)\n", result);
        return 1;
    }
}

/**
 * Handle 'hash' command for cryptographic hashing
 * Syntax: polybuild crypto hash --algorithm <alg> --input <data>
 */
static int handle_hash_command(int argc, char* argv[]) {
    const char* algorithm = CRYPTO_ALG_SHA512;
    const char* input = NULL;
    const char* input_file = NULL;
    bool hex_output = true;
    
    static struct option long_options[] = {
        {"algorithm", required_argument, 0, 'a'},
        {"input", required_argument, 0, 'i'},
        {"file", required_argument, 0, 'f'},
        {"format", required_argument, 0, 'F'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    int opt;
    int option_index = 0;
    
    while ((opt = getopt_long(argc, argv, "a:i:f:F:h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'a':
                algorithm = optarg;
                break;
            case 'i':
                input = optarg;
                break;
            case 'f':
                input_file = optarg;
                break;
            case 'F':
                hex_output = (strcmp(optarg, "hex") == 0);
                break;
            case 'h':
                print_command_usage("hash");
                return 0;
            case '?':
                print_command_usage("hash");
                return 1;
        }
    }
    
    if (!input && !input_file) {
        printf("Error: Either --input or --file argument required\n");
        print_command_usage("hash");
        return 1;
    }
    
    printf("Generating cryptographic hash...\n");
    printf("  Algorithm: %s\n", algorithm);
    printf("  Input source: %s\n", input ? "direct input" : "file input");
    printf("  Output format: %s\n", hex_output ? "hexadecimal" : "base64");
    
    char output[512] = {0};
    const char* hash_input = input ? input : input_file;
    
    int result = crypto_generate_hash(algorithm, hash_input, output, sizeof(output));
    
    if (result == CRYPTO_SUCCESS) {
        printf("V Hash generation completed successfully\n");
        printf("Hash: %s\n", output);
        return 0;
    } else {
        printf("? Hash generation failed (error code: %d)\n", result);
        return 1;
    }
}

/**
 * Print comprehensive help information for crypto module
 */
static int print_crypto_help(void) {
    printf("PolyBuild Crypto Module - Schema-driven Cryptographic Operations\n");
    printf("Version: %s\n\n", crypto_get_version());
    
    printf("USAGE:\n");
    printf("  polybuild crypto <command> [options]\n\n");
    
    printf("COMMANDS:\n");
    for (int i = 0; crypto_commands[i].name != NULL; i++) {
        if (crypto_commands[i].description) {
            printf("  %-12s %s\n", crypto_commands[i].name, crypto_commands[i].description);
        }
    }
    
    printf("\nSUPPORTED ALGORITHMS:\n");
    printf("  %-20s SHA-512 cryptographic hash function\n", CRYPTO_ALG_SHA512);
    printf("  %-20s PBKDF2 with HMAC-SHA512 key derivation\n", CRYPTO_ALG_PBKDF2);
    printf("  %-20s AES-256 in GCM mode encryption\n", CRYPTO_ALG_AES256_GCM);
    
    printf("\nEXAMPLES:\n");
    printf("  # Register SHA512 primitive with custom config\n");
    printf("  polybuild crypto register --primitive SHA512 --config schemas/sha512.json\n\n");
    
    printf("  # Validate default crypto configuration\n");
    printf("  polybuild crypto validate --config schemas/crypto-v1.json\n\n");
    
    printf("  # Generate SHA512 hash of input string\n");
    printf("  polybuild crypto hash --algorithm SHA512 --input \"build_artifact\"\n\n");
    
    printf("  # Generate hash from file content\n");
    printf("  polybuild crypto hash --algorithm SHA512 --file build/output.bin\n\n");
    
    printf("For command-specific help: polybuild crypto <command> --help\n");
    return 0;
}

/**
 * Print usage information for specific command
 */
static void print_command_usage(const char* command) {
    printf("Usage: polybuild crypto %s [options]\n\n", command);
    
    if (strcmp(command, "register") == 0) {
        printf("Register cryptographic primitive with configuration\n\n");
        printf("OPTIONS:\n");
        printf("  -p, --primitive <name>  Primitive name (SHA512, PBKDF2_HMAC_SHA512, AES256_GCM)\n");
        printf("  -c, --config <path>     Configuration file path (default: config.json)\n");
        printf("  -h, --help              Show this help message\n");
        
    } else if (strcmp(command, "validate") == 0) {
        printf("Validate cryptographic configuration schema\n\n");
        printf("OPTIONS:\n");
        printf("  -c, --config <path>     Configuration file to validate\n");
        printf("  -s, --schema <path>     Schema file path (alternative to --config)\n");
        printf("  -v, --verbose           Enable verbose validation output\n");
        printf("  -h, --help              Show this help message\n");
        
    } else if (strcmp(command, "hash") == 0) {
        printf("Generate cryptographic hash using specified algorithm\n\n");
        printf("OPTIONS:\n");
        printf("  -a, --algorithm <alg>   Hash algorithm (default: SHA512)\n");
        printf("  -i, --input <data>      Input data string to hash\n");  
        printf("  -f, --file <path>       Input file to hash\n");
        printf("  -F, --format <fmt>      Output format (hex|base64, default: hex)\n");
        printf("  -h, --help              Show this help message\n");
    }
}
