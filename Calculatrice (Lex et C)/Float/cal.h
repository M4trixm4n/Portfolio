#include "pile.h"

#define NBVAR 20

enum lexical_values {DOT, QMARK, EXMARK, DOLLAR, OP, NUM, VAR, AUTRE};
typedef enum lexical_values Lexic;

enum states {WAIT, DISCMD, STOCMD, EVALCMD, SKIP};
typedef enum states State;

State gererEtat (Lexic type, State etat, char* entree, Pile *p, float *mem);
State gererDOT (State etat, float *mem);
State gererQMARK (State etat);
State gererEXMARK (State etat);
State gererDOLLAR (State etat);
State gererOP (State etat, char* entree);
State gererNUM (State etat, char* entree);
State gererVAR (State etat, char* entree, float *mem);
State gererAUTRE (State etat);
