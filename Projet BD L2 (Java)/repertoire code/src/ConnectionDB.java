package src;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class ConnectionDB {

    //Connection, statement et resultset globaux
    private Connection c = null;
    private Statement stmt = null;
    private ResultSet rs;
    private ResultSetMetaData rsmd;

    //objets à créer selon la requête (seulement en select)
    Object[] names;
    Object[][] data;

    /*constructeur, prend en entrée la requête en sql (string) et l'action à effectuer : SELECT, INSERT ou DELETE (en string) */

    public ConnectionDB (String query, String action) {
        if (action == "SELECT") {
            this.requestSelect(query);
        }
        if (action == "INSERT") {
            this.requestInsert(query);
        }
        if (action == "DELETE") {
            this.requestDelete(query);
        }
    }

    private int nb_col; //nombre de colonnes sélectionnées (pour select)

    /*si action = select */

    public void requestSelect (String query) {
        try {
            String url = "jdbc:sqlite:./bin/projet.db";
            c = DriverManager.getConnection(url);
            stmt = c.createStatement();
            rs = stmt.executeQuery(query);
            rsmd = rs.getMetaData(); //faire la requête et récupérer les données

            nb_col = rsmd.getColumnCount();

            //remplir les tableau à return avec les données
            names = new Object[nb_col];
            for (int i = 1; i <= nb_col; i += 1) {
                names[i-1] = rsmd.getColumnName(i);
            }
            List<Object[]> dataTemp = new ArrayList<>();
            while (rs.next()) {
                Object[] ligne = new Object[nb_col];
                for (int i = 1; i <= nb_col; i += 1) {
                    switch (rsmd.getColumnType(i)) {
                        case java.sql.Types.INTEGER:
                            ligne[i-1] = (rs.getInt((String)names[i-1]));
                            break;
                        case java.sql.Types.VARCHAR:
                            ligne[i-1] = (rs.getString((String)names[i-1]));
                            break;
                        case java.sql.Types.DATE:
                            ligne[i-1] = (rs.getDate((String)names[i-1]));
                            break;
                        case java.sql.Types.TIME:
                            ligne[i-1] = (rs.getTime((String)names[i-1]));
                            break;
                    }
                }
                dataTemp.add(ligne);
            }
            data = new Object[dataTemp.size()][nb_col];
            for (int i = 0; i < dataTemp.size(); i += 1) {
                data[i] = dataTemp.get(i);
            }
        } catch (SQLException e) {
            throw new Error("Problem", e);
        }
    }

    /*getters */

    public ResultSet getResultSet () {
        return rs;
    }

    public Object[] getHeaders () {
        return this.names;
    }

    public Object[][] getData () {
        return data;
    }

    public int getNbColonnes () {
        return nb_col;
    }

    /* action = insert */

    public void requestInsert (String query) {
        try {
            String url = "jdbc:sqlite:./bin/projet.db";
            c = DriverManager.getConnection(url);
            stmt = c.createStatement();
            stmt.executeUpdate(query); //executer la requête
            
        } catch (SQLException e) {
            throw new Error("Problem", e);
        }
    }

    /* action = delete */

    public void requestDelete (String query) {
        try {
            String url = "jdbc:sqlite:./bin/projet.db";
            c = DriverManager.getConnection(url);
            stmt = c.createStatement();
            stmt.executeUpdate(query); //executer la requête
            
        } catch (SQLException e) {
            throw new Error("Problem", e);
        }
    }
}
