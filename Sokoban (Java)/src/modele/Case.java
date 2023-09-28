package modele;

public abstract class Case{
    private int x;
    private int y;
    private boolean movable;
    private boolean player = false;
    private boolean caisse = false;

    public Case (int x, int y, boolean movable) {
        this.x = x;
        this.y = y;
        this.movable = movable;
    }

    /*return l'attribut movable (true si c'est une cse où le joueur peut bouger (sol ou destination)) */

    public boolean movable () {
        return this.movable;
    }

    /*met une caisse ici */

    public void moveCaisse () {
        this.caisse = true;
    }

    /*retire la caisse d'ici */

    public void removeCaisse () {
        this.caisse = false;
    }

    public boolean getCaisse () {
        return caisse;
    }

    /*met un joueur ici */

    public void movePlayer () {
        this.player = true;
    }

    /*retire un joueur de la case */

    public void removePlayer () {
        this.player = false;
    }

    public boolean getPlayer () {
        return player;
    }

    public abstract void playerLeave ();

    public int[] getCoord () {
        int[] coord = {x, y};
        return coord;
    }

    public abstract boolean moveHere (boolean move);

    public abstract boolean moveCaisseHere ();

    public abstract String getType();
}
