package vueGraphique;

import modele.Carte;

public class Sokoban {
    public static void main(String[] args) {
        Carte carte = new Carte("./src/modele/map1.txt");
        new VueSokoban(carte);
    }
}
