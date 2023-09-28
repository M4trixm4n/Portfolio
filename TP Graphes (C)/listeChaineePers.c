#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "listeChaineePers.h"

/// @brief ajoute la personne @param p à la liste @param l
void ajouteListePersonne (ListeChaineePersonne** l, Personne* p) {
    if (!(*l)) {
        (*l) = (ListeChaineePersonne*) malloc(sizeof(ListeChaineePersonne));
        if (!(*l)) {
            perror("Problème malloc ajouteAboPersonne");
            exit(EXIT_FAILURE);
        }
        (*l)->p = p;
        (*l)->suivant = NULL;
    } else {
        ListeChaineePersonne* tmp = *l;
        while (tmp->suivant) {
            tmp = tmp->suivant;
        }
        ListeChaineePersonne* new = (ListeChaineePersonne*) malloc(sizeof(ListeChaineePersonne));
        if (!new) {
            perror("Problème malloc new ajouteAboPersonne");
            exit(EXIT_FAILURE);
        }
        new->p = p;
        new->suivant = NULL;
        tmp->suivant = new;
    }
}

/// @brief supprime un abonnement (jamais utilisée ni testée)
/// @param l 
/// @param p 
void enleveAboPersonne (ListeChaineePersonne** l, Personne* p) {
    ListeChaineePersonne* prec;
    ListeChaineePersonne* tmp = *l;
    if (!l) {
        printf("L'abonnement n'existe pas.\n");
        return;
    }
    if (!strcmp(tmp->p->nom, p->nom)) {
        prec = tmp;
        tmp = tmp->suivant;
        free(prec);
        return;
    }
    while (strcmp(tmp->p->nom, p->nom) && tmp) {
        prec = tmp;
        tmp = tmp->suivant;
    }
    prec->suivant = tmp->suivant;
    free(tmp);
}

/// @brief affiche toutes les personnes d'une liste (seulement leur nom)
/// @param l 
void afficheNomsPersonnes (ListeChaineePersonne** l) {
    if (!l) {
        printf("Liste vide. \n");
        return;
    }
    printf("%s", (*l)->p->nom);
    ListeChaineePersonne* tmp = *l;
    while (tmp->suivant) {
        tmp = tmp->suivant;
        printf(", %s", tmp->p->nom);
    }
}

void freeListeChaineePersonne (ListeChaineePersonne* l) {
    if (l->suivant) {
        freeListeChaineePersonne(l->suivant);
    }
    free(l);
}