#include "../../Includes/defuser_wizard.h"

void	goto_layout_help(t_data *data_p)
{
	data_p->dispopts_s.cmd_output = MSG_HELP;
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
	data_p->dispopts_s.cmd_output = MSG_LAYOUT1;
	data_p->dispopts_s.view = 1;
	data_p->dispopts_s.layout = 1;
	main_layout(data_p);
}

void	goto_layout_2(t_data *data_p)
{
	if (data_p->dispopts_s.layout == 1)
		main_menu_loading("Intrusion en cours...");
	data_p->dispopts_s.view = 2;
	if (open_usb_port(&data_p->portopts_s))
	{
		save_error(data_p->dispopts_s.bomb_output, BIG_BUFFER, " ! " BIN_NAME,
			"port error");
		return ;
	}
	safer_strncpy(data_p->dispopts_s.bomb_output, "Acces autorise, "
		"connecte via port USB *Bienvenue ^^*\n", BOMBOUT_BUFFER, 128);
	data_p->dispopts_s.cmd_output = MSG_LAYOUT2;
	data_p->dispopts_s.layout = 2;
	main_layout(data_p);
}

void	goto_layout_3(t_data *data_p)
{
	if (data_p->dispopts_s.layout == 2)
		main_menu_loading("Scan des fichiers de la bombe...");
	safer_strncpy(data_p->dispopts_s.bomb_output,
		"installed modules\n\n"
		" * DISPLAY CONTROLLER (UNO_R3)\t- [ID:LS3249] - Communication protocol : Wire l2C\n"
		" * TIME_CONTROL_UNIT (ELEGO_R3)\t- [ID:CR57a5] - ACCESS LOCKED\n"
		" * DATA_STORAGE (ESP32_V4)\t- [ID:PK84u9] - Firewall breach\n"
		" * MOTOR_ACTUATOR (Servo motor)\t- [ID:RM3oRF] - ACCESS RESTRICTED\n",
		BOMBOUT_BUFFER, 512);
	data_p->dispopts_s.cmd_output = MSG_LAYOUT3;
	data_p->dispopts_s.view = 2;
	data_p->dispopts_s.layout = 3;
	main_layout(data_p);
}

void	goto_layout_firewalloff(t_data *data_p)
{
	data_p->dispopts_s.cmd_output = MSG_LAYOUT_FIREWALLOFF;
	data_p->dispopts_s.view = 2;
	data_p->dispopts_s.layout = 4;
	main_layout(data_p);
}

void	goto_layout_labyrinth(t_data *data_p)
{
	data_p->dispopts_s.cmd_output = MSG_LAYOUT_LAB;
	data_p->dispopts_s.view = 2;
	data_p->dispopts_s.layout = 5;
	main_layout(data_p);
}

void	goto_layout_bytes(t_data *data_p)
{
	data_p->dispopts_s.cmd_output = MSG_LAYOUT_BYTES;
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
	data_p->dispopts_s.cmd_output = MSG_LAYOUT_PASSWORD;
	data_p->dispopts_s.view = 2;
	data_p->dispopts_s.layout = 7;
	main_layout(data_p);
}
