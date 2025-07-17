/*
 * intent_dag_integration.h - Integration between Intent Resolution and DAG
 * OBINexus Computing - PolyBuild Architecture
 */

#ifndef POLYBUILD_INTENT_DAG_H
#define POLYBUILD_INTENT_DAG_H

#include "dag.h"
#include "trie.h"
#include <stdint.h>
#include <stdbool.h>

// =============================================================================
// INTENT-DAG INTEGRATION TYPES
// =============================================================================

typedef enum {
    INTENT_VERB_VALIDATE = 0,
    INTENT_VERB_BUILD = 1,
    INTENT_VERB_COMPILE = 2,
    INTENT_VERB_LINK = 3,
    INTENT_VERB_TEST = 4,
    INTENT_VERB_DEPLOY = 5,
    INTENT_VERB_CLEAN = 6,
    INTENT_VERB_REROUTE = 7,
    INTENT_VERB_CONFIGURE = 8
} IntentVerb;

typedef enum {
    INTENT_NOUN_POLICY = 0,
    INTENT_NOUN_TARGET = 1,
    INTENT_NOUN_SOURCE = 2,
    INTENT_NOUN_DEPENDENCY = 3,
    INTENT_NOUN_ARTIFACT = 4,
    INTENT_NOUN_PIPELINE = 5,
    INTENT_NOUN_CONFIGURATION = 6,
    INTENT_NOUN_MANIFEST = 7
} IntentNoun;

typedef enum {
    INTENT_STAGE_TODO = 0,
    INTENT_STAGE_DOING = 1,
    INTENT_STAGE_DONE = 2
} IntentStage;

typedef struct {
    IntentVerb verb;
    IntentNoun noun;
    char* binding_value;
    IntentStage stage;
    uint32_t priority;
    bool triggers_action;
    char* semantic_context;
    DAGNode* dag_representation;  // Link to DAG node
} IntentResolution;

typedef struct {
    char binary_encoding[8];  // "0101101"
    uint8_t topology_type;    // Decoded from bits 2-3
    uint8_t fault_tolerance;  // Decoded from bit 4
    uint8_t concurrency_model;// Decoded from bits 5-6
    bool semantic_validation; // Decoded from bit 0
} TopologyDecoding;

// =============================================================================
// TOPOLOGY DECODING FUNCTIONS
// =============================================================================

/**
 * @brief Decode binary topology string into structured format
 * @param binary_str Binary string like "0101101"
 * @return Decoded topology specification
 */
TopologyDecoding* decode_topology_binary(const char* binary_str);

/**
 * @brief Apply topology constraints to DAG resolution
 * @param dag_nodes Array of DAG nodes
 * @param node_count Number of nodes
 * @param topology Topology specification
 * @return 0 on success, -1 on failure
 */
int apply_topology_constraints(DAGNode* dag_nodes[], size_t node_count, 
                              TopologyDecoding* topology);

// =============================================================================
// INTENT RESOLUTION FUNCTIONS  
// =============================================================================

/**
 * @brief Parse intent expression into structured resolution
 * @param expression String like "validate policy live"
 * @param topology Topology context for resolution
 * @return Parsed intent resolution structure
 */
IntentResolution* parse_intent_expression(const char* expression, 
                                        TopologyDecoding* topology);

/**
 * @brief Create DAG node from intent resolution
 * @param intent Intent to convert to DAG node
 * @return DAG node representing the intent
 */
DAGNode* create_dag_from_intent(IntentResolution* intent);

/**
 * @brief Resolve intent through stage transitions (TODO->DOING->DONE)
 * @param intent Intent to resolve
 * @param topology Topology context
 * @return true if intent completed, false if still processing
 */
bool resolve_intent_stages(IntentResolution* intent, TopologyDecoding* topology);

/**
 * @brief Create trie pattern for intent verb-noun matching
 * @param root Trie root node
 * @param intent Intent to create pattern for
 * @return 0 on success, -1 on failure
 */
int insert_intent_pattern(TrieNode* root, IntentResolution* intent);

// =============================================================================
// XML SEMANTIC ENFORCEMENT
// =============================================================================

typedef struct {
    char* namespace_uri;
    char* element_name;
    char** allowed_values;
    size_t value_count;
    bool required;
} SemanticRule;

typedef struct {
    SemanticRule* rules;
    size_t rule_count;
    bool strict_validation;
    char* schema_version;
} SemanticValidator;

/**
 * @brief Create semantic validator for XML manifests
 * @return Initialized semantic validator
 */
SemanticValidator* create_semantic_validator(void);

/**
 * @brief Validate intent against semantic rules
 * @param intent Intent to validate
 * @param validator Semantic validator
 * @return true if valid, false if validation failed
 */
bool validate_intent_semantics(IntentResolution* intent, SemanticValidator* validator);

/**
 * @brief Validate topology encoding against semantic rules
 * @param topology Topology to validate
 * @param validator Semantic validator
 * @return true if valid, false if validation failed
 */
bool validate_topology_semantics(TopologyDecoding* topology, SemanticValidator* validator);

// =============================================================================
// INTEGRATION WORKFLOW FUNCTIONS
// =============================================================================

/**
 * @brief Complete intent-driven build workflow
 * @param xml_manifest Path to XML manifest file
 * @param output_dag Resulting DAG structure
 * @return 0 on success, error code on failure
 */
int execute_intent_workflow(const char* xml_manifest, DAGNode*** output_dag, size_t* dag_size);

/**
 * @brief Apply parallel processing based on topology
 * @param intents Array of intent resolutions
 * @param intent_count Number of intents
 * @param topology Topology specification
 * @return 0 on success, -1 on failure
 */
int apply_parallel_intent_processing(IntentResolution** intents, size_t intent_count,
                                   TopologyDecoding* topology);

/**
 * @brief Generate build actions from resolved intents
 * @param intents Array of resolved intents  
 * @param intent_count Number of intents
 * @param output_actions Generated build actions
 * @return Number of actions generated
 */
size_t generate_build_actions(IntentResolution** intents, size_t intent_count,
                            char*** output_actions);

// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

/**
 * @brief Convert intent verb to string representation
 * @param verb Intent verb enum
 * @return String representation
 */
const char* intent_verb_to_string(IntentVerb verb);

/**
 * @brief Convert intent noun to string representation  
 * @param noun Intent noun enum
 * @return String representation
 */
const char* intent_noun_to_string(IntentNoun noun);

/**
 * @brief Convert intent stage to string representation
 * @param stage Intent stage enum
 * @return String representation
 */
const char* intent_stage_to_string(IntentStage stage);

/**
 * @brief Print intent resolution for debugging
 * @param intent Intent to print
 */
void print_intent_resolution(IntentResolution* intent);

/**
 * @brief Print topology decoding for debugging
 * @param topology Topology to print
 */
void print_topology_decoding(TopologyDecoding* topology);

/**
 * @brief Free intent resolution structure
 * @param intent Intent to free
 */
void free_intent_resolution(IntentResolution* intent);

/**
 * @brief Free topology decoding structure
 * @param topology Topology to free
 */
void free_topology_decoding(TopologyDecoding* topology);

/**
 * @brief Free semantic validator structure
 * @param validator Validator to free
 */
void free_semantic_validator(SemanticValidator* validator);

#endif /* POLYBUILD_INTENT_DAG_H */

/*
 * Example Usage:
 * 
 * // 1. Decode topology from binary
 * TopologyDecoding* topology = decode_topology_binary("0101101");
 * 
 * // 2. Parse intent expressions
 * IntentResolution* intent1 = parse_intent_expression("validate policy live", topology);
 * IntentResolution* intent2 = parse_intent_expression("build target release", topology);
 * 
 * // 3. Create DAG from intents
 * DAGNode* dag1 = create_dag_from_intent(intent1);
 * DAGNode* dag2 = create_dag_from_intent(intent2);
 * 
 * // 4. Apply topology constraints
 * DAGNode* dag_nodes[] = {dag1, dag2};
 * apply_topology_constraints(dag_nodes, 2, topology);
 * 
 * // 5. Resolve through stages
 * while (!resolve_intent_stages(intent1, topology)) {
 *     // Continue processing until DONE
 * }
 * 
 * // 6. Generate build actions
 * IntentResolution* intents[] = {intent1, intent2};
 * char** actions;
 * size_t action_count = generate_build_actions(intents, 2, &actions);
 */
