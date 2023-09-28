#include "pilePersonne.h"

#include <stdlib.h>
#include <stdio.h>

/// @brief initialise la pile
/// @param nbPersonnes 
/// @return 
Pile* init(int nbPersonnes) {
    Pile* p = (Pile*) malloc(sizeof(Pile));
    if (!p) {
        perror("Problème malloc pile init");
        exit(EXIT_FAILURE);
    }
    p->tab = (Personne**) malloc(sizeof(Personne*)*nbPersonnes);
    if (!p->tab) {
        perror("Problème malloc tab init");
        exit(EXIT_FAILURE);
    }
    p->taille = nbPersonnes;
    p->nbElts = 0;
    return p;
}

/// @brief empiler
/// @param pile 
/// @param p 
void empiler(Pile* pile, Personne* p) {
    if (pile->nbElts == pile->taille) {
        printf("La pile est pleine.\n");
        return;
    }
    pile->tab[pile->nbElts] = p;
    pile->nbElts++;
}

/// @brief depiler
/// @param pile 
/// @return 
Personne* depiler(Pile* pile) {
    if (pile->nbElts == 0) {
        printf("La pile est vide. \n");
        return NULL;
    }
    return pile->tab[--(pile->nbElts)];
}

/// @brief free
/// @param pile 
void freePile(Pile* pile) {
    free(pile->tab);
    free(pile);
}

/// @brief parcourt du tableau de la pile pour trouver p
/// @param pile 
/// @param p 
/// @return 1 si p appartient à la pile, 0 sinon
int contient(Pile* pile, Personne*p) {
    for (int i = 0; i < pile->nbElts; i += 1) {
        if (pile->tab[i] == p) {
            return 1;
        }
    }
    return 0;
}