#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

int nbElements;

typedef struct Stats {
    int selectionPerm;
    int selectionComp;

    int insertionPerm;
    int insertionComp;
    
    int bullePerm;
    int bulleComp;
    
    int fusionPerm;
    int fusionComp;
    
    int rapidePerm;
    int rapideComp;
    
    int tasPerm;
    int tasComp;
} Stats;

Stats *stat;

int * listeRandom (int elts) {
    int * tab = malloc(elts*sizeof(int));
    for (int i = 0; i < elts; i += 1) {
        tab[i] = rand() % 100;
    }
    return tab;
}

void afficheListe (int *liste, int taille) {
    assert (liste != NULL);
    int i;
    for (i = 0; i < taille; i += 1) {
        if (i < taille - 1) {
        printf("%d, ", liste[i]);
        } else {
            printf("%d \n", liste[i]);
        }
    }
}

int testTri (int *liste, int taille) {
    assert (liste != NULL);
    int i, res = 1;
    int elt = liste[0];
    for (i = 1; i < taille; i += 1) {
        if (elt > liste[i]) {
            res = 0;
        }
    }
    return res;
}

void swap (int* nb1, int* nb2) {
    int temp = *nb1;
    *nb1 = *nb2;
    *nb2 = temp;
}

int chercheIndMini (int* liste, int ind, int taille) {
    int indice = ind, mini = liste[ind], i;
    if (ind == taille - 1) {
        return ind;
    } 
    else {
        for (i = ind+1; i < taille; i += 1) {
            if (liste[i] < mini) {
                indice = i;
                mini = liste[i];
            }
            stat->selectionComp += 1;
        }
        return indice;
    }
}

void triSelection (int* liste, int taille) {
    int i;
    for (i = 0; i < taille; i += 1) {
        swap(&liste[i], &liste[chercheIndMini(liste, i, taille)]);
        stat->selectionPerm += 1;
    }
}

void insertion (int ind, int* liste) {
    int i = ind-1, j = ind;
    while (liste[i] > liste[j]) {
        stat->insertionComp += 1;
        if (i == 0) {
            swap(&liste[i], &liste[j]);
            stat->insertionPerm += 1;
            return;
        }
        swap(&liste[i], &liste[j]);
        stat->insertionPerm += 1;
        i--;
        j--;
    }
    stat->insertionComp += 1;
}

void triInsertion (int* liste, int taille) {
    int i;
    for (i = 1; i < taille; i += 1) {
        insertion(i, liste);
    }
}

void triBulle (int* liste, int taille) {
    int i, compteur = 1;
    while (compteur) {
        compteur = 0;
        for (i = 0; i < taille-1; i += 1) {
            if (liste[i] > liste[i+1]) {
                swap(&liste[i], &liste[i+1]);
                stat->bullePerm += 1;
                compteur += 1;
            }
            stat->bulleComp += 1;
        }
    }
}

void triRapide (int* liste, int start, int end) {
    int pivot = liste[start], ind = start;
    int i;
    for (i = start + 1; i < end; i += 1) {
        stat->rapideComp += 1;
        if (liste[i] < pivot) {
            swap(&liste[i],&liste[ind+1]);
            swap(&liste[ind], &liste[ind+1]);
            stat->rapidePerm += 2;
            ind += 1;
        }
    }
    if (ind-start > 0) {
        triRapide(liste, start, ind);
    }
    if (end-ind+1 >= 0) {
        triRapide(liste, ind+1, end);
    }
}

void partitionner (int *liste, int start, int end) {
    int pivot = (end - start)/2;
    int * temp = malloc(sizeof(int)*(end-start+1));
    int i = start, j = start+pivot+1, k = 0;
    while ((i <= start+pivot) && (j <= end)) {
        stat->fusionComp += 1;
        if (liste[j] < liste[i]) {
            temp[k] = liste[j];
            stat->fusionPerm += 1;
            k += 1;
            j += 1;
        } else {
            temp[k] = liste[i];
            k += 1;
            i += 1;
        }
    }
    while (i <= start+pivot) {
        temp[k] = liste[i];
        k += 1;
        i += 1;
    }
    while (j <= end) {
        stat->fusionPerm += 1;
        temp[k] = liste[j];
        k += 1;
        j += 1;
    }
    j = 0;
    for (i = start; i <= end; i += 1) {
        liste[i] = temp[j];
        j += 1;
    }
    free(temp);
}

void triFusion (int * liste, int start, int end) {
    int pivot = (end - start)/2;
    if (end - start == 0) {
        return;
    } else {
        triFusion(liste, start, start+pivot);
        triFusion(liste, start+pivot+1, end);
        partitionner(liste, start, end);
    }
}

typedef struct _Tas Tas;
struct _Tas {
    int * tab;
    int size;
    int capacity;
};

void addElement(int elt, Tas * li){
    int i = li->size, end;
    li->tab[li->size] = elt;
    li->size++;
    end = 0;
    while(i > 0 && end == 0){
        end = li->tab[i] < li->tab[(i-1)/2];
        stat->tasComp += 1;
        if(!end){
            swap(&(li->tab[i]),&(li->tab[(i-1)/2]));
            stat->tasPerm += 1;
            i = (i-1)/2;
        }
    }
}

void refaireTas (int i, Tas *li) {
    if (i*2+2 < li->capacity) {
        if (li->tab[i] < li->tab[i*2+1] && li->tab[i*2+1] >= li->tab[i*2+2]) {
            stat->tasComp += 2;
            swap(&li->tab[i], &li->tab[i*2+1]);
            stat->tasPerm += 1;
            refaireTas(i*2+1, li);
        } else if (li->tab[i] < li->tab[i*2+2] && li->tab[i*2+1] <= li->tab[i*2+2]) {
            stat->tasComp += 4;
            swap(&li->tab[i], &li->tab[i*2+2]);
            stat->tasPerm += 1;
            refaireTas(i*2+2, li);
        } else {
            stat->tasComp += 4;
            return;
        }
    } else if (i*2+1 < li->capacity) {
        stat->tasComp += 2;
        if (li->tab[i] < li->tab[i*2+1]) {
            swap(&li->tab[i], &li->tab[i*2+1]);
            stat->tasPerm += 1;
            refaireTas(i*2+1, li);
        } else {
            return;
        }
    } else {
        return;
    }
}

void deleteMaxElement (Tas *li) {
    int i = 0;
    int max = li->tab[0];
    li->capacity -= 1;
    li->tab[0] = li->tab[li->capacity];
    li->tab[li->capacity] = max;
    refaireTas(i, li);
}

void triTas (int taille, int * tab){
    Tas *li = (Tas*) malloc(sizeof(Tas));
    li->tab = malloc(sizeof(int)*taille);
    int i;
    for (i = 0; i < taille; i += 1) {
        li->tab[i] = tab[i];
    }
    int n = taille;
    li->size = 0;
    li->capacity = 0;
    while(li->size < n){
        addElement(tab[li->size], li);
        li->capacity += 1;
    }
    while(li->capacity > 1){
        deleteMaxElement(li);
    }
    li->size = n;
    free(li->tab);
}

void initStats() {
    stat = (Stats*) malloc(sizeof(Stats));
    stat->selectionComp = 0;
    stat->selectionPerm = 0;
    stat->insertionComp = 0;
    stat->insertionPerm = 0;
    stat->bulleComp = 0;
    stat->bullePerm= 0;
    stat->fusionComp = 0;
    stat->fusionPerm = 0;
    stat->rapideComp = 0;
    stat->rapidePerm = 0;
    stat->tasComp = 0;
    stat->tasPerm = 0;

}

void mainPlatform (int nb_expes, int taille_mini, int nb_listes) {
    int i, j, l;
    int *tab, *tab1, *tab2, *tab3, *tab4, *tab5, *tab6;
    initStats();
    for (i = 0; i < nb_listes; i += 1) {
        for (j = 0; j < nb_expes; j += 1) {
            int tailleActuelle = taille_mini*(i+1);
            tab = listeRandom(tailleActuelle);
            tab1 = malloc(sizeof(int)*tailleActuelle);
            tab2 = malloc(sizeof(int)*tailleActuelle);
            tab3 = malloc(sizeof(int)*tailleActuelle);
            tab4 = malloc(sizeof(int)*tailleActuelle);
            tab5 = malloc(sizeof(int)*tailleActuelle);
            tab6 = malloc(sizeof(int)*tailleActuelle);
            for (l = 0; l < tailleActuelle; l += 1) {
                tab1[l] = tab[l];
                tab2[l] = tab[l];
                tab3[l] = tab[l];
                tab4[l] = tab[l];
                tab5[l] = tab[l];
                tab6[l] = tab[l];
            }
            triSelection(tab1, tailleActuelle);
            triInsertion(tab2, tailleActuelle);
            triBulle(tab3, tailleActuelle);
            triFusion(tab4, 0, tailleActuelle-1);
            triRapide(tab5, 0, tailleActuelle);
            triTas(tailleActuelle, tab6);
            free(tab1);
            free(tab2);
            free(tab3);
            free(tab4);
            free(tab5);
            free(tab6);
            // printf("%d %d %d %d %d %d %d\n", tailleActuelle, stat->selectionComp+stat->selectionPerm, stat->insertionComp+stat->insertionPerm, stat->bulleComp+stat->bullePerm, stat->fusionComp+stat->fusionPerm, stat->rapideComp+stat->rapidePerm, stat->tasComp+stat->tasPerm);
            printf("%d, %d, %d, %d, %d, %d, %d\n", tailleActuelle, stat->selectionComp+stat->selectionPerm, stat->insertionComp+stat->insertionPerm, stat->bulleComp+stat->bullePerm, stat->fusionComp+stat->fusionPerm, stat->rapideComp+stat->rapidePerm, stat->tasComp+stat->tasPerm);
            initStats();
        }
    }
}

void afficheStats () {
    printf("===================================================== \n");
    printf("Tri Selection : \n");
    printf("Nombre Permutations : %d \n", stat->selectionPerm);
    printf("Nombre Comparaisons : %d \n", stat->selectionComp);
    printf("===================================================== \n");
    printf("Tri Insertion : \n");
    printf("Nombre Permutations : %d \n", stat->insertionPerm);
    printf("Nombre Comparaisons : %d \n", stat->insertionComp);
    printf("===================================================== \n");
    printf("Tri Bulle : \n");
    printf("Nombre Permutations : %d \n", stat->bullePerm);
    printf("Nombre Comparaisons : %d \n", stat->bulleComp);
    printf("===================================================== \n");
    printf("Tri Fusion : \n");
    printf("Nombre Permutations : %d \n", stat->fusionPerm);
    printf("Nombre Comparaisons : %d \n", stat->fusionComp);
    printf("===================================================== \n");
    printf("Tri Rapide : \n");
    printf("Nombre Permutations : %d \n", stat->rapidePerm);
    printf("Nombre Comparaisons : %d \n", stat->rapideComp);
    printf("===================================================== \n");
    printf("Tri par Tas : \n");
    printf("Nombre Permutations : %d \n", stat->tasPerm);
    printf("Nombre Comparaisons : %d \n", stat->tasComp);
    printf("===================================================== \n");
}


int main () {
    srand(time(NULL));
    mainPlatform(100, 50, 5);
    return 0;
}