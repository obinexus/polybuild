/*
 * crypto.h - Main crypto module header
 * Version: 1.0.0 - OBINexus Crypto Module
 * Author: Aegis Project Team
 */

#ifndef POLYBUILD_CRYPTO_H
#define POLYBUILD_CRYPTO_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

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

// Crypto-specific operations
int crypto_register_primitive(const char* primitive_name, const char* config_path);
int crypto_validate_schema(const char* schema_path);
int crypto_generate_hash(const char* algorithm, const char* input, char* output, size_t output_len);

// Error codes
#define CRYPTO_SUCCESS           0
#define CRYPTO_ERROR_INVALID     -1
#define CRYPTO_ERROR_NOT_FOUND   -2
#define CRYPTO_ERROR_CONFIG      -3
#define CRYPTO_ERROR_VALIDATION  -4

// Supported algorithms
#define CRYPTO_ALG_SHA512        "SHA512"
#define CRYPTO_ALG_PBKDF2        "PBKDF2_HMAC_SHA512"
#define CRYPTO_ALG_AES256_GCM    "AES256_GCM"

// Configuration limits
#define CRYPTO_MIN_PBKDF2_ITERATIONS  600000
#define CRYPTO_MAX_ALGORITHM_NAME_LEN 64
#define CRYPTO_MAX_CONFIG_PATH_LEN    512

// Forward declarations for subsystem headers
#include "primitives/sha512.h"
#include "primitives/pbkdf2.h"
#include "primitives/aes_gcm.h"
#include "validation/validator.h"
#include "schemas/schema_parser.h"
#include "integration/binding_interface.h"

#endif // POLYBUILD_CRYPTO_H

/*
 * primitives/sha512.h - SHA512 primitive implementation
 * Version: 1.0.0 - OBINexus Crypto Module
 */

#ifndef POLYBUILD_CRYPTO_SHA512_H
#define POLYBUILD_CRYPTO_SHA512_H

#include <stdint.h>
#include <stddef.h>

#define SHA512_DIGEST_LENGTH 64
#define SHA512_BLOCK_SIZE    128

// SHA512 context structure
typedef struct {
    uint64_t state[8];
    uint64_t count[2];
    uint8_t buffer[SHA512_BLOCK_SIZE];
} sha512_context_t;

// SHA512 operations
int sha512_init(sha512_context_t* ctx);
int sha512_update(sha512_context_t* ctx, const uint8_t* data, size_t len);
int sha512_final(sha512_context_t* ctx, uint8_t* digest);
int sha512_hash(const uint8_t* input, size_t input_len, uint8_t* output);

// Configuration validation
int validate_sha512_config(const char* config_path);

#endif // POLYBUILD_CRYPTO_SHA512_H

/*
 * primitives/pbkdf2.h - PBKDF2 primitive implementation
 * Version: 1.0.0 - OBINexus Crypto Module
 */

#ifndef POLYBUILD_CRYPTO_PBKDF2_H
#define POLYBUILD_CRYPTO_PBKDF2_H

#include <stdint.h>
#include <stddef.h>

#define PBKDF2_MIN_ITERATIONS  600000
#define PBKDF2_MAX_ITERATIONS  10000000
#define PBKDF2_SALT_LENGTH     32
#define PBKDF2_KEY_LENGTH      64

// PBKDF2 configuration structure
typedef struct {
    uint32_t iterations;
    uint8_t salt[PBKDF2_SALT_LENGTH];
    size_t key_length;
} pbkdf2_config_t;

// PBKDF2 operations
int pbkdf2_derive_key(const char* password, size_t password_len, 
                      const uint8_t* salt, size_t salt_len,
                      uint32_t iterations, uint8_t* key, size_t key_len);
int pbkdf2_generate_salt(uint8_t* salt, size_t salt_len);
int pbkdf2_validate_params(const pbkdf2_config_t* config);

// Configuration validation
int validate_pbkdf2_config(const char* config_path);

#endif // POLYBUILD_CRYPTO_PBKDF2_H

/*
 * primitives/aes_gcm.h - AES-GCM primitive implementation
 * Version: 1.0.0 - OBINexus Crypto Module
 */

#ifndef POLYBUILD_CRYPTO_AES_GCM_H
#define POLYBUILD_CRYPTO_AES_GCM_H

#include <stdint.h>
#include <stddef.h>

#define AES_GCM_KEY_LENGTH   32  // AES-256
#define AES_GCM_IV_LENGTH    12  // 96-bit IV
#define AES_GCM_TAG_LENGTH   16  // 128-bit tag

// AES-GCM configuration structure
typedef struct {
    uint8_t key[AES_GCM_KEY_LENGTH];
    uint8_t iv[AES_GCM_IV_LENGTH];
    size_t aad_len;
} aes_gcm_config_t;

// AES-GCM operations
int aes_gcm_encrypt(const uint8_t* key, const uint8_t* iv,
                    const uint8_t* plaintext, size_t plaintext_len,
                    const uint8_t* aad, size_t aad_len,
                    uint8_t* ciphertext, uint8_t* tag);

int aes_gcm_decrypt(const uint8_t* key, const uint8_t* iv,
                    const uint8_t* ciphertext, size_t ciphertext_len,
                    const uint8_t* aad, size_t aad_len,
                    const uint8_t* tag, uint8_t* plaintext);

// Configuration validation
int validate_aes_gcm_config(const char* config_path);

#endif // POLYBUILD_CRYPTO_AES_GCM_H

/*
 * validation/validator.h - Crypto validation subsystem
 * Version: 1.0.0 - OBINexus Crypto Module
 */

#ifndef POLYBUILD_CRYPTO_VALIDATOR_H
#define POLYBUILD_CRYPTO_VALIDATOR_H

#include <stdbool.h>

// Validation result structure
typedef struct {
    bool is_valid;
    int error_code;
    char error_message[256];
} validation_result_t;

// Validation operations
validation_result_t validate_crypto_config(const char* config_path);
validation_result_t validate_algorithm_allowlist(const char* algorithm);
validation_result_t validate_parameter_bounds(const char* algorithm, const char* param, int value);
bool is_algorithm_deprecated(const char* algorithm);

#endif // POLYBUILD_CRYPTO_VALIDATOR_H
