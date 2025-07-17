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
 * @brief Taxonomy category for DAG node classification
 */
typedef enum {
    TAX_UNKNOWN,
    TAX_ACTION,
    TAX_RESOURCE,
    TAX_PROPERTY,
    TAX_CONTROLLER
} TaxonomyCategory;

/**
 * @brief Node state enumeration
 */
typedef enum {
    STATE_UNKNOWN,
    STATE_TRUE,
    STATE_FALSE
} NodeState;

/**
 * @brief Edge representation for DAG connections
 */
typedef struct DAGEdge {
    struct DAGNode* target;
    float weight;
} DAGEdge;

/**
 * @brief Node representation for DAG structure
 */
typedef struct DAGNode {
    TokenType type;
    TaxonomyCategory category;
    NodeState state;
    DAGEdge* in_edges;
    DAGEdge* out_edges;
    size_t in_count;
    size_t out_count;
} DAGNode;

/**
 * @brief Initialize the DAG subsystem
 * @return 0 on success, non-zero on failure
 */
int dag_init(void);

/**
 * @brief Create a new DAG node
 * @param t Token type for the node
 * @param cat Taxonomy category for the node
 * @return Pointer to the newly created node or NULL on failure
 */
DAGNode* dag_node_create(TokenType t, TaxonomyCategory cat);

/**
 * @brief Add an edge between two nodes
 * @param from Source node
 * @param to Target node
 * @param weight Edge weight (importance factor)
 */
void dag_add_edge(DAGNode *from, DAGNode *to, float weight);

/**
 * @brief Resolve node states through the graph
 * @param nodes Array of nodes to resolve
 * @param node_count Number of nodes in the array
 */
void dag_resolve(DAGNode *nodes[], size_t node_count);

#endif /* POLYBUILD_DAG_H */
