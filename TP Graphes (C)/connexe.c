#include "connexe.h"

#include <stdio.h>
#include <stdlib.h>

/// @brief initialise un tableau d'int de taille (tailleTab-1), puis mettre chaque case à -1
/// @param tailleTab 
/// @return 
int *initTabComposantes (int tailleTab) {
    int *tabComposantes = (int*) malloc(sizeof(int)*(tailleTab-1));
    if (!tabComposantes) {
        perror("Problème malloc initTabComposantes");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < tailleTab-1; i += 1) {
        tabComposantes[i] = -1;
    }
    return tabComposantes;
}

/// @brief début de l'agorithme pour calculer une composante, choisit une personne n'appartenant pas à une composante, puis fait appel à ajoutePersonneComposante
/// @param tabComposantes 
/// @param composante numéro de la composante
/// @param tabPersonnes 
/// @param tailleTab 
void calculeUneComposanteConnexe (int **tabComposantes, int composante, Personne*** tabPersonnes, int tailleTab) {
    if (composante == -1) {
        printf("Valeur de la composante incorrect (doit être différente de -1).\n");
        return;
    }
    int debut = 1;
    for (int i = 0; i < tailleTab-1; i += 1) {
        if ((*tabComposantes)[i] == composante) {
            debut = 0;
        }
    }
    if (debut) {
        Personne* pDepart = NULL;
        for (int i = 0; i < tailleTab-1; i += 1) {
            if ((*tabComposantes)[i] == -1) {
                pDepart = (*tabPersonnes)[i];
                break;
            }
        }
        if (!pDepart) {
            printf("Chaque personne appartient déja à une composante. Impossible d'en créer une nouvelle.\n");
            return;
        }
        ajoutePersonneComposante(tabComposantes, composante, tabPersonnes, tailleTab, &pDepart);
    }
}

/// @brief s'appelle en récursif parmi les abonnements et abonnés de p pour créer une composante
/// @param tabComposantes 
/// @param composante 
/// @param tabPersonnes 
/// @param tailleTab 
/// @param p personne de départ au premier appel, puis un abonnement ou abonné de cette personne, et ainsi de suite
void ajoutePersonneComposante (int **tabComposantes, int composante, Personne*** tabPersonnes, int tailleTab, Personne** p) {
    (*p)->numCompo = composante;
    (*tabComposantes)[getIndPersonneTab(tabPersonnes, tailleTab, *p)] = composante;
    for (int i = 0; i < 2; i += 1) {
        ListeChaineePersonne* tmp = i == 0 ? (*p)->abonnements : (*p)->suiviPar;
        while (tmp) {
            int ind = getIndPersonneTab(tabPersonnes, tailleTab, tmp->p);
            if ((*tabComposantes)[ind] == -1) {
                ajoutePersonneComposante(tabComposantes, composante, tabPersonnes, tailleTab, &(tmp->p));
                tmp = tmp->suivant;
            } else if ((*tabComposantes)[ind] == composante) {
                tmp = tmp->suivant;
                continue;
            } else {
                perror("Rencontre d'une personne appartenant à une autre composante pendant la création d'une composante %d.");
                exit(EXIT_FAILURE);
            }
        }
    }
}

/// @brief pour savoir si chaque personne appartient déjà à une composante
/// @param tabComposantes 
/// @param tailleTab 
/// @return 1 si il reste au moins une composante à créer, 0 sinon
int nouvelleComposanteExiste (int **tabComposantes, int tailleTab) {
    for (int i = 0; i < tailleTab-1; i += 1) {
        if ((*tabComposantes)[i] == -1) {
            return 1;
        }
    }
    return 0;
}

/// @brief calcule une composante avec calculeUneComposanteConnexe tant qu'il en reste à créer
/// @param tabComposantes 
/// @param tabPersonnes 
/// @param tailleTab 
void calculeLesComposantesConnexes (int **tabComposantes, Personne*** tabPersonnes, int tailleTab) {
    int i = 1;
    while (nouvelleComposanteExiste(tabComposantes, tailleTab)) {
        calculeUneComposanteConnexe(tabComposantes, i, tabPersonnes, tailleTab);
        i += 1;
    }
}

/// @brief affichage d'une composante connexe
/// @param tabPersonnes 
/// @param tailleTab 
/// @param composante 
void afficheUneComposante (Personne ***tabPersonnes, int tailleTab, int composante) {
    printf("Les personnes appartenant à la composante %d sont : \n", composante);
    printf("====================================================\n");
    for (int i = 0; i < tailleTab-1; i += 1) {
        if ((*tabPersonnes)[i]->numCompo == composante) {
            affichePersonne((*tabPersonnes)[i]);
            printf("=--------------------------------------------------=\n");
        }
    }
}

/// @brief fonction pour savoir si un int @param x appartient à un tableau d'int @param tab
/// @param tailleTab 
/// @return 1 si oui, 0 sinon
int appartientTableau (int* tab, int tailleTab, int x) {
    for (int i = 0; i < tailleTab; i += 1) {
        if (tab[i] == x) {
            return 1;
        }
    }
    return 0;
}

/// @brief affiche des composantes
/// @param tabPersonnes 
/// @param tailleTab 
/// @param tabComposantes 
void afficheLesComposantes (Personne ***tabPersonnes, int tailleTab, int **tabComposantes) {
    int* parcouru = (int*) malloc(sizeof(int));
    if (!parcouru) {
        perror("Problème malloc afficheLesComposantes");
        exit(EXIT_FAILURE);
    }
    parcouru[0] = -1;
    int taille = 1;
    int capacity = 1;
    for (int i = 0; i < tailleTab-1; i += 1) {
        if (!appartientTableau(parcouru, taille, (*tabComposantes)[i])) {
            afficheUneComposante(tabPersonnes, tailleTab, (*tabComposantes)[i]);
            if (capacity) {
                parcouru[taille-1] = (*tabComposantes)[i];
                capacity -= 1;
            } else {
                parcouru = (int*) realloc(parcouru, sizeof(int)*(++taille));
                if (!parcouru) {
                    perror("Problème realloc afficheLesComposantes");
                    exit(EXIT_FAILURE);
                }
                parcouru[taille-1] = (*tabComposantes)[i];
            }
        }
    }
    free(parcouru);
}