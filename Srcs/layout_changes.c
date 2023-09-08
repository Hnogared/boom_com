/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layout_changes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnogared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 17:39:25 by hnogared          #+#    #+#             */
/*   Updated: 2023/09/08 22:43:37 by hnogared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/defuser_wizard.h"

void	goto_layout_help(portopts **conn_options, dispopts **disp_options)
{
	strncpy((*disp_options)->cmd_output, " * Bienvenue sur l'aide au desamorcage.\n"
		" * "
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Retour\n", BIG_BUFFER - 1);
	(*disp_options)->cmd_output[BIG_BUFFER - 2] = 0;
	(*disp_options)->layout *= -1;
	menu_defusing(conn_options, disp_options);
}

void	goto_layout_1(portopts **conn_options, dispopts **disp_options)
{
	if ((*conn_options)->fd > -1)
	{
		close((*conn_options)->fd);
		(*conn_options)->baudrate = 0;
		(*conn_options)->fd = -1;
		(*conn_options)->port[0] = 0;
	}
	(*disp_options)->bomb_output[0] = 0;
	strncpy((*disp_options)->cmd_output, " * Bienvenue sur l'interface de desamorcage.\n"
		" * Ce programme a pour but de vous guider lors du desamorcage d'engins"
		" potentiellement letaux.\n\n"
		" > Veuillez taper une de ces options (sans oublier de taper la touche 'entrer') :\n"
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Infiltrer la bombe\n", BIG_BUFFER - 1);
	(*disp_options)->cmd_output[BIG_BUFFER - 2] = 0;
	(*disp_options)->view = 1;
	(*disp_options)->layout = 1;
	menu_defusing(conn_options, disp_options);
}

void	goto_layout_2(portopts **conn_options, dispopts **disp_options, bool loading)
{
	if (loading == true)
		main_menu_loading("Intrusion en cours...");
	(*disp_options)->view = 2;
	if (open_usb_port(conn_options))
	{
		strncpy((*disp_options)->bomb_output, "!> CONNECTION_ERROR >> ", BIG_BUFFER);
		strncpy((*disp_options)->bomb_output + 20, strerror(errno), BIG_BUFFER);
		(*disp_options)->bomb_output[BIG_BUFFER - 1] = 0;
		return ;
	}
	strncpy((*disp_options)->bomb_output,
		"Acces autorise, JOE@DEFUSER42 connecte via port USB *Bienvenue ^^*\n", BIG_BUFFER - 1);
	(*disp_options)->bomb_output[BIG_BUFFER - 2] = 0;
	strncpy((*disp_options)->cmd_output, " > Connecte a la bombe\n\n"
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Deconnection de la bombe\n"
		" # [3] Scan du systeme\n", BIG_BUFFER - 1);
	(*disp_options)->cmd_output[BIG_BUFFER - 2] = 0;
	(*disp_options)->layout = 2;
	menu_defusing(conn_options, disp_options);
}

void	goto_layout_3(portopts **conn_options, dispopts **disp_options, bool loading)
{
	if (loading == true)
		main_menu_loading("Scan des fichiers de la bombe...");
	strncpy((*disp_options)->bomb_output, "installed modules\n\n"
		" * DISPLAY CONTROLLER (UNO_R3)\t- [ID:LS3249] - Communication protocol : Wire l2C\n"
		" * TIME_CONTROL_UNIT (ELEGO_R3)\t- [ID:CR57a5] - ACCESS LOCKED\n"
		" * DATA_STORAGE (ESP32_V4)\t- [ID:PK84u9] - Firewall breach\n"
		" * MOTOR_ACTUATOR (Servo motor)\t- [ID:RM3oRF] - ACCESS RESTRICTED\n", BIG_BUFFER - 1);
	(*disp_options)->bomb_output[BIG_BUFFER - 2] = 0;
	strncpy((*disp_options)->cmd_output, " > Scan effectue\n"
		" * Vulnerabilite detectee au niveau du firewall.\n"
		" * Une attaque afin de le desactiver peut etre lancee avec la commande : "
		"'@firewall-ext <MODULE_ID>'\n"
		" * /!\\ Lors de l'utilisation de commandes commencant par '@', elles seront directement"
		" envoyees a la bombe sans etre corrigees auparavant par le desamorceur.\n\n"
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Deconnection de la bombe\n", BIG_BUFFER - 1);
	(*disp_options)->cmd_output[BIG_BUFFER - 2] = 0;
	(*disp_options)->view = 2;
	(*disp_options)->layout = 3;
	menu_defusing(conn_options, disp_options);
}

void	goto_layout_firewalloff(portopts **conn_options, dispopts **disp_options)
{
	strncpy((*disp_options)->cmd_output, " > Attaque reussie\n"
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
		" # [2] Deconnection de la bombe\n", BIG_BUFFER - 1);
	(*disp_options)->cmd_output[BIG_BUFFER - 2] = 0;
	(*disp_options)->view = 2;
	(*disp_options)->layout = 4;
	menu_defusing(conn_options, disp_options);
}

void	goto_layout_labyrinth(portopts **conn_options, dispopts **disp_options)
{
	strncpy((*disp_options)->cmd_output, " > Firewall desactive, corruption de la memoire\n"
		" * Vous devriez avoir acces a la memoire de la bombe.\n"
		" * Lancez "
		" # [1] Quitter le programme (ou tapez 'exit')\n"
		" # [2] Deconnection de la bombe\n", BIG_BUFFER - 1);
	(*disp_options)->cmd_output[BIG_BUFFER - 2] = 0;
	(*disp_options)->view = 2;
	(*disp_options)->layout = 5;
	menu_defusing(conn_options, disp_options);
}
