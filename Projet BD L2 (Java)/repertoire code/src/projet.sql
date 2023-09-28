.open projet.db
pragma foreign_keys = true;
.headers on
.mode column

drop table if exists musique;
drop table if exists groupe;
drop table if exists artiste;
drop table if exists album;
drop table if exists concert;
drop table if exists categorie;
drop table if exists performer;
drop table if exists faire_partie;
drop table if exists composer;

drop view if exists groupe_lieu_concert;
drop view if exists groupe_date_concert;
drop view if exists artistes_musique;
drop view if exists vues_album;

create table musique (
    id_musique int not null primary key check (id_musique >= 0),
    titre varchar[100],
    annee int not null check (annee <= 2022),
    nb_vues int not null check (nb_vues >= 0),
    duree time,
    groupe_concepteur varchar[50] not null references groupe(nom_groupe) on delete cascade,
    langue varchar[30],
    featuring varchar[100]
);

create table groupe (
    nom_groupe varchar[50] primary key,
    nb_membres int not null check (nb_membres > 0),
    origine varchar[30]
);

create table artiste (
    nom_de_scene varchar[50] primary key,
    nom varchar[50],
    prenom varchar[30],
    age int not null check (age > 0),
    nom_groupe varchar[50] not null references groupe(nom_groupe) on delete cascade,
    domaine varchar[50]
);

create table album (
    copyright_album varchar[200] primary key,
    nom_album varchar[50],
    annee int not null check (annee <= 2022),
    nom_groupe varchar[50] not null references groupe(nom_groupe) on delete cascade
);

create table concert (
    id_concert int not null primary key check (id_concert >= 0),
    date_ varchar[30],
    lieu varchar[200]
);

create table categorie (
    genre varchar[20] primary key,
    description varchar[500]
);

create table performer (
    id_concert int not null references concert(id_concert) on delete cascade check (id_concert >= 0),
    nom_groupe varchar[50] not null references groupe(nom_groupe) on delete cascade
);

create table faire_partie (
    id_musique int not null references musique(id_musique) on delete cascade check (id_musique >= 0),
    genre varchar[50] not null references categorie(genre) on delete cascade
);

create table composer (
    copyright_album varchar[200] not null references album(copyright_album) on delete cascade,
    id_musique int not null references musique(id_musique) on delete cascade check (id_musique >= 0)
);

create view groupe_lieu_concert as
    select lieu, nom_groupe 
    from concert natural join performer 
    group by lieu;

create view groupe_date_concert as
    select date_, nom_groupe 
    from concert natural join performer 
    group by date_;

create view artistes_musique as
    select artiste.nom_de_scene, musique.titre
    from artiste natural join musique natural join groupe
    where artiste.nom_groupe = musique.groupe_concepteur
    order by musique.titre ASC;

create view vues_album as
    select nom_album, nom_groupe, sum(nb_vues)
    from musique natural join composer natural join album
    group by nom_album
    order by sum(nb_vues) DESC;

create view musiques_genre as
    select titre, genre 
    from faire_partie natural join musique
    group by id_musique
    order by genre ASC;

create trigger etoile_musique
    after insert on musique
    begin
        update musique set titre = new.titre || "*"
        where id_musique=new.id_musique and new.nb_vues>=100000000;
    end;

create trigger langue_musique
    after insert on musique
    begin 
        update musique set langue = "Langue non spécifiée ou absence de parole"
        where id_musique = new.id_musique and (new.langue is null or new.langue = "");
    end;

create trigger featuring_musique
    after insert on musique
    begin 
        update musique set featuring = "Pas de featuring"
        where id_musique = new.id_musique and (new.featuring is null or new.featuring = "");
    end;

create trigger origine_groupe
    after insert on groupe
    begin
        update groupe set origine = "Origine non spécifiée"
        where origine = new.origine and (new.origine is null or new.origine = "");
    end;

insert into groupe values ("Radiohead", 5, "Angleterre");
insert into groupe values ("Nekfeu", 1, "France");
insert into groupe values ("Daft Punk", 2, "France");

insert into album values ("1997 XL Recordings Ltd", "OK Computer", 1997, "Radiohead");
insert into album values ("1993 XL Recordings Ltd", "Pablo Honey", 1993, "Radiohead");
insert into album values ("1995 XL Recordings Ltd", "The Bends", 1995, "Radiohead");
insert into album values ("2019 Seine Zoo, Polydor (France)", "Les étoiles vagabondes", 2019, "Nekfeu");
insert into album values ("Warner Music France / ADA France, 2001 Daft Life Ltd", "Discovery", 2001, "Daft Punk");

insert into artiste(nom_de_scene, nom, prenom, age, nom_groupe, domaine) values ("Thom Yorke", "Yorke", "Thomas", 53, "Radiohead", "Chanteur / guitariste");
insert into artiste(nom_de_scene, nom, prenom, age, nom_groupe, domaine) values ("Jonny Greenwood", "Greenwood", "Jonathan", 50, "Radiohead", "Guitariste / pianiste");
insert into artiste(nom_de_scene, nom, prenom, age, nom_groupe, domaine) values ("Colin Greenwood", "Greenwood", "Colin", 52, "Radiohead", "Bassiste");
insert into artiste(nom_de_scene, nom, prenom, age, nom_groupe, domaine) values ("Ed O'Brien", "O'Brien", "Edward", 53, "Radiohead", "Guitariste");
insert into artiste(nom_de_scene, nom, prenom, age, nom_groupe, domaine) values ("Phil Selway", "Selway", "Philip", 54, "Radiohead", "Batteur");
insert into artiste(nom_de_scene, nom, prenom, age, nom_groupe, domaine) values ("Nekfeu", "Samaras", "Ken", 31, "Nekfeu", "Chanteur");
insert into artiste(nom_de_scene, nom, prenom, age, nom_groupe, domaine) values ("Thomas Bangalter", "Bangalter", "Thomas", 47, "Daft Punk", "Compositeur");
insert into artiste(nom_de_scene, nom, prenom, age, nom_groupe, domaine) values ("Guillaume Emmanuel De Homem-Christo", "De Homem-Christo", "Guillaume Emmanuel", 48, "Daft Punk", "Compositeur");

insert into concert values (1, "2017/07/07", "Glasgow");
insert into concert values (2, "2017/07/02", "Arras");
insert into concert values (3, "2017/06/16", "Milan");
insert into concert values (4, "2021/07/09", "Nîmes");
insert into concert values (5, "2020/07/03", "Arras");
insert into concert values (6, "2020/03/06", "Réunion");
insert into concert values (7, "2010/10/20", "New-York City");
insert into concert values (8, "2009/06/30", "Berlin");

insert into performer values (1, "Radiohead");
insert into performer values (2, "Radiohead");
insert into performer values (3, "Radiohead");
insert into performer values (4, "Nekfeu");
insert into performer values (5, "Nekfeu");
insert into performer values (6, "Nekfeu");
insert into performer values (7, "Daft Punk");
insert into performer values (8, "Daft Punk");

insert into categorie values ("Rock Alternatif", "Le rock alternatif est un genre musical, à base de guitare, basse et batterie, qui est apparu dans les années 1980. Il trouve ses origines dans le Post-punk, la new wave et le Garage rock. Le qualificatif 'alternatif' vient du fait que beaucoup de groupes ont dû faire appel à des labels indépendants.");
insert into categorie values ("Rap français", "Le hip-hop français, ou rap français, est un courant musical du Hip-hop ayant émergé en France au milieu des années 1980 principalement diffusé sur des radios pirates. Tout en restant continuellement inspiré par les rappeurs d'outre-Atlantique, il a su acquérir une personnalité propre, oscillant entre revendications sociopolitiques, messages positifs ou festifs et tentation commerciale.");
insert into categorie values ("Électronique 'French Touch'", "La 'French Touch' ou 'French house', est un courant musical né en France en 1990, reconnu internationalement comme la déclinaison française de la musique house. Si, au départ, elle s'inscrivait dans cette dernière par l'utilisation de samples souvent puisés dans le funk et le disco, au fil des années, ses sources d'inspiration se sont étendues à tous les styles musicaux.");

insert into musique (id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (1, "Airbag", 1997, 45769230, '00:04:47', "Radiohead", "Anglais");
insert into musique (id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (2, "Paranoid Android", 1997, 138823786, '00:06:27', "Radiohead", "Anglais");
insert into musique (id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (3, "Subterranean Homesick Alien", 1997, 29580310, '00:04:27', "Radiohead", "Anglais");
insert into musique (id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (4, "Exit Music (for a film)", 1997, 102680768, '00:04:27', "Radiohead", "Anglais");
insert into musique (id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (5, "Let Down", 1997, 58070002, '00:04:59', "Radiohead", "Anglais");
insert into musique (id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (6, "Karma Police", 1997, 319964935, '00:04:29', "Radiohead", "Anglais");
insert into musique (id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (7, "Fitter Happier", 1997, 18367436, '00:01:57', "Radiohead", "Anglais");
insert into musique (id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (8, "Electioneering", 1997, 23389136, '00:03:50', "Radiohead", "Anglais");
insert into musique (id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (9, "Climbing Up the Walls", 1997, 28651626, '00:04:45', "Radiohead", "Anglais");
insert into musique (id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (10, "No Surprises", 1997, 232728126, '00:03:49', "Radiohead", "Anglais");
insert into musique (id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (11, "Lucky", 1997, 39271244, '00:04:18', "Radiohead", "Anglais");
insert into musique (id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (12, "The Tourist", 1997, 23871090, '00:05:26', "Radiohead", "Anglais");

insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (13, "You", 1993, 14102625, '00:03:28', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (14, "Creep", 1993, 865984376, '00:03:58', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (15, "How Do You?", 1993, 7419542, '00:02:12', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (16, "Stop Whispering", 1993, 7364772, '00:05:25', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (17, "Thinking About You", 1993, 11973539, '00:02:41', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (18, "Anyone Can Play Guitar", 1993, 10877846, '00:03:37', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (19, "Ripcord", 1993, 5035951, '00:03:09', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (20, "Vegetable", 1993, 5133506, '00:03:12', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (21, "Prove Yourself", 1993, 4751425, '00:02:25', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (22, "I Can't", 1993, 4596455, '00:04:13', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (23, "Lurgee", 1993, 4681488, '00:03:07', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (24, "Blow Out", 1993, 6262785, '00:04:42', "Radiohead", "Anglais");

insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (25, "Planet Telex", 1995, 24976475, '00:04:19', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (26, "The Bends", 1995, 25693206, '00:04:06', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (27, "High and Dry", 1995, 236607366, '00:04:17', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (28, "Fake plastic trees", 1995, 185704525, '00:04:50', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (29, "Bones", 1995, 14995817, '00:03:09', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (30, "(Nice dream)", 1995, 22034948, '00:03:53', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (31, "Just", 1995, 60124412, '00:03:54', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (32, "My Iron Lung", 1995, 33038836, '00:04:36', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (33, "Bullet Proof… I wish i was", 1995, 18104825, '00:03:27', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (34, "Black Star", 1995, 22417027, '00:04:07', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (35, "Sulk", 1995, 13965276, '00:03:42', "Radiohead", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (36, "Street Spirit (Fade out)", 1995, 63845672, '00:04:13', "Radiohead", "Anglais");

insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (37, "Les étoiles vagabondes", 2019, 32942399, '00:03:54', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (38, "Alunissons", 2019, 33363666, '00:04:38', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (39, "Cheum", 2019, 45769230, '00:03:16', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (40, "Natsukashii", 2019, 17714064, '00:03:11', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (41, "Takotsubo", 2019, 24049736, '00:04:44', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (42, "Menteur menteur", 2019, 24482957, '00:03:45', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (43, "Ecrire", 2019, 25843890, '00:03:41', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (44, "Ciel noir", 2019, 21784406, '00:04:13', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue, featuring) values (45, "L'air du temps", 2019, 14811572, '00:02:51', "Nekfeu", "Français", "FRAMAL, Doums");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (46, "De mon mieux", 2019, 20850229, '00:03:48', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (47, "Le bruit qui court", 2019, 23302529, '00:04:00', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue, featuring) values (48, "Elle pleut", 2019, 61900741, '00:04:46', "Nekfeu", "Français", "Nemir");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue, featuring) values (49, "Dans l'univers", 2019, 75671120, '00:04:04', "Nekfeu", "Français", "Vanessa Paradis");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (50, "Premier pas", 2019, 26182272, '00:02:33', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (51, "Dernier soupir", 2019, 12561914, '00:03:33', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue, featuring) values (52, "Tricheur", 2019, 84649652, '00:04:08', "Nekfeu", "Français", "Damso");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (53, "Voyage léger", 2019, 25181951, '00:03:22', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (54, "Interlude Fifty", 2019, 9903642, '00:01:31', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue, featuring) values (55, "Compte les hommes", 2019, 17555933, '00:04:10', "Nekfeu", "Français", "Alpha Wann");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (56, "Oui et non", 2019, 14287878, '00:03:05', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (57, "Koala mouillé", 2019, 14681335, '00:03:00', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (58, "Jeux vidéos et débats", 2019, 11300963, '00:04:13', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (59, "Ken Kaneki", 2019, 28897074, '00:02:42', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue, featuring) values (60, "CDGLAXJFKHNDATH", 2019, 13772095, '00:03:23', "Nekfeu", "Français", "2zer, Mekra");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue, featuring) values (61, "Rouge à lèvres", 2019, 12962195, '00:03:48', "Nekfeu", "Français", "BJ The Chicago Kid");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (62, "Sous les nuages", 2019, 19545318, '00:02:40', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (63, "Energie sombre", 2019, 21488886, '00:03:24', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (64, "Oya Kaya", 2019, 22277396, '00:05:03', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue, featuring) values (65, "Pixels", 2019, 15132610, '00:03:53', "Nekfeu", "Français", "Crystal Kay");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (66, "De mes cendres", 2019, 15759247, '00:03:36', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (67, "Nouvel homme", 2019, 11293635, '00:03:09', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (68, "Chanson d'amour", 2019, 21841800, '00:03:42', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (69, "1er rôle", 2019, 15475933, '00:04:25', "Nekfeu", "Français");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (70, "A la base", 2019, 12096309, '00:04:26', "Nekfeu", "Français");

insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (71, "One More Time", 2001, 394590187, '00:05:20', "Daft Punk", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur) values (72, "Aerodynamic", 2001, 50048005, '00:03:32', "Daft Punk");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (73, "Digital Love", 2001, 98652886, '00:05:01', "Daft Punk", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (74, "Harder, Better, Faster, Stronger", 2001, 252344570, '00:03:44', "Daft Punk", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (75, "Crescendolls", 2001, 25937016, '00:03:31', "Daft Punk", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur) values (76, "Nightvision", 2001, 20466489, '00:01:44', "Daft Punk");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (77, "Superheroes", 2001, 23603689, '00:03:57', "Daft Punk", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (78, "High Life", 2001, 22285823, '00:03:21', "Daft Punk", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (79, "Something About Us", 2001, 156852078, '00:03:52', "Daft Punk", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur) values (80, "Voyager", 2001, 50311547, '00:03:47', "Daft Punk");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur) values (81, "Veridis Quo", 2001, 70092569, '00:05:45', "Daft Punk");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur) values (82, "Short Circuit", 2001, 16889785, '00:03:27', "Daft Punk");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (83, "Face to Face", 2001, 50087430, '00:04:00', "Daft Punk", "Anglais");
insert into musique(id_musique, titre, annee, nb_vues, duree, groupe_concepteur, langue) values (84, "Too Long", 2001, 18997019, '00:10:00', "Daft Punk", "Anglais");

insert into composer values ("1997 XL Recordings Ltd", 1);
insert into composer values ("1997 XL Recordings Ltd", 2);
insert into composer values ("1997 XL Recordings Ltd", 3);
insert into composer values ("1997 XL Recordings Ltd", 4);
insert into composer values ("1997 XL Recordings Ltd", 5);
insert into composer values ("1997 XL Recordings Ltd", 6);
insert into composer values ("1997 XL Recordings Ltd", 7);
insert into composer values ("1997 XL Recordings Ltd", 8);
insert into composer values ("1997 XL Recordings Ltd", 9);
insert into composer values ("1997 XL Recordings Ltd", 10);
insert into composer values ("1997 XL Recordings Ltd", 11);
insert into composer values ("1997 XL Recordings Ltd", 12);

insert into faire_partie values (1, "Rock Alternatif");
insert into faire_partie values (2, "Rock Alternatif");
insert into faire_partie values (3, "Rock Alternatif");
insert into faire_partie values (4, "Rock Alternatif");
insert into faire_partie values (5, "Rock Alternatif");
insert into faire_partie values (6, "Rock Alternatif");
insert into faire_partie values (7, "Rock Alternatif");
insert into faire_partie values (8, "Rock Alternatif");
insert into faire_partie values (9, "Rock Alternatif");
insert into faire_partie values (10, "Rock Alternatif");
insert into faire_partie values (11, "Rock Alternatif");
insert into faire_partie values (12, "Rock Alternatif");

insert into composer values ("1993 XL Recordings Ltd", 13);
insert into composer values ("1993 XL Recordings Ltd", 14);
insert into composer values ("1993 XL Recordings Ltd", 15);
insert into composer values ("1993 XL Recordings Ltd", 16);
insert into composer values ("1993 XL Recordings Ltd", 17);
insert into composer values ("1993 XL Recordings Ltd", 18);
insert into composer values ("1993 XL Recordings Ltd", 19);
insert into composer values ("1993 XL Recordings Ltd", 20);
insert into composer values ("1993 XL Recordings Ltd", 22);
insert into composer values ("1993 XL Recordings Ltd", 23);
insert into composer values ("1993 XL Recordings Ltd", 24);
insert into composer values ("1993 XL Recordings Ltd", 25);

insert into faire_partie values (13, "Rock Alternatif");
insert into faire_partie values (14, "Rock Alternatif");
insert into faire_partie values (15, "Rock Alternatif");
insert into faire_partie values (16, "Rock Alternatif");
insert into faire_partie values (17, "Rock Alternatif");
insert into faire_partie values (18, "Rock Alternatif");
insert into faire_partie values (19, "Rock Alternatif");
insert into faire_partie values (20, "Rock Alternatif");
insert into faire_partie values (21, "Rock Alternatif");
insert into faire_partie values (22, "Rock Alternatif");
insert into faire_partie values (23, "Rock Alternatif");
insert into faire_partie values (24, "Rock Alternatif");

insert into composer values ("1995 XL Recordings Ltd", 25);
insert into composer values ("1995 XL Recordings Ltd", 26);
insert into composer values ("1995 XL Recordings Ltd", 27);
insert into composer values ("1995 XL Recordings Ltd", 28);
insert into composer values ("1995 XL Recordings Ltd", 29);
insert into composer values ("1995 XL Recordings Ltd", 30);
insert into composer values ("1995 XL Recordings Ltd", 31);
insert into composer values ("1995 XL Recordings Ltd", 32);
insert into composer values ("1995 XL Recordings Ltd", 33);
insert into composer values ("1995 XL Recordings Ltd", 34);
insert into composer values ("1995 XL Recordings Ltd", 35);
insert into composer values ("1995 XL Recordings Ltd", 36);

insert into faire_partie values (25, "Rock Alternatif");
insert into faire_partie values (26, "Rock Alternatif");
insert into faire_partie values (27, "Rock Alternatif");
insert into faire_partie values (28, "Rock Alternatif");
insert into faire_partie values (29, "Rock Alternatif");
insert into faire_partie values (30, "Rock Alternatif");
insert into faire_partie values (31, "Rock Alternatif");
insert into faire_partie values (32, "Rock Alternatif");
insert into faire_partie values (33, "Rock Alternatif");
insert into faire_partie values (34, "Rock Alternatif");
insert into faire_partie values (35, "Rock Alternatif");
insert into faire_partie values (36, "Rock Alternatif");

insert into composer values ("2019 Seine Zoo, Polydor (France)", 37);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 38);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 39);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 40);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 41);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 42);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 43);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 44);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 45);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 46);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 47);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 48);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 49);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 50);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 51);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 52);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 53);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 54);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 55);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 56);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 57);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 58);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 59);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 60);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 61);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 62);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 63);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 64);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 65);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 66);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 67);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 68);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 69);
insert into composer values ("2019 Seine Zoo, Polydor (France)", 70);

insert into faire_partie values (37, "Rap français");
insert into faire_partie values (38, "Rap français");
insert into faire_partie values (39, "Rap français");
insert into faire_partie values (40, "Rap français");
insert into faire_partie values (41, "Rap français");
insert into faire_partie values (42, "Rap français");
insert into faire_partie values (43, "Rap français");
insert into faire_partie values (44, "Rap français");
insert into faire_partie values (45, "Rap français");
insert into faire_partie values (46, "Rap français");
insert into faire_partie values (47, "Rap français");
insert into faire_partie values (48, "Rap français");
insert into faire_partie values (49, "Rap français");
insert into faire_partie values (50, "Rap français");
insert into faire_partie values (51, "Rap français");
insert into faire_partie values (52, "Rap français");
insert into faire_partie values (53, "Rap français");
insert into faire_partie values (54, "Rap français");
insert into faire_partie values (55, "Rap français");
insert into faire_partie values (56, "Rap français");
insert into faire_partie values (57, "Rap français");
insert into faire_partie values (58, "Rap français");
insert into faire_partie values (59, "Rap français");
insert into faire_partie values (60, "Rap français");
insert into faire_partie values (61, "Rap français");
insert into faire_partie values (62, "Rap français");
insert into faire_partie values (63, "Rap français");
insert into faire_partie values (64, "Rap français");
insert into faire_partie values (65, "Rap français");
insert into faire_partie values (66, "Rap français");
insert into faire_partie values (67, "Rap français");
insert into faire_partie values (68, "Rap français");
insert into faire_partie values (69, "Rap français");
insert into faire_partie values (70, "Rap français");

insert into composer values ("Warner Music France / ADA France, 2001 Daft Life Ltd", 71);
insert into composer values ("Warner Music France / ADA France, 2001 Daft Life Ltd", 72);
insert into composer values ("Warner Music France / ADA France, 2001 Daft Life Ltd", 73);
insert into composer values ("Warner Music France / ADA France, 2001 Daft Life Ltd", 74);
insert into composer values ("Warner Music France / ADA France, 2001 Daft Life Ltd", 75);
insert into composer values ("Warner Music France / ADA France, 2001 Daft Life Ltd", 76);
insert into composer values ("Warner Music France / ADA France, 2001 Daft Life Ltd", 77);
insert into composer values ("Warner Music France / ADA France, 2001 Daft Life Ltd", 78);
insert into composer values ("Warner Music France / ADA France, 2001 Daft Life Ltd", 79);
insert into composer values ("Warner Music France / ADA France, 2001 Daft Life Ltd", 80);
insert into composer values ("Warner Music France / ADA France, 2001 Daft Life Ltd", 81);
insert into composer values ("Warner Music France / ADA France, 2001 Daft Life Ltd", 82);
insert into composer values ("Warner Music France / ADA France, 2001 Daft Life Ltd", 83);
insert into composer values ("Warner Music France / ADA France, 2001 Daft Life Ltd", 84);

insert into faire_partie values (71, "Électronique 'French Touch'");
insert into faire_partie values (72, "Électronique 'French Touch'");
insert into faire_partie values (73, "Électronique 'French Touch'");
insert into faire_partie values (74, "Électronique 'French Touch'");
insert into faire_partie values (75, "Électronique 'French Touch'");
insert into faire_partie values (76, "Électronique 'French Touch'");
insert into faire_partie values (77, "Électronique 'French Touch'");
insert into faire_partie values (78, "Électronique 'French Touch'");
insert into faire_partie values (79, "Électronique 'French Touch'");
insert into faire_partie values (80, "Électronique 'French Touch'");
insert into faire_partie values (81, "Électronique 'French Touch'");
insert into faire_partie values (82, "Électronique 'French Touch'");
insert into faire_partie values (83, "Électronique 'French Touch'");
insert into faire_partie values (84, "Électronique 'French Touch'");