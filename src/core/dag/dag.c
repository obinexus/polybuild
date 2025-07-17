#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "dag.h"

// Forward declaration of helper function
static void resolve_node_recursive(DAGNode *nodes[], size_t node_count, 
                                  size_t current, bool *visited, bool *resolved);
/**
 * Initialize the DAG subsystem
 * Returns 0 on success, non-zero on failure
 */
int dag_init(void) {
    // Placeholder implementation
    return 0;
}
                                
DAGNode* dag_node_create(TokenType t, TaxonomyCategory cat) {
    DAGNode *node = (DAGNode*)calloc(1, sizeof(DAGNode));
    if (!node) return NULL;
    
    node->type = t;
    node->category = cat;
    node->state = STATE_UNKNOWN;
    node->in_edges = NULL;
    node->out_edges = NULL;
    node->in_count = 0;
    node->out_count = 0;
    
    return node;
}

void dag_add_edge(DAGNode *from, DAGNode *to, float weight) {
    if (!from || !to) return;
    
    // Allocate/reallocate outgoing edge array
    from->out_edges = (DAGEdge**)realloc(from->out_edges, 
                                       (from->out_count + 1) * sizeof(DAGEdge*));
    if (!from->out_edges) return;

    // Allocate memory for the new edge
    from->out_edges[from->out_count] = (DAGEdge*)malloc(sizeof(DAGEdge));
    if (!from->out_edges[from->out_count]) return;
    
    // Allocate/reallocate incoming edge array
    to->in_edges = (DAGEdge**)realloc(to->in_edges, 
                                    (to->in_count + 1) * sizeof(DAGEdge*));
    if (!to->in_edges) return;

    // Allocate memory for the new edge  
    to->in_edges[to->in_count] = (DAGEdge*)malloc(sizeof(DAGEdge));
    if (!to->in_edges[to->in_count]) return;
    
    
      // Set up the new outgoing edge
    from->out_edges[from->out_count]->target = to;
    from->out_edges[from->out_count]->weight = weight;
    from->out_count++;
    
    // Set up the new incoming edge  
    to->in_edges[to->in_count]->target = from;
    to->in_edges[to->in_count]->weight = weight; 
    to->in_count++;
}

void dag_resolve(DAGNode *nodes[], size_t node_count) {
    if (!nodes || node_count == 0) {
        return;
    }
    
    // Track visited and resolved states for topological sort
    bool *visited = (bool *)calloc(node_count, sizeof(bool));
    bool *resolved = (bool *)calloc(node_count, sizeof(bool));
    
    if (!visited || !resolved) {
        free(visited);
        free(resolved);
        return;
    }
    
    // Perform depth-first topological resolution
    for (size_t i = 0; i < node_count; i++) {
        if (!visited[i]) {
            resolve_node_recursive(nodes, node_count, i, visited, resolved);
        }
    }
    
    free(visited);
    free(resolved);
}

static void resolve_node_recursive(DAGNode *nodes[], size_t node_count, 
                                  size_t current, bool *visited, bool *resolved) {
    visited[current] = true;
    
    // Process all outgoing edges
    DAGNode *node = nodes[current];
    for (size_t i = 0; i < node->out_count; i++) {
        // Find index of the target node
        DAGNode *target = node->out_edges[i]->target;
        size_t target_idx = 0;
        
        // Find the index of this target node in our nodes array
        for (size_t j = 0; j < node_count; j++) {
            if (nodes[j] == target) {
                target_idx = j;
                break;
            }
        }
        
        // Recursive resolution of dependencies
        if (!visited[target_idx]) {
            resolve_node_recursive(nodes, node_count, target_idx, visited, resolved);
        }
    }
    
    // Now resolve this node's truth value
    if (!resolved[current]) {
        // Default to true for root nodes (no incoming edges)
        if (node->in_count == 0) {
            node->state = STATE_TRUE;
        } else {
            // Determine truth based on weighted incoming edges
            float true_weight = 0.0f;
            float false_weight = 0.0f;
            
            for (size_t i = 0; i < node->in_count; i++) {
                // Find the source node
                DAGNode *source = NULL;
                
                // Search for the source node that has this edge
                for (size_t j = 0; j < node_count; j++) {
                    for (size_t k = 0; k < nodes[j]->out_count; k++) {
                        if (nodes[j]->out_edges[k]->target == node) {
                            source = nodes[j];
                            break;
                        }
                    }
                    if (source) break;
                }
                
                if (!source) continue;
                
                // Accumulate weighted influence from source nodes
                float edge_weight = node->in_edges[i]->weight;
                if (source->state == STATE_TRUE) {
                    true_weight += edge_weight;
                } else if (source->state == STATE_FALSE) {
                    false_weight += edge_weight;
                }
            }
            
            // Final truth determination based on weighted influences
            if (true_weight > false_weight) {
                node->state = STATE_TRUE;
            } else if (false_weight > true_weight) {
                node->state = STATE_FALSE;
            } else {
                // Equal weights or no resolved inputs
                node->state = STATE_UNKNOWN;
            }
        }
        
        resolved[current] = true;
    }
}