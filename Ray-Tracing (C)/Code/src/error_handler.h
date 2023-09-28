#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include "ray_tracing.h"

#include <stdbool.h>

typedef struct _image Image;

/**
 * @brief Gestionnaire d'erreurs
 *
 */
typedef enum _error
{
    ERROR_MISSING_ARGUMENT,     ///< [PARSE] Il manque une valeur ou plus sur une des ligne
    ERROR_TOO_MANY_ARGUMENTS,   ///< [PARSE] Il y a au moins une valeur en trop sur une des lignes
    ERROR_INVALID_VALUE,        ///< ([PARSE]) Une valeur possède une valeur incorrecte (souvent valeur négative où il en faut une positive)
    ERROR_INVALID_TYPE,         ///< [PARSE] Une valeur est du mauvais type (souvent float/double au lieu de int)
    ERROR_ALLOCATION_FAILED,    ///< Problème au moment de l'allocation (out of memory ?)
    ERROR_MISSING_VERTEX_LIMIT, ///< [PARSE] Il manque le maxverts dans le fichier de test, impossible de créer le vertex
    ERROR_OUT_OF_BOUNDS,        ///< ([PARSE]) Trop de vertex créés
    ERROR_INVALID_KEYWORD,      ///< [PARSE] Mot clé invalide ou non implémenté rencontré pendant le parsing
    ERROR_INVALID_ARGUMENT,     ///< Passage d'un argument non valide à une fonction
    ERROR_PARSING_CANCELLED,    ///< [PARSE] Une erreur est survenue, arrêt total du parsing
    ERROR_INVALID_HEADER,       ///< [PARSE] Header du fichier test invalide
    ERROR_IMPOSSIBLE,           ///< Erreur censée ne jamais se produire
    ERROR_ALREADY_DEFINED,      ///< [PARSE] Impossible de redéfinir maxverts
    ERROR_FILE_OPENING,         ///< [PARSE] Problème pendant l'ouverture du fichier
    ERROR_NULL_VECTOR,          ///< ([PARSE]) Un plan ou une lumière directionnelle ne peut pas posséder un vecteur (direction) nul
    ERROR_MISSING_SIZE,         ///< ([PARSE]) Il manque la taille de l'image
    ERROR_UNTIMELY_CALL,        ///< Fonction appelée avant que des valeurs indispensables soient définies
    ERROR_SAVING_FAILED,        ///< L'image n'a pas pu être sauvegardée via FreeImage
    ERROR_LOADING_FAILED,       ///< L'image n'a pas pu être chargée via FreeImage
    ERROR_DIFFERENT_SIZE        ///< Taille d'image différente de celle attendue
} Error;

/**
 * @brief Fonction qui gère ce qu'il faut faire selon l'erreur donnée
 *
 * @param err Erreur de l'enum Error (enum _error)
 * @param im Image fournie, libérée si besoin
 * @param message Message supplémentaire pour aider l'utilisateur à comprendre ou trouver son erreur
 */
void set_error(Error err, Image *im, char *message);

#endif
