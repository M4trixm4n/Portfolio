#include "villes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/// @brief crée la structure et la retourne
/// @return v
Villes* initVilles () {
    Villes* v = (Villes*) malloc(sizeof(Villes));
    v->nbVilles = 0;
    v->distances = NULL;
    v->villes = NULL;
    return v;
}

/// @brief ajoute @param ville à @param v
void ajouteVille (Villes** v, char* ville) {
    if (!(*v)->nbVilles) {
        (*v)->villes = (char**) malloc(sizeof(char*));
        if (!(*v)->villes) {
            perror("Problème malloc char** ajouteVille.");
            exit(EXIT_FAILURE);
        }
    } else {
        (*v)->villes = (char**) realloc((*v)->villes, sizeof(char*)*((*v)->nbVilles+1));
        if (!(*v)->villes) {
            perror("Problème realloc char** ajouteVille.");
            exit(EXIT_FAILURE);
        }
    }
    (*v)->nbVilles += 1;
    (*v)->villes[(*v)->nbVilles-1] = (char*) malloc(sizeof(char)*(strlen(ville)+1));
    strcpy((*v)->villes[(*v)->nbVilles-1], ville);
    if (ville) {
        free(ville);
    }
}

/// @brief 
/// @param v 
/// @param ville 
/// @return indice de @param ville dans @param v->villes
int getIndiceVille (Villes** v, char* ville) {
    for (int i = 0; i < (*v)->nbVilles; i += 1) {
        if (!strcmp(ville, (*v)->villes[i])) {
            return i;
        }
    }
    return -1;
}

/// @brief crée la matrice des distances et l'initialise à 0
/// @param v 
void initMatriceDistances (Villes** v) {
    (*v)->distances = (int**) malloc(sizeof(int*)*(*v)->nbVilles);
    for (int i = 0; i < (*v)->nbVilles; i += 1) {
        (*v)->distances[i] = (int*) calloc((*v)->nbVilles, sizeof(int));
    }
}

/// @brief affiche @param v en entier
void afficheVilles (Villes** v) {
    printf("Nom des villes : \n");
    for (int i = 0; i < (*v)->nbVilles; i += 1) {
        if (i == (*v)->nbVilles-1) {
            printf("%s \n", (*v)->villes[i]);
        } else {
            printf("%s, ", (*v)->villes[i]);
        }
    }
    printf("Distances (en matrice): \n");
    for (int i = 0; i < (*v)->nbVilles; i += 1) {
        for (int j = 0; j < (*v)->nbVilles; j += 1) {
            if (j == (*v)->nbVilles-1) {
                printf("%d \n", (*v)->distances[i][j]);
            } else {
                printf ("%d ", (*v)->distances[i][j]);
            }
        }
    }
}

/// @brief free la structure @param v ainsi que v->villes et v->distances
/// @param v 
void freeVilles (Villes** v) {
    for (int i = 0; i < (*v)->nbVilles; i += 1) {
        free((*v)->villes[i]);
        free((*v)->distances[i]);
    }
    free((*v)->villes);
    free((*v)->distances);
    free(*v);
}