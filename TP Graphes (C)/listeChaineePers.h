#pragma once

#include "personne.h"

/// @brief Liste chainée de personne (pointeur de personne et non leur nom uniquement)
typedef struct lcp {
    struct p *p;
    struct lcp *suivant;
} ListeChaineePersonne;

void ajouteListePersonne (ListeChaineePersonne** l, struct p* p);
void enleveAboPersonne (ListeChaineePersonne** l, struct p* p);
void afficheNomsPersonnes (ListeChaineePersonne** l);
void freeListeChaineePersonne (ListeChaineePersonne* l);