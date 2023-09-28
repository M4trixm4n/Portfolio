#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cal.h"

/*  */

Pile *pile;

State gererEtat (Lexic type, State etat, char* entree, Pile *p, int *mem) {
    pile = p;
    switch (type) {
        case DOT:
            etat = gererDOT(etat, mem);
            break;
        case QMARK:
            etat = gererQMARK(etat);
            break;
        case EXMARK:
            etat = gererEXMARK(etat);
            break;
        case DOLLAR:
            etat = gererDOLLAR(etat);
            break;
        case OP:
            etat = gererOP(etat, entree);
            break;
        case NUM:
            etat = gererNUM(etat, entree);
            break;
        case VAR:
            etat = gererVAR(etat, entree, mem);
            break;
        case AUTRE:
            etat = gererAUTRE(etat);
            break;
        default:
            break;
    }
    return etat;
}

State gererDOT (State etat, int *mem) {
    switch (etat) {
        case WAIT:
            exit(EXIT_SUCCESS);
            break;
        case EVALCMD:
            mem[0] = depiler(pile);
            printf("%d", mem[0]);
            etat = WAIT;
            break;
        default:
            etat = WAIT;
    }
    return etat;
}

State gererQMARK (State etat) {
    switch (etat) {
        case WAIT:
            etat = EVALCMD;
            break;
        case SKIP:
            break;
        default:
            printf("Erreur");
            etat = SKIP;
            break;
    }
    return etat;
}

State gererEXMARK (State etat) {
    switch (etat) {
        case WAIT:
            etat = STOCMD;
            break;
        case SKIP:
            break;
        default:
            printf("Erreur");
            etat = SKIP;
            break;
    }
    return etat;
}

State gererDOLLAR (State etat) {
    switch (etat) {
        case WAIT:
            etat = DISCMD;
            break;
        case SKIP:
            break;
        default:
            printf("Erreur");
            etat = SKIP;
            break;    
    }
    return etat;
}

State gererOP (State etat, char* entree) {
    switch (etat) {
        case EVALCMD:
            int val2 = depiler(pile);
            int val1 = depiler(pile);
            switch (entree[0]) {
                case '+':
                    empiler(val1+val2, pile);
                    break;
                case '-':
                    empiler(val1-val2, pile);
                    break;
                case '/':
                    empiler(val1/val2, pile);
                    break;
                case '*':
                    empiler(val1*val2, pile);
                    break;
                default:
                    break;
            }
        case SKIP:
            break;
        default:
            printf("Erreur");
            etat = SKIP;
            break;
    }
    return etat;
}

State gererNUM (State etat, char* entree) {
    switch (etat) {
        case EVALCMD:
            empiler(atoi(entree), pile);
            break;
        case SKIP:
            break;
        default:
            printf("Erreur");
            etat = SKIP;
            break;
    }
    return etat;
}

State gererVAR (State etat, char* entree, int *mem) {
    entree = strrev(entree);
    int var = 0, i;
    for (i = 0; i < strlen(entree)-1; i += 1) {
        char tmp[1];
        tmp[0] = entree[i];
        var += atoi(tmp)*(pow(10, i));
    }
    switch (etat) {
        case DISCMD:
            printf("%d", mem[var]);
            break;
        case EVALCMD:
            empiler(mem[var], pile);
            break;
        case SKIP:
            break;
        case STOCMD:
            mem[var] = mem[0];
            break;
        default:
            etat = SKIP;
            break;
    }
    return etat;
}

State gererAUTRE (State etat) {
    switch (etat) {
        case SKIP:
            break;
        default:
            printf("Erreur");
            etat = SKIP;
            break;
    }
    return etat;
}