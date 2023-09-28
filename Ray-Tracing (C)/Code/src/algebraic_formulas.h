#ifndef ALGEBRAIC_FORMULAS_H
#define ALGEBRAIC_FORMULAS_H

#include "bibliotheque_mathematique.h"
#include "ray_tracing.h"

#include <math.h>

/**
 * @brief Structure utilisée pour stocker deux valeurs flottantes. Utilisée uniquement pour la taille des pixels.
 * 
 */
typedef struct _doublet {
    double x; ///< Double x
    double y; ///< Double y
} Doublet;

/**
 * @brief Calcule le repère u v w.
 * 
 * @param image Image
 * @return Tableau de 3 triplets.
 */
Triplet *orthonormal (Image *image);

/**
 * @brief Calcule le FOV en radians.
 * 
 * @param fov FOV (dans image->cam.fov)
 * @return double
 */
double radian_fov (double fov);

/**
 * @brief Calcule la taille d'un pixel. La stocke dans un Doublet et la retourne.
 * 
 * @param image Image
 * @return Doublet 
 */
Doublet pixel_size (Image *image);

/**
 * @brief Calcule le vecteur direction d et le retourne.
 * 
 * @param image Image
 * @param i Coordonnée i du pixel actuel
 * @param j Coordonnée j du pixel actuel
 * @param ortho Repère u v w
 * @param size Taille d'un pixel
 * @return Triplet (Vecteur)
 */
Triplet pixel_direction_vector (Image *image, int i, int j, Triplet *ortho, Doublet size);

/**
 * @brief Calcule la distance minimale aux sphères pour une direction. Parcours les sphères, cherche des intersections, et retourne la plus proche
 * 
 * @param image Image
 * @param d Vecteur direction
 * @param s Sphère la plus proche que l'on stocke (pointeur de sphère pour modification d'un variable externe)
 * @param eye Point où est l'oeil, donc soit la caméra soit un autre point pour regarder les lumières (vérification ombres)
 * @return Distance minimale si une sphère est trouvée, -1 sinon
 */
double min_dist_to_sphere (Image *image, Triplet d, Sphere *s, Triplet eye, Sphere self);

/**
 * @brief Calcule le point d'intersection étant donné un vecteur direction et une distance
 * 
 * @param eye Où l'on se situe
 * @param pixelDirVec Vecteur direction
 * @param dist Distance
 * @return Triplet (Point)
 */
Triplet intersection_point (Triplet eye, Triplet pixelDirVec, double dist);

/**
 * @brief Calcule la normale en un certain point d'un sphère
 * 
 * @param intersectionPoint Point d'intersection
 * @param circleCenter Centre de la sphère
 * @return Triplet (Vecteur)
 */
Triplet normal_interpoint_sphere (Triplet intersectionPoint, Triplet circleCenter);

/**
 * @brief Calcule la normale d'un triangle
 * 
 * @param t Triangle
 * @return Triplet (Vecteur)
 */
Triplet normal_interpoint_triangle (Triangle t);

/**
 * @brief Calcule la lumière reçue par une lumière directionnelle selon la normale d'un point
 * 
 * @param normPointDir Normale d'un point d'intersection avec un objet
 * @param dl DirectionalLight
 * @return Triplet (Couleur)
 */
Triplet light_direction (Triplet normPointDir, DirectionalLight dl);

/**
 * @brief Calcule la lumière reçue par une lumière ponctuelle
 * 
 * @param lightPoint LightPoint
 * @param intersectionPoint Point d'intersection avec un objet
 * @param normPointDir Normale du point d'intersection
 * @return Triplet (Couleur)
 */
Triplet light_source_color (LightPoint lightPoint, Triplet intersectionPoint, Triplet normPointDir);

/**
 * @brief Créer un tableau de taille [2][maximum de lumière] de int. Toutes les valeurs sont initialisées à 1.
 * 
 * @param im Image
 * @return Tableau à 2 dimension de int.
 */
int **create_obstructed_lights (Image *im);

/**
 * @brief Free le tableau à 2 dimensions de int obstructedLights
 * 
 * @param tab Tableau à free
 */
void free_obstructed_lights (int **tab);

/**
 * @brief Calcule la couleur d'un point selon les lumières.
 * 
 * @param im Image
 * @param normalInterPoint Normale du point d'intersection
 * @param color Couleur de l'objet rencontré
 * @param interPoint Point d'intersection avec l'objet
 * @param obstructedLights Lumières cachées (qui produisent des ombres)
 * @return Triplet (Couleur)
 */
Triplet coul (Image *im, Triplet normalInterPoint, Triplet color, Triplet interPoint, int **obstructedLights);

/**
 * @brief Calcule la distance jusqu'à un plan
 * 
 * @param p Plan
 * @param image Image
 * @param d Vecteur direction
 * @param eye Oeil (caméra ou point)
 * @return Distance
 */
double distance_to_plane (Plane p, Image *image, Triplet d, Triplet eye);

/**
 * @brief Vérifie si un point appartient à un triangle.
 * 
 * @param t Triangle
 * @param point Point pour lequel on cherche si il appartient au triangle
 * @param n Normale du triangle
 * @return 1 si oui, 0 sinon
 */
int point_belongs_to_triangle (Triangle t, Triplet point, Triplet n);

/**
 * @brief Cherche la distance à un triangle en regardant dans une direction.
 * 
 * @param t Triangle
 * @param image Image
 * @param d Vecteur direction
 * @param eye Oeil (caméra ou point)
 * @return Distance si on touche le triangle, -1 sinon
 */
double distance_to_triangle (Triangle t, Image *image, Triplet d, Triplet eye);

/**
 * @brief Calcule le vecteur direction entre deux points.
 * 
 * @param a Point a
 * @param b Point b
 * @return Triplet (Vecteur)
 */
Triplet vector_from_to (Triplet a, Triplet b);

/**
 * @brief Retourne la couleur d'un pixel
 * 
 * @param struc int représentant une structure (0 pour sphère, 1 pour triangle et 2 pour plan)
 * @param interPoint Point d'intersection avec l'objet (peut être nul)
 * @param s Sphère
 * @param tr Triangle
 * @param pl Plane
 * @param obstructedLights Tableau des lumières obstruées
 * @param im Image
 * @param t Distance
 * @return Triplet 
 */
Triplet pixel_color(int struc, Triplet interPoint, Sphere s, Triangle tr, Plane pl, int **obstructedLights, Image *im, int t);

#endif
