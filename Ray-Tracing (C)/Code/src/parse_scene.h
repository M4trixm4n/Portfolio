#ifndef PARSESCENE_H
#define PARSESCENE_H

#include "error_handler.h"
#include "ray_tracing.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _image Image;     // Forward Declaration
typedef struct _camera Camera;   // Forward Declaration


/**
 * @brief Structure stockant toutes les données récupérées pendant la lecture de la scène
 * 
 */
typedef struct _ParseData {
    Triplet ambient; ///< Couleur ambiante
    Triplet diffuse; ///< Couleur d'objets
    Triplet specular; ///< Réflechissement
    int shininess; ///< Brillance
    int maxverts; ///< Nombre maximum de points
    Triplet *vertex; ///< Tableau des points créés
    int nbVertex; ///< Nombre de points actuellement créés
    bool shadow; ///< Booléen indiquant si les ombres sont activées ou non
} ParseData;

/**
 * @brief Démarre le parse en ouvrant le fichier et en appelant parse_head, puis ferme le fichier.
 *
 * @param fileName Nom du fichier à ouvrir
 * @return Image contenant l'entièreté des valeurs de la scène
 */
Image *parse_start (char *fileName);

/**
 * @brief Parse le début du fichier exclusivement: size, output et camera.
 * @brief Retourne une erreur si size ou camera n'ont pas été initialisés avant le rencontre d'un mot clé différent.
 * @brief Appelle parse_body une fois la tête du fichier lue.
 *
 * @param im Image à remplir et à retourner
 * @param f Fichier lu ligne par ligne
 * @return Image contenant l'entièreté des valeurs de la scène
 */
Image *parse_head (Image *im, FILE *f);

/**
 * @brief Parse tout le fichier après la tête.
 *
 * @param im Image à remplir et à retourner
 * @param f Fichier lu ligne par ligne
 * @return Image contenant l'entièreté des valeurs de la scène
 */
Image *parse_body (Image *im, FILE *f);

/**
 * @brief Redirige les arguments de la ligne vers la bonne fonction (pour le body) selon le premier argument (mot clé)
 *
 * @param im Image à remplir
 * @param args Liste des arguments de la ligne
 * @param nbArgs Nombre d'arguments
 * @return 1 s'il y a une erreur, 0 sinon
 */
int router_body (Image *im, char **args, int nbArgs);

/**
 * @brief Redirige les arguments de la ligne vers la bonne fonction (pour la head) selon le premier argument (mot clé)
 *
 * @param im Image à remplir
 * @param args Liste des arguments de la ligne
 * @param nbArgs Nombre d'arguments
 * @return 1 s'il y a une erreur, 0 sinon
 */
int router_head (Image *im, char **args, int nbArgs);

/**
 * @brief Vérifie si x est un float positif entre 0 et 1
 *
 * @param x Float
 * @return 1 si valide, 0 sinon
 */
int valid_float (double x);

/**
 * @brief Vérifie si x est un int et non un float
 *
 * @param x Float (censé être un int)
 * @return 1 si valide, 0 sinon
 */
int valid_int (double x);

/**
 * @brief Parse une ligne de type "size"
 *
 * @param im Image à remplir
 * @param args Tous les arguments de la ligne
 * @param nbArgs Nombre d'arguments de la ligne
 * @return 1 s'il y a une erreur, 0 sinon
 */
int parse_data_size (Image *im, char **args, int nbArgs);

/**
 * @brief Parse une ligne de type "output"
 *
 * @param im Image à remplir
 * @param args Tous les arguments de la ligne
 * @param nbArgs Nombre d'arguments de la ligne
 * @return 1 s'il y a une erreur, 0 sinon
 */
int parse_data_output (Image *im, char **args, int nbArgs);

/**
 * @brief Parse une ligne de type "camera"
 *
 * @param im Image à remplir
 * @param args Tous les arguments de la ligne
 * @param nbArgs Nombre d'arguments de la ligne
 * @return 1 s'il y a une erreur, 0 sinon
 */
int parse_data_camera (Image *im, char **args, int nbArgs);

/**
 * @brief Parse une ligne de type "ambient"
 *
 * @param im Image à remplir
 * @param args Tous les arguments de la ligne
 * @param nbArgs Nombre d'arguments de la ligne
 * @return 1 s'il y a une erreur, 0 sinon
 */
int parse_data_ambient (Image *im, char **args, int nbArgs);

/**
 * @brief Parse une ligne de type "diffuse"
 *
 * @param im Image à remplir
 * @param args Tous les arguments de la ligne
 * @param nbArgs Nombre d'arguments de la ligne
 * @return 1 s'il y a une erreur, 0 sinon
 */
int parse_data_diffuse (Image *im, char **args, int nbArgs);

/**
 * @brief Parse une ligne de type "specular"
 *
 * @param im Image à remplir
 * @param args Tous les arguments de la ligne
 * @param nbArgs Nombre d'arguments de la ligne
 * @return 1 s'il y a une erreur, 0 sinon
 */
int parse_data_specular (Image *im, char **args, int nbArgs);

/**
 * @brief Parse une ligne de type "shininess"
 *
 * @param im Image à remplir
 * @param args Tous les arguments de la ligne
 * @param nbArgs Nombre d'arguments de la ligne
 * @return 1 s'il y a une erreur, 0 sinon
 */
int parse_data_shininess (Image *im, char **args, int nbArgs);

/**
 * @brief Parse une ligne de type "directional"
 *
 * @param im Image à remplir
 * @param args Tous les arguments de la ligne
 * @param nbArgs Nombre d'arguments de la ligne
 * @return 1 s'il y a une erreur, 0 sinon
 */
int parse_data_directional (Image *im, char **args, int nbArgs);

/**
 * @brief Parse une ligne de type "point"
 *
 * @param im Image à remplir
 * @param args Tous les arguments de la ligne
 * @param nbArgs Nombre d'arguments de la ligne
 * @return 1 s'il y a une erreur, 0 sinon
 */
int parse_data_point (Image *im, char **args, int nbArgs);

/**
 * @brief Parse une ligne de type "maxverts"
 *
 * @param im Image à remplir
 * @param args Tous les arguments de la ligne
 * @param nbArgs Nombre d'arguments de la ligne
 * @return 1 s'il y a une erreur, 0 sinon
 */
int parse_data_maxverts (Image *im, char **args, int nbArgs);

/**
 * @brief Parse une ligne de type "vertex"
 *
 * @param im Image à remplir
 * @param args Tous les arguments de la ligne
 * @param nbArgs Nombre d'arguments de la ligne
 * @return 1 s'il y a une erreur, 0 sinon
 */
int parse_data_vertex (Image *im, char **args, int nbArgs);

/**
 * @brief Parse une ligne de type "sphere"
 *
 * @param im Image à remplir
 * @param args Tous les arguments de la ligne
 * @param nbArgs Nombre d'arguments de la ligne
 * @return 1 s'il y a une erreur, 0 sinon
 */
int parse_data_sphere (Image *im, char **args, int nbArgs);

/**
 * @brief Parse une ligne de type "tri"
 *
 * @param im Image à remplir
 * @param args Tous les arguments de la ligne
 * @param nbArgs Nombre d'arguments de la ligne
 * @return 1 s'il y a une erreur, 0 sinon
 */
int parse_data_triangle (Image *im, char **args, int nbArgs);

/**
 * @brief Parse une ligne de type "plane"
 *
 * @param im Image à remplir
 * @param args Tous les arguments de la ligne
 * @param nbArgs Nombre d'arguments de la ligne
 * @return 1 s'il y a une erreur, 0 sinon
 */
int parse_data_plane (Image *im, char **args, int nbArgs);

/**
 * @brief Parse une ligne de type "shadow"
 *
 * @param im Image à remplir
 * @param args Tous les arguments de la ligne
 * @param nbArgs Nombre d'arguments de la ligne
 * @return 1 s'il y a une erreur, 0 sinon
 */
int parse_data_shadow (Image *im, char **args, int nbArgs);

/**
 * @brief Affiche une structure ParseData
 *
 * @param data Données à afficher
 */
void print_parse_data (ParseData *data);

/**
 * @brief Free une structure ParseData
 *
 * @param data
 */
void free_parse_data (ParseData *data);

/**
 * @brief Free les arguments lus sur une ligne
 *
 * @param args Liste d'arguments
 * @param nbArgs Nombre d'arguments
 */
void free_args (char **args, int nbArgs);

/**
 * @brief A utiliser à la place de return 1; quand une erreur se produit: Libère l'image et return 1 (seulement pour moins de répétitions)
 *
 * @param im Image à free
 * @return 1 pour erreur
 */
int error_return (Image *im);

#endif
