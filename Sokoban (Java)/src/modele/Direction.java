package modele;

public class Direction {
    Carte carte;
    
    public Direction (Carte c) {
        this.carte = c;
    }

    /*prend un string en input, et cherche le premier caractère correspondant à une direction et appelle leur fonction */

    public boolean seDeplacer (String input) {
        if (input.length() > 1) {
            if (input.contains("z") || input.contains("q") || input.contains("s") || input.contains("d")) {
                for (int i = 0; i < input.length(); i += 1) {
                    switch (input.charAt(i)) {
                        case 'z': 
                            this.bougerHaut();
                            return true;
                        case 'q':
                            this.bougerGauche();
                            return true;
                        case 's':
                            this.bougerBas();
                            return true;
                        case 'd':
                            this.bougerDroite();
                            return true;
                        default:
                            continue;
                    }
                }
            } else {
                return false;
            }
        } else {
            switch (input.charAt(0)) {
                case 'z': 
                    this.bougerHaut();
                    return true;
                case 'q':
                    this.bougerGauche();
                    return true;
                case 's':
                    this.bougerBas();
                    return true;
                case 'd':
                    this.bougerDroite();
                    return true;
                default:
                    return false;
            }
        }
        return false;
    }

    /*bouger un joueur cers le haut */

    public boolean bougerHaut () {
        int[] coord = carte.getCoordPlayer();
        if (coord[0] == 0) {
            return false;
        }
        if (carte.isMoveable(coord[0]-1, coord[1])) {
            if (carte.getCaisse(coord[0]-1, coord[1])) {
                if (coord[0] > 1) {
                    return carte.tryMovePlayerAndCaisse(coord[0], coord[1], coord[0]-1, coord[1], coord[0]-2, coord[1]);
                } else {
                    return false;
                }
            } else {
                return carte.movePlayerHere(coord[0], coord[1], coord[0]-1, coord[1]);
            }
        } else {
            return false;
        }
    }

    /*vers la gauche */

    public boolean bougerGauche () {
        int[] coord = carte.getCoordPlayer();
        if (coord[1] == 0) {
            return false;
        }
        if (carte.isMoveable(coord[0], coord[1]-1)) {
            if (carte.getCaisse(coord[0], coord[1]-1)) {
                if (coord[0] > 1) {
                    return carte.tryMovePlayerAndCaisse(coord[0], coord[1], coord[0], coord[1]-1, coord[0], coord[1]-2);
                } else {
                    return false;
                }
            } else {
                return carte.movePlayerHere(coord[0], coord[1], coord[0], coord[1]-1);
            }
        } else {
            return false;
        }
    }

    /*le bas */

    public boolean bougerBas () {
        int[] coord = carte.getCoordPlayer();
        if (coord[0] == carte.getHauteur()-1) {
            return false;
        }
        if (carte.isMoveable(coord[0]+1, coord[1])) {
            if (carte.getCaisse(coord[0]+1, coord[1])) {
                if (coord[0] > 1) {
                    return carte.tryMovePlayerAndCaisse(coord[0], coord[1], coord[0]+1, coord[1], coord[0]+2, coord[1]);
                } else {
                    return false;
                }
            } else {
                return carte.movePlayerHere(coord[0], coord[1], coord[0]+1, coord[1]);
            }
        } else {
            return false;
        }
    }

    /*droite */

    public boolean bougerDroite () {
        int[] coord = carte.getCoordPlayer();
        if (coord[1] == carte.getLargeur()-1) {
            return false;
        }
        if (carte.isMoveable(coord[0], coord[1]+1)) {
            if (carte.getCaisse(coord[0], coord[1]+1)) {
                if (coord[0] > 1) {
                    return carte.tryMovePlayerAndCaisse(coord[0], coord[1], coord[0], coord[1]+1, coord[0], coord[1]+2);
                } else {
                    return false;
                }
            } else {
                return carte.movePlayerHere(coord[0], coord[1], coord[0], coord[1]+1);
            }
        } else {
            return false;
        }
    }
}
