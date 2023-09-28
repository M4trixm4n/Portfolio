#define MAX 30

typedef struct {
    float t[MAX];
    int taille;
} Pile;

void initPileVide (Pile *p);
void empiler (float x, Pile *p);
float depiler (Pile *p);
float estPileVide (Pile p);
float elementSommetPile (Pile p);