Comment compiler (avoir les .class):
Run:
javac -cp "bin\sqlite-jdbc.jar;bin;src" -d "bin" src\ConnectionDB.java
javac -cp "bin\sqlite-jdbc.jar;bin;src" -d "bin" src\interfaceGraphique.java
javac -cp "bin\sqlite-jdbc.jar;bin;src" -d "bin" src\projetMain.java

Comment exécuter:
(Le .jar ne fonctionne pas pour une erreur que nous ne comprenons pas, donc nous exécutons le fichier .java contenant le main)
Run:
java -cp "bin\sqlite-jdbc.jar;bin" src/projetMain


(Si vous voulez refresh la database, supprimez projet.db dans le bin, faites .read projet.sql en sqlite3, et déplacez le fichier db nouvellement créé dans le bin)