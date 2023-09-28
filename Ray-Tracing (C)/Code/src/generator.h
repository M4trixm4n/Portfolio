#ifndef GENERATOR_H
#define GENERATOR_H

#include "algebraic_formulas.h"
#include "parse_scene.h"
#include "triangles_plans.h"

#include <FreeImage.h>
#include <stdio.h>

/**
 * @brief Crée une image .png à partir du tableau de couleurs représentant les pixels dans l'image passe en paramètre
 *
 * @param im Image à convertir en .png
 */
void generate_image(Image *im);

/**
 * @brief Crée une structure image en prenant en compte les lumières, ombres, et autres paramètres, en faisant appel au lecteur de scène.
 *
 * @param filename Nom du fichier
 * @return Image*
 */
Image *generate_all(char *filename);

/**
 * @brief Lance seulement le nécéssaire pour passer le test du parsing de la scène
 * 
 * @param im Image contenant les infos nécéssaires
 */
void test_scene(Image *im);

#endif
