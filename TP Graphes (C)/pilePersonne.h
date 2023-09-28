#pragma once

#include "personne.h"

/// @brief pile de personnes
typedef struct pile_ {
    Personne **tab;
    int nbElts;
    int taille;
} Pile;

Pile* init(int nbPersonnes);
void empiler(Pile* pile, Personne* p);
Personne* depiler(Pile* pile);
void freePile(Pile* pile);
int contient(Pile* pile, Personne*p);