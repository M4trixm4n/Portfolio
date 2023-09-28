#pragma once

/// @brief tableau des villes, matrice des distances, et le nombre de villes
typedef struct villes {
    char ** villes;
    int nbVilles;
    int ** distances;
} Villes;

Villes* initVilles();
void ajouteVille (Villes** v, char* ville);
int getIndiceVille (Villes** v, char* ville);
void initMatriceDistances (Villes** v);
void afficheVilles (Villes** v);
void freeVilles (Villes** v);