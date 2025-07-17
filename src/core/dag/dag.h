/**
 * @file dag.h
 * @brief Directed Acyclic Graph implementation for PolyBuild
 * @author OBINexus Computing
 */

#ifndef POLYBUILD_DAG_H
#define POLYBUILD_DAG_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../trie/taxonomy.h" 

/**
 * @brief Token type enumeration for DAG nodes
 */
typedef enum {
    TOKEN_UNKNOWN,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER, 
    TOKEN_STRING,
    TOKEN_OPERATOR
} TokenType;

// Removed TaxonomyCategory enum (now in taxonomy.h)

/**
 * @brief Node state enumeration
 */
typedef enum {
    STATE_UNKNOWN,
    STATE_TRUE,
    STATE_FALSE   
} NodeState;

// Rest of the file remains the same

#endif /* POLYBUILD_DAG_H */