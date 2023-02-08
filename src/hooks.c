/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hooks.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/07 21:35:08 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/08 00:49:01 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

//todo: switch to a system that logs keydown and key release instead of is_key_down bs
void	keyhook(mlx_key_data_t keydata, void *param)
{
	t_cub3d *data;
	float	magnitude;

	data = param;
	magnitude = 1;
	if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(data->mlx);
	// if (keydata.action == MLX_RELEASE)
	// 	return ;
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT_SHIFT))
		magnitude = 1.5;
	float rot_speed = 0.1;
	float movespeed = 0.1f;
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
	{
		if (data->level->map[(int)(data->player->y + data->player->side_dir_y * movespeed)][(int)data->player->x] != '1')
			data->player->y += data->player->side_dir_y * movespeed;
		if (data->level->map[(int)data->player->y][(int)(data->player->x + data->player->side_dir_x * movespeed)] != '1')
			data->player->x += data->player->side_dir_x * movespeed;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
	{
		if (data->level->map[(int)(data->player->y - data->player->side_dir_y * movespeed)][(int)data->player->x] != '1')
			data->player->y -= data->player->side_dir_y * movespeed;
		if (data->level->map[(int)data->player->y][(int)(data->player->x - data->player->side_dir_x * movespeed)] != '1')
			data->player->x -= data->player->side_dir_x * movespeed;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
	{
		if (data->level->map[(int)(data->player->y + data->player->dir_y * movespeed)][(int)data->player->x] != '1')
			data->player->y += data->player->dir_y * movespeed;
		if (data->level->map[(int)data->player->y][(int)(data->player->x + data->player->dir_x * movespeed)] != '1')
			data->player->x += data->player->dir_x * movespeed;

	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
	{
		
		if (data->level->map[(int)(data->player->y - data->player->dir_y * movespeed)][(int)data->player->x] != '1')
			data->player->y -= data->player->dir_y * movespeed;
		if (data->level->map[(int)data->player->y][(int)(data->player->x - data->player->dir_x * movespeed)] != '1')
			data->player->x -= data->player->dir_x * movespeed;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT_CONTROL))
	{
		data->crouching = !data->crouching;
	}
}

void	mousehook(mouse_key_t button, action_t action, modifier_key_t mods, void *param)
{
	t_cub3d *data;

	data = param;
}

void	cursorhook(double xpos, double ypos, void *param)
{
	t_cub3d *data;
	data = param;

	if (xpos != WIDTH / 2)
	{
		double turn = (WIDTH / 2) - xpos;
		turn /= 50;

		double oldDirX = data->player->dir_x;
		data->player->dir_x = data->player->dir_x * cos(turn) - data->player->dir_y * sin(turn);
		data->player->dir_y = oldDirX * sin(turn) + data->player->dir_y * cos(turn);

		double old_side_dir_x = data->player->side_dir_x;
		data->player->side_dir_x = data->player->side_dir_x * cos(turn) - data->player->side_dir_y * sin(turn);
		data->player->side_dir_y = old_side_dir_x * sin(turn) + data->player->side_dir_y * cos(turn);

		double oldPlaneX = data->player->plane_x;
		data->player->plane_x = data->player->plane_x * cos(turn) - data->player->plane_y * sin(turn);
		data->player->plane_y = oldPlaneX * sin(turn) + data->player->plane_y * cos(turn);
	}
	if (ypos != HEIGHT / 2)
	{
		
		double turn = (HEIGHT / 2) - ypos;
		// turn /= 5;

		data->pitch += turn;
		if (data->pitch >  (HEIGHT / 3))
			data->pitch =  (HEIGHT / 3);
		if (data->pitch < (-HEIGHT / 3))
			data->pitch = (-HEIGHT / 3);
	}
	mlx_set_mouse_pos(data->mlx, WIDTH / 2, HEIGHT / 2);
}

void	loophook(void *param)
{
	t_cub3d	*data;

	data = param;
	draw_3d(data);
	bad_draw(data);
}
