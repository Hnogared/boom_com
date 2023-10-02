#include "../Includes/defuser_wizard.h"

void	goto_layout_help(t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	strncpy(dispopts_p->cmd_output, " * Bienvenue sur l'aide au desamorcage.\n"
		" * "
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Retour\n", BIG_BUFFER - 2);
	dispopts_p->cmd_output[BIG_BUFFER - 1] = 0;
	dispopts_p->layout *= -1;
	menu_defusing(portopts_p, dispopts_p);
}

void	goto_layout_1(t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	if (portopts_p->fd > -1)
	{
		close(portopts_p->fd);
		portopts_p->baudrate = 0;
		portopts_p->fd = -1;
		portopts_p->port[0] = 0;
	}
	dispopts_p->bomb_output[0] = 0;
	strncpy(dispopts_p->cmd_output, " * Bienvenue sur l'interface de desamorcage.\n"
		" * Ce programme a pour but de vous guider lors du desamorcage d'engins"
		" potentiellement letaux.\n\n"
		" > Veuillez taper une de ces options (sans oublier de taper la touche 'entrer') :\n"
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Infiltrer la bombe\n", BIG_BUFFER - 2);
	dispopts_p->cmd_output[BIG_BUFFER - 1] = 0;
	dispopts_p->view = 1;
	dispopts_p->layout = 1;
	menu_defusing(portopts_p, dispopts_p);
}

void	goto_layout_2(t_portopts *portopts_p, t_dispopts *dispopts_p, bool loading)
{
	if (loading == true)
		main_menu_loading("Intrusion en cours...");
	dispopts_p->view = 2;
	if (open_usb_port(portopts_p))
	{
		strncpy(dispopts_p->bomb_output, "!> CONNECTION_ERROR >> ", BIG_BUFFER - 2);
		strncpy(dispopts_p->bomb_output + 20, strerror(errno), BIG_BUFFER - 2);
		dispopts_p->bomb_output[BIG_BUFFER - 1] = 0;
		return ;
	}
	strncpy(dispopts_p->bomb_output,
		"Acces autorise, JOE@DEFUSER42 connecte via port USB *Bienvenue ^^*\n", BIG_BUFFER - 2);
	dispopts_p->bomb_output[BIG_BUFFER - 1] = 0;
	strncpy(dispopts_p->cmd_output, " > Connecte a la bombe\n\n"
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Deconnection de la bombe\n"
		" # [3] Scan du systeme\n", BIG_BUFFER - 2);
	dispopts_p->cmd_output[BIG_BUFFER - 1] = 0;
	dispopts_p->layout = 2;
	menu_defusing(portopts_p, dispopts_p);
}

void	goto_layout_3(t_portopts *portopts_p, t_dispopts *dispopts_p, bool loading)
{
	if (loading == true)
		main_menu_loading("Scan des fichiers de la bombe...");
	strncpy(dispopts_p->bomb_output, "installed modules\n\n"
		" * DISPLAY CONTROLLER (UNO_R3)\t- [ID:LS3249] - Communication protocol : Wire l2C\n"
		" * TIME_CONTROL_UNIT (ELEGO_R3)\t- [ID:CR57a5] - ACCESS LOCKED\n"
		" * DATA_STORAGE (ESP32_V4)\t- [ID:PK84u9] - Firewall breach\n"
		" * MOTOR_ACTUATOR (Servo motor)\t- [ID:RM3oRF] - ACCESS RESTRICTED\n", BIG_BUFFER - 2);
	dispopts_p->bomb_output[BIG_BUFFER - 1] = 0;
	strncpy(dispopts_p->cmd_output, " > Scan effectue\n"
		" * Vulnerabilite detectee au niveau du firewall.\n"
		" * Une attaque afin de le desactiver peut etre lancee avec la commande : "
		"'@firewall-ext <MODULE_ID>'\n"
		" * /!\\ Lors de l'utilisation de commandes commencant par '@', elles seront directement"
		" envoyees a la bombe sans etre corrigees auparavant par le desamorceur.\n\n"
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Deconnection de la bombe\n", BIG_BUFFER - 2);
	dispopts_p->cmd_output[BIG_BUFFER - 1] = 0;
	dispopts_p->view = 2;
	dispopts_p->layout = 3;
	menu_defusing(portopts_p, dispopts_p);
}

void	goto_layout_firewalloff(t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	strncpy(dispopts_p->cmd_output, " > Attaque reussie\n"
		" * Le firewall a ete partiellement desactive\n"
		" * Le pave de huit boutons vous permettra d'entrer la sequence de desactivation totale.\n"
		" * Une roue lumineuse devrait vous guider dans cette etape (voir fig. 1).\n\n"
		"          [][]\n"
		"        []    []\n"
		"      []        []\n"
		"      []        []\n"
		"        []    []\n"
		"          [][]\n"
		"      fig. 1\n\n"
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Deconnection de la bombe\n", BIG_BUFFER - 2);
	dispopts_p->cmd_output[BIG_BUFFER - 1] = 0;
	dispopts_p->view = 2;
	dispopts_p->layout = 4;
	menu_defusing(portopts_p, dispopts_p);
}

void	goto_layout_labyrinth(t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	strncpy(dispopts_p->cmd_output, " > Firewall desactive, acces a la memoire ouvert\n"
		" * 'bit_stuffer' est un programme de corruption de memoire.\n"
		" * Lancez-le et faites overflow la memoire de la bombe pour en retirer des donnees\n\n"
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Deconnection de la bombe\n"
		" # [3] Executer 'bit_stuffer'\n", BIG_BUFFER - 2);
	dispopts_p->cmd_output[BIG_BUFFER - 1] = 0;
	dispopts_p->view = 2;
	dispopts_p->layout = 5;
	menu_defusing(portopts_p, dispopts_p);
}

void	goto_layout_bytes(t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	strncpy(dispopts_p->cmd_output, " > Overflow de la memoire\n"
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Deconnection de la bombe\n"
		" # [3] Extraction des donnees corrompues\n", BIG_BUFFER - 2);
	dispopts_p->cmd_output[BIG_BUFFER - 1] = 0;
	dispopts_p->view = 2;
	dispopts_p->layout = 6;
	menu_defusing(portopts_p, dispopts_p);
}

void	goto_layout_password(t_portopts *portopts_p, t_dispopts *dispopts_p, bool mode)
{
	if (mode && write(portopts_p->fd, "bytestuffer PK84u9", 18) == -1)
	{
		strncpy(dispopts_p->bomb_output, "!> WRITING ERROR >> ", BIG_BUFFER);
		strncpy(dispopts_p->bomb_output + 20, strerror(errno), BIG_BUFFER);
		dispopts_p->bomb_output[BIG_BUFFER - 1] = 0;
		goto_layout_password(portopts_p, dispopts_p, false);
		return ;
	}
	strncpy(dispopts_p->cmd_output, " > Affichage de la memoire corrompue\n"
		" * La memoire peut etre divisee en octets (8 bits) afin d'etre lue.\n"
		" * Pour calculer la valeur d'un octet, ajoutez les valeurs de ses bits comme ci-dessous :\n"
		" *\n"
		" *  0 1 0 0 1 0 1 0 = 0*128 + 1*64 + 0*32 + 0*16 + 1*8 + 0*4 + 1*2 + 0*1 = 74\n"
		" *  0 0 1 1 0 1 0 0 = 0*128 + 0*64 + 1*32 + 1*16 + 0*8 + 1*4 + 0*2 + 0*1 = 52\n\n"
		" * Veuillez vous referer a tableau ascii (man ascii) pour l'interpretation de chaque octet.\n"
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Deconnection de la bombe\n", BIG_BUFFER - 2);
	dispopts_p->cmd_output[BIG_BUFFER - 1] = 0;
	dispopts_p->view = 2;
	dispopts_p->layout = 7;
	menu_defusing(portopts_p, dispopts_p);
}
