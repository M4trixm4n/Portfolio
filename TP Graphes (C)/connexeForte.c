#include "connexeForte.h"
#include "connexe.h"
#include <stdio.h>
#include <stdlib.h>

/// @brief initialise un tableau d'int de taille @param tailleTab-1, et chaque case est mise à -1
/// @return le teableau initialisé
int* initTabComposantesFortes (int tailleTab) {
    int* tabComposantesFortes = (int*) malloc(sizeof(int)*(tailleTab-1));
    if (!tabComposantesFortes) {
        perror("Probleme malloc initTabComposantesFortes");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < tailleTab-1; i += 1) {
        tabComposantesFortes[i] = -1;
    }
    return tabComposantesFortes;
}

/// @brief début de l'algorithme pour calculer une composante fortement connexe, choisit une personne p sans composante,
/// @brief puis appelle ajoutPersonneComposanteForte pour chaque abonnement tmp->p qui possède un chemin vers p (calculé avec distancePersonnes qui retourne un nombre positif si un chemin exite)
/// @param numComposante 
/// @param tabComposanteFortes 
/// @param tabPersonnes 
/// @param tailleTab 
void calculeUneComposanteFortementConnexe (int numComposante, int** tabComposanteFortes, Personne*** tabPersonnes, int tailleTab) {
    int indPersonne = -1;
    for (int i = 0; i < tailleTab-1; i += 1) {
        if ((*tabComposanteFortes)[i] == -1) {
            indPersonne = i;
            (*tabComposanteFortes)[i] = numComposante;
            break;
        }
    }
    if (indPersonne == -1) {
        printf("Chaque personne appartient déjà à une composante fortement connexe");
        return;
    }
    Personne* p = (*tabPersonnes)[indPersonne];
    if (!p->abonnements) {
        return;
    }
    ListeChaineePersonne* tmp = p->abonnements;
    while (tmp) {
        if (distancePersonnes(tmp->p, p, tabPersonnes, tailleTab, NULL) != -1) {
            ajoutPersonneComposanteForte(numComposante, tabComposanteFortes, tabPersonnes, tailleTab, tmp->p, p);
        }
        tmp = tmp->suivant;
    }
}

/// @brief ajoute @param p à cette composante @param numComposante puis parcours les abonnements de p, rappellant cette fonction quand l'abonnement possède un chemin vers @param pDebut
/// @param numComposante 
/// @param tabComposanteFortes 
/// @param tabPersonnes 
/// @param tailleTab 
void ajoutPersonneComposanteForte (int numComposante, int** tabComposanteFortes, Personne*** tabPersonnes, int tailleTab, Personne* p, Personne* pDebut) {
    int indPersonne = getIndPersonneTab(tabPersonnes, tailleTab, p);
    if ((*tabComposanteFortes)[indPersonne] == numComposante) {
        return;
    }
    (*tabComposanteFortes)[indPersonne] = numComposante;
    ListeChaineePersonne* tmp = p->abonnements;
    if (!tmp) {
        return;
    }
    while (tmp) {
        if (distancePersonnes(tmp->p, pDebut, tabPersonnes, tailleTab, NULL) != -1) {
            ajoutPersonneComposanteForte(numComposante, tabComposanteFortes, tabPersonnes, tailleTab, tmp->p, pDebut);
        }
        tmp = tmp->suivant;
    }
}

/// @brief vérifie s'il reste une composante fortement connexe à créer
/// @param tabComposantesFortes 
/// @param tailleTab 
/// @return si oui 1, sinon 0
int nouvelleComposanteForteExiste (int **tabComposantesFortes, int tailleTab) {
    for (int i = 0; i < tailleTab-1; i += 1) {
        if ((*tabComposantesFortes)[i] == -1) {
            return 1;
        }
    }
    return 0;
}

/// @brief calcule chaque composante fortement connexe
/// @param tabComposantesFortes 
/// @param tabPersonnes 
/// @param tailleTab 
void calculeLesComposantesFortementConnexe (int** tabComposantesFortes, Personne*** tabPersonnes, int tailleTab) {
    int i = 1;
    while (nouvelleComposanteForteExiste(tabComposantesFortes, tailleTab)) {
        calculeUneComposanteFortementConnexe(i, tabComposantesFortes, tabPersonnes, tailleTab);
        // afficheLesComposantesFortementConnexe(tabPersonnes, tabComposantesFortes, tailleTab);
        i += 1;
    }
}

/// @brief affiche une composante fortement connexe
/// @param tabPersonnes 
/// @param tabComposantesFortes 
/// @param tailleTab 
/// @param numComposante 
void afficheUneComposanteFortementConnexe (Personne ***tabPersonnes, int** tabComposantesFortes, int tailleTab, int numComposante) {
    printf("Les personnes appartenant à la composante fortement connexe %d sont : \n", numComposante);
    printf("======================================================================\n");
    for (int i = 0; i < tailleTab-1; i += 1) {
        if ((*tabComposantesFortes)[i] == numComposante) {
            affichePersonne((*tabPersonnes)[i]);
            printf("=--------------------------------------------------=\n");
        }
    }
}

/// @brief affiche toutes les composante fortement connexe
/// @param tabPersonnes 
/// @param tabComposantesFortes 
/// @param tailleTab 
void afficheLesComposantesFortementConnexe (Personne*** tabPersonnes, int** tabComposantesFortes, int tailleTab) {
    int* parcouru = (int*) malloc(sizeof(int));
    if (!parcouru) {
        perror("Problème malloc afficheLesComposantesFortementConnexe");
        exit(EXIT_FAILURE);
    }
    parcouru[0] = -1;
    int taille = 1;
    int capacity = 1;
    for (int i = 0; i < tailleTab-1; i += 1) {
        if (!appartientTableau(parcouru, taille, (*tabComposantesFortes)[i])) {
            afficheUneComposanteFortementConnexe(tabPersonnes, tabComposantesFortes, tailleTab, (*tabComposantesFortes)[i]);
            if (capacity) {
                parcouru[taille-1] = (*tabComposantesFortes)[i];
                capacity -= 1;
            } else {
                parcouru = (int*) realloc(parcouru, sizeof(int)*(++taille));
                if (!parcouru) {
                    perror("Problème realloc afficheLesComposantesFortementConnexe");
                    exit(EXIT_FAILURE);
                }
                parcouru[taille-1] = (*tabComposantesFortes)[i];
            }
        }
    }
    free(parcouru);
}