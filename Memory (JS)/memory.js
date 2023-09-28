/* Constructeur Tile*/

function Tile(id, isRev=false) {
    this.id = id;
    Object.defineProperty(this, 'isRevealed', {
        value: isRev,
        writable: true,
        enumerable: false,
        configurable: false
    });
    Object.defineProperty(this, 'x', {
        value: -1,
        writable: true,
        enumerable: false,
        configurable: false
    });
    Object.defineProperty(this, 'y', {
        value: -1,
        writable: true,
        enumerable: false,
        configurable: false
    });
}

/*Prototypes Tile*/

Tile.prototype = {
    setCoord: function(x, y) {
        this.x = x;
        this.y = y;
    },
    display: function() {
        var idxy = this.x*5 + this.y;
        var id_temp = this.id
        var img = document.getElementById(idxy);
        if (this.isRevealed == false) {
            img.src = "img0.jpg";
            img.style.borderColor = "yellow";
        }
        else {
            img.src = "img" + this.id.toString() + ".jpg";
            img.style.borderColor = "blue";
        }
    },
    flipTile: function() {
        if (this.isRevealed) {
            this.isRevealed = false;
        }
        else {
            this.isRevealed = true;
        }
        this.display();
    },
}

/*Constructeur Board*/

function Board(tiles) {
    this.tiles = tiles;
}

/*Prototypes Board*/

Board.prototype = {
    display: function () {
        this.tiles.forEach(display());
    },
    reset: function () {
        retournerTuiles();
        clearTimeout(timeout);
        tournees = [];
        tours = 0;
        var t = creation_tiles();
        b = new Board(t);
        add_images_event();
        retirerMessage();
        victoire = false;
        defaite = false;
        correctes = [];
        timerOn = false;
        nb_correctes = 0;
        retirerBoutonRejouer();
        if (montre == true) {
            var timer = document.getElementById("timer");
            timer.firstChild.nodeValue = "2:00";
        }
        if (duel == true) {
            deux_joueurs();
        }
        if (ordi == true) {
            contre_ordinateur();
        }
    }
};

/*Création de toutes les tuile avec un id random*/

function creation_tiles () {
    var tiles = [];
    var ids = [1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13];
    var xy = [[0,0],[0,1],[0,2],[0,3],[0,4],[1,0],[1,1],[1,2],[1,3],[1,4],[2,0],[2,1],[2,2],[2,3],[2,4],[3,0],[3,1],[3,2],[3,3],[3,4],[4,0],[4,1],[4,2],[4,3],[4,4]];
    for (var i = 0; i < 25; i += 1) {
        let rand = Math.floor(Math.random() * ids.length);
        let tmp = ids.splice(rand,1);
        let t = new Tile(tmp[0]);
        t.setCoord(xy[0][0], xy[0][1]);
        xy.shift();
        tiles[i] = t;
    }
    return tiles;
}

/*retourne les tuiles révélées pour montrer le dos de carte*/

function retournerTuiles () {
    var i;
    for (i = 0; i < b.tiles.length; i += 1) {
        if (b.tiles[i].isRevealed == true) {
            b.tiles[i].flipTile();
        }
    }
}

/*Après avoir cliqué sur une tuile avec un event, remove l'event, retourne la tuile, regarde si vous en avez retourné 2, si oui, passe à verif()*/

function retourner () {
    if (montre == true && timerOn == false) {
        activeTimer();
    }
    nb_tournees += 1;
    var id_img = this.id;
    b.tiles[id_img].flipTile();
    tournees[tournees.length] = b.tiles[id_img];
    this.removeEventListener("click", retourner);
    if (nb_tournees == 2) {
        verif();
    }
}

/*Regarde si les deux tuiles retournées ont la même image, si oui, retient les emplacements des images, sinon,
retire les event de chaque tuile pour bloquer le jeu, attend  2 sec, puis retourne les 2 fausses tuiles et remet les event de chaque tuile*/

var timeout;

function verif() {
    nb_tournees = 0;
    tours += 1;
    console.log(tournees);
    var idxy1 = tournees[0].x*5 + tournees[0].y;
    var idxy2 = tournees[1].x*5 + tournees[1].y;
    if (tournees[0].id != tournees[1].id) {
        removeActiveEvents();
        timeout = setTimeout(function () {
            tournees[0].flipTile();
            tournees[1].flipTile();
            ima[idxy1].addEventListener("click", retourner);
            ima[idxy2].addEventListener("click", retourner);
            if (ordi == true) {
                memoireOrdi[idxy1] = tournees[0].id;
                memoireOrdi[idxy2] = tournees[1].id;
            }
            tournees = [];
            addActiveEvents();
        }, 2000);
        if (duel == true || ordi == true) {
            changerTourJoueur(false);
        }
    }
    else {
        nb_correctes += 2;
        tournees = [];
        correctes[correctes.length] = idxy1;
        correctes[correctes.length] = idxy2;
        if (nb_correctes == 24 && montre == true) {
            gagne();
        }
        if (duel == true) {
            changerTourJoueur(true);
        }
        if (ordi == true) {
            memoireOrdi[idxy1] = 0;
            memoireOrdi[idxy2] = 0;
            changerTourJoueur(true);
        }
    }
}

/*vérifie si un entier appartient à une liste*/

function appartient_liste (entier, liste) {
    for (var i = 0; i < liste.length; i += 1) {
        if (entier == liste[i]) {
            return true;
        }
    }
    return false;
}

/*retire les event de toutes les tuiles toujours pas trouvées*/

function removeActiveEvents () {
    for (var i = 0; i < ima.length; i += 1) {
        if (!(appartient_liste(i, correctes))) {
            ima[i].removeEventListener("click", retourner);
        }
    }
}

/*remet les event de toutes les tuiles pas encore trouvées*/

function addActiveEvents () {
    for (var i = 0; i < ima.length; i += 1) {
        if (!(appartient_liste(i, correctes))) {
            ima[i].addEventListener("click", retourner);
        }
    }
}

/*met les event sur chaque tuile apres avoir retiré un event potentiellement existant deja (reset)*/

function add_images_event() {
    for (var i = 0; i < ima.length; i += 1) {
        ima[i].removeEventListener("click", retourner);
        ima[i].addEventListener("click", retourner);
    }
}

/*met les event sur les boutons radio de mode de jeu, déclenchés avec un changement (clic)*/

function addEventRadio () {
    var radios = document.getElementsByClassName("rad");
    for (var i = 0; i < radios.length; i += 1) {
        radios[i].addEventListener("change", changerModeJeu);
    }
}

/*change le mode de jeu et le css des boutons pour montre l'option sélectionnée*/

function changerModeJeu () {
    var label;
    var autres = [];
    tours = 0;
    if (this.id == "rad1") {
        autres[autres.length] = document.getElementById("r2");
        autres[autres.length] = document.getElementById("r3");
        contre_la_montre();
        deux_joueurs_invis();
        effacerAffichageTourJoueur();
        b.reset();
    }
    else if (this.id == "rad2") {
        autres[autres.length] = document.getElementById("r1");
        autres[autres.length] = document.getElementById("r3");
        deux_joueurs();
        b.reset();
    }
    else if (this.id == "rad3") {
        autres[autres.length] = document.getElementById("r2");
        autres[autres.length] = document.getElementById("r1");
        contre_ordinateur();
        b.reset();
    }
    for (var i = 0; i < 2; i += 1) {
        autres[i].style.transform = "";
        autres[i].style.borderBottom = "10px solid #696969";
        autres[i].style.fontWeight = "normal";
    }
}

/*affiche un timer pour le mode de jeu contre la montre*/

function timer() {
    label = document.getElementById("timer");
    label.style.backgroundColor = "#A9A9A9";
    label.style.borderBottom = "5px solid black";
    label.style.borderTop = "5px solid black";
    label.firstChild.nodeValue = "2:00";
}

/*supprime le timer*/

function no_timer() {
    label = document.getElementById("timer");
    label.style.backgroundColor = "";
    label.style.borderBottom = "";
    label.style.borderTop = "";
    label.firstChild.nodeValue = " ";
    clearInterval(intervalTimer);
    label.firstChild.nodeValue = " ";
}

/*mode de jeu contre la montre*/

function contre_la_montre () {
    var label;
    montre = true;
    duel = false;
    ordi = false;
    timerOn = false;
    label = document.getElementById("r1");
    label.style.transform = "translate(0px,5px)";
    label.style.borderBottom = "3px solid";
    label.style.fontWeight = "bold";
    timer();
}

/*mode de jeu 2 joueurs*/

var pointsJ1;
var pointsJ2;

function deux_joueurs () {
    var label;
    pointsJ1 = 0;
    pointsJ2 = 0;
    tourJ1 = true;
    tourJ2 = false;
    montre = false;
    duel = true;
    ordi = false;
    timerOn = false;
    label = document.getElementById("r2");
    document.getElementById("j1_a").style.visibility = "visible";
    document.getElementById("j1_b").style.visibility = "visible";
    document.getElementById("j2_a").firstChild.nodeValue = "Joueur 2";
    document.getElementById("j2_a").style.visibility = "visible";
    document.getElementById("j2_b").style.visibility = "visible";
    label.style.transform = "translate(0px,5px)";
    label.style.borderBottom = "3px solid";
    label.style.fontWeight = "bold";
    no_timer();
    affichageTourJoueur(true);
}

/*rend les compteurs de points du mode 2 joueurs invisible*/

function deux_joueurs_invis () {
    document.getElementById("j1_a").style.visibility = "hidden";
    document.getElementById("j1_b").style.visibility = "hidden";
    document.getElementById("j2_a").style.visibility = "hidden";
    document.getElementById("j2_b").style.visibility = "hidden";
}

/*mode de jeu contre l'ordinateur*/

var memoireOrdi;

function contre_ordinateur () {
    var label;
    memoireOrdi = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
    tourJ1 = true;
    tourJ2 = false;
    pointsJ1 = 0;
    pointsJ2 = 0;
    montre = false;
    duel = false;
    ordi = true;
    timerOn = false;
    label = document.getElementById("r3");
    label.style.transform = "translate(0px,5px)";
    label.style.borderBottom = "3px solid";
    label.style.fontWeight = "bold";
    no_timer();
    document.getElementById("j1_a").style.visibility = "visible";
    document.getElementById("j1_b").style.visibility = "visible";
    document.getElementById("j2_a").firstChild.nodeValue = "Ordinateur";
    document.getElementById("j2_a").style.visibility = "visible";
    document.getElementById("j2_b").style.visibility = "visible";
    affichageTourJoueur();
    activeClicJoueur();
}

var intervalTimer;

/*fait fonctionner le timer*/

var temps;

function activeTimer() {
    timerOn = true;
    temps = 120;
    var label = document.getElementById("timer");
    intervalTimer = setInterval(function () {
        let minutes = parseInt(temps/60, 10);
        let secondes = parseInt(temps%60, 10);
        secondes = secondes < 10 ? "0" + secondes : secondes;
        label.firstChild.nodeValue = minutes + ":" + secondes;
        if (temps == 0) {
            defaite = true;
            perdu();
            clearInterval(intervalTimer);
        }
        if (victoire == true) {
            clearInterval(intervalTimer);
        }
        temps -= 1;
    }, 1000);
}

/*appelle les fonctions nécessaires en cas de défaite: retire tous les events des tuiles, affiche le message de défaite, crée le bouton rejouer*/

function perdu () {
    removeActiveEvents();
    messageDefaite();
    creerBoutonRejouer();
}

/*affiche un message de défaite*/

function messageDefaite () {
    var msg = document.getElementById("msg");
    msg.style.backgroundColor = "#A9A9A9";
    msg.firstChild.nodeValue = "Perdu ! Retentez votre chance !";
}

/*retirer le message de défaite/victoire*/

function retirerMessage () {
    var msg = document.getElementById("msg");
    msg.style.backgroundColor = "";
    msg.firstChild.nodeValue = " ";
}

/*appelle les fonctions nécessaires en cas de victoire: affiche le message de victoire, retire tous les events des tuiles, retourne la tuile sans paire, crée le bouton rejouer*/

function gagne () {
    victoire = true;
    messageVictoire();
    removeActiveEvents();
    retournerTuileIsolee();
    creerBoutonRejouer();
    var pseudo = window.prompt("Choisissez un pseudo.");
    enregistrerScoreLeaderboard(pseudo);
}

/*cherche et retourne la tuile sans paire*/

function retournerTuileIsolee () {
    var i;
    for (i = 0; i < 25; i += 1) {
        if (!(appartient_liste(i, correctes))) {
            b.tiles[i].flipTile();
        }
    }
}

/*affiche un message de victoire*/
    
function messageVictoire () {
    var msg = document.getElementById("msg");
    msg.style.backgroundColor = "#A9A9A9";
    msg.firstChild.nodeValue = "Gagné ! Vous pouvez rejouer !";
}

/*crée un bouton rejouer: css, node, event*/

function creerBoutonRejouer () {
    var div = document.getElementById("rejouer");
    var bouton = document.createElement("button");
    bouton.setAttribute("id", "rej");
    bouton.setAttribute("type", "button");
    var texte = document.createTextNode("Rejouer");
    bouton.appendChild(texte);
    div.appendChild(bouton);
    bouton = document.getElementById("rej");
    bouton.style.position = "fixed";
    bouton.style.left = "0";
    bouton.style.marginTop = "8%";
    bouton.style.width = "20%";
    bouton.style.backgroundColor = "lightblue";
    bouton.style.fontSize = "30pt";
    bouton.addEventListener("click", b.reset);
}

/*retire le bouton rejouer*/

function retirerBoutonRejouer () {
    var div = document.getElementById("rejouer");
    if (div.childNodes.length > 0) {
        var bouton = document.getElementById("rej");
        div.removeChild(bouton);
    }
}

/*ajoute les events sur les boutons radio du leaderboard pour changer du leaderboard et changer le css des boutons*/

function addEventRadioLeaderboard () {
    var rad = document.getElementsByClassName("hall");
    for (var i = 0; i < rad.length; i += 1) {
        rad[i].addEventListener("change", changerBoutonCSS);
    }
}

/*change entre le leaderboard temps et tours*/

function changerLeaderboard () {
    if (leaderboard_temps == true) {
        leaderboard_temps = false;
        leaderboard_tours = true;
    }
    else {
        leaderboard_temps = true;
        leaderboard_tours = false;
    }
    afficherLeaderboard();
}

/*change le leaderboard en fonction de l'option selectionnée*/

function afficherLeaderboard () {
    if (leaderboard_temps == true) {
        creerLeaderboardTemps();
    }
    else {
        creerLeaderboardTours();
    }
}

/*cree le leaderboard temps*/

function creerLeaderboardTemps () {
    var tab = JSON.parse(getCookie("score"));
    tab = tab.sort((a, b) => a[1] - b[1]);
    tab.splice(15);
    var td = document.getElementsByClassName("tds");
    for (var i = 0; i < tab.length; i += 1) {
        td[2*i].firstChild.nodeValue = tab[i][0];
        td[2*i+1].firstChild.nodeValue = tab[i][1];
    }
}

/*cree le leaderboard tours*/

function creerLeaderboardTours () {
    var tab = JSON.parse(getCookie("score"));
    tab = tab.sort((a, b) => a[2] - b[2])
    tab.splice(15);
    var td = document.getElementsByClassName("tds");
    for (var i = tab.length - 1; i >= 0; i -= 1) {
        td[2*i].firstChild.nodeValue = tab[i][0];
        td[2*i+1].firstChild.nodeValue = tab[i][2];
    }
}

/*enregistre le score dans le cookie*/

function enregistrerScoreLeaderboard (pseudo) {
    if (getCookie("score") == undefined) {
        document.cookie = "score=" + JSON.stringify(new Array()) + ";";
    }
    var value = JSON.parse(getCookie("score"));
    var score = [pseudo, 120 - temps, tours];
    value[value.length] = score;
    document.cookie = 'score=' + JSON.stringify(value) + ';';
    afficherLeaderboard();
}

/*retourne la value du nom de valeur donnée, trouvé sur internet*/

function getCookie(cname) {
  let name = cname + "=";
  let decodedCookie = decodeURIComponent(document.cookie);
  let ca = decodedCookie.split(';');
  for(let i = 0; i < ca.length; i+= 1) {
    let c = ca[i];
    while (c.charAt(0) == ' ') {
      c = c.substring(1);
    }
    if (c.indexOf(name) == 0) {
      return c.substring(name.length, c.length);
    }
  }
}

/*change le css du bouton sélectionné dans le leaderboard*/

function changerBoutonCSS () {
    var type = document.getElementById("type");
    if (this.id == "hall1") {
        document.getElementById("h1").style.backgroundColor = "#87E990";
        document.getElementById("h2").style.backgroundColor = "#e3e3e3";
        type.firstChild.nodeValue = "Temps (en secondes)";
    }
    else {
        document.getElementById("h2").style.backgroundColor = "#87E990";
        document.getElementById("h1").style.backgroundColor = "#e3e3e3";
        type.firstChild.nodeValue = "Nombre de Tours";
    }
    if (document.cookie.length > 0) {
        changerLeaderboard();
    }
}

/*regarde quel bouton du leaderboard est coché par défaut et change le css en conséquence*/

var leaderboard_temps;
var leaderboard_tours;

function chercheRadioCoche () {
    var rad = document.getElementsByClassName("hall");
    var type = document.getElementById("type");
    if (rad[0].checked == true) {
        document.getElementById("h1").style.backgroundColor = "#87E990";
        type.firstChild.nodeValue = "Temps (en secondes)";
        leaderboard_temps = true;
        leaderboard_tours = false;
    }
    else {
        document.getElementById("h2").style.backgroundColor = "#87E990";
        type.firstChild.nodeValue = "Nombre de Tours";
        leaderboard_temps = false;
        leaderboard_tours = true;
    }
}

var tourJ1 = true;
var tourJ2 = false;

/*affiche les points et les bordures du joueur qui doit jouer*/

function affichageTourJoueur (resultat) {
    var j1 = document.getElementById("j1_b");
    var j2 = document.getElementById("j2_b");
    j1.firstChild.nodeValue = "Point: " + pointsJ1.toString();
    j2.firstChild.nodeValue = "Point: " + pointsJ2.toString();
    if (resultat == false) {
        setTimeout(function () {
            if (tourJ1 == true) {
                var div = document.getElementById("j1");
                div.style.border = "4px solid red";
                div.style.borderRadius = "15px";
                div = document.getElementById("j2");
                div.style.border = "";
            }
            else {
                var div = document.getElementById("j2");
                div.style.border = "4px solid red";
                div.style.borderRadius = "15px";
                div = document.getElementById("j1");
                div.style.border = "";
            }
        }, 2000);
    }
    else {
        if (tourJ1 == true) {
            var div = document.getElementById("j1");
            div.style.border = "4px solid red";
            div.style.borderRadius = "15px";
            div = document.getElementById("j2");
            div.style.border = "";
        }
            else {
            var div = document.getElementById("j2");
            div.style.border = "4px solid red";
            div.style.borderRadius = "15px";
            div = document.getElementById("j1");
            div.style.border = "";
        }
    }
}

/*cache les affichages du mode deux joueurs*/

function effacerAffichageTourJoueur () {
    var div = document.getElementById("j1");
    div.style.border = "";
    div = document.getElementById("j2");
    div.style.border = "";
}

/*ajoute un point au joueur si il a toruvé une paire, sinon change de joueur*/

function changerTourJoueur (resultat) {
    if (nb_correctes == 24 && duel == true) {
        messageVictoireDuel();
        gagneDuel();
    }
    if (nb_correctes == 24 && ordi == true) {
        messageVictoireOrdi();
        gagneOrdi();
    }
    if (resultat == true) {
        if (tourJ1 == true) {
            pointsJ1 += 1;
        }
        else {
            pointsJ2 += 1;
        }
    }
    else {
        if (tourJ1 == true) {
            tourJ1 = false;
            tourJ2 = true;
        }
        else {
            if (ordi == true) {
                setTimeout(activeClicJoueur, 2000);
            }
            tourJ1 = true;
            tourJ2 = false;
        }
    }
    if (tourJ2 == true && ordi == true && nb_correctes != 24) {
        desactiveClicJoueur();
        setTimeout(jouerOrdi, 2000);
    }
    affichageTourJoueur(resultat);
}

/*affiche un message annonçant le joueur gagnant*/

function messageVictoireDuel () {
    if (pointsJ1 > pointsJ2) {
        var msg = document.getElementById("msg");
        msg.style.backgroundColor = "#A9A9A9";
        msg.firstChild.nodeValue = "Joueur 1 gagne ! Vous pouvez rejouer !";
    }
    else if (pointsJ1 < pointsJ2) {
        var msg = document.getElementById("msg");
        msg.style.backgroundColor = "#A9A9A9";
        msg.firstChild.nodeValue = "Joueur 2 gagne ! Vous pouvez rejouer !";
    }
    else {
        var msg = document.getElementById("msg");
        msg.style.backgroundColor = "#A9A9A9";
        msg.firstChild.nodeValue = "Egalité ! Vous pouvez rejouer !";
    }
}

/*pareil que gagne() mais pour le mode deux joueurs*/

function gagneDuel () {
    victoire = true;
    messageVictoireDuel();
    removeActiveEvents();
    retournerTuileIsolee();
    creerBoutonRejouer();
}

/*regarde s'il existe une paire dans la memoire de l'ordi*/

function cherchePaireMemoireOrdi () {
    for (var i = 0; i < memoireOrdi.length - 1; i += 1) {
        if (memoireOrdi[i] != 0) {
            let res = appartient_liste(memoireOrdi[i], memoireOrdi.slice(i+1));
            if (res == true) {
                return cherchePaireIndice(i);
            }
        }
    }
    return false;
}

/*retourne les indices de la paire*/

function cherchePaireIndice (ind) {
    var mem = memoireOrdi.slice(ind+1);
    for (var i = 0; i < mem.length; i += 1) {
        if (memoireOrdi[ind] == mem[i]) {
            memoireOrdi[ind] = 0;
            memoireOrdi[ind+i+1] = 0;
            return [ind, ind+i+1];
        }
    }
}

/*fait jouer l'ordinateur*/

function jouerOrdi () {
    var paire = cherchePaireMemoireOrdi();
    console.log(paire);
    if (paire != false) {
        jouerOrdiPaire(paire);
    }
    else {
        jouerOrdiRandom();
    }
}

/*l'ordinateur retourne la paire de tuiles donnée*/

function jouerOrdiPaire(paire) {
    setTimeout(function () {
        document.getElementById(paire[0]).click();
    }, 1000);
    setTimeout(function () {
        document.getElementById(paire[1]).click();
    }, 2000);
}

/*cherche une paire aléatoire jouable*/

function jouerOrdiRandom () {
    var rand1 = Math.floor(Math.random() * 25);
    var rand2 = Math.floor(Math.random() * 25);
    while (appartient_liste(rand1, correctes)) {
        rand1 = Math.floor(Math.random() * 25);
    }
    while (appartient_liste(rand2, correctes) || rand2 == rand1) {
        rand2 = Math.floor(Math.random() * 25);
    }
    console.log(rand1);
    console.log(rand2);
    jouerOrdiPaire([rand1, rand2]);
}

/*affiche le message de victoire en mode de jeu contre l'ordinateur*/

function messageVictoireOrdi () {
    if (pointsJ1 > pointsJ2) {
        var msg = document.getElementById("msg");
        msg.style.backgroundColor = "#A9A9A9";
        msg.firstChild.nodeValue = "Joueur 1 gagne ! Vous pouvez rejouer !";
    }
    else if (pointsJ1 < pointsJ2) {
        var msg = document.getElementById("msg");
        msg.style.backgroundColor = "#A9A9A9";
        msg.firstChild.nodeValue = "L'ordinateur gagne ! Vous pouvez rejouer !";
    }
    else {
        var msg = document.getElementById("msg");
        msg.style.backgroundColor = "#A9A9A9";
        msg.firstChild.nodeValue = "Egalité ! Vous pouvez rejouer !";
    }
}

/*empèche le joueur de jouer pendant que l'ordinateur joue*/

function desactiveClicJoueur () {
    for (var i = 0; i < ima.length; i += 1) {
        ima[i].style.pointerEvents = "none";
    }
}

/* permet de nouveau au joueur de jouer*/

function activeClicJoueur () {
    for (var i = 0; i < ima.length; i += 1) {
        ima[i].style.pointerEvents = "auto";
    }
}

/*appelle les fonctions nécessaire en fin de partie contre l'ordinateur*/

function gagneOrdi () {
    victoire = true;
    messageVictoireOrdi();
    removeActiveEvents();
    retournerTuileIsolee();
    creerBoutonRejouer();
}

/*tout ce qui est nécéssaire au chargement de la page pour initialiser le mode de jeu*/

function initialisationModeDeJeu () {
    document.getElementById("rad1").checked = true;
    document.getElementById("r1").style.transform = "translate(0px,5px)";
    document.getElementById("r1").style.borderBottom = "3px solid";
    document.getElementById("r1").style.fontWeight = "bold";
    timer();
}

/*tout ce qui est nécéssaire au chargement de la page pour initialiser le leaderboard*/

function initialisationLeaderboard () {
    addEventRadioLeaderboard();
    chercheRadioCoche();
    if (document.cookie.length > 0) {
        afficherLeaderboard();
    }
}

/*tout ce qui est nécéssaire au chargement de la page pour initialiser le plateau de jeu*/

function initialisationPlateau () {
    var tiles = creation_tiles();
    add_images_event();
    addEventRadio();
    b = new Board(tiles);
}

/*appel de chaque fonction d'initialisation*/

function initialisation () {
    initialisationLeaderboard();
    initialisationModeDeJeu();
    initialisationPlateau();
}


window.onload = main;

function main() {
    initialisation();
}

var nb_tournees = 0;
var tournees = [];
var tours = 0;
var ima = document.getElementsByClassName("ima");
var nb_correctes = 0;
var correctes = [];
var b;
var montre = true;
var duel = false;
var ordi = false;
var timerOn = false;
var victoire = false;
var defaite = false;