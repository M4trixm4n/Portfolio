#ifndef TRIANGLES_PLANS_H
#define TRIANGLES_PLANS_H

#include "ray_tracing.h"
#include "algebraic_formulas.h"

/**
 * @brief Génère une image contenant seulement les triangles et les plans, et non les sphères.
 * 
 * @param im Image
 */
void generate_tri_planes_image (Image *im);

/**
 * @brief Remplis obstructedLights pour chaque lumière interceptée pour un certain point par les sphères.
 * 
 * @param interPoint Point
 * @param im Image
 * @param obstructedLights Lumières obstruées
 * @param s Si le point appartient à une sphère (cette sphère s), ignore cette sphère dans les calculs
 */
void check_shadow_spheres (Triplet interPoint, Image *im, int **obstructedLights, Sphere s);

/**
 * @brief Remplis obstructedLights pour chaque lumière interceptée pour un certain point par les triangles.
 * 
 * @param interPoint Point
 * @param im Image
 * @param obstructedLights Lumières obstruées
 * @param tr Si le point appartient à un triangle (ce triangle tr), ignore ce triangle dans les calculs
 */
void check_shadow_triangles (Triplet interPoint, Image *im, int **obstructedLights, Triangle tr);

/**
 * @brief Applique les ombres si elles sont activées
 * 
 * @param im Image
 * @param interPoint Point d'intersection
 * @param obstructedLights Tableau à remplir
 * @param s Sphere
 * @param tr Triangle
 */
void apply_shadow (Image *im, Triplet interPoint, int **obstructedLights, Sphere s, Triangle tr);

#endif
