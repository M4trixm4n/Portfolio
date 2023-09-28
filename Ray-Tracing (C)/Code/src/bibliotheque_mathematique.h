#ifndef BIBLIOTHEQUE_MATHEMATIQUE_H

#define BIBLIOTHEQUE_MATHEMATIQUE_H 0

#include <math.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

/**
 *
 * @typedef Triplet
 * @brief Struct that represent a triplet : 3 double
 */
typedef struct
{
    double x;  ///< Le membre x
    double y;  ///< Le membre y
    double z;  ///< Le membre z
    int type;  ///< Le type du triplet, 0 for point, 1 for vector, 2 for color
    int error; ///< S'il s'est passé une erreur lors de la création du triplet, 1 if an error occured, 0 otherwise
} Triplet;

/**
 * @brief Addition between two triplets (Vector)
 *
 * @param t1 First Triplet
 * @param t2 Second Triplet
 * @return The addition of both triplets as a Triplet
 */
Triplet addition(Triplet t1, Triplet t2);
/**
 * @brief Substraction of the first triplet by the second one (Point, Vector)
 *
 * @param t1 First Triplet
 * @param t2 Second Triplet
 * @return The substraction of both triplets as a Triplet
 */
Triplet substraction(Triplet t1, Triplet t2);
/**
 * @brief Scalar multiplication of a triplet (Point, Vector, Color)
 *
 * @param t1 The Triplet
 * @param d The factor
 * @return The result of the multiplication as a Triplet
 */
Triplet scalar_multiplication(Triplet t1, double d);
/**
 * @brief Scalar product between two triplets (Vector)
 *
 * @param t1 The first Triplet
 * @param t2 The second Triplet
 * @return The result of the product
 */
double scalar_product(Triplet t1, Triplet t2);
/**
 * @brief Determine if the output of the function scalar product is the expected one, and therefor if the scalar product is allowed between t1 and t2
 *
 * @param t1
 * @param t2
 * @return 0 if the scalar product is not allowed, >0 otherwise
 */
int scalar_product_test(Triplet t1, Triplet t2);
/**
 * @brief Vector product between two triplets (Vector)
 *
 * @param t1 The first Triplet
 * @param t2 The second Triplet
 * @return The result of the product as a Triplet
 */
Triplet vector_product(Triplet t1, Triplet t2);
/**
 * @brief Schur product between two triplets (Color)
 *
 * @param t1 The first Triplet
 * @param t2 The second Triplet
 * @return The result of the product as a Triplet
 */
Triplet schur_product(Triplet t1, Triplet t2);
/**
 * @brief Determines the length of a vector (Vector)
 *
 * @param t The vector as a Triplet
 * @return The length of the vector
 */
double length(Triplet t);
/**
 * @brief Normalize a vector (Vector)
 *
 * @param t The vector as a Triplet
 * @return The result of the normalization as a Triplet
 */
Triplet norm(Triplet t);
/**
 * @brief Print a Triplet : values of x, y, z
 *
 * @param t The triplet we want to print
 */
void print_triplet(Triplet t);

/**
 * @brief Used for the test of a triplet (checkTriplet.sh)
 *
 * @param input The input
 */
void parse_check_triplet(char *input);

/**
 * @brief Check if two triplets are equals
 *
 * @param t1 The first triplet
 * @param t2 The second triplet
 * @return 1 if they are equals, 0 ohterwise
 */
int equals_triplet(Triplet t1, Triplet t2);

#endif
