/**
 * @file taxonomy.h
 * @brief Taxonomy categories for classification
 * @author OBINexus Computing
 */

#ifndef POLYBUILD_TAXONOMY_H
#define POLYBUILD_TAXONOMY_H

/**
 * @brief Taxonomy category for classification
 */

typedef enum {
    TAX_UNKNOWN,
    TAX_ACTION,  
    TAX_RESOURCE,
    TAX_PROPERTY,
    TAX_CONTROLLER
} TaxonomyCategory;
#endif /* POLYBUILD_TAXONOMY_H */
