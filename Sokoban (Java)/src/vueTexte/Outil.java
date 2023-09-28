package vueTexte;

import java.io.IOException;

public class Outil {

    /*lit une string et la retourne quand on appuit sur entrer */

    public String lireCaractere(){
        int rep = ' ';
        String buf = "";
        try{
            rep = System.in.read();
            buf += (char) rep;
            while (rep != '\n')
                rep = System.in.read();
                buf += rep;
        } 
        catch (IOException e) {};
        return buf;
    }

}