#define MAX 30

typedef struct {
    int t[MAX];
    int taille;
} Pile;

void initPileVide (Pile *p);
void empiler (int x, Pile *p);
int depiler (Pile *p);
int estPileVide (Pile p);
int elementSommetPile (Pile p);