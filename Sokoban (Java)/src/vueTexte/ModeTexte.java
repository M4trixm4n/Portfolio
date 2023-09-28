package vueTexte;

import modele.Carte;
import modele.Direction;

public class ModeTexte {
    Carte carte;
    Direction dir;

    public ModeTexte (Carte c) {
        this.carte = c;
        this.dir = new Direction(c);
        this.jeu();
    }

    /*fonction principale, tant que la partie n'est pas finie, demande une entrée pour jouer */

    public void jeu () {
        while (!(partieFinie())) {
            Outil o = new Outil();
            System.out.println(carte);
            System.out.println("Entrez une direction (z, q, s, d) :");
            String input = o.lireCaractere();
            boolean move = dir.seDeplacer(input);
            if (move == false) {
                System.out.println("Mauvaise entrée, veuillez réessayer.");
            }
        }
    }

    /*vérifie si la partie est finie */

    public boolean partieFinie () {
        if (carte.verifFinPartie()) {
            System.out.println(carte);
            System.out.println("Partie finie");
            return true;
        }
        return false;
    }
}
