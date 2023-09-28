Compilation du .c :
gcc TP1.c -o TP1

Si vous voulez changer les valeurs de tests pour les tris, changez les paramètre de l'appel de fonction mainPlatform dans le main

Le .c sort actuellement les données pour un .csv, pour le changer vers .dat, trouver les 2 printf dans la fonction 
mainPlatform et échanger la ligne commentée

Produire le .csv:
./TP1 > sortie.csv

Produire le .dat:
./TP1 > data.dat

Pour créer le graphique:
gnuplot resultats.gnu