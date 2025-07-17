/**
 * @file trie.h
 * @brief Trie data structure implementation for PolyBuild
 * @author OBINexus Computing
 */

#ifndef POLYBUILD_TRIE_H
#define POLYBUILD_TRIE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <regex.h>
#include "taxonomy.h"

/**
 * @brief Trie node for pattern matching
 */
typedef struct TrieNode {
    char* pattern_str;
    regex_t pattern;
    TaxonomyCategory category;
    float weight;
    bool terminal;
    struct TrieNode* children[256]; // One for each possible byte
} TrieNode;

/**
 * @brief Initialize the trie subsystem
 * @return 0 on success, non-zero on failure
 */
int trie_init(void);

/**
 * @brief Create a new trie node
 * @param pattern_str Regular expression pattern string
 * @param cat Taxonomy category for classification
 * @param weight Pattern importance weight
 * @return Pointer to the newly created node or NULL on failure
 */
TrieNode* trie_node_create(const char* pattern_str,
                           TaxonomyCategory cat,
                           float weight);

/**
 * @brief Check if text matches the node's pattern
 * @param node The trie node to check against
 * @param text Text to match
 * @param len Length of the text
 * @return True if match, false otherwise
 */
bool trie_match_node(TrieNode* node, const char* text, size_t len);

/**
 * @brief Insert a pattern into the trie
 * @param root Root node of the trie
 * @param pattern_str Pattern string to insert
 * @param cat Taxonomy category for the pattern
 * @param weight Pattern importance weight
 */
void trie_insert(TrieNode* root,
                 const char* pattern_str,
                 TaxonomyCategory cat,
                 float weight);

#endif /* POLYBUILD_TRIE_H */
