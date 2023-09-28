#ifndef RAYTRACING_H
#define RAYTRACING_H

#include "bibliotheque_mathematique.h"
#include "parse_scene.h"

/**
 * @brief Représente une sphère
 *
 */
typedef struct _sphere
{
    Triplet center; ///< Centre de la sphère
    double radius; ///< Rayon de la sphère
    Triplet color; ///< Couleur de la sphère
} Sphere;

/**
 * @brief Représente un triangle
 *
 */
typedef struct _triangle
{
    Triplet x; ///< Premier point du triangle
    Triplet y; ///< Deuxième point du triangle
    Triplet z; ///< Troisième point du triangle
    Triplet color; ///< Couleur du triangle
} Triangle;

/**
 * @brief Représente un plan
 *
 */
typedef struct _plane
{
    Triplet point; ///< Point de référence pour le plan
    Triplet normal; ///< Normale du plan
    Triplet color; ///< Couleur du plan
} Plane;

/**
 * @brief Structure stockant les objets créés pendant la lecture de la scène
 *
 */
typedef struct _objects
{
    Sphere *spheres; ///< Tableau de sphères
    Triangle *triangles; ///< Tableau de triangles
    Plane *planes; ///< Tableau de plans
    int nbSpheres; ///< Nombre de sphères stockées
    int nbTriangles; ///< Nombre de triangles stockés
    int nbPlanes; ///< Nombre de plans stockés
} Objects;

/**
 * @brief Représente un point de lumière
 *
 */
typedef struct _lightPoint
{
    Triplet point; ///< Point où est située la source de lumière
    Triplet color; ///< Couleur de la lumière
} LightPoint;

/**
 * @brief Représente une lumière directionnelle globale
 *
 */
typedef struct _directionalLight
{
    Triplet vector; ///< Direction de la lumière
    Triplet color; ///< Couleur de la lumière
} DirectionalLight;

/**
 * @brief Structure stockant toutes les sources de lumière
 *
 */
typedef struct _lights
{
    LightPoint *points; ///< Tableau de LightPoint
    DirectionalLight *directionals; ///< Tableau de DirectionalLight
    int nbPoints; ///< Nombre de points de lumière stockés
    int nbDirectionals; ///< Nombre de lumière directionnelles stockées
} Lights;

/**
 * @brief Représente la caméra ou l'oeil qui observe la scène
 *
 */
typedef struct _camera
{
    Triplet from; ///< Coordonnées de la caméra
    Triplet at; ///< Coordonnées de où la caméra regarde
    Triplet up; ///< Direction vers le haut de l'oeil
    int fov; ///< Largeur du champ de vision
} Camera;

/**
 * @brief Structure image contenant tous les objets et informations lues et créées pendant la lecture de la scène
 *
 */
typedef struct _image
{
    int width; ///< Largeur de l'image à créer (en pixels)
    int height; ///< Hauteur de l'image à créer (en pixels)
    Triplet **pixels; ///< Tableau de pixels (couleur) de l'image finale
    char *name; ///< Nom de l'image à créer
    Camera cam; ///< Structure Camera contenant les informations sur l'observateur de la scène
    struct _ParseData *data; ///< Structure ParseData contenant les informations nécessaire à la création des objets et lumières
    Objects *obj; ///< Structure Objects contenant tous les objets
    Lights *lights; ///< Structure Lights contenant toutes les sources de lumière
} Image;

/**
 * @brief Initialise la majorité des champs de l'image passée en paramètre
 *
 * @param im Image vide, seulement malloc
 * @return 1 s'il y a une erreur, 0 sinon
 */
int init_image(Image *im);

/**
 * @brief Créer un objet Triangle et l'ajouter aux structures stockées dans l'image
 *
 * @param im Image où on ajoute le triangle
 * @param x Premier point
 * @param y Deuxième point
 * @param z Troisième point
 * @return 1 s'il y a une erreur, 0 sinon
 */
int create_object_triangle(Image *im, Triplet x, Triplet y, Triplet z);

/**
 * @brief Créer un objet Sphere et l'ajouter aux structures stockées dans l'image
 *
 * @param im Image où on ajoute le triangle
 * @param center Centre de la sphère
 * @param radius Rayon de la sphère
 * @return 1 s'il y a une erreur, 0 sinon
 */
int create_object_sphere(Image *im, Triplet center, double radius);

/**
 * @brief Créer un objet Plane et l'ajouter aux structures stockées dans l'image
 *
 * @param im Image où on ajoute le triangle
 * @param point Point de référence du plan
 * @param vector Normale du plan
 * @return 1 s'il y a une erreur, 0 sinon
 */
int create_object_plane(Image *im, Triplet point, Triplet vector);

/**
 * @brief Créer une lumière LightPoint et l'ajouter aux structures stockées dans l'image
 *
 * @param im Image où on ajoute le triangle
 * @param point Coordonnées du point de lumière
 * @param color Couleur de la lumière
 * @return 1 s'il y a une erreur, 0 sinon
 */
int create_light_point(Image *im, Triplet point, Triplet color);

/**
 * @brief Créer une lumière DirectionalLight et l'ajouter aux structures stockées dans l'image
 *
 * @param im Image où on ajoute le triangle
 * @param vector Direction de la lumière
 * @param color Couleur de la lumière
 * @return int 1 s'il y a une erreur, 0 sinon
 */
int create_light_directional(Image *im, Triplet vector, Triplet color);

/**
 * @brief Affiche la camera passée en paramètre
 *
 * @param cam Objet Camera
 */
void print_camera(Camera cam);

/**
 * @brief Affiche l'image passée en paramètre
 *
 * @param im Objet Image
 */
void print_image(Image *im);

/**
 * @brief Désalloue l'espace nécessaire à l'attribut pixel de l'image s'il a été alloué
 *
 * @param im L'image
 */
void free_pixels(Image *im);

/**
 * @brief Free la structure Objects dans l'image
 * 
 * @param im Objet Image
 */
void free_objects (Image *im);

/**
 * @brief Free la structure Lights dans l'image
 * 
 * @param im Objet Image
 */
void free_lights (Image *im);

/**
 * @brief Libère l'espace mémoire occupé par l'image passée en paramètre
 *
 * @param im Objet Image
 */
void free_image(Image *im);

/**
 * @brief Fonction qui va allouer l'espace nécessaire à l'attribut pixel de l'image en fonction des dimensions de l'image
 *
 * @param im L'image
 */
void allocate_pixels(Image *im);

/**
 * @brief Détermine si les sphères sont égales
 *
 * @param s1 La première sphère
 * @param s2 La seconde sphère
 * @return 1 si elles sont égales, 0 sinon
 */
int equals_sphere(Sphere s1, Sphere s2);

/**
 * @brief Détermine si les triangles sont égaux
 *
 * @param t1 Le premier triangle
 * @param t2 Le second triangle
 * @return 1 si ils sont égaux, 0 sinon
 */
int equals_triangle(Triangle t1, Triangle t2);

/**
 * @brief Détermine si les plans sont égaux
 *
 * @param p1 Le premier plan
 * @param p2 Le second plan
 * @return 1 si ils sont égaux, 0 sinon
 */
int equals_plane(Plane p1, Plane p2);

/**
 * @brief Afficher les trois points d'un triangle et sa couleur
 * 
 * @param t Triangle à afficher
 */
void print_triangle (Triangle t);

/**
 * @brief Affiche une sphère
 * 
 * @param s Sphère à afficher
 */
void print_sphere (Sphere s);

#endif
