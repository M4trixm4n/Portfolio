package modele;

public class Mur extends Case{

    public Mur (int x, int y) {
        super(x, y, false);
    }

    /*one ne peut pas bouger ici */

    public boolean moveHere (boolean move) {
        return false;
    }

    /*on ne peut pas y mettre de caisse */

    public boolean moveCaisseHere () {
        return false;
    }

    /*??? */

    public void playerLeave () {
        super.removePlayer();
    }

    /*return le type de la case */

    public String getType() {
        return "Mur";
    }

    public String toString() {
        return "#";
    }
}
