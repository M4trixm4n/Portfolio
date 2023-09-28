package modele;

public class Vide extends Case{

    public Vide (int x, int y) {
        super(x, y, false);
    }

    /*on peut pas bouger là */

    public boolean moveHere (boolean move) {
        return false;
    }

    /*on peut pas y mettre une caisse */

    public boolean moveCaisseHere () {
        return false;
    }

    /*??? */

    public void playerLeave () {
        super.removePlayer();
    }

    /*return le type de la case */

    public String getType() {
        return "Vide";
    }

    public String toString() {
        return "/";
    }
}
