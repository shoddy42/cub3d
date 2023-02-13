/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hooks.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/07 21:35:08 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/13 12:44:39 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	keyhook(mlx_key_data_t keydata, void *param)
{
	t_cub3d	*data;
	float	magnitude;
	float	movespeed;

	data = param;
	if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(data->mlx);
	magnitude = 1;
	movespeed = 0.1f * magnitude;
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT_SHIFT))
		magnitude = 1.5;
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
		move_right(data, data->player, data->level->map, movespeed);
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
		move_left(data, data->player, data->level->map, movespeed);
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
		move_forward(data, data->player, data->level->map, movespeed);
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
		move_back(data, data->player, data->level->map, movespeed);
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
		kbm_turn_side(data, 1);
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT))
		kbm_turn_side(data, -1);
}

void	loophook(void *param)
{
	t_cub3d	*data;

	data = param;
	draw_3d(data);
	draw_minimap(data);
}
