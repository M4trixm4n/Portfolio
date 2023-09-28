#include "parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// @brief parse le fichier rentré
/// @warning ne marche qu'avec un fichier avec une structure identique aux fichiers disponibles sur moodle (tabulation de 2 espaces, lignes avec un crochet seul pour les abonnements et distances)
/// @param nomFichier 
/// @param tabPersonnes 
/// @param tailleTab 
/// @param v 
void parse (char* nomFichier, Personne*** tabPersonnes, int* tailleTab, Villes** v) {
    FILE* f = fopen(nomFichier, "r");
    char* ligne;
    int partie = 0;

    while (f) {
        ligne = (char*) malloc(sizeof(char)*200);
        fgets(ligne, 200, f);
        switch (partie) {
            case 0:
                if (ligne[0] != '{') {
                    fclose(f);
                    perror("Mauvais format de fichier texte. Il doit commencer et finir par des accolades ({}).");
                    exit(EXIT_FAILURE);
                }
                free(ligne);
                partie += 1;
                break;
            case 1:
                parsePersonnesInfos(ligne, tabPersonnes, tailleTab);
                free(ligne);
                partie += 1;
                break;
            case 2:
                parseVillesInfos(ligne, v);
                free(ligne);
                partie += 1;
                break;
            case 3:
                if (ligne[2] == '[') {
                    free(ligne);
                    break;
                }
                if (ligne[2] == ']') {
                    free(ligne);
                    partie += 1;
                    break;
                }
                parsePersonne(ligne, tabPersonnes, tailleTab);
                free(ligne);
                break;
            case 4:
                if (ligne[2] == '[') {
                    free(ligne);
                    break;
                }
                if (ligne[2] == ']') {
                    free(ligne);
                    partie += 1;
                    break;
                }
                parseVilles(ligne, v);
                free(ligne);
                break;
            case 5:
                free(ligne);
                fclose(f);
                return;
                break;
            default:
                perror("How did we get here ?");
                exit(EXIT_FAILURE);
        }
    }
}

/// @brief 
/// @param ligne 
/// @param tabPersonnes 
/// @param tailleTab 
void parsePersonnesInfos (char* ligne, Personne*** tabPersonnes, int* tailleTab) {
    char* token;
    char* separateurs;
    separateurs = "[], \t\r\n";
    token = strtok(ligne, separateurs);
    while (token) {
        ajoutePersonne(tabPersonnes, tailleTab, token, NULL);
        token = strtok(NULL, separateurs);
    }
}

/// @brief 
/// @param ligne 
/// @param v 
void parseVillesInfos (char* ligne, Villes** v) {
    char* token;
    char* separateurs;
    separateurs = "[], \t\r\n";
    token = strtok(ligne, separateurs);
    while (token) {
        char* ville = (char*) malloc(sizeof(char)*(strlen(token)+1));
        if (!ville) {
            perror("Problème malloc perseVillesInfos");
            exit(EXIT_FAILURE);
        }
        strcpy(ville, token);
        ajouteVille(v, ville);
        token = strtok(NULL, separateurs);
    }
}

/// @brief 
/// @param ligne 
/// @param tabPersonnes 
/// @param tailleTab 
void parsePersonne (char* ligne, Personne*** tabPersonnes, int* tailleTab) {
    char* token;
    char* separateurs;
    char** tmpLigne = (char**) malloc(sizeof(char*));
    if (!tmpLigne) {
        perror("Probleme malloc tmpLigne parsePersonne");
        exit(EXIT_FAILURE);
    }
    int nbMots = 0;
    separateurs = "[], :{}\n\r\t";
    token = strtok(ligne, separateurs);
    while (token) {
        tmpLigne = (char**) realloc(tmpLigne, sizeof(char*)*(nbMots+1));
        if (!tmpLigne) {
            perror("Problème realloc parsePersonne");
            exit(EXIT_FAILURE);
        }
        tmpLigne[nbMots] = (char*) malloc(sizeof(char)*(strlen(token)+1));
        if (!tmpLigne[nbMots]) {
            perror("Probleme malloc mot parsePersonne");
            exit(EXIT_FAILURE);
        }
        strcpy(tmpLigne[nbMots], token);
        nbMots += 1;
        token = strtok(NULL, separateurs);
    }
    char* ville = (char*) malloc(sizeof(char)*(strlen(tmpLigne[nbMots-1])+1));
    if (!ville) {
        perror("Problème malloc parsePersonne");
        exit(EXIT_FAILURE);
    }
    strcpy(ville, tmpLigne[nbMots-1]);
    changerVille(getPersonne(tabPersonnes, *tailleTab, tmpLigne[0]), ville);
    for (int i = 1; i < nbMots-1; i += 1) {
        ajouteAbo(getPersonne(tabPersonnes, *tailleTab, tmpLigne[0]), getPersonne(tabPersonnes, *tailleTab, tmpLigne[i]));
        free(tmpLigne[i]);
    }
    free(tmpLigne[0]);
    free(tmpLigne[nbMots-1]);
    free(tmpLigne);
}

/// @brief 
/// @param ligne 
/// @param v 
void parseVilles (char* ligne, Villes **v) {
    char* token;
    char *separateurs;
    separateurs = "() ,\t\r\n";
    char *ville1 = (char*) malloc(sizeof(char)*40), *ville2 = (char*) malloc(sizeof(char)*40);
    if (!ville1) {
        perror("Probleme malloc ville1 parseVilles");
        exit(EXIT_FAILURE);
    }
    if (!ville2) {
        perror("Probleme malloc ville2 parseVilles");
        exit(EXIT_FAILURE);
    }
    int distance, cpt = 0;
    token = strtok(ligne, separateurs);
    while (token) {
        if (!cpt) {
            strcpy(ville1, token);
        } else if (cpt == 1) {
            distance = atoi(token);
        } else if (cpt == 2) {
            strcpy(ville2, token);
        } else {
            perror("Trop d'arguments pour les distances entre 2 villes.");
            exit(EXIT_FAILURE);
        }
        cpt += 1;
        token = strtok(NULL, separateurs);
    }
    int ind1 = getIndiceVille(v, ville1), ind2 = getIndiceVille(v, ville2);
    if (!(*v)->distances) {
        initMatriceDistances(v);
    }
    (*v)->distances[ind1][ind2] = distance;
    (*v)->distances[ind2][ind1] = distance;
    free(ville1);
    free(ville2);
}