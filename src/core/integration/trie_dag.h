/**
 * @file trie_dag.h
 * @brief Integration between Trie and DAG components
 * @author OBINexus Computing
 */

#ifndef POLYBUILD_TRIE_DAG_H
#define POLYBUILD_TRIE_DAG_H
#include "../dag/dag.h" 
#include "../trie/taxonomy.h"

/**
 * @brief Create DAG nodes from trie matches
 * @param root Trie root node
 * @param text Text to process
 * @param len Text length
 * @return Array of created DAG nodes (must be freed by caller)
 */
DAGNode** create_dag_from_trie_matches(TrieNode* root, const char* text, size_t len);

/**
 * @brief Initialize the trie-dag integration
 * @return 0 on success, non-zero on failure
 */
int trie_dag_init(void);

#endif /* POLYBUILD_TRIE_DAG_H */
