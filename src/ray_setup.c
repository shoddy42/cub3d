/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_setup.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/08 01:21:36 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/08 01:36:37 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	setup_ray(t_draw *draw, int x, t_cub3d *data)
{
	t_player	*player;

	ft_bzero(draw, sizeof(t_draw));
	player = data->player;
	draw->camera_x = 2 * x / (double)WIDTH - 1;
	draw->ray_dir_x = player->dir_x + player->plane_x * draw->camera_x;
	draw->ray_dir_y = player->dir_y + player->plane_y * draw->camera_x;
	draw->map_x = (int)player->x;
	draw->map_y = (int)player->y;
	if (draw->ray_dir_x == 0)
		draw->delta_dist_x = 1e30;
	else
		draw->delta_dist_x = fabs(1 / draw->ray_dir_x);
	if (draw->ray_dir_y == 0)
		draw->delta_dist_y = 1e30;
	else
		draw->delta_dist_y = fabs(1 / draw->ray_dir_y);
}

void	get_step(t_draw *draw, t_cub3d *data)
{
	t_player	*player;

	player = data->player;
	if (draw->ray_dir_x < 0)
	{
		draw->step_x = -1;
		draw->side_dist_x = (player->x - draw->map_x) * draw->delta_dist_x;
	}
	else
	{
		draw->step_x = 1;
		draw->side_dist_x = (draw->map_x + 1.0 - player->x)
			* draw->delta_dist_x;
	}
	if (draw->ray_dir_y < 0)
	{
		draw->step_y = -1;
		draw->side_dist_y = (player->y - draw->map_y) * draw->delta_dist_y;
	}
	else
	{
		draw->step_y = 1;
		draw->side_dist_y = (draw->map_y + 1.0 - player->y)
			* draw->delta_dist_y;
	}
}
