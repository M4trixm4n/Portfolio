#pragma once

#include "listeChaineePers.h"

/// @brief @param numCompo est seulement utilisée pour les composantes connexes simples, @param rang et @param rangAttache sont pour l'algorithme de Tarjan
typedef struct p {
    char* nom;
    char* ville;
    struct lcp *abonnements;
    struct lcp *suiviPar;
    int numCompo;
    int rang;
    int rangAttache;
} Personne;

void ajoutePersonne (Personne*** tabPersonnes, int *tailleTab, char* nom, char* ville);
void changerVille (Personne* p, char* ville);
Personne* getPersonne (Personne*** tabPersonnes, int tailleTab, char* nom);
void ajouteAbo (Personne* p1, Personne* p2);
void enleveAbo (Personne* p1, Personne* p2);
void affichePersonne (Personne* p);
void afficheTabPersonne (Personne*** tabPersonne, int tailleTab);
int distancePersonnes (Personne* p1, Personne* p2, Personne*** tabPersonne, int tailleTab, int* parcourus);
int getIndPersonneTab (Personne*** tabPersonne, int tailleTab, Personne* p);
float distanceMoyenne (Personne*** tabPersonne, int tailleTab);
void symetriser (Personne*** tabPersonne, int tailleTab);
void freePersonne (Personne* p);
void freeTabPersonne (Personne*** tabPersonne, int tailleTab);