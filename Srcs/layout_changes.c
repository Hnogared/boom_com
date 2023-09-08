/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layout_changes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnogared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 17:39:25 by hnogared          #+#    #+#             */
/*   Updated: 2023/09/08 18:49:47 by hnogared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/defuser_wizard.h"

void	goto_layout0(portopts **conn_options, dispopts **disp_options)
{
	if ((*conn_options)->fd > -1)
	{
		close((*conn_options)->fd);
		(*conn_options)->baudrate = 0;
		(*conn_options)->fd = -1;
		(*conn_options)->port[0] = 0;
	}
	(*disp_options)->bomb_output[0] = 0;
	strncpy((*disp_options)->cmd_output, "Bienvenue sur l'interface de desamorcage.\n"
		"Veuillez taper une de ces options :\n\n"
		" # [1] Exit (ou tapez 'exit')\n"
		" # [2] Infiltrer la bombe.\n", BIG_BUFFER - 1);
	(*disp_options)->cmd_output[BIG_BUFFER - 2] = 0;
	(*disp_options)->view = 1;
	(*disp_options)->layout = 0;
}

void	goto_layout1(portopts **conn_options, dispopts **disp_options, bool loading)
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
		"JOE@BOMB ~> acces autorise, JOE@DEFUSER42 connecte via port USB *Bienvenue ^^*\n", BIG_BUFFER - 1);
	(*disp_options)->bomb_output[BIG_BUFFER - 2] = 0;
	strncpy((*disp_options)->cmd_output, "> Connecte a la bombe\n\n"
		" # [1] Exit (ou tapez 'exit')\n"
		" # [2] Deconnection de la bombe\n"
		" # [3] Scan du systeme\n", BIG_BUFFER - 1);
	(*disp_options)->cmd_output[BIG_BUFFER - 2] = 0;
	(*disp_options)->layout = 1;
}

void	goto_layout2(dispopts **disp_options, bool loading)
{
	if (loading == true)
		main_menu_loading("Scan des fichiers de la bombe...");
	strncpy((*disp_options)->bomb_output, "installed modules\n\n"
		" #DISPLAY CONTROLLER (UNO_R3)#  #TIME_CONTROL_UNIT (ELEGO_R3)#  #DATA_STORAGE (ESP32_V4)#"
		"  #MOTOR_ACTUATOR (Servo motor)\n", BIG_BUFFER - 1);
	(*disp_options)->bomb_output[BIG_BUFFER - 2] = 0;
	strncpy((*disp_options)->cmd_output, "> Scan effectue\n\n"
		" \e[33m* Faille trouvee au niveau du firewall *\e[0m"
		" # [1] Exit (ou tapez 'exit')\n"
		" # [2] Deconnection de la bombe\n"
		" # [3] Resultats du scan\n", BIG_BUFFER - 1);
	(*disp_options)->cmd_output[BIG_BUFFER - 2] = 0;
	(*disp_options)->view = 2;
	(*disp_options)->layout = 2;
}
