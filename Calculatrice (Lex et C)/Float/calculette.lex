%{
    #include "cal.h"

    State etat = WAIT; 
    Pile *p;
    float mem[NBVAR];
%}

%%

\. {etat = gererEtat(DOT, etat, yytext, p, mem);}
\? {etat = gererEtat(QMARK, etat, yytext, p, mem);}
\! {etat = gererEtat(EXMARK, etat, yytext, p, mem);}
\$ {etat = gererEtat(DOLLAR, etat, yytext, p, mem);}
[+-/*] {etat = gererEtat(OP, etat, yytext, p, mem);}
[0-9]+(\.[0-9]+)? {etat = gererEtat(NUM, etat, yytext, p, mem);}
m[0-9]+ {etat = gererEtat(VAR, etat, yytext, p, mem);}
[ ] ;
. {etat = gererEtat(AUTRE, etat, yytext, p, mem);}

%%

int main () {
    p = (Pile*) malloc(sizeof(Pile));
    initPileVide(p);
    yylex();
    return 0;
}

int yywrap () {
    return 1;
}