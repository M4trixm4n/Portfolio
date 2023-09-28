#include "tarjan.h"
#include <stdio.h>
#include <stdlib.h>
#include "connexe.h"

/// macro pour minimum
#define min(a, b) (((a) < (b)) ? (a) : (b))

/// @brief regarde si @param p appartient à @param tab
/// @param tailleTab 
/// @return 1 si oui, 0 sinon
int contientLeSommet (Personne** tab, Personne* p, int tailleTab) {
    for (int i = 0; i < tailleTab-1; i += 1) {
        if (tab[i] == p) {
            return 1;
        }
    }
    return 0;
}

/// @brief regarde si chaque personne de @param tabPersonnes appartient à @param tab
/// @param tab 
/// @param tabPersonnes 
/// @param tailleTab 
/// @return 
int contientTousLesSommets (Personne** tab, Personne*** tabPersonnes, int tailleTab) {
    for (int i = 0; i < tailleTab-1; i += 1)  {
        if (!contientLeSommet(tab, (*tabPersonnes)[i], tailleTab)) {
            return 0;
        }
    }
    return 1;
}

/// @brief cherche la première personne dans @param tabPersonnes pas encore ajoutée à @param tab
/// @param tailleTab 
/// @return 1 si oui, 0 sinon
Personne* premierSommetNonVu (Personne** tab, Personne*** tabPersonnes, int tailleTab) {
    for (int i = 0; i < tailleTab-1; i += 1) {
        if (!contientLeSommet(tab, (*tabPersonnes)[i], tailleTab)) {
            return (*tabPersonnes)[i];
        }
    }
    return NULL;
}

/// @brief calcule toutes les composantes fortement connexe avec l'algorithme de tarjan (avec des appels récursifs de parcoursDescendants)
/// @param tabPersonnes 
/// @param tailleTab 
/// @return 
int* calculCFCTarjan (Personne*** tabPersonnes, int tailleTab) {
    Personne** dejaVu = (Personne**) malloc(sizeof(Personne*)*(tailleTab));
    if (!dejaVu) {
        perror("Problème malloc dejaVu calculCFCTarjan");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < tailleTab-1; i += 1) {
        dejaVu[i] = NULL;
    }
    int nbEltVus = 0;
    int r = 0;
    int numCompo = 0;
    int* composantes = (int*) malloc(sizeof(int)*(tailleTab-1));
    if (!composantes) {
        perror("Probleme malloc composantes calculCFCTarjan");
        exit(EXIT_FAILURE);
    }
    Pile* pile = init(tailleTab);
    while (!contientTousLesSommets(dejaVu, tabPersonnes, tailleTab)) {
        Personne* p = premierSommetNonVu(dejaVu, tabPersonnes, tailleTab);
        parcoursDescendants(p, &dejaVu, tabPersonnes, &nbEltVus, tailleTab, pile, r, &numCompo, &composantes);
    }
    free(dejaVu);
    freePile(pile);
    return composantes;
}

/// @brief calcule une composante fortement connexe
/// @param p 
/// @param dejaVu 
/// @param tabPersonnes 
/// @param nbEltsVus 
/// @param tailleTab 
/// @param pile 
/// @param r 
/// @param numCompo 
/// @param composantes 
void parcoursDescendants (Personne* p, Personne*** dejaVu, Personne*** tabPersonnes, int *nbEltsVus, int tailleTab, Pile* pile, int r, int* numCompo, int** composantes) {
    empiler(pile, p);
    (*dejaVu)[(*nbEltsVus)++] = p;
    r += 1;
    p->rang = r;
    p->rangAttache = r;
    ListeChaineePersonne* tmp = p->abonnements;
    while (tmp) {
        if (!contientLeSommet(*dejaVu, tmp->p, tailleTab)) {
            parcoursDescendants(tmp->p, dejaVu, tabPersonnes, nbEltsVus, tailleTab, pile, r, numCompo, composantes);
            p->rangAttache = min(tmp->p->rangAttache, p->rangAttache);
        } else if (contient(pile, p)) {
            p->rangAttache = min(tmp->p->rang, p->rangAttache);
        }
        tmp = tmp->suivant;
    }
    Personne* tmpP;
    if (p->rang == p->rangAttache) {
        *numCompo += 1;
        do {
            tmpP = depiler(pile);
            int tmpNum = *numCompo;
            (*composantes)[getIndPersonneTab(tabPersonnes, tailleTab, tmpP)] = tmpNum;
        } while (tmpP != p);
    }
}

/// @brief affiche une composante fortement connexe
/// @param tabPersonnes 
/// @param tabComposantesTarjan 
/// @param tailleTab 
/// @param numComposante 
void afficheUneCFCTarjan (Personne*** tabPersonnes, int** tabComposantesTarjan, int tailleTab, int numComposante) {
    printf("Les personnes appartenant à la composante fortement connexe %d sont : \n", numComposante);
    printf("======================================================================\n");
    for (int i = 0; i < tailleTab-1; i += 1) {
        if ((*tabComposantesTarjan)[i] == numComposante) {
            affichePersonne((*tabPersonnes)[i]);
            printf("=--------------------------------------------------=\n");
        }
    }
}

/// @brief affiche toutes les composantes fortement connexe
/// @param tabComposantesTarjan 
/// @param tailleTab 
/// @param tabPersonnes 
void afficheLesCFCTarjan (int** tabComposantesTarjan, int tailleTab, Personne*** tabPersonnes) {
    int* parcouru = (int*) malloc(sizeof(int));
    if (!parcouru) {
        perror("Problème malloc afficheLesComposantesFortementConnexe");
        exit(EXIT_FAILURE);
    }
    parcouru[0] = -1;
    int taille = 1;
    int capacity = 1;
    for (int i = 0; i < tailleTab-1; i += 1) {
        if (!appartientTableau(parcouru, taille, (*tabComposantesTarjan)[i])) {
            afficheUneCFCTarjan(tabPersonnes, tabComposantesTarjan, tailleTab, (*tabComposantesTarjan)[i]);
            if (capacity) {
                parcouru[taille-1] = (*tabComposantesTarjan)[i];
                capacity -= 1;
            } else {
                parcouru = (int*) realloc(parcouru, sizeof(int)*(++taille));
                if (!parcouru) {
                    perror("Problème realloc afficheLesComposantesFortementConnexe");
                    exit(EXIT_FAILURE);
                }
                parcouru[taille-1] = (*tabComposantesTarjan)[i];
            }
        }
    }
    free(parcouru);
}