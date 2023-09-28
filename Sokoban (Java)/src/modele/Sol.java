package modele;

public class Sol extends Case{
    private boolean player = false;
    private boolean caisse;

    public Sol (int x, int y) {
        super(x, y, true);
        caisse = false;
    }

    /*return true si la case est vide */

    public boolean caseVide () {
        return player == false && caisse == false;
    }

    /*bouge le joueur sur cette case si possible, move est true si la case est vide ou bien si la caisse peut etre bougée sur le case suivante */

    public boolean moveHere (boolean move) {
        if (this.caseVide()) {
            super.movePlayer();
            player = true;
            return true;
        }
        if (caisse == true && move == true) {
            super.movePlayer();
            super.removeCaisse();
            player = true;
            caisse = false;
            return true;
        }
        return false;
    }

    /*bouge une caisse sur cette case si possible */

    public boolean moveCaisseHere () {
        if (this.caseVide()) {
            super.moveCaisse();
            caisse = true;
            return true;
        }
        return false;
    }

    /*retire le joueur de cette case */

    public void playerLeave () {
        super.removePlayer();
        this.player = false;
    }

    /*return le type de la case */

    public String getType() {
        return "Sol";
    }

    public String toString() {
        if (player) {
            return "@";
        }
        if (caisse) {
            return "$";
        }
        return " ";
    }
}