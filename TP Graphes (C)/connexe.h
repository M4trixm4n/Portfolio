#pragma once

#include "personne.h"

int *initTabComposantes (int tailleTab);
void calculeUneComposanteConnexe (int **tabComposantes, int composante, Personne*** tabPersonnes, int tailleTab);
void ajoutePersonneComposante (int **tabComposantes, int composante, Personne*** tabPersonnes, int tailleTab, Personne** p);
void calculeLesComposantesConnexes (int **tabComposantes, Personne*** tabPersonnes, int tailleTab);
int nouvelleComposanteExiste (int **tabComposantes, int tailleTab);
void afficheUneComposante (Personne ***tabPersonnes, int tailleTab, int composante);
void afficheLesComposantes (Personne ***tabPersonnes, int tailleTab, int **tabComposantes);
int appartientTableau (int* tab, int tailleTab, int x);