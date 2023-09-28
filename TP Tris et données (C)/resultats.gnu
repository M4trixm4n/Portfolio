set terminal png
set output "courbes.png"
set title 'Comparaison des algorithmes de tris'
set xlabel 'Taille de la liste'
set ylabel "Nombre d'opérations"
set style fill transparent solid 0.5 noborder
plot 'data.dat' using 1:2 title 'Tri par sélection' with linespoints,\
    'data.dat' using 1:3 title 'Tri par insertion' with linespoints,\
    'data.dat' using 1:4 title 'Tri à bulles' with linespoints,\
    'data.dat' using 1:5 title 'Tri fusion' with linespoints,\
    'data.dat' using 1:6 title 'Tri rapide' with linespoints,\
    'data.dat' using 1:7 title 'Tri par tas' with linespoints