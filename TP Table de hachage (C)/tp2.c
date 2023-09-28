#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////
// Liste Chainée
///////////////////////////////////////////

char mot[25];

typedef struct lc {
    char mot[25];
    int occurences;
    struct lc *suivant;
} ListeChainee;

int estMinuscule (char c) {
    return (c >= 97 && c <= 122);
}

int estMajuscule (char c) {
    return (c >= 65 && c <= 90);
}

int estLettre (char c) {
    return (estMajuscule(c) || estMinuscule(c));
}

void minToMaj (char *c) {
    if (estMinuscule(*c)) {
        *c = *c - 32;
    }
}

void lireMot (FILE *fichier) {
    char c;
    int cpt = 0;
    while ((c = fgetc(fichier)) != EOF) {
        if (estLettre(c)){
            char *car = &c;
            minToMaj(car);
            mot[cpt] = *car;
            cpt += 1;
        } else {
            if (cpt == 0) {
                continue;
            } else {
                break;
            }
        }
    }
    mot[cpt] = '\0';
    if (c == EOF) { 
    }
}

int compareMots (char* mot1, char* mot2) {
    int i = 0;
    if (strcmp(mot1, mot2) == 0) {
        return 0;
    }
    while (mot1[i] == mot2[i]) {
        i += 1;
    }
    if (mot1[i] < mot2[i] || mot1[i] == '\0') {
        return -1;
    } else {
        return 1;
    }
    return 1;
}

void ajoutListe (char* mot, ListeChainee **liste) {
    if (!strlen((*liste)->mot)) {
        strcpy((*liste)->mot, mot);
        (*liste)->occurences = 1;
        (*liste)->suivant = NULL;
    } else {
        ListeChainee *actuel = *liste;
        ListeChainee *suiv = (*liste)->suivant;
        int res = compareMots(mot, actuel->mot);
        if (res == 0) {
            actuel->occurences += 1;
            return;
        }
        ListeChainee *new = (ListeChainee*) malloc(sizeof(ListeChainee));
        if (!new) {
            perror("Problème malloc ajoutListe");
            exit(EXIT_FAILURE);
        }
        strcpy(new->mot, mot);
        new->occurences = 1;
        new->suivant = NULL;
        if (res == -1) {
            new->suivant = *liste;
            *liste = new;
        } else {
            while (suiv != NULL) {
                res = compareMots(mot, suiv->mot);
                if (res == 0) {
                    suiv->occurences += 1;
                    free(new);
                    return;
                } else if (res == -1) {
                    actuel->suivant = new;
                    new->suivant = suiv;
                    return;
                } else {
                    actuel = suiv;
                    suiv = actuel->suivant;
                }
            }
            actuel->suivant = new;
            new->suivant = NULL;
        }
    }
}

void afficheListe (ListeChainee *liste) {
    ListeChainee *tmp = liste;
    int cpt = 0;
    while (tmp != NULL) {
        cpt += 1;
        printf("Mot : %s, Occurences : %d \n", tmp->mot, tmp->occurences);
        tmp = tmp->suivant;
    }
    printf("Nombre de mots: %d \n", cpt);
}

void lireTexte (FILE *fichier, ListeChainee **liste) {
    lireMot(fichier);
    while (strlen(mot)) {
        ajoutListe(mot, liste);
        lireMot(fichier);
    }
}

void lireFichier (char* nomFichier, ListeChainee **liste) {
    FILE *fichier = NULL;
    fichier = fopen(nomFichier, "r");
    lireTexte(fichier, liste);
    fclose(fichier);
}

int nbMotsFichier (ListeChainee *liste) {
    int cpt = 0;
    while (liste != NULL) {
        cpt += liste->occurences;
        liste = liste->suivant;
    }
    return cpt;
}

void freeListe (ListeChainee *liste) {
    if (liste->suivant) {
        freeListe(liste->suivant);
    }
    free(liste);
}

//////////////////////////////////////////////////
// Table de Hachage
//////////////////////////////////////////////////

// fonction de hachage : ajout lettres du mot modulo 100 (taille table)

typedef struct f {
    char * nomFichier;
    int nbMots;
} File;

typedef struct lch {
    char mot[25];
    int *occurences;
    struct lch *suivant;
    float *tf;
    float idf;
    float *tf_idf;
} ListeChaineeHash;

typedef struct {
    ListeChaineeHash* addr[100];
    File * tabFile;
    int nbFichiers;
} TableHachage;

int hash (char * mot) {
    int sum = 0, i;
    for (i = 0; i < strlen(mot); i += 1) {
        sum += mot[i];
    }
    return sum%100;
}

void initTableHachage (TableHachage ** h, int argc) {
    File *tabFile = (File*) malloc(sizeof(File)*argc);
    int i;
    for (i = 0; i < 100; i += 1) {
        ListeChaineeHash *l = (ListeChaineeHash*) malloc(sizeof(ListeChaineeHash));
        if (!l) {
            perror("Problème alloc initTableHachage ListeChaineeHash");
            exit(EXIT_FAILURE);
        }
        l->mot[0] = '\0';
        l->occurences = (int*) calloc(argc+1, sizeof(int));
        if (!l->occurences){
            perror("Problème calloc occurences initTableHachage");
            exit(EXIT_FAILURE);
        }
        l->tf = (float*) calloc(argc+1, sizeof(float));
        if (!l->tf){
            perror("Problème calloc tf initTableHachage");
            exit(EXIT_FAILURE);
        }
        l->tf_idf = (float*) calloc(argc+1, sizeof(float));
        if (!l->tf_idf){
            perror("Problème calloc tf_idf initTableHachage");
            exit(EXIT_FAILURE);
        }
        l->idf = 0.0;
        l->suivant = NULL;
        (*h)->addr[i] = l;
    }
    (*h)->tabFile = tabFile;
    (*h)->nbFichiers = argc;
}

float calculIdf (TableHachage * h, ListeChaineeHash ** tmp) {
    int apparait = 0, i;
    for (i = 0; i < h->nbFichiers; i += 1) {
        if ((*tmp)->occurences[i]) {
            apparait += 1;
        }
    }
    return h->nbFichiers/apparait;
}

void ajoutTableHachage (ListeChainee * l, TableHachage ** h, int indFile) {
    while (l->mot != NULL) {
        int ind = hash(l->mot), br = 0;
        ListeChaineeHash* tmp = (*h)->addr[ind];
        while (strlen(tmp->mot)) {
            if (!strcmp(tmp->mot, l->mot)) {
                tmp->occurences[indFile] = l->occurences;
                tmp->tf[indFile] = (float) ((float)(l->occurences) / (float)((*h)->tabFile[indFile].nbMots));
                tmp->idf = calculIdf(*h, &tmp);
                tmp->tf_idf[indFile] = tmp->idf * tmp->tf[indFile];
                br = 1;
            }
            tmp = tmp->suivant;
        }
        if (br) {
            l = l->suivant;
            continue;
        }
        strcpy(tmp->mot, l->mot);
        tmp->occurences[indFile] = l->occurences;
        tmp->tf[indFile] = (float) ((float)(l->occurences) / (float)((*h)->tabFile[indFile].nbMots));
        tmp->idf = calculIdf(*h, &tmp);
        tmp->tf_idf[indFile] = tmp->idf * tmp->tf[indFile];

        ListeChaineeHash *suiv = (ListeChaineeHash*) malloc(sizeof(ListeChaineeHash));
        suiv->suivant = NULL;
        suiv->occurences = (int*) calloc((*h)->nbFichiers+1, sizeof(int));
        if (!suiv->occurences) {
            perror("Problème calloc occurences ajoutTableHachage");
            exit(EXIT_FAILURE);
        }
        suiv->tf = (float*) calloc((*h)->nbFichiers+1, sizeof(float));
        if (!suiv->tf) {
            perror("Problème calloc tf ajoutTableHachage");
            exit(EXIT_FAILURE);
        }
        suiv->tf_idf = (float*) calloc((*h)->nbFichiers+1, sizeof(float));
        if (!suiv->tf_idf) {
            perror("Problème calloc tf_idf ajoutTableHachage");
            exit(EXIT_FAILURE);
        }
        tmp->suivant = suiv;
        strcpy(tmp->suivant->mot, "");

        l = l->suivant;
    }
}

void afficheTableHachage (TableHachage h) {
    int i;
    for (i = 0; i < 100; i += 1) {
        printf("[%d]", i);
        if (!strcmp((h.addr[i])->mot, "")) {
            printf("NULL \n");
        } else {
            while (strlen((h.addr[i])->mot)) {
                printf(" --> Mot: %s, Occurences: ", (h.addr[i])->mot);
                int j;
                for (j = 0; j < h.nbFichiers; j += 1) {
                    if (h.addr[i]->occurences[j]) {
                        printf("~%s~ : %d ", h.tabFile[j].nomFichier, h.addr[i]->occurences[j]);
                    }
                }
                (h.addr[i]) = (h.addr[i])->suivant;
            }
            printf("\n");
        }
    }
}

void afficheFrequencesTableHachage (TableHachage h) {
    int i;
    for (i = 0; i < 100; i += 1) {
        printf("[%d]", i);
        if (!strcmp((h.addr[i])->mot, "")) {
            printf("NULL \n");
        } else {
            while (strlen((h.addr[i])->mot)) {
                printf(" --> Mot: %s, Tf = ~", (h.addr[i])->mot);
                int j;
                for (j = 0; j < h.nbFichiers; j += 1) {
                    if (h.addr[i]->occurences[j]) {
                        printf("%f~", h.addr[i]->tf[j]);
                    }
                }
                printf(", Idf = %f, Tf_idf = ~", h.addr[i]->idf);
                for (j = 0; j < h.nbFichiers; j += 1) {
                    if (h.addr[i]->occurences[j]) {
                        printf("%f~", h.addr[i]->tf_idf[j]);
                    }
                }
                (h.addr[i]) = (h.addr[i])->suivant;
            }
            printf("\n");
        }
    }
}

void freeListeHachage (ListeChaineeHash *l) {
    if (l->suivant) {
        freeListeHachage(l->suivant);
    }
    free(l->occurences);
    free(l->tf);
    free(l->tf_idf);
    free(l);
}

void freeTableHachage (TableHachage *h) {
    int i;
    for (i = 0; i < 100; i += 1) {
        freeListeHachage(h->addr[i]);
    }
    for (i = 0; i < h->nbFichiers; i += 1) {
        free(h->tabFile[i].nomFichier);
    }
    free(h->tabFile);
    free(h);
}

int appartient (int j, int* ind, int size) {
    int i;
    for (i = 0; i < size; i += 1) {
        if (ind[i] == j) {
            return 1;
        }
    }
    return 0;
}

int* rechercheMaxiFloat (float * f, int size) {
    int *ind = (int*)malloc(sizeof(int)*size);
    int i, j, indice;
    float maxi;
    for (i = 0; i < size; i += 1) {
        maxi = f[0];
        indice = 0;
        while (appartient(indice, ind, size)) {
            indice += 1;
            maxi = f[indice];
        }
        for (j = indice + 1; j < size; j += 1) {
            if (f[j] > maxi && !appartient(j, ind, size) && f[j] && j != indice)  {
                maxi = f[j];
                indice = j;
            }
        }
        ind[i] = indice;
    }
    return ind;
}

void rechercheMotUtilisateur (TableHachage *h) {
    printf("Tapez le mot recherché. \n");
    while (1) {
        char input[26];
        fgets(input, 25, stdin);
        input[strlen(input) - 1] = '\0';
        int i;
        for (i = 0; i < strlen(input); i += 1) {
            if (estLettre(input[i])) {
                estMinuscule(input[i]) ? minToMaj(&input[i]) : (1) ;
            } else {
                input[i] = '\0';
                break;
            }
        }
        ListeChaineeHash *mot = h->addr[hash(input)];
        while (strcmp(mot->mot, input) && strlen(mot->mot)) {
            mot = mot->suivant;
        }
        if (!strlen(mot->mot)) {
            printf("Le mot n'existe dans aucun document. \n");
            break;
        }
        int cpt = 0;
        for (i = 0; i < h->nbFichiers; i += 1) {
            if (mot->occurences[i]) {
                cpt += 1;
            }
        }
        switch (h->nbFichiers) {
            case 0:
                printf("Veuillez rentrer des documents à l'appel de la fonction.\n");
                break;
            case 1:
                printf("Il n'y a qu'un fichier en entrée, et le mot \"%s\"", input);
                cpt == 1 ? printf(" apparait dans ce fichier : %s.\n", h->tabFile[0].nomFichier) : printf(" n'apparait pas dans ce fichier.\n");
                break;
            case 2:
                printf("Il y a 2 fichiers en entrée, et le mot \"%s\"", input);
                if (cpt == 0) {
                    printf(" n'apparait pas dans ces fichiers.\n");
                } else if (cpt == 1) {
                    mot->tf_idf[0] ? printf(" apparait seulement dans le fichier : %s.\n", h->tabFile[0].nomFichier) : printf(" apparait seulement dans le fichier : %s.\n", h->tabFile[1].nomFichier);
                } else {
                    printf(" apparait dans les 2, dans cet ordre selon leur fréquence d'apparition : \n");
                    int *ind = rechercheMaxiFloat(mot->tf, h->nbFichiers);
                    for (i = 0; i < h->nbFichiers; i += 1) {
                        printf("%d. %s\n", i+1, h->tabFile[ind[i]].nomFichier);
                    }
                    free(ind);
                }
                break;
            case 3:
                printf("Il y a 3 fichiers en entrée, et le mot \"%s\"", input);
                if (cpt == 0) {
                    printf(" n'apparait pas dans ces fichiers.\n");
                } else if (cpt == 1) {
                    int *ind = rechercheMaxiFloat(mot->tf, h->nbFichiers);
                    printf(" apparait dans 1 fichier : %s.\n", h->tabFile[ind[0]].nomFichier);
                    free(ind);
                } else if (cpt == 2) {
                    printf(" apparait dans 2 fichiers, dans cet ordre selon leur fréquence d'apparition : \n");
                    int *ind = rechercheMaxiFloat(mot->tf, h->nbFichiers);
                    for (i = 0; i < 2; i += 1) {
                        printf("%d. %s\n", i+1, h->tabFile[ind[i]].nomFichier);
                    }
                    free(ind);
                } else {
                    printf(" apparait dans les 3, dans cet ordre selon leur fréquence d'apparition : \n");
                    int *ind = rechercheMaxiFloat(mot->tf, h->nbFichiers);
                    for (i = 0; i < 3; i += 1) {
                        printf("%d. %s\n", i+1, h->tabFile[ind[i]].nomFichier);
                    }
                    free(ind);
                }
                break;
            default:
                printf("Il y a %d fichiers en entrée, et le mot \"%s\"", h->nbFichiers, input);
                if (cpt == 0) {
                    printf(" n'apparait pas dans ces fichiers.\n");
                } else if (cpt == 1) {
                    int *ind = rechercheMaxiFloat(mot->tf, h->nbFichiers);
                    printf(" apparait dans 1 fichier : %s.\n", h->tabFile[ind[0]].nomFichier);
                    free(ind);
                } else if (cpt == 2) {
                    printf(" apparait dans 2 fichiers, dans cet ordre selon leur fréquence d'apparition : \n");
                    int *ind = rechercheMaxiFloat(mot->tf, h->nbFichiers);
                    for (i = 0; i < 2; i += 1) {
                        printf("%d. %s\n", i+1, h->tabFile[ind[i]].nomFichier);
                    }
                    free(ind);
                } else {
                    printf(" apparait dans 3 fichiers ou plus, voici le top 3 selon leur fréquence d'apparition : \n");
                    int *ind = rechercheMaxiFloat(mot->tf, h->nbFichiers);
                    for (i = 0; i < 3; i += 1) {
                        printf("%d. %s\n", i+1, h->tabFile[ind[i]].nomFichier);
                    }
                    free(ind);
                }
                break;
        }
        break;
    }
}

/////////////////////////////////////////
// Arbres
/////////////////////////////////////////

typedef struct a {
    int occ;
    ListeChainee *mots;
    struct a *fg, *fd;
} Noeud, *Arbre;

Arbre initArbre () {
    Arbre a = (Arbre) malloc(sizeof(Arbre)*4);
    if (!a) {
        perror("Problème Allocation creerArbre");
        exit(EXIT_FAILURE);
    }
    a->occ = 0;
    a->mots = NULL;
    a->fg = NULL;
    a->fd = NULL;
    return a;
}

void inserer (Arbre *a, char *mot, int occ) {
    if (!a) {
        *a = (Arbre) malloc(sizeof(Arbre));
        if (!a) {
            perror("Problème Allocation inserer Arbre");
            exit(EXIT_FAILURE);
        }
    } 
    if (!(*a)->occ) {
        (*a)->occ = occ;
        if (!(*a)->mots) {
            (*a)->mots = (ListeChainee*) malloc(sizeof(ListeChainee));   
        }
        if (!(*a)->mots) {
            perror("Problème Allocation inserer ListeChainee");
            exit(EXIT_FAILURE);
        }
        (*a)->mots->occurences = occ;
        strcpy((*a)->mots->mot, mot);
        (*a)->mots->suivant = NULL;

        (*a)->fg = initArbre();
        (*a)->fd = initArbre();

    } else if ((*a)->occ == occ) {
        ListeChainee *tmp = (*a)->mots;
        while (tmp->suivant) {
            tmp = tmp->suivant;
        }
        tmp->suivant = (ListeChainee*) malloc(sizeof(ListeChainee));
        if (!tmp->suivant) {
            perror("Problème Allocation inserer ListeChainee");
            exit(EXIT_FAILURE);
        }
        tmp->suivant->occurences = occ;
        strcpy(tmp->suivant->mot, mot);
        tmp->suivant->suivant = NULL;
    } else {
        if (occ < (*a)->occ) {
            inserer(&((*a)->fg), mot, occ);
        } else {
            inserer(&((*a)->fd), mot, occ);
        }
    }
}

Arbre creerArbre (TableHachage *h) {
    Arbre a = initArbre();
    int i, j, sum;
    ListeChaineeHash *tmp;
    for (i = 0; i < 100; i += 1) {
        tmp = h->addr[i];
        while (strlen(tmp->mot)) {
            sum = 0;
            for (j = 0; j < h->nbFichiers; j += 1) {
                sum += tmp->occurences[j];
            }
            inserer(&a, tmp->mot, sum);
            tmp = tmp->suivant;
        }
    }
    return a;
}

void parcoursArbre (Arbre a) {
    if (!a) {
        return;
    }
    if (a->fg) {
        if (a->fg->occ) {
            parcoursArbre(a->fg);
        }
    }
    ListeChainee *tmp = a->mots;
    printf("%d Occurences ", tmp->occurences);
    while (tmp) {
        printf("--- %s ", tmp->mot);
        tmp = tmp->suivant;
    }
    printf("\n");
    if (a->fd) {
        if (a->fd->occ) {
            parcoursArbre(a->fd);
        }
    }
}

int parcoursPetitesFrequences (Arbre a, int n) {
    if (!a) {
        return n;
    }
    if (a->fg) {
        if (a->fg->occ) {
            n = parcoursPetitesFrequences(a->fg, n);
        }
    }
    if (n) {
        ListeChainee *tmp = a->mots;
        printf("%d Occurences ", tmp->occurences);
        while (tmp) {
            printf("--- %s ", tmp->mot);
            tmp = tmp->suivant;
        }
        printf("\n");
        n -= 1;
    }
    if (a->fd) {
        if (a->fd->occ) {
            n = parcoursPetitesFrequences(a->fd, n);
        }
    }
    return n;
}

int parcoursGrandesFrequences (Arbre a, int n) {
    if (!a) {
        return n;
    }
    if (a->fd) {
        if (a->fd->occ) {
            n = parcoursGrandesFrequences(a->fd, n);
        }
    }
    if (n) {
        ListeChainee *tmp = a->mots;
        printf("%d Occurences ", tmp->occurences);
        while (tmp) {
            printf("--- %s ", tmp->mot);
            tmp = tmp->suivant;
        }
        printf("\n");
        n -= 1;
    }
    if (a->fg) {
        if (a->fg->occ) {
            n = parcoursGrandesFrequences(a->fg, n);
        }
    }
    return n;
}

void freeArbre (Arbre a) {
    if (!a) {
        return;
    }
    if (a->mots) {
        freeListe(a->mots);
    }
    if (a->fg) {
        freeArbre(a->fg);
    }
    if (a->fd) {
        freeArbre(a->fd);
    }
    if (a) {
        free(a);
    }
}

//////////////////////////////////////
// Utilisateur
//////////////////////////////////////

void interactionUtilisateur (TableHachage *h, Arbre a) {
    int br = 0;
    do {
        if (br) {
            break;
        }

        printf("\n");
        printf("Tapez 0 pour quitter.\nTapez 1 pour afficher la table de hachage.\n");
        printf("Tapez 2 pour afficher les fréquences des mots de la table de hachage.\n");
        printf("Tapez 3 pour rechercher un mot dans le corpus.\n");
        printf("Tapez 4 pour afficher tous les mots du corpus par ordre d'occurrences.\n");
        printf("Tapez 5 pour afficher un certain nombre de mots à occurrences basses.\n");
        printf("Tapez 6 pour afficher un certain nombre de mots à occurrences hautes.\n");

        char input[30];
        char input2[30];
        fgets(input, 29, stdin);

        switch (input[0]) {
            case '0':
                br = 1;
                break;
            case '1':
                afficheTableHachage(*h);
                break;
            case '2':
                afficheFrequencesTableHachage(*h);
                break;
            case '3':
                rechercheMotUtilisateur(h);
                break;
            case '4':
                parcoursArbre(a);
                break;
            case '5':
                printf("Entrez un nombre supérieur à 0.\n");
                fgets(input2, 29, stdin);
                input2[strlen(input2)-1] = '\0';
                parcoursPetitesFrequences(a, atoi(input2));
                break;
            case '6':
                printf("Entrez un nombre supérieur à 0.\n");
                fgets(input2, 29, stdin);
                input2[strlen(input2)-1] = '\0';
                parcoursGrandesFrequences(a, atoi(input2));
                break;
            default:
                printf("Mauvaise entrée.\n");
                break;
        }
    } while (1);
}



//////////////////////////////////////
// Main
//////////////////////////////////////

void compress (int argc, char **argv) {
    TableHachage *h = (TableHachage*) malloc(sizeof(TableHachage));
    if (!h) {
        perror("Problème Allocation compress");
        exit(EXIT_FAILURE);
    }
    initTableHachage(&h, argc-1);
    int i;
    for (i = 0; i < argc-1; i += 1) {
        ListeChainee *liste = (ListeChainee*) malloc(sizeof(ListeChainee));
        if (!liste) {
            perror("Problème alloc compress liste");
            exit(EXIT_FAILURE);
        }
        (liste)->mot[0] = '\0';
        lireFichier(argv[i+1], &liste);
        File f;
        f.nomFichier = (char*) malloc(sizeof(char)*strlen(argv[i+1])+1);
        if (!f.nomFichier) {
            perror("Problème alloc compress nomFichier");
            exit(EXIT_FAILURE);
        }
        f.nomFichier[0] = '\0';
        strcpy(f.nomFichier, argv[i+1]);
        f.nbMots = nbMotsFichier(liste);
        h->tabFile[i] = f;
        ajoutTableHachage(liste, &h, i);
        freeListe(liste);
    }
    Arbre a = creerArbre(h);

    interactionUtilisateur(h, a);

    freeArbre(a);
    freeTableHachage(h);
}



int main (int argc, char **argv) {
    compress(argc, argv);
    return 0;
}