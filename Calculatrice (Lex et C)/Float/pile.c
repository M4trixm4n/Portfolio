#include <stdio.h>
#include <assert.h>
#include "pile.h"
#include <stdlib.h>

void initPileVide (Pile *p) {
    p->taille = 0;
}

void empiler (float x, Pile *p) {
    assert (p->taille <= MAX);
    p->t[p->taille] = x;
    p->taille += 1;
}

float depiler (Pile *p) {
    assert (p->taille > 0);
    p->taille -= 1;
    return p->t[p->taille];
}

float estPileVide (Pile p) {
    return (p.taille == 0);
}

float elementSommetPile (Pile p) {
    return p.t[p.taille-1];
}