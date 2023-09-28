package modele;

import java.util.ArrayList;
import java.util.List;

public class Carte {
    private List<String> lignes;
    private Case[][] carte;
    private int nbLignes;
    private int tailleLignes;
    private int[] coordPlayer = new int[2];
    private List<Integer[]> destinations = new ArrayList<Integer[]>();
    private String file;
    
    public Carte (String file) {
        this.file = file;
        Lecture map = new Lecture(file);
        this.lignes = map.getLignes();
        nbLignes = map.getNbLignes();
        tailleLignes = map.getTaille();
        carte = new Case[nbLignes][tailleLignes];
        this.creerCarte();
    }

    /*créer la carte */

    public void creerCarte () {
        for (int i = 0; i < nbLignes; i += 1) {
            String line = lignes.get(i);
            for (int j = 0; j < tailleLignes; j += 1) {
                switch (line.charAt(j)) {
                    case '/':
                        carte[i][j] = new Vide(i, j);
                        break;
                    case '#':
                        carte[i][j] = new Mur(i, j);
                        break;
                    case '.':
                        carte[i][j] = new Destination(i, j);
                        Integer[] dest = {i,j};
                        destinations.add(dest);
                        break;
                    case ' ':
                        carte[i][j] = new Sol(i, j);
                        break;
                    case '$':
                        carte[i][j] = new Sol(i, j);
                        ((Sol) carte[i][j]).moveCaisseHere();
                        break;
                    case '@':
                        carte[i][j] = new Sol(i, j);
                        ((Sol) carte[i][j]).moveHere(true);
                        coordPlayer[0] = i;
                        coordPlayer[1] = j;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    /*return true si la case est un sol ou une destination */

    public boolean isMoveable (int x, int y) {
        return this.carte[x][y].movable();
    }

    public int[] getCoordPlayer () {
        return coordPlayer;
    }

    public void setCoordPlayer (int x, int y) {
        this.coordPlayer[0] = x;
        this.coordPlayer[1] = y;
    }

    public int getHauteur () {
        return nbLignes;
    }

    public int getLargeur () {
        return tailleLignes;
    }

    public boolean getCaisse (int x, int y) {
        return carte[x][y].getCaisse();
    }

    public String getFile () {
        return this.file;
    }

    /*est appelée si le joueur veut bouger dans la direction d'une caisse. si le mouvement est possible, bouge la joueur et la caisse et retire le joueur de la case d'avant
    xStart et yStart sont les coordonées de départ du joueur, xPlayer et yPlayer est la où il veut aller, xCaisse et yCaisse est là où la caisse doit aller */

    public boolean tryMovePlayerAndCaisse (int xStart, int yStart, int xPlayer, int yPlayer, int xCaisse, int yCaisse) {
        boolean move = carte[xPlayer][yPlayer].moveHere(carte[xCaisse][yCaisse].moveCaisseHere());
        if (move) {
            carte[xStart][yStart].playerLeave();
            this.setCoordPlayer(xPlayer, yPlayer);
        }
        return move;
    }

    /*bouge le joueur sur la case [xPlayer, yPlayer] */

    public boolean movePlayerHere (int xStart, int yStart, int xPlayer, int yPlayer) {
        boolean move = carte[xPlayer][yPlayer].moveHere(false);
        if (move) {
            carte[xStart][yStart].playerLeave();
            this.setCoordPlayer(xPlayer, yPlayer);
        }
        return move;
    }

    /*vérifie si la partie est finie */

    public boolean verifFinPartie () {
        for (int i = 0; i < destinations.size(); i += 1) {
            if (carte[destinations.get(i)[0]][destinations.get(i)[1]].getCaisse() == false) {
                return false;
            }
        }
        return true;
    }

    public Case getCase(int x, int y) {
        return carte[x][y];
    }

    public String toString () {
        String res = "";
        for (int i = 0; i < nbLignes; i += 1) {
            for (int j = 0; j < tailleLignes; j += 1) {
                res += carte[i][j].toString();
            }
            res += "\n";
        }
        return res;
    }
}
