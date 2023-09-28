package vueTexte;

import modele.Carte;

public class SokobanTexte {
    public static void main (String[] args) {
        Carte carte = new Carte("./src/modele/map1.txt");
        new ModeTexte(carte);
    }
}
