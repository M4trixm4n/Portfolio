#pragma once

#include "personne.h"
#include "pilePersonne.h"

int* calculCFCTarjan (Personne*** tabPersonnes, int tailleTab);
void parcoursDescendants (Personne* p, Personne*** dejaVu, Personne*** tabPersonnes, int *nbEltsVus, int tailleTab, Pile* pile, int r, int* numCompo, int** composantes);
int contientTousLesSommets (Personne** tab, Personne*** tabPersonnes, int tailleTab);
int contientLeSommet (Personne** tab, Personne* p, int tailleTab);
Personne* premierSommetNonVu (Personne** tab, Personne*** tabPersonnes, int tailleTab);
void afficheLesCFCTarjan (int** tabComposantesTarjan, int tailleTab, Personne*** tabPersonnes);
void afficheUneCFCTarjan (Personne*** tabPersonnes, int** tabComposantesTarjan, int tailleTab, int numComposante);