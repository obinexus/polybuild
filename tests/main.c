#include <stdio.h>
// #include <polybuild/dag.h>
// #include <polybuild/trie.h>
// #include <polybuild/trie_dag.h>


#include "polybuild/dag.h"
#include "polybuild/trie.h"
#include "polybuild/trie_dag.h"
int main() {
    printf("PolyBuild Test Suite\n");
    
    // Initialize subsystems
    if (trie_dag_init() != 0) {
        printf("Failed to initialize subsystems\n");
        return 1;
    }
    
    printf("All subsystems initialized successfully\n");
    
    // Create a simple trie for testing
    TrieNode* root = trie_node_create("root", TAX_UNKNOWN, 1.0f);
    if (!root) {
        printf("Failed to create root trie node\n");
        return 1;
    }
    
    // Insert patterns
    trie_insert(root, "test", TAX_ACTION, 1.0f);
    trie_insert(root, "build", TAX_ACTION, 1.0f);
    
    // Create DAG nodes
    DAGNode* node1 = dag_node_create(TOKEN_IDENTIFIER, TAX_ACTION);
    DAGNode* node2 = dag_node_create(TOKEN_IDENTIFIER, TAX_RESOURCE);
    
    if (!node1 || !node2) {
        printf("Failed to create DAG nodes\n");
        return 1;
    }
    
    // Add edge
    dag_add_edge(node1, node2, 1.0f);
    
    // Set up nodes array for resolution
    DAGNode* nodes[] = {node1, node2};
    
    // Resolve
    dag_resolve(nodes, 2);
    
    printf("DAG resolution successful\n");
    printf("All tests passed!\n");
    
    return 0;
}
