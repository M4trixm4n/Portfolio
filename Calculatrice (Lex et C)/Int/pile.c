#include <stdio.h>
#include <assert.h>
#include "pile.h"
#include <stdlib.h>

void initPileVide (Pile *p) {
    p->taille = 0;
}

void empiler (int x, Pile *p) {
    assert (p->taille <= MAX);
    p->t[p->taille] = x;
    p->taille += 1;
}

int depiler (Pile *p) {
    assert (p->taille > 0);
    p->taille -= 1;
    return p->t[p->taille];
}

int estPileVide (Pile p) {
    return (p.taille == 0);
}

int elementSommetPile (Pile p) {
    return p.t[p.taille-1];
}