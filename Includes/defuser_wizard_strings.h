#ifndef DEFUSER_WIZARD_STRINGS_H
# define DEFUSER_WIZARD_STRINGS_H

/* ************************************************************************** */
/* strings defines                                                            */
/* ************************************************************************** */
# define MSG_SUDO	" * Tentative de connection en administrateur.\n"\
	" * Veuillez taper le mot de passe afin de l'envoyer a la bombe\n"\
	" * /!\\ Veuillez etre sur d'avoir le bon mot de passe /!\\\n\n"\
	" # [1] Quitter le programme (ou tapez 'exit')\n"\
	" # [2] Retour\n"

# define MSG_HELP	" * Bienvenue sur l'aide au desamorcage.\n *\n"\
	" # [1] Quitter le programme (ou tapez 'exit')\n"\
	" # [2] Retour\n"

# define MSG_LAYOUT1	" * Bienvenue sur l'interface de desamorcage.\n"\
	" * Ce programme a pour but de vous guider lors du desamorcage d'engins"\
	" potentiellement letaux.\n\n"\
	" > Veuillez taper une de ces options "\
	"(sans oublier de taper la touche 'entrer') :\n"\
	" # [1] Quitter le programme (ou tapez 'exit')\n"\
	" # [2] Infiltrer la bombe\n"

# define MSG_LAYOUT2	" > Connecte a la bombe\n\n"\
	" # [1] Quitter le programme (ou tapez 'exit')\n"\
	" # [2] Deconnection de la bombe\n"\
	" # [3] Scan du systeme\n"

# define MSG_LAYOUT3	" > Scan effectue\n"\
	" * Vulnerabilite detectee au niveau du firewall.\n"\
	" * Une attaque afin de le desactiver peut etre lancee avec la "\
	"commande 'firewall-ext', suivie de l'ID module contenant la faille.\n"\
	" * Par exemple, afin d'attaquer l'ID FOO42 lancez : "\
	"'firewall-ext FOO42'\n"\
	" # [1] Quitter le programme (ou tapez 'exit')\n"\
	" # [2] Deconnection de la bombe\n"

# define MSG_LAYOUT_FIREWALLOFF	" > Attaque reussie\n"\
	" * Le firewall a ete partiellement desactive\n"\
	" * Le pave de huit boutons vous permettra d'entrer la sequence de "\
	"desactivation totale.\n"\
	" * Une roue lumineuse devrait vous guider dans cette etape\n\n"\
	"          [][]\n"\
	"        []    []\n"\
	"      []        []\n"\
	"      []        []\n"\
	"        []    []\n"\
	"          [][]\n"\
	" # [1] Quitter le programme (ou tapez 'exit')\n"\
	" # [2] Deconnection de la bombe\n"

# define MSG_LAYOUT_LAB	" > Firewall desactive, acces a la memoire ouvert\n"\
	" * 'bit_stuffer' est un programme de corruption de memoire.\n"\
	" * Lancez-le et faites overflow la memoire de la bombe pour en "\
	"retirer des donnees\n\n"\
	" # [1] Quitter le programme (ou tapez 'exit')\n"\
	" # [2] Deconnection de la bombe\n"\
	" # [3] Executer 'bit_stuffer'\n"

# define MSG_LAYOUT_BYTES	" > Overflow de la memoire\n"\
	" # [1] Quitter le programme (ou tapez 'exit')\n"\
	" # [2] Deconnection de la bombe\n"\
	" # [3] Extraction des donnees corrompues\n"

# define MSG_LAYOUT_PASSWORD	" > Affichage de la memoire corrompue\n"\
	" * La memoire peut etre divisee en octets (8 bits) afin d'etre lue.\n"\
	" * Pour calculer la valeur d'un octet, ajoutez les valeurs de ses "\
	"bits comme ci-dessous :\n"\
	" *  0 1 0 0 1 0 1 0 = "\
	"0*128 + 1*64 + 0*32 + 0*16 + 1*8 + 0*4 + 1*2 + 0*1 = 74\n"\
	" *  0 0 1 1 0 1 0 0 = 0 + 0 + 32 + 16 + 0 + 4 + 0 + 0 = 52\n\n"\
	" * Veuillez vous referer a tableau ascii (man ascii) "\
	"pour l'interpretation de chaque octet.\n"\
	" # [1] Quitter le programme (ou tapez 'exit')\n"\
	" # [2] Deconnection de la bombe\n"

#endif

/*
	" * /!\\ Lors de l'utilisation de commandes commencant par '@', "\
	"elles seront directement envoyees a la bombe sans etre corrigees "\
	"auparavant par le desamorceur.\n\n"\
*/
