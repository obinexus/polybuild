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

/**
 * @brief Node state enumeration 
 */
typedef enum {
    STATE_UNKNOWN,
    STATE_TRUE,
    STATE_FALSE
} NodeState;

typedef struct DAGNode {
  TokenType type;
  TaxonomyCategory category;
  NodeState state;
  struct DAGEdge** in_edges;  
  struct DAGEdge** out_edges;
  size_t in_count;
  size_t out_count;
  DAGEdge** in_edges;
  DAGEdge** out_edges;
} DAGNode;

typedef struct DAGEdge {
    struct DAGNode* target;
    float weight;
} DAGEdge;

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