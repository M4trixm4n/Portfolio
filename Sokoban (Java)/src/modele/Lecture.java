package modele;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class Lecture {
    private String name;
    private List<String> lignes;
    private int nb_lignes = 0;
    private int taille_lignes;

    public Lecture (String name) {
        this.name = name;
        this.lignes = new ArrayList<String>();
        this.readFile();
    }

    /*get le texte d'un .txt et le mettre dans une liste de String */

    public void readFile () {
        BufferedReader reader;
		try {
			reader = new BufferedReader(new FileReader(name));
			String line = reader.readLine();
            taille_lignes = line.length();
			while (line != null) {
                nb_lignes += 1;
                lignes.add(line);
				line = reader.readLine();
			}
			reader.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

    public java.util.List<String> getLignes () {
        return lignes;
    }

    public int getTaille () {
        return taille_lignes;
    }

    public int getNbLignes () {
        return nb_lignes;
    }
}
