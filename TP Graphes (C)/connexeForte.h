#pragma once

#include "personne.h"

int* initTabComposantesFortes (int tailleTab);
void calculeUneComposanteFortementConnexe (int numComposante, int** tabComposantesFortes, Personne*** tabPersonnes, int tailleTab);
void ajoutPersonneComposanteForte (int numComposante, int** tabComposantesFortes, Personne*** tabPersonnes, int tailleTab, Personne* p, Personne* pDebut);
int nouvelleComposanteForteExiste (int **tabComposantesFortes, int tailleTab);
void calculeLesComposantesFortementConnexe (int** tabComposantesFortes, Personne*** tabPersonnes, int tailleTab);
void afficheUneComposanteFortementConnexe (Personne*** tabPersonnes, int** tabComposantesFortes, int tailleTab, int numComposante);
void afficheLesComposantesFortementConnexe (Personne*** tabPersonnes, int** tabComposantesFortes, int tailleTab);
