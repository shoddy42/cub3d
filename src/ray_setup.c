/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_setup.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/08 01:21:36 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/13 07:59:14 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	setup_ray(t_draw *draw, int x, t_cub3d *data)
{
	t_player	*player;

	ft_bzero(draw, sizeof(t_draw));
	player = data->player;
	draw->camera_x = 2 * x / (double)WIDTH - 1;
	draw->ray_dir[X] = player->dir[X] + player->plane_x * draw->camera_x;
	draw->ray_dir[Y] = player->dir[Y] + player->plane_y * draw->camera_x;
	draw->map_x = (int)player->pos[X];
	draw->map_y = (int)player->pos[Y];
	if (draw->ray_dir[X] == 0)
		draw->delta_dist[X] = 1e30;
	else
		draw->delta_dist[X] = fabs(1 / draw->ray_dir[X]);
	if (draw->ray_dir[Y] == 0)
		draw->delta_dist[Y] = 1e30;
	else
		draw->delta_dist[Y] = fabs(1 / draw->ray_dir[Y]);
}

void	get_step(t_draw *draw, t_cub3d *data)
{
	t_player	*player;

	player = data->player;
	if (draw->ray_dir[X] < 0)
	{
		draw->step_x = -1;
		draw->side_dist[X] = (player->pos[X] - draw->map_x) * draw->delta_dist[X];
	}
	else
	{
		draw->step_x = 1;
		draw->side_dist[X] = (draw->map_x + 1.0 - player->pos[X])
			* draw->delta_dist[X];
	}
	if (draw->ray_dir[Y] < 0)
	{
		draw->step_y = -1;
		draw->side_dist[Y] = (player->pos[Y] - draw->map_y) * draw->delta_dist[Y];
	}
	else
	{
		draw->step_y = 1;
		draw->side_dist[Y] = (draw->map_y + 1.0 - player->pos[Y])
			* draw->delta_dist[Y];
	}
}
