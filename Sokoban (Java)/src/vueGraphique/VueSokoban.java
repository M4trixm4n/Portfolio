package vueGraphique;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

import modele.Carte;
import modele.Case;
import modele.Direction;


public class VueSokoban {

    private JFrame frame;
    private JPanel top;
    private JPanel grille;

    private int nbDeplacement = 0;
    private JLabel deplacements = new JLabel("Nombre de déplacements : " + nbDeplacement);

    private Carte carte;
    private int hauteur;
    private int largeur;

    private ImageComponent[][] images;
    private Direction dir;
    private String directionJoueur = "Bas";

    private JComboBox<String> niveaux;
    private int level = 0;
    
    public VueSokoban (Carte c) {
        this.carte = c;
        hauteur = carte.getHauteur();
        largeur = carte.getLargeur();
        dir = new Direction(c);
        this.creerFrame();
    }

    /*key listener répondant aux flèche du clavier pour bouger */

    KeyListener movement = new KeyListener() {

        public void keyTyped(KeyEvent e) {
            
        }

        public void keyPressed(KeyEvent e) {
            if (!(partieFinie())) {
                switch (e.getKeyCode()) {
                    case KeyEvent.VK_UP:
                        dir.seDeplacer("z");
                        directionJoueur = "Haut";
                        nbDeplacement += 1;
                        deplacements.setText("Nombre de déplacements : " + nbDeplacement);
                        break;
                    case KeyEvent.VK_LEFT:
                        dir.seDeplacer("q");
                        directionJoueur = "Gauche";
                        nbDeplacement += 1;
                        deplacements.setText("Nombre de déplacements : " + nbDeplacement);
                        break;
                    case KeyEvent.VK_DOWN:
                        dir.seDeplacer("s");
                        directionJoueur = "Bas";
                        nbDeplacement += 1;
                        deplacements.setText("Nombre de déplacements : " + nbDeplacement);
                        break;
                    case KeyEvent.VK_RIGHT:
                        dir.seDeplacer("d");
                        directionJoueur = "Droite";
                        nbDeplacement += 1;
                        deplacements.setText("Nombre de déplacements : " + nbDeplacement);
                        break;
                    default:
                        break;
                }
                grille.removeAll();
                setImages();
                grille.invalidate();
                grille.validate();
                grille.repaint();
            }
        }

        public void keyReleased(KeyEvent e) {
            
        }

    };

    /*recommencer la partie */

    ActionListener rec = new ActionListener () {
        public void actionPerformed (ActionEvent e) {
            frame.setVisible(false);
            carte = new Carte(carte.getFile());
            nbDeplacement = 0;
            dir = new Direction(carte);
            creerFrame();
        }
    };

    /*changer le niveau */

    ActionListener next = new ActionListener () {
        @SuppressWarnings("unchecked")
        public void actionPerformed (ActionEvent e) {
            frame.setVisible(false);
            String niveau = (String)(((JComboBox<String>)e.getSource()).getSelectedItem());
            String file = "";
            if (niveau == "Niveau 1") {
                file = "./src/modele/map1.txt";
                level = 0;
            }
            else if (niveau == "Niveau 2") {
                file = "./src/modele/map2.txt";
                level = 1;
            }
            carte = new Carte(file);
            hauteur = carte.getHauteur();
            largeur = carte.getLargeur();
            nbDeplacement = 0;
            dir = new Direction(carte);
            creerFrame();
        }
    };

    /*créer la fenetre et ses éléments */

    public void creerFrame () {
        frame = new JFrame("Sokoban");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLayout(new BorderLayout());
        
        top = new JPanel();
        top.setLayout(new FlowLayout());
        
        JButton restart = new JButton("Recommencer");
        restart.addActionListener(rec);
        deplacements = new JLabel("Nombre de déplacements : " + nbDeplacement);
        String[] niv = {"Niveau 1", "Niveau 2"};
        niveaux = new JComboBox<String>(niv);
        niveaux.setSelectedIndex(level);
        niveaux.addActionListener(next);
        
        top.add(restart);
        top.add(deplacements);
        top.add(niveaux);
        
        grille = new JPanel();
        grille.setLayout(new GridLayout(hauteur, largeur));
        grille.setFocusable(true);
        grille.requestFocusInWindow();
        
        frame.setSize(largeur*30, (hauteur*30) + 30);
        frame.add(top, BorderLayout.NORTH);
        frame.add(grille, BorderLayout.CENTER);
        frame.setFocusable(true);
        frame.requestFocusInWindow();

        this.setImages();
        frame.addKeyListener(movement);
        frame.setVisible(true);
    }

    /*remplir la grille avec les images correspondantes à la carte */

    public void setImages() {
        images = new ImageComponent[hauteur][largeur];
        for (int i = 0; i < hauteur; i += 1) {
            for (int j = 0; j < largeur; j += 1) {
                Case ij = carte.getCase(i, j);
                switch (ij.getType()) {
                    case "Destination":
                        if (ij.getCaisse()) {
                            images[i][j] = new ImageComponent(new ImageIcon("./src/img/caisse2.gif").getImage());
                        } else if (ij.getPlayer()) {
                            images[i][j] = new ImageComponent(new ImageIcon("./src/img/" + directionJoueur + ".gif").getImage());
                        } else {
                            images[i][j] = new ImageComponent(new ImageIcon("./src/img/but.gif").getImage());
                        }                        break;
                    case "Sol":
                        if (ij.getCaisse()) {
                            images[i][j] = new ImageComponent(new ImageIcon("./src/img/caisse1.gif").getImage());
                        } else if (ij.getPlayer()) {
                            images[i][j] = new ImageComponent(new ImageIcon("./src/img/" + directionJoueur + ".gif").getImage());
                        } else {
                            images[i][j] = new ImageComponent(new ImageIcon("./src/img/sol.gif").getImage());
                        }
                        break;
                    case "Vide":
                        images[i][j] = new ImageComponent(new ImageIcon().getImage());
                        break;
                    case "Mur":
                        images[i][j] = new ImageComponent(new ImageIcon("./src/img/mur.gif").getImage());
                        break;
                    default:
                        images[i][j] = new ImageComponent(new ImageIcon().getImage());
                        break;
                }
                images[i][j].setSize(30, 30);
                grille.add(images[i][j]);
            }
        }
    }

    /*verif fin de partie */

    public boolean partieFinie () {
        if (carte.verifFinPartie()) {
            System.out.println("Partie finie");
            return true;
        }
        return false;
    }
}