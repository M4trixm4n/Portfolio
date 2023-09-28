Compiler: make
Supprimer les .o : make clean
Supprimer les .o et l'executable: make mrproper


Mon TP devrait être similaire à ce qui est demandé, une différence pourrait être que j'ai créé
une structure Villes, qui contient le nom des villes, leur nombre, et la distance entre chacune.
Aussi ne faites pas trop attention aux malloc/free dans ajoutePersonne dans personne.c, j'avais commencé le code
il y a un moment, et ai préféré rajouter un free pour le faire marcher plutot que de retirer un malloc et d'avoir
une erreur sur valgrind (ce qui me demandrait de comprendre le code ou de le réécrire, mais c'est assez serré niveau temps).

Utilisation:
    Je crée le graphe, le symétrise, et calcule les composantes connexes simples et fortes d'office.
    Pour afficher les composantes voulues, décommenter dans le main la bonne fonction d'affichage (il y en a 3) et recompiler.
    Vous pouvez aussi afficher le tableau de personnes ou la structure Villes en décommentant une des 2 commandes sous celles d'affichage.

Remarques:
-   valgrind indique une erreur sur le fopen dans parse.c, mais je ne sais pas d'où elle vient, 
    et elle ne semble pas exister sur internet. Donc soit problème avec mon pc, soit une petite erreur que je ne vois pas.
-   Il est impératif d'exécuter avec un fichier similaire à ceux sur moodle. Il faut les accolades au début et à la fin, 
    et les crochets seuls sur une ligne pour les parties avec les abonnements et les distances. Il faut aussi que les
    tabulations fassent 2 espaces pour que je puisse lire '[' avec ligne[2] pour la ligne juste avant la partie 
    sur les abonnements (de même pour les autres lignes).