#pragma once

#include "personne.h"
#include "villes.h"

void parse (char* nomFichier, Personne*** tabPersonnes, int* tailleTab, Villes** v);
void parsePersonnesInfos (char* ligne, Personne*** tabPersonnes, int* tailleTab);
void parseVillesInfos (char* ligne, Villes** v);
void parsePersonne (char* ligne, Personne*** tabPersonnes, int* tailleTab);
void parseVilles (char* ligne, Villes** v);