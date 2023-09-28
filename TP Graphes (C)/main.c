#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "personne.h"
#include "villes.h"
#include "parse.h"
#include "connexe.h"
#include "connexeForte.h"
#include "tarjan.h"

Personne **tabPersonnes;
int tailleTab = 1;
Villes* v;
int *tabComposantes;
int *tabComposantesFortes;
int *tabComposantesTarjan;


int main (int argc, char** argv) {
    if (argc < 2) {
        perror("Il faut un ou plusieurs noms de fichiers textes en argument.");
        exit(EXIT_FAILURE);
    }

    v = initVilles();

    tabPersonnes = (Personne**) malloc(sizeof(Personne*));
    if (!tabPersonnes) {
        perror("Problème malloc main");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= argc; i += 1) {
        parse(argv[i], &tabPersonnes, &tailleTab, &v);
    }

    tabComposantes = initTabComposantes(tailleTab);
    tabComposantesFortes = initTabComposantesFortes(tailleTab);

    symetriser(&tabPersonnes, tailleTab);

    calculeLesComposantesConnexes(&tabComposantes, &tabPersonnes, tailleTab);
    calculeLesComposantesFortementConnexe(&tabComposantesFortes, &tabPersonnes, tailleTab);
    tabComposantesTarjan = calculCFCTarjan(&tabPersonnes, tailleTab);

    /////////////////////// ICI POUR L'AFFICHAGE ///////////////////////
    // afficheLesComposantes(&tabPersonnes, tailleTab, &tabComposantes);
    // afficheLesComposantesFortementConnexe(&tabPersonnes, &tabComposantesFortes, tailleTab);
    // afficheLesCFCTarjan(&tabComposantesTarjan, tailleTab, &tabPersonnes);
    //////////////////////////////////////////////////////////////////////

    /////////////////////// AFFICHER TABPERSONNES OU VILLES ///////////////////////
    // afficheTabPersonne(&tabPersonnes, tailleTab);
    // afficheVilles(&v);
    ///////////////////////////////////////////////////////////////////////////////

    freeTabPersonne(&tabPersonnes, tailleTab);
    free(tabComposantes);
    free(tabComposantesFortes);
    free(tabComposantesTarjan);
    freeVilles(&v);
    return 0;
}