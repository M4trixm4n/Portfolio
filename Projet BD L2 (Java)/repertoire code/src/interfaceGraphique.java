package src;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;

import java.awt.*;
import java.awt.event.*;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;

public class interfaceGraphique {

    /*fenetre principale*/
    
    private JFrame frame;
    
    /*haut et bas de la fenetre séparés*/
    
    private JPanel top;
    private JPanel bottom;
    
    /*tableau qui contiendra toutes les données, scrollable, avec des données par défaut, jamais affichées*/

    Object[] headers = {"start"};
    Object[][] data = {{"start"}};

    private JTable liste = new JTable(data, headers);
    private JScrollPane scroll = new JScrollPane(liste);

    /*constructeur*/

    public interfaceGraphique () {
        this.creerFenetre();
    }

    /*crée la fenetre principale*/

    public void creerFenetre () {
        this.frame = new JFrame("Projet");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setBounds(150, 20, 1000, 650);    //taille de la fenetre
        frame.getContentPane().setLayout(new BoxLayout(frame.getContentPane(), BoxLayout.Y_AXIS)); //layout de la fenetre en BoxLayout poour délimiter haut et bas

        top = new JPanel(); //panel du haut
        top.setLayout(null); //layout null pour placer facilement les éléments

        bottom = new JPanel(); //panel du bas
        bottom.setLayout(new BorderLayout()); //layout du bas en BorderLayout

        frame.add(top);
        frame.add(bottom); //j'ajoute le haut et le bas à la fenetre

        top.setPreferredSize(new Dimension(frame.getWidth(), 80)); //taille de la barre du haut
        bottom.setPreferredSize(new Dimension(frame.getWidth(), frame.getHeight() - 50)); //taille de la barre du bas

        this.barreTop();
        this.tableau(); //appel des fonctions principale pour le haut et le bas

        bottom.add(liste.getTableHeader(), BorderLayout.NORTH);
        bottom.add(scroll, BorderLayout.CENTER); //ajout des noms de colonnes et du tableau de données dans la partie du bas
        
        frame.pack();
        frame.setVisible(true);
    }

    /*toutes les données nécessaire pour remplir la combobox de tri*/
    
    String[] trierParMusique = {"Annee", "Nb_vues", "Duree", "Ordre Alphabétique"};
    
    String[] trierParGroupe = {"Ordre Alphabétique"};
    
    String[] trierParArtiste = {"Ordre Alphabétique"};
    
    String[] trierParAlbum = {"Annee", "Ordre Alphabétique"};
    
    String[] trierParConcert = {"Date"};

    String[] trierParGenre = {"Ordre Alphabétique"};

    String[] trierParViews = {"Groupes qui performent en concert à un tel lieu", "Groupes qui performent en concert à une telle date", "Chaque artistes ayant participé à une musique", "Classement des albums par nombre de vues total", "Musiques par genre"};
    
    /*toutes les variables necessaires pour la barre du haut, globales pour pouvoir les modifier et les utiliser de partout*/

    JComboBox<String> choix; //affecte le type du résultat
    JComboBox<String> attribut = new JComboBox<String>(trierParMusique); //affecte dans quel ordre est trié le tableau de données
    JButton ajouter = new JButton("Ajouter"); //bouton ajouter
    JButton supprimer = new JButton("Supprimer"); //bouton supprimer
    JLabel label1 = new JLabel("Que cherchez-vous ?");
    JLabel label2 = new JLabel("Trier par:");

    /*action listener qui s'execute lors du clic sur le bouton ajouter*/

    ActionListener boutonAjouter = new ActionListener () {
        public void actionPerformed (ActionEvent e) {
            if (selectedOption != "Views") {
                fenetreAjouter();
            }
        }
    };

    /*action listener qui s'execute lors du clic sur le bouton supprimer*/

    ActionListener boutonSupprimer = new ActionListener () {
        public void actionPerformed (ActionEvent e) {
            if (selectedOption != "Views") {
                fenetreSupprimer();
            }
        }
    };

    /*creation et placement des éléments dans la barre du haut */

    public void barreTop() {
        int w = frame.getWidth(); //largeur de la fenetre

        label1.setBounds(15, 10, (w/4)-20, 20);
        label2.setBounds((2*w/4)+15, 10, (w/4)-20, 20); //placement des labels indiquant la fonction des combobox
        ajouter.setBounds((3*w/4)+15, 8, (w/4)-30, 20); //placement du bouton ajouter
        supprimer.setBounds((w/4)+15, 8, (w/4)-30, 20);

        top.add(label1);
        top.add(label2);
        top.add(ajouter);
        top.add(supprimer); //ajout de ces éléments

        ajouter.addActionListener(boutonAjouter); //ajout de l'action listener sur le bouton ajouter
        supprimer.addActionListener(boutonSupprimer); //idem pour le bouton supprimer

        String[] tables = {"Musique", "Groupe", "Artiste", "Album", "Concert", "Genre", "Views"}; //choix de recherches

        choix = new JComboBox<String>(tables);

        choix.setBounds(10, 40, (w/2)-20, 30);
        attribut.setBounds((2*w/4)+10, 40, (w/2)-20, 30);

        top.add(choix);
        top.add(attribut);

        this.setChercherPar((String)choix.getSelectedItem()); //appel fonction
        choix.addActionListener(changementChoix); 
        attribut.addActionListener(changementTri);
        attribut.addActionListener(changementViews); //ajout des action listener qui écoutent le changement du choix dans les combobox
    }

    private boolean autoriseChangement = true; //variable test, n'a peut-être aucun effet

    /*ecoute le changement du choix dans la premiere combobox */

    ActionListener changementChoix = new ActionListener () {
        @SuppressWarnings("unchecked")
        public void actionPerformed (ActionEvent e) {
            autoriseChangement = false;
            setChercherPar((String)(((JComboBox<String>)e.getSource()).getSelectedItem()));
            autoriseChangement = true;
            tableau();
        }
    };

    /*ecoute le changement du choix dans la deuxieme combobox */

    ActionListener changementTri = new ActionListener () {
        @SuppressWarnings("unchecked")
        public void actionPerformed (ActionEvent e) {
            String temp = (String)(((JComboBox<String>)e.getSource()).getSelectedItem());
            if (autoriseChangement && temp != null) {
                selectedTri = temp;
                tableau();
            }
        }
    };

    /*ecoute le changement du choix dans la deuxieme combobox quand la première indique Views*/

    ActionListener changementViews = new ActionListener () {
        @SuppressWarnings("unchecked")
        public void actionPerformed (ActionEvent e) {
            String temp = (String)(((JComboBox<String>)e.getSource()).getSelectedItem());
            if (selectedOption == "Views" && autoriseChangement && temp != null) {
                selectedTri = temp;
                tableau();
            }
        }
    };

    private String selectedOption = "Musique"; //option choisie (type du resultat)
    private String selectedTri = "Annee"; //tri choisi

    /*au changement de la première combobox, change la 2eme  en conséquences */

    public void setChercherPar (String selected) {
        selectedOption = selected; //enregistre l'option choisie
        attribut.removeAllItems(); //enlève tous les choix de la 2eme combobox
        
        //ajout des choix dans le combobox de tri
        if (selected == "Musique") { //option musique
            this.tableau();
            for (int i = 0; i < trierParMusique.length; i += 1) {
                attribut.addItem(trierParMusique[i]);
            }
        }
        if (selected == "Groupe") { //option groupe
            for (int i = 0; i < trierParGroupe.length; i += 1) {
                attribut.addItem(trierParGroupe[i]);
            }
        }
        if (selected == "Artiste") { //option artiste
            for (int i = 0; i < trierParArtiste.length; i += 1) {
                attribut.addItem(trierParArtiste[i]);
            }
        }
        if (selected == "Album") { //option album
            for (int i = 0; i < trierParAlbum.length; i += 1) {
                attribut.addItem(trierParAlbum[i]);
            }
        }
        if (selected == "Concert") { //option concert
            for (int i = 0; i < trierParConcert.length; i += 1) {
                attribut.addItem(trierParConcert[i]);
            }
        }
        if (selected == "Genre") {
            for (int i = 0; i < trierParGenre.length; i += 1) {
                attribut.addItem(trierParGenre[i]);
            }
        }
        if (selected == "Views") { //option views
            for (int i = 0; i < trierParViews.length; i += 1) {
                attribut.addItem(trierParViews[i]);
            }
        }
        frame.invalidate();
        frame.validate();
        frame.repaint();
    }

    /*remplis le tableau avec les données nécessaire et rend les cases du tableau inéditable */

    public void tableau () {
        bottom.remove(liste.getTableHeader());
        bottom.remove(scroll);

        ConnectionDB res;
        //switch qui appelle la requete sql nécessaire dans chaque cas et affiche le résultat dans le tableau
        switch (selectedOption) {
            case "Musique":
                switch (selectedTri) {
                    case "Annee":
                        res = new ConnectionDB("select * from musique order by annee DESC;", "SELECT");
                        headers = new Object[res.getHeaders().length];
                        data = new Object[res.getData().length][res.getNbColonnes()];
                        headers = res.getHeaders();
                        data = res.getData();
                        break;
                    case "Nb_vues":
                        res = new ConnectionDB("select * from musique order by nb_vues DESC;", "SELECT");
                        headers = new Object[res.getHeaders().length];
                        data = new Object[res.getData().length][res.getNbColonnes()];
                        headers = res.getHeaders();
                        data = res.getData();
                        break;
                    case "Duree":
                        res = new ConnectionDB("select * from musique order by duree ASC;", "SELECT");
                        headers = new Object[res.getHeaders().length];
                        data = new Object[res.getData().length][res.getNbColonnes()];
                        headers = res.getHeaders();
                        data = res.getData();
                        break;
                    case "Ordre Alphabétique":
                        res = new ConnectionDB("select * from musique order by titre ASC;", "SELECT");
                        headers = new Object[res.getHeaders().length];
                        data = new Object[res.getData().length][res.getNbColonnes()];
                        headers = res.getHeaders();
                        data = res.getData();
                        break;
                    default:
                        res = new ConnectionDB("select * from musique order by annee DESC;", "SELECT");
                        headers = new Object[res.getHeaders().length];
                        data = new Object[res.getData().length][res.getNbColonnes()];
                        headers = res.getHeaders();
                        data = res.getData();
                        break;
                }
                break;
            case "Groupe":
                res = new ConnectionDB("select * from groupe order by nom_groupe ASC;", "SELECT");
                headers = new Object[res.getHeaders().length];
                data = new Object[res.getData().length][res.getNbColonnes()];
                headers = res.getHeaders();
                data = res.getData();
                break;
            case "Artiste":
                res = new ConnectionDB("select * from artiste order by nom_de_scene ASC", "SELECT");
                headers = new Object[res.getHeaders().length];
                data = new Object[res.getData().length][res.getNbColonnes()];
                headers = res.getHeaders();
                data = res.getData();
                break;
            case "Album":
                switch (selectedTri) {
                    case "Annee":
                        res = new ConnectionDB("select * from album order by annee DESC", "SELECT");
                        headers = new Object[res.getHeaders().length];
                        data = new Object[res.getData().length][res.getNbColonnes()];
                        headers = res.getHeaders();
                        data = res.getData();
                        break;
                    case "Ordre Alphabétique":
                        res = new ConnectionDB("select * from album order by nom_album ASC", "SELECT");
                        headers = new Object[res.getHeaders().length];
                        data = new Object[res.getData().length][res.getNbColonnes()];
                        headers = res.getHeaders();
                        data = res.getData();
                        break;
                    default:
                        res = new ConnectionDB("select * from album order by annee DESC", "SELECT");
                        headers = new Object[res.getHeaders().length];
                        data = new Object[res.getData().length][res.getNbColonnes()];
                        headers = res.getHeaders();
                        data = res.getData();
                        break;
                }
                break;
            case "Concert":
                res = new ConnectionDB("select * from concert order by date_ DESC", "SELECT");
                headers = new Object[res.getHeaders().length];
                data = new Object[res.getData().length][res.getNbColonnes()];
                headers = res.getHeaders();
                data = res.getData();
                break;
            case "Genre":
                res = new ConnectionDB("select * from categorie order by genre ASC", "SELECT");
                headers = new Object[res.getHeaders().length];
                data = new Object[res.getData().length][res.getNbColonnes()];
                headers = res.getHeaders();
                data = res.getData();
                break;
            case "Views":
                switch (selectedTri) {
                    case "Groupes qui performent en concert à un tel lieu":
                        res = new ConnectionDB("select * from groupe_lieu_concert", "SELECT");
                        headers = new Object[res.getHeaders().length];
                        data = new Object[res.getData().length][res.getNbColonnes()];
                        headers = res.getHeaders();
                        data = res.getData();
                        break;
                    case "Groupes qui performent en concert à une telle date":
                        res = new ConnectionDB("select * from groupe_date_concert", "SELECT");
                        headers = new Object[res.getHeaders().length];
                        data = new Object[res.getData().length][res.getNbColonnes()];
                        headers = res.getHeaders();
                        data = res.getData();
                        break;
                    case "Chaque artistes ayant participé à une musique":
                        res = new ConnectionDB("select * from artistes_musique", "SELECT");
                        headers = new Object[res.getHeaders().length];
                        data = new Object[res.getData().length][res.getNbColonnes()];
                        headers = res.getHeaders();
                        data = res.getData();
                        break;
                    case "Classement des albums par nombre de vues total":
                        res = new ConnectionDB("select * from vues_album", "SELECT");
                        headers = new Object[res.getHeaders().length];
                        data = new Object[res.getData().length][res.getNbColonnes()];
                        headers = res.getHeaders();
                        data = res.getData();
                        break;
                    case "Musiques par genre":
                        res = new ConnectionDB("select * from musiques_genre", "SELECT");
                        headers = new Object[res.getHeaders().length];
                        data = new Object[res.getData().length][res.getNbColonnes()];
                        headers = res.getHeaders();
                        data = res.getData();
                        break;
                    default:
                        res = new ConnectionDB("select * from groupe_lieu_concert", "SELECT");
                        headers = new Object[res.getHeaders().length];
                        data = new Object[res.getData().length][res.getNbColonnes()];
                        headers = res.getHeaders();
                        data = res.getData();
                        break;
                }
                break;
            default:
                System.out.println("Erreur");
        }

        //ramplace les données du tableau par les nouvelles
        liste = new JTable(data, headers);

        //rend les cellules du tableau non modifiable pour l'utilisateur
        DefaultTableModel tableModel = new DefaultTableModel(data, headers) {
            @Override
            public boolean isCellEditable(int row, int column) {
               return false;
            }
        };

        liste.setModel(tableModel);

        scroll = new JScrollPane(liste);

        bottom.add(liste.getTableHeader(), BorderLayout.NORTH);
        bottom.add(scroll, BorderLayout.CENTER);

        frame.invalidate();
        frame.validate();
        frame.repaint();
    }

    /*fonction non appelée, vérifie si le tableau de données est vide*/

    public Object[][] checkDataVide (Object[][] data, Object[] headers) {
        if (data.length != 0) {
            return data;
        }
        Object[][] res = new Object[1][headers.length];
        for (int i = 0; i < headers.length; i += 1) {
            res[0][i] = "Vide";
        }
        return res;
    }

    /*cherche le plus grand id de musique et le retourne + 1 pour créer une musique */

    public int selectIdMusiqueMax () {
        ConnectionDB c = new ConnectionDB ("SELECT max(id_musique) from musique", "SELECT");
        Object o = c.getData()[0][0];
        if (o == null) {
            return 1;
        }
        int i = (int) o;
        return i + 1;
    }

    /*pareil pour les concerts */

    public int selectIdConcertMax () {
        ConnectionDB c = new ConnectionDB ("SELECT max(id_concert) from concert", "SELECT");
        Object o = c.getData()[0][0];
        if (o == null) {
            return 1;
        }
        int i = (int) o;
        return i + 1;
    }

    private JFrame add; //fenetre qui s'ouvre dans on clique sur ajouter
    private JFrame remove; //pareil pour supprimer

    /*action listener par défaut, changé avant utilisation */
    
    ActionListener boutonValider = new ActionListener () {
        public void actionPerformed (ActionEvent e) {
            
        }
    };

    /*fonction qui crée la fenetre ajouter et cache la précédente, ainsi que gère les options disponibles dans la fenêtre en fonction de l'option de recherche */

    public void fenetreAjouter () {
        frame.setVisible(false);
        add = new JFrame("Ajouter " + selectedOption);
        add.setBounds(50, 50, 300, 600);
        add.setLayout(null);

        JButton valider = new JButton("Valider");
        JButton annuler = new JButton("Annuler");
        valider.setBounds(20, 500, (add.getWidth()/2)-40, 20);
        annuler.setBounds((add.getWidth()/2)+20, 500, (add.getWidth()/2)-40, 20);
        add.add(valider);
        add.add(annuler); //bouton valider pour ajouter, ou annuler pour annuler l'opération

        ActionListener boutonAnnuler = new ActionListener () {
            public void actionPerformed (ActionEvent e) {
                frame.setVisible(true);
                add.setVisible(false);
            }
        }; //action listener pour le bouton annuler

        valider.addActionListener(boutonValider);
        annuler.addActionListener(boutonAnnuler);

        if (selectedOption == "Musique") { //option musique avec ajout des éléments et de l'action listener sur valider en conséquence
            valider.removeActionListener(boutonValider);

            JLabel titre = new JLabel("Titre");
            JLabel annee = new JLabel("Année");
            JLabel nb_vues = new JLabel("Nombres de vues (sur Spotify)");
            JLabel duree = new JLabel("Durée (format mm:ss)");
            JLabel groupe_concepteur = new JLabel("Groupe (artiste seul inclus)");
            JLabel langue = new JLabel("Langue chantée");
            JLabel featuring = new JLabel("Features d'autres groupes (optionnel)");
            JLabel album = new JLabel("Copyright de l'album auquel la musique appartient");
            JLabel genre = new JLabel("Genre principal de la musique");

            JTextArea titreT = new JTextArea();
            JTextArea anneeT = new JTextArea();
            JTextArea nb_vuesT = new JTextArea();
            JTextArea dureeT = new JTextArea();
            JTextArea groupe_concepteurT = new JTextArea();
            JTextArea langueT = new JTextArea();
            JTextArea featuringT = new JTextArea();
            JTextArea albumT = new JTextArea();
            ConnectionDB genres = new ConnectionDB("SELECT genre from categorie", "SELECT");
            Object[] g = new Object[genres.getData().length];
            for (int i = 0; i < genres.getData().length; i += 1) {
                g[i] = genres.getData()[i][0];
            }
            JComboBox<Object> genreT = new JComboBox<Object>(g);

            titre.setBounds(20, 10, add.getWidth()-50, 20);
            titreT.setBounds(20, 35, add.getWidth()-50, 20);
            annee.setBounds(20, 60, add.getWidth()-50, 20);
            anneeT.setBounds(20, 85, add.getWidth()-50, 20);
            nb_vues.setBounds(20, 110, add.getWidth()-50, 20);
            nb_vuesT.setBounds(20, 135, add.getWidth()-50, 20);
            duree.setBounds(20, 160, add.getWidth()-50, 20);
            dureeT.setBounds(20, 185, add.getWidth()-50, 20);
            groupe_concepteur.setBounds(20, 210, add.getWidth()-50, 20);
            groupe_concepteurT.setBounds(20, 235, add.getWidth()-50, 20);
            langue.setBounds(20, 260, add.getWidth()-50, 20);
            langueT.setBounds(20, 285, add.getWidth()-50, 20);
            featuring.setBounds(20, 310, add.getWidth()-50, 20);
            featuringT.setBounds(20, 335, add.getWidth()-50, 20);
            album.setBounds(20, 360, add.getWidth()-50, 20);
            albumT.setBounds(20, 385, add.getWidth()-50, 20);
            genre.setBounds(20, 410, add.getWidth()-50, 20);
            genreT.setBounds(20, 435, add.getWidth()-50, 20);

            add.add(titre);
            add.add(titreT);
            add.add(annee);
            add.add(anneeT);
            add.add(nb_vues);
            add.add(nb_vuesT);
            add.add(duree);
            add.add(dureeT);
            add.add(groupe_concepteur);
            add.add(groupe_concepteurT);
            add.add(langue);
            add.add(langueT);
            add.add(featuring);
            add.add(featuringT);
            add.add(album);
            add.add(albumT);
            add.add(genre);
            add.add(genreT);

            ActionListener boutonValider = new ActionListener () {
                public void actionPerformed (ActionEvent e) {
                    String titreR = titreT.getText();
                    if (titreR == "" || titreR == null) {
                        System.out.println("Mauvais titre");
                        return;
                    }
                    String anneeRe = anneeT.getText();
                    if (anneeRe == "" || anneeRe == null || Integer.valueOf(anneeRe) <= 0 || Integer.valueOf(anneeRe) >= 2023) {
                        System.out.println("Mauvaise année");
                        return;
                    }
                    int anneeR;
                    try {
                        anneeR = Integer.parseInt(anneeRe);
                    } catch (NumberFormatException nfe) {
                        System.out.println("Mauvaise année");
                        return;
                    }
                    String nb_vuesRe = nb_vuesT.getText();
                    if (nb_vuesRe == "" || nb_vuesRe == null) {
                        System.out.println("Mauvais nomnbre de vues");
                        return;
                    }
                    int nb_vuesR;
                    try {
                        nb_vuesR = Integer.parseInt(nb_vuesT.getText());
                    } catch (NumberFormatException nfe) {
                        System.out.println("Mauvais nomnbre de vues");
                        return;
                    }
                    String dureeRe = dureeT.getText();
                    if (dureeRe == "" || dureeRe == null) {
                        System.out.println("Mauvaise durée");
                        return;
                    }
                    DateFormat format = new SimpleDateFormat("mm:ss");
                    java.sql.Time dureeR;
                    try {
                        dureeR = new java.sql.Time(format.parse(dureeRe).getTime());
                    } catch (ParseException pe) {
                        System.out.println("Mauvaise durée");
                        return;
                    }
                    
                    String groupeR = groupe_concepteurT.getText();
                    if (groupeR == null || groupeR == "") {
                        System.out.println("Mauvais nom de groupe");
                        return;
                    }
                    String langueR = langueT.getText();
                    String featuringR = featuringT.getText();
                    String albumR = albumT.getText();
                    if (albumR == "" || albumR == null) {
                        System.out.println("Mauvais nom d'album");
                        return;
                    }
                    String genreR = (String)genreT.getSelectedItem();
                    int id = selectIdMusiqueMax();
                    new ConnectionDB("insert into musique values (" + id + ", \"" + titreR + "\", " + anneeR + ", " + nb_vuesR + ", '" + dureeR + "', \"" + groupeR + "\", \"" + langueR + "\", \"" + featuringR + "\");", "INSERT");
                    new ConnectionDB("insert into composer values(\"" + albumR + "\", " + id + ");", "INSERT");
                    new ConnectionDB("insert into faire_partie values(" + id + ", \"" + genreR + "\");", "INSERT");
                    frame.setVisible(true);
                    add.setVisible(false);
                    add.getContentPane().removeAll();

                    frame.invalidate();
                    frame.validate();
                    frame.repaint();
                }
            };
            valider.addActionListener(boutonValider);
        }
        else if (selectedOption == "Groupe") { //pareil pour groupe
            JLabel nom_groupe = new JLabel("Nom du groupe");
            JLabel nb_membres = new JLabel("Nombre de membres (peut être 1)");
            JLabel origine = new JLabel("Origine (Pays)");
            JTextArea nom_groupeT = new JTextArea();
            JTextArea nb_membresT = new JTextArea();
            JTextArea origineT = new JTextArea();

            nom_groupe.setBounds(20, 10, add.getWidth()-50, 20);
            nom_groupeT.setBounds(20, 35, add.getWidth()-50, 20);
            nb_membres.setBounds(20, 60, add.getWidth()-50, 20);
            nb_membresT.setBounds(20, 85, add.getWidth()-50, 20);
            origine.setBounds(20, 110, add.getWidth()-50, 20);
            origineT.setBounds(20, 135, add.getWidth()-50, 20);

            add.add(nom_groupe);
            add.add(nom_groupeT);
            add.add(nb_membres);
            add.add(nb_membresT);
            add.add(origine);
            add.add(origineT);

            ActionListener boutonValider = new ActionListener () {
                public void actionPerformed (ActionEvent e) {
                    String nom_groupeR = nom_groupeT.getText();
                    if (nom_groupeR == "" || nom_groupeR == null) {
                        System.out.println("Mauvais nom de groupe");
                        return;
                    }
                    String nb_membresRe = nb_membresT.getText();
                    if (nb_membresRe == "" || nb_membresRe == null || Integer.valueOf(nb_membresRe) <= 0 || Integer.valueOf(nb_membresRe) >= 2023) {
                        System.out.println("Mauvaise année");
                        return;
                    }
                    int nb_membresR;
                    try {
                        nb_membresR = Integer.parseInt(nb_membresRe);
                    } catch (NumberFormatException nfe) {
                        System.out.println("Mauvaise année");
                        return;
                    }
                    String origineR = origineT.getText();
                    new ConnectionDB("insert into groupe values (\"" + nom_groupeR + "\", " + nb_membresR + ", \"" + origineR + "\");", "INSERT");
                    frame.setVisible(true);
                    add.setVisible(false);
                    add.getContentPane().removeAll();

                    frame.invalidate();
                    frame.validate();
                    frame.repaint();            
                }
            };
            valider.addActionListener(boutonValider);
        }
        else if (selectedOption == "Artiste") { //pareil pour artiste
            JLabel nom_de_scene = new JLabel("Nom de scène de l'artiste");
            JLabel nom = new JLabel("Nom de l'artiste");
            JLabel prenom = new JLabel("Prénom de l'artiste");
            JLabel age = new JLabel("Age de l'artiste");
            JLabel nom_groupe = new JLabel("Groupe de l'artiste (nom de scène si seul)");
            JLabel domaine = new JLabel("Domaine (ex: chanteur, batteur etc.)");

            JTextArea nom_de_sceneT = new JTextArea();
            JTextArea nomT = new JTextArea();
            JTextArea prenomT = new JTextArea();
            JTextArea ageT = new JTextArea();
            JTextArea nom_groupeT = new JTextArea();
            JTextArea domaineT = new JTextArea();

            nom_de_scene.setBounds(20, 10, add.getWidth()-50, 20);
            nom_de_sceneT.setBounds(20, 35, add.getWidth()-50, 20);
            nom.setBounds(20, 60, add.getWidth()-50, 20);
            nomT.setBounds(20, 85, add.getWidth()-50, 20);
            prenom.setBounds(20, 110, add.getWidth()-50, 20);
            prenomT.setBounds(20, 135, add.getWidth()-50, 20);
            age.setBounds(20, 160, add.getWidth()-50, 20);
            ageT.setBounds(20, 185, add.getWidth()-50, 20);
            nom_groupe.setBounds(20, 210, add.getWidth()-50, 20);
            nom_groupeT.setBounds(20, 235, add.getWidth()-50, 20);
            domaine.setBounds(20, 260, add.getWidth()-50, 20);
            domaineT.setBounds(20, 285, add.getWidth()-50, 20);

            add.add(nom_de_scene);
            add.add(nom_de_sceneT);
            add.add(nom);
            add.add(nomT);
            add.add(prenom);
            add.add(prenomT);
            add.add(age);
            add.add(ageT);
            add.add(nom_groupe);
            add.add(nom_groupeT);
            add.add(domaine);
            add.add(domaineT);

            ActionListener boutonValider = new ActionListener() {
                public void actionPerformed (ActionEvent e) {
                    String nom_de_sceneR = nom_de_sceneT.getText();
                    if (nom_de_sceneR == "" || nom_de_sceneR == null) {
                        System.out.println("Mauvais nom de scène");
                        return;
                    }
                    String nomR = nomT.getText();
                    if (nomR == "" || nomR == null) {
                        System.out.println("Mauvais nom");
                        return;
                    }
                    String prenomR = prenomT.getText();
                    if (prenomR == "" || prenomR == null) {
                        System.out.println("Mauvais prénom");
                        return;
                    }
                    String ageRe = ageT.getText();
                    if (ageRe == "" || ageRe == null || Integer.valueOf(ageRe) <= 0 || Integer.valueOf(ageRe) >= 2023) {
                        System.out.println("Mauvaise année");
                        return;
                    }
                    int ageR;
                    try {
                        ageR = Integer.parseInt(ageRe);
                    } catch (NumberFormatException nfe) {
                        System.out.println("Mauvais âge");
                        return;
                    }
                    String nom_groupeR = nom_groupeT.getText();
                    if (nom_groupeR == "" || nom_groupeR == null) {
                        System.out.println("Mauvais nom de groupe");
                        return;
                    }
                    String domaineR = domaineT.getText();
                    if (domaineR == "" || domaineR == null) {
                        System.out.println("Mauvais domaine");
                        return;
                    }
                    new ConnectionDB("insert into artiste values(\"" + nom_de_sceneR + "\", \"" + nomR + "\", \"" + prenomR + "\", " + ageR + ", \"" + nom_groupeR + "\", \"" + domaineR + "\");", "INSERT");
                    frame.setVisible(true);
                    add.setVisible(false);
                    add.getContentPane().removeAll();

                    frame.invalidate();
                    frame.validate();
                    frame.repaint();
                }
            };
            valider.addActionListener(boutonValider);
        }
        else if (selectedOption == "Album") { //pareil pour album
            JLabel copyright_album = new JLabel("Copyright de l'album");
            JLabel nom_album = new JLabel("Nom de l'album");
            JLabel annee = new JLabel("Année");
            JLabel nom_groupe = new JLabel("Nom du groupe");

            JTextArea copyright_albumT = new JTextArea();
            JTextArea nom_albumT = new JTextArea();
            JTextArea anneeT = new JTextArea();
            JTextArea nom_groupeT = new JTextArea();

            copyright_album.setBounds(20, 10, add.getWidth()-50, 20);
            copyright_albumT.setBounds(20, 35, add.getWidth()-50, 20);
            nom_album.setBounds(20, 60, add.getWidth()-50, 20);
            nom_albumT.setBounds(20, 85, add.getWidth()-50, 20);
            annee.setBounds(20, 110, add.getWidth()-50, 20);
            anneeT.setBounds(20, 135, add.getWidth()-50, 20);
            nom_groupe.setBounds(20, 160, add.getWidth()-50, 20);
            nom_groupeT.setBounds(20, 185, add.getWidth()-50, 20);

            add.add(copyright_album);
            add.add(copyright_albumT);
            add.add(nom_album);
            add.add(nom_albumT);
            add.add(annee);
            add.add(anneeT);
            add.add(nom_groupe);
            add.add(nom_groupeT);

            ActionListener boutonValider = new ActionListener() {
                public void actionPerformed (ActionEvent e) {
                    String copyright_albumR = copyright_albumT.getText();
                    if (copyright_albumR == "" || copyright_albumR == null) {
                        System.out.println("Mauvais copyright");
                        return;
                    }
                    String nom_albumR = nom_albumT.getText();
                    if (nom_albumR == "" || nom_albumR == null) {
                        System.out.println("Mauvais nom d'album");
                        return;
                    }
                    String nom_groupeR = nom_groupeT.getText();
                    if (nom_groupeR == "" || nom_groupeR == null) {
                        System.out.println("Mauvais nom de groupe");
                        return;
                    }
                    String anneeRe = anneeT.getText();
                    if (anneeRe == "" || anneeRe == null || Integer.valueOf(anneeRe) <= 0 || Integer.valueOf(anneeRe) >= 2023) {
                        System.out.println("Mauvaise année");
                        return;
                    }
                    int anneeR;
                    try {
                        anneeR = Integer.parseInt(anneeRe);
                    } catch (NumberFormatException nfe) {
                        System.out.println("Mauvais âge");
                        return;
                    }

                    new ConnectionDB("insert into album values(\"" + copyright_albumR + "\", \"" + nom_albumR + "\", " + anneeR + ", \"" + nom_groupeR + "\");", "INSERT");
                    frame.setVisible(true);
                    add.setVisible(false);
                    add.getContentPane().removeAll();

                    frame.invalidate();
                    frame.validate();
                    frame.repaint();
                }
            };
            valider.addActionListener(boutonValider);
        }
        else if (selectedOption == "Concert") { //pareil pour concert
            JLabel date_ = new JLabel("Date du concert (format YYYY/MM/JJ)");
            JLabel lieu = new JLabel("Lieu du concert");
            JLabel nom_groupe = new JLabel("Groupe produisant le concert");

            JTextArea date_T = new JTextArea();
            JTextArea lieuT = new JTextArea();
            JTextArea nom_groupeT = new JTextArea();

            date_.setBounds(20, 10, add.getWidth()-50, 20);
            date_T.setBounds(20, 35, add.getWidth()-50, 20);
            lieu.setBounds(20, 60, add.getWidth()-50, 20);
            lieuT.setBounds(20, 85, add.getWidth()-50, 20);
            nom_groupe.setBounds(20, 110, add.getWidth()-50, 20);
            nom_groupeT.setBounds(20, 135, add.getWidth()-50, 20);

            add.add(date_);
            add.add(date_T);
            add.add(lieu);
            add.add(lieuT);
            add.add(nom_groupe);
            add.add(nom_groupeT);

            ActionListener boutonValider = new ActionListener () {
                public void actionPerformed (ActionEvent e) {
                    String date_R = date_T.getText();
                    if (date_R == "" || date_R == null) {
                        System.out.println("Mauvaise date");
                        return;
                    }
                    String lieuR = lieuT.getText();
                    if (lieuR == "" || lieuR == null) {
                        System.out.println("Mauvais copyright");
                        return;
                    }
                    String nom_groupeR = nom_groupeT.getText();
                    if (nom_groupeR == "" || nom_groupeR == null) {
                        System.out.println("Mauvais copyright");
                        return;
                    }
                    int id = selectIdConcertMax();
                    new ConnectionDB("insert into concert values(" + id + ", \"" + date_R + "\", \"" + lieuR + "\");", "INSERT");
                    new ConnectionDB("insert into performer values(" + id + ", \"" + nom_groupeR + "\");", "INSERT");
                    frame.setVisible(true);
                    add.setVisible(false);
                    add.getContentPane().removeAll();

                    frame.invalidate();
                    frame.validate();
                    frame.repaint();
                }
            };
            valider.addActionListener(boutonValider);
        }
        else if (selectedOption == "Genre") { //pareil pour genre
            JLabel genre = new JLabel("Genre");
            JLabel description = new JLabel("Description du genre");

            JTextArea genreT = new JTextArea();
            JTextField descriptionT = new JTextField();

            genre.setBounds(20, 10, add.getWidth()-50, 20);
            genreT.setBounds(20, 35, add.getWidth()-50, 20);
            description.setBounds(20, 60, add.getWidth()-50, 20);
            descriptionT.setBounds(20, 85, add.getWidth()-50, 20);

            add.add(genre);
            add.add(genreT);
            add.add(description);
            add.add(descriptionT);

            ActionListener boutonValider = new ActionListener () {
                public void actionPerformed (ActionEvent e) {
                    String genreR = genreT.getText();
                    if (genreR == "" || genreR == null) {
                        System.out.println("Mauvaise date");
                        return;
                    }
                    String descriptionR = descriptionT.getText();
                    if (descriptionR == "" || descriptionR == null) {
                        System.out.println("Mauvaise date");
                        return;
                    }

                    new ConnectionDB("insert into categorie values(\"" + genreR + "\", \"" + descriptionR + "\");", "INSERT");
                    frame.setVisible(true);
                    add.setVisible(false);
                    add.getContentPane().removeAll();

                    frame.invalidate();
                    frame.validate();
                    frame.repaint();
                }
            };
            valider.addActionListener(boutonValider);
        }

        add.setVisible(true);
    }

    /*fenetre supprimer en entier */

    public void fenetreSupprimer () {
        remove = new JFrame("Supprimer" + selectedOption);
        remove.setBounds(50, 50, 300, 150);
        remove.setLayout(null);

        JButton valider = new JButton("Valider");
        JButton annuler = new JButton("Annuler");
        valider.setBounds(20, 80, (remove.getWidth()/2)-40, 20);
        annuler.setBounds((remove.getWidth()/2)+20, 80, (remove.getWidth()/2)-40, 20);
        remove.add(valider);
        remove.add(annuler); //bouton valider et annuler

        ActionListener boutonAnnuler = new ActionListener () {
            public void actionPerformed (ActionEvent e) {
                remove.setVisible(false);
            }
        };

        valider.addActionListener(boutonValider);
        annuler.addActionListener(boutonAnnuler);

        JLabel label = new JLabel("Choisissez quel item supprimer");
        remove.add(label);
        JComboBox<Object> suppr;
        label.setBounds(20, 10, remove.getWidth()-50, 20);

        //choix différents selon l'option choisie
        if (selectedOption == "Musique") {
            valider.removeActionListener(boutonValider);
            ConnectionDB c = new ConnectionDB("select id_musique from musique order by id_musique", "SELECT");
            Object[][] o = c.getData();
            suppr = new JComboBox<Object>();
            for (int i = 0; i < o.length; i += 1) {
                suppr.addItem(o[i][0]);
            }
            suppr.setBounds(20, 35, remove.getWidth()-50, 20);
            remove.add(suppr);

            ActionListener boutonValider = new ActionListener () {
                public void actionPerformed (ActionEvent e) {
                    int del = (int)suppr.getSelectedItem();
                    new ConnectionDB("DELETE from musique where id_musique = " + del + ";", "DELETE");
                    remove.setVisible(false);
                }
            };
            valider.addActionListener(boutonValider);
        }
        else if (selectedOption == "Groupe") {
            valider.removeActionListener(boutonValider);
            ConnectionDB c = new ConnectionDB("select nom_groupe from groupe order by nom_groupe", "SELECT");
            Object[][] o = c.getData();
            suppr = new JComboBox<Object>();
            for (int i = 0; i < o.length; i += 1) {
                suppr.addItem(o[i][0]);
            }
            suppr.setBounds(20, 35, remove.getWidth()-50, 20);
            remove.add(suppr);

            ActionListener boutonValider = new ActionListener () {
                public void actionPerformed (ActionEvent e) {
                    String del = (String)suppr.getSelectedItem();
                    new ConnectionDB("DELETE from groupe where nom_groupe = \"" + del + "\";", "DELETE");
                    remove.setVisible(false);
                }
            };
            valider.addActionListener(boutonValider);
        }
        else if (selectedOption == "Artiste") {
            valider.removeActionListener(boutonValider);
            ConnectionDB c = new ConnectionDB("select nom_de_scene from artiste order by nom_de_scene", "SELECT");
            Object[][] o = c.getData();
            suppr = new JComboBox<Object>();
            for (int i = 0; i < o.length; i += 1) {
                suppr.addItem(o[i][0]);
            }
            suppr.setBounds(20, 35, remove.getWidth()-50, 20);
            remove.add(suppr);

            ActionListener boutonValider = new ActionListener () {
                public void actionPerformed (ActionEvent e) {
                    String del = (String)suppr.getSelectedItem();
                    new ConnectionDB("DELETE from artiste where nom_de_scene = \"" + del + "\";", "DELETE");
                    remove.setVisible(false);
                }
            };
            valider.addActionListener(boutonValider);
        }
        else if (selectedOption == "Album") {
            valider.removeActionListener(boutonValider);
            ConnectionDB c = new ConnectionDB("select copyright_album from album order by copyright_album", "SELECT");
            Object[][] o = c.getData();
            suppr = new JComboBox<Object>();
            for (int i = 0; i < o.length; i += 1) {
                suppr.addItem(o[i][0]);
            }
            suppr.setBounds(20, 35, remove.getWidth()-50, 20);
            remove.add(suppr);

            ActionListener boutonValider = new ActionListener () {
                public void actionPerformed (ActionEvent e) {
                    String del = (String)suppr.getSelectedItem();
                    new ConnectionDB("DELETE from album where copyright_album = \"" + del + "\";", "DELETE");
                    remove.setVisible(false);
                }
            };
            valider.addActionListener(boutonValider);
        }
        else if (selectedOption == "Concert") {
            valider.removeActionListener(boutonValider);
            ConnectionDB c = new ConnectionDB("select id_concert from concert order by id_concert", "SELECT");
            Object[][] o = c.getData();
            suppr = new JComboBox<Object>();
            for (int i = 0; i < o.length; i += 1) {
                suppr.addItem(o[i][0]);
            }
            suppr.setBounds(20, 35, remove.getWidth()-50, 20);
            remove.add(suppr);

            ActionListener boutonValider = new ActionListener () {
                public void actionPerformed (ActionEvent e) {
                    int del = (int)suppr.getSelectedItem();
                    new ConnectionDB("DELETE from concert where id_concert = " + del + ";", "DELETE");
                    remove.setVisible(false);
                }
            };
            valider.addActionListener(boutonValider);
        }
        else if (selectedOption == "Genre") {
            valider.removeActionListener(boutonValider);
            ConnectionDB c = new ConnectionDB("select genre from categorie order by genre", "SELECT");
            Object[][] o = c.getData();
            suppr = new JComboBox<Object>();
            for (int i = 0; i < o.length; i += 1) {
                suppr.addItem(o[i][0]);
            }
            suppr.setBounds(20, 35, remove.getWidth()-50, 20);
            remove.add(suppr);

            ActionListener boutonValider = new ActionListener () {
                public void actionPerformed (ActionEvent e) {
                    String del = (String)suppr.getSelectedItem();
                    new ConnectionDB("DELETE from categorie where genre = \"" + del + "\";", "DELETE");
                    remove.setVisible(false);
                }
            };
            valider.addActionListener(boutonValider);
        }
        remove.setVisible(true);
    }
}