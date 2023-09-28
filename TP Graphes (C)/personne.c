#include "personne.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/// @brief ajoute une personne à @param tabPersonnes
/// @param tailleTab 
/// @param nom 
/// @param ville est NULL si l'on parse avec un fichier, elle est rajoutée plus tard avec changerVille()
void ajoutePersonne (Personne*** tabPersonnes, int *tailleTab, char* nom, char* ville) {
    Personne* p = (Personne*) malloc(sizeof(Personne));
    if (!p) {
        perror("Problème malloc ajoutePersonne");
        exit(EXIT_FAILURE);
    }
    p->nom = (char*) malloc(sizeof(char)*(strlen(nom)+1));
    if (!p->nom) {
        perror("Problème malloc nom ajoutePersonne");
        exit(EXIT_FAILURE);
    }
    strcpy(p->nom, nom);
    if (ville) {
        p->ville = (char*) malloc(sizeof(char)*strlen(ville));
        if (!p->ville) {
            perror("Problème malloc ville ajoutePersonne");
            exit(EXIT_FAILURE);
        }
        strcpy(p->ville, ville);
    } else {
        p->ville = NULL;
    }
    p->abonnements = NULL;
    p->suiviPar = NULL;
    p->numCompo = 0;
    p->rang = -1;
    p->rangAttache = -1;

    if (*tailleTab > 1) {
        free((*tabPersonnes)[*tailleTab-1]);
    }
    (*tabPersonnes)[*tailleTab-1] = p;
    *tailleTab += 1;
    *tabPersonnes = (Personne**) realloc(*tabPersonnes, (*tailleTab)*sizeof(Personne*));
    (*tabPersonnes)[*tailleTab-1] = (Personne*) malloc(sizeof(Personne*));
    if (!(*tabPersonnes)) {
        perror("Problème realloc ajoutePersonne");
        exit(EXIT_FAILURE);
    }
}

/// @brief retourne le pointeur de la personne possédant le nom @param nom
/// @param tabPersonnes 
/// @param tailleTab 
/// @param nom 
/// @return 
Personne* getPersonne (Personne*** tabPersonnes, int tailleTab, char* nom) {
    int i;
    for (i = 0; i < tailleTab-1; i += 1) {
        if (!strcmp(nom, (*tabPersonnes)[i]->nom)) {
            return (*tabPersonnes)[i];
        }
    }
    return NULL;
}

/// @brief wrapper pour ajouter un abonnement
/// @param p1 
/// @param p2 
void ajouteAbo (Personne* p1, Personne* p2) {
    ajouteListePersonne(&p1->abonnements, p2);
}

/// @brief wrapper pour retirer un abonnement 
/// @param p1 
/// @param p2 
void enleveAbo (Personne* p1, Personne* p2) {
    enleveAboPersonne(&p1->abonnements, p2);    
}

/// @brief affiche une personne (nom, ville, abonnements, abonnées)
/// @param p 
void affichePersonne (Personne* p) {
    printf("%s habite à %s.\n", p->nom, p->ville);
    if (p->abonnements) {
        printf("Ses abonnemments sont : ");
        afficheNomsPersonnes(&(p->abonnements));
        printf("\n");
    } else {
        printf("Il n'est abonné à personne.\n");
    }
    if (p->suiviPar) {
        printf("Il est suivi par : ");
        afficheNomsPersonnes(&(p->suiviPar));
        printf("\n");
    } else {
        printf("Il n'est suivi par personne.\n");
    }
}

/// @brief changer la ville d'une personne
/// @param p 
/// @param ville 
void changerVille (Personne* p, char* ville) {
    if (p->ville) {
        free(p->ville);
    }
    p->ville = (char*) malloc(sizeof(char)*(strlen(ville)+1));
    if (!p->ville) {
        perror("Problème malloc changerVille.");
        exit(EXIT_FAILURE);
    }
    strcpy(p->ville, ville);
    if (ville) {
        free(ville);
    }
}

/// @brief affiche chaque personne de @param tabPersonne
/// @param tabPersonne 
/// @param tailleTab 
void afficheTabPersonne (Personne*** tabPersonne, int tailleTab) {
    for (int i = 0; i < tailleTab-1; i += 1) {
        affichePersonne((*tabPersonne)[i]);
    } 
}

/// @brief retourne l'indice de @param p dans @param tabPersonne
/// @param tabPersonne 
/// @param tailleTab 
/// @param p 
/// @return 
int getIndPersonneTab (Personne*** tabPersonne, int tailleTab, Personne* p) {
    for (int i = 0; i < tailleTab-1; i += 1) {
        if (!strcmp(p->nom, (*tabPersonne)[i]->nom)) {
            return i;
        }
    }
    return -1;
}

/// @brief calcule la distance entre 2 personne
/// @param p1 
/// @param p2 
/// @param tabPersonne 
/// @param tailleTab 
/// @param parcouruRec 
/// @return valeur positive si ils appartiennent à la même composante fortement connexe, -1 sinon
int distancePersonnes (Personne* p1, Personne* p2, Personne*** tabPersonne, int tailleTab, int* parcouruRec) {
    if (p1 == p2) {
        return 0;
    }
    int* parcouru = (int*) calloc(tailleTab-1, sizeof(int));
    if (parcouruRec) {
        for (int i = 0; i < tailleTab-1; i += 1) {
            parcouru[i] = parcouruRec[i];
        }
    }
    int indP1 = getIndPersonneTab(tabPersonne, tailleTab, p1);
    parcouru[indP1] += 1;
    ListeChaineePersonne* abos = p1->abonnements;
    if (!abos) {
        free(parcouru);
        return -1;
    }
    do {
        int ind = getIndPersonneTab(tabPersonne, tailleTab, abos->p);
        if (parcouru[ind]) {
            parcouru[ind] += 1;
            abos = abos->suivant;
            continue;
        }
        if (!strcmp(abos->p->nom, p2->nom)) {
            free(parcouru);
            return 1;
        } else {
            parcouru[ind] = 1;
        }
        abos = abos->suivant;
    } while (abos);
    
    int minDist = 0;
    int compteurSkip = 1;
    for (int i = 0; i < tailleTab-1; i += 1) {
        if (parcouru[i] != 1 || i == indP1) {
            compteurSkip += 1;
            continue;
        }
        int dist = distancePersonnes((*tabPersonne)[i], p2, tabPersonne, tailleTab, parcouru);
        if (dist > 0) {
            if (minDist) {
                if (dist < minDist) {
                    minDist = dist;
                }
            } else {
                minDist = dist;
            }
        }
    }
    free(parcouru);
    if (compteurSkip == tailleTab-1 || !minDist) {
        return -1;
    }
    return 1 + minDist;
}

/// @brief moyenne de toute les distancePersonnes
/// @param tabPersonne 
/// @param tailleTab 
/// @return 
float distanceMoyenne (Personne*** tabPersonne, int tailleTab) {
    float somme = 0;
    for (int i = 0; i < tailleTab-1; i += 1) {
        for (int j = 0; j < tailleTab-1; j += 1) {
            printf("%s et %s\n", (*tabPersonne)[i]->nom, (*tabPersonne)[j]->nom);
            somme += distancePersonnes((*tabPersonne)[i], (*tabPersonne)[j], tabPersonne, tailleTab, NULL);
        }
    }
    return somme/((tailleTab-1) * (tailleTab-1));
}

/// @brief ajoute les abonnées à chaque personne (suiviPar)
/// @param tabPersonne 
/// @param tailleTab 
void symetriser (Personne*** tabPersonne, int tailleTab) {
    for (int i = 0; i < tailleTab-1; i += 1) {
        ListeChaineePersonne* tmp = (*tabPersonne)[i]->abonnements;
        while (tmp) {
            ajouteListePersonne(&tmp->p->suiviPar, (*tabPersonne)[i]);
            tmp = tmp->suivant;
        }
    }
}

/// @brief free une personne @param p
/// @param p 
void freePersonne (Personne* p) {
    free(p->nom);
    free(p->ville);
    if (p->abonnements) {
        freeListeChaineePersonne(p->abonnements);
    }
    if (p->suiviPar) {
        freeListeChaineePersonne(p->suiviPar);
    }
    free(p);
}

/// @brief free @param tabPersonne
/// @param tabPersonne 
/// @param tailleTab 
void freeTabPersonne (Personne*** tabPersonne, int tailleTab) {
    for (int i = 0; i < tailleTab-1; i += 1) {
        freePersonne((*tabPersonne)[i]);
    }
    free((*tabPersonne)[tailleTab-1]);
    free(*tabPersonne);
}