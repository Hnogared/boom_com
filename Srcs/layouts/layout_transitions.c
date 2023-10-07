#include "../../Includes/defuser_wizard.h"

void	goto_layout_help(t_data *data_p)
{
	data_p->dispopts_s.cmd_output = " * Bienvenue sur l'aide au desamorcage.\n *\n"
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Retour\n";
	data_p->dispopts_s.layout *= -1;
	main_layout(data_p);
}

void	goto_layout_1(t_data *data_p)
{
	if (data_p->portopts_s.fd > -1)
	{
		close(data_p->portopts_s.fd);
		data_p->portopts_s.baudrate = 0;
		data_p->portopts_s.fd = -1;
		data_p->portopts_s.port[0] = 0;
	}
	data_p->dispopts_s.bomb_output[0] = 0;
	data_p->dispopts_s.cmd_output = " * Bienvenue sur l'interface de desamorcage.\n"
		" * Ce programme a pour but de vous guider lors du desamorcage d'engins"
		" potentiellement letaux.\n\n"
		" > Veuillez taper une de ces options "
		"(sans oublier de taper la touche 'entrer') :\n"
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Infiltrer la bombe\n";
	data_p->dispopts_s.view = 1;
	data_p->dispopts_s.layout = 1;
	main_layout(data_p);
}

void	goto_layout_2(t_data *data_p, bool loading)
{
	if (loading == true)
		main_menu_loading("Intrusion en cours...");
	data_p->dispopts_s.view = 2;
	if (open_usb_port(&data_p->portopts_s))
	{
		save_error(data_p->dispopts_s.bomb_output, BIG_BUFFER, BIN_NAME,
			"port error");
	//	main_layout(portopts_p, dispopts_p);
		return ;
	}
	safer_strncpy(data_p->dispopts_s.bomb_output, "Acces autorise, "
		"connecte via port USB *Bienvenue ^^*\n", BOMBOUT_BUFFER, 128);
	data_p->dispopts_s.cmd_output = " > Connecte a la bombe\n\n"
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Deconnection de la bombe\n"
		" # [3] Scan du systeme\n";
	data_p->dispopts_s.layout = 2;
	main_layout(data_p);
}

void	goto_layout_3(t_data *data_p, bool loading)
{
	if (loading == true)
		main_menu_loading("Scan des fichiers de la bombe...");
	safer_strncpy(data_p->dispopts_s.bomb_output,
		"installed modules\n\n"
		" * DISPLAY CONTROLLER (UNO_R3)\t- [ID:LS3249] - Communication protocol : Wire l2C\n"
		" * TIME_CONTROL_UNIT (ELEGO_R3)\t- [ID:CR57a5] - ACCESS LOCKED\n"
		" * DATA_STORAGE (ESP32_V4)\t- [ID:PK84u9] - Firewall breach\n"
		" * MOTOR_ACTUATOR (Servo motor)\t- [ID:RM3oRF] - ACCESS RESTRICTED\n",
		BOMBOUT_BUFFER, 512);
	data_p->dispopts_s.cmd_output = " > Scan effectue\n"
		" * Vulnerabilite detectee au niveau du firewall.\n"
		" * Une attaque afin de le desactiver peut etre lancee avec la commande : "
		"'@firewall-ext <MODULE_ID>'\n"
		" * /!\\ Lors de l'utilisation de commandes commencant par '@', elles seront directement"
		" envoyees a la bombe sans etre corrigees auparavant par le desamorceur.\n\n"
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Deconnection de la bombe\n";
	data_p->dispopts_s.view = 2;
	data_p->dispopts_s.layout = 3;
	main_layout(data_p);
}

void	goto_layout_firewalloff(t_data *data_p)
{
	data_p->dispopts_s.cmd_output = " > Attaque reussie\n"
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
		" # [2] Deconnection de la bombe\n";
	data_p->dispopts_s.view = 2;
	data_p->dispopts_s.layout = 4;
	main_layout(data_p);
}

void	goto_layout_labyrinth(t_data *data_p)
{
	data_p->dispopts_s.cmd_output = " > Firewall desactive, acces a la memoire ouvert\n"
		" * 'bit_stuffer' est un programme de corruption de memoire.\n"
		" * Lancez-le et faites overflow la memoire de la bombe pour en "
		"retirer des donnees\n\n"
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Deconnection de la bombe\n"
		" # [3] Executer 'bit_stuffer'\n";
	data_p->dispopts_s.view = 2;
	data_p->dispopts_s.layout = 5;
	main_layout(data_p);
}

void	goto_layout_bytes(t_data *data_p)
{
	data_p->dispopts_s.cmd_output = " > Overflow de la memoire\n"
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Deconnection de la bombe\n"
		" # [3] Extraction des donnees corrompues\n";
	data_p->dispopts_s.view = 2;
	data_p->dispopts_s.layout = 6;
	main_layout(data_p);
}

void	goto_layout_password(t_data *data_p, bool mode)
{
	if (mode && write(data_p->portopts_s.fd, "bytestuffer PK84u9", 18) == -1)
	{
		save_error(data_p->dispopts_s.bomb_output, BIG_BUFFER, BIN_NAME,
			"port write error");
		goto_layout_password(data_p, false);
		return ;
	}
	data_p->dispopts_s.cmd_output = " > Affichage de la memoire corrompue\n"
		" * La memoire peut etre divisee en octets (8 bits) afin d'etre lue.\n"
		" * Pour calculer la valeur d'un octet, ajoutez les valeurs de ses bits comme ci-dessous :\n"
		" *\n"
		" *  0 1 0 0 1 0 1 0 = 0*128 + 1*64 + 0*32 + 0*16 + 1*8 + 0*4 + 1*2 + 0*1 = 74\n"
		" *  0 0 1 1 0 1 0 0 = 0*128 + 0*64 + 1*32 + 1*16 + 0*8 + 1*4 + 0*2 + 0*1 = 52\n\n"
		" * Veuillez vous referer a tableau ascii (man ascii) pour l'interpretation de chaque octet.\n"
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Deconnection de la bombe\n";
	data_p->dispopts_s.view = 2;
	data_p->dispopts_s.layout = 7;
	main_layout(data_p);
}
