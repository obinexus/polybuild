#include "trie_dag.h"
#include <stdlib.h>
#include <string.h>

// Maximum number of DAG nodes to create from trie matches
#define MAX_MATCHES 100

/**
 * Initialize the trie-dag integration
 */
int trie_dag_init(void) {
    // Ensure both trie and dag subsystems are initialized
    int trie_result = trie_init();
    int dag_result = dag_init();
    
    return (trie_result == 0 && dag_result == 0) ? 0 : -1;
}

/**
 * Create DAG nodes from trie matches
 */
DAGNode** create_dag_from_trie_matches(TrieNode* root, const char* text, size_t len) {
    if (!root || !text || len == 0) {
        return NULL;
    }
    
    // Allocate result array
    DAGNode** result = (DAGNode**)calloc(MAX_MATCHES + 1, sizeof(DAGNode*));
    if (!result) {
        return NULL;
    }
    
    // Match count
    size_t match_count = 0;
    
    // Process each character position as a potential start
    for (size_t i = 0; i < len && match_count < MAX_MATCHES; i++) {
        // Try to match at each remaining length
        for (size_t j = 1; j <= len - i && match_count < MAX_MATCHES; j++) {
            // For each character, check all children of root
            for (int k = 0; k < 256 && match_count < MAX_MATCHES; k++) {
                TrieNode* child = root->children[k];
                if (child && trie_match_node(child, text + i, j)) {
                    // Create a DAG node for this match
                    DAGNode* node = dag_node_create(TOKEN_STRING, child->category);
                    if (node) {
                        result[match_count++] = node;
                    }
                }
            }
        }
    }
    
    // NULL terminate the array
    result[match_count] = NULL;
    
    return result;
}
