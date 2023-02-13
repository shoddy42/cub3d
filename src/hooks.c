/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hooks.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/07 21:35:08 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/13 10:37:08 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	keyhook(mlx_key_data_t keydata, void *param)
{
	t_cub3d	*data;
	float	magnitude;

	data = param;
	if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(data->mlx);
	magnitude = 1;
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT_SHIFT))
		magnitude = 1.5;
	float movespeed = 0.1f * magnitude;
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
	{
		if (data->level->map[(int)(data->player->pos[Y] + data->player->side_dir[Y] * movespeed)][(int)data->player->pos[X]] != '1')
			data->player->pos[Y] += data->player->side_dir[Y] * movespeed;
		if (data->level->map[(int)data->player->pos[Y]][(int)(data->player->pos[X] + data->player->side_dir[X] * movespeed)] != '1')
			data->player->pos[X] += data->player->side_dir[X] * movespeed;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
	{
		if (data->level->map[(int)(data->player->pos[Y] - data->player->side_dir[Y] * movespeed)][(int)data->player->pos[X]] != '1')
			data->player->pos[Y] -= data->player->side_dir[Y] * movespeed;
		if (data->level->map[(int)data->player->pos[Y]][(int)(data->player->pos[X] - data->player->side_dir[X] * movespeed)] != '1')
			data->player->pos[X] -= data->player->side_dir[X] * movespeed;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
	{
		if (data->level->map[(int)(data->player->pos[Y] + data->player->dir[Y] * movespeed)][(int)data->player->pos[X]] != '1')
			data->player->pos[Y] += data->player->dir[Y] * movespeed;
		if (data->level->map[(int)data->player->pos[Y]][(int)(data->player->pos[X] + data->player->dir[X] * movespeed)] != '1')
			data->player->pos[X] += data->player->dir[X] * movespeed;

	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
	{
		if (data->level->map[(int)(data->player->pos[Y] - data->player->dir[Y] * movespeed)][(int)data->player->pos[X]] != '1')
			data->player->pos[Y] -= data->player->dir[Y] * movespeed;
		if (data->level->map[(int)data->player->pos[Y]][(int)(data->player->pos[X] - data->player->dir[X] * movespeed)] != '1')
			data->player->pos[X] -= data->player->dir[X] * movespeed;
	}
}

void	loophook(void *param)
{
	t_cub3d	*data;

	data = param;
	draw_3d(data);
	bad_draw(data);
}
