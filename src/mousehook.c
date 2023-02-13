/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mousehook.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/13 07:34:16 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/13 12:56:11 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	kbm_turn_side(t_cub3d *data, int dir)
{
	t_player	*player;
	double		turn;
	double		old_dir_x;
	double		old_plane_x;
	double		old_side_dir_x;

	player = data->player;
	turn = 0.1f * dir;
	old_dir_x = data->player->dir[X];
	player->dir[X] = player->dir[X] * cos(turn) - player->dir[Y] * sin(turn);
	player->dir[Y] = old_dir_x * sin(turn) + player->dir[Y] * cos(turn);
	old_side_dir_x = player->side_dir[X];
	player->side_dir[X] = player->side_dir[X] * cos(turn)
		- player->side_dir[Y] * sin(turn);
	player->side_dir[Y] = old_side_dir_x * sin(turn)
		+ player->side_dir[Y] * cos(turn);
	old_plane_x = player->plane_x;
	player->plane_x = player->plane_x * cos(turn) - player->plane_y * sin(turn);
	player->plane_y = old_plane_x * sin(turn) + player->plane_y * cos(turn);
}

void	mouse_turn_side(t_cub3d *data, double xpos)
{
	t_player	*player;
	double		turn;
	double		old_dir_x;
	double		old_plane_x;
	double		old_side_dir_x;

	player = data->player;
	turn = (WIDTH / 2) - xpos;
	turn /= 50;
	old_dir_x = data->player->dir[X];
	player->dir[X] = player->dir[X] * cos(turn) - player->dir[Y] * sin(turn);
	player->dir[Y] = old_dir_x * sin(turn) + player->dir[Y] * cos(turn);
	old_side_dir_x = player->side_dir[X];
	player->side_dir[X] = player->side_dir[X] * cos(turn)
		- player->side_dir[Y] * sin(turn);
	player->side_dir[Y] = old_side_dir_x * sin(turn)
		+ player->side_dir[Y] * cos(turn);
	old_plane_x = player->plane_x;
	player->plane_x = player->plane_x * cos(turn) - player->plane_y * sin(turn);
	player->plane_y = old_plane_x * sin(turn) + player->plane_y * cos(turn);
}

void	cursorhook(double xpos, double ypos, void *param)
{
	t_cub3d	*data;
	double	turn;

	data = param;
	if (xpos != WIDTH / 2)
		mouse_turn_side(data, xpos);
	if (ypos != HEIGHT / 2)
	{		
		turn = (HEIGHT / 2) - ypos;
		data->pitch += turn;
		if (data->pitch > (HEIGHT / 3))
			data->pitch = (HEIGHT / 3);
		if (data->pitch < (-HEIGHT / 3))
			data->pitch = (-HEIGHT / 3);
	}
	mlx_set_mouse_pos(data->mlx, WIDTH / 2, HEIGHT / 2);
}

// void	mousehook(mouse_key_t button, action_t action,
// 	modifier_key_t mods, void *param)
// {
// 	t_cub3d	*data;

// 	data = param;
// }
