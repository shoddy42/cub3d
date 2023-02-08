/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   raycasting.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/08 01:22:25 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/08 01:41:47 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	hit_wall(t_draw *draw, t_cub3d *data)
{
	bool	hit;

	hit = false;
	while (hit == false)
	{
		if (draw->side_dist_x < draw->side_dist_y)
		{
			draw->side_dist_x += draw->delta_dist_x;
			draw->map_x += draw->step_x;
			draw->side = 0;
		}
		else
		{
			draw->side_dist_y += draw->delta_dist_y;
			draw->map_y += draw->step_y;
			draw->side = 1;
		}
		if (data->level->map[draw->map_y][draw->map_x] == '1')
			hit = true;
	}
	if (draw->side == 0)
		draw->wall_dist = (draw->side_dist_x - draw->delta_dist_x);
	else
		draw->wall_dist = (draw->side_dist_y - draw->delta_dist_y);
}

void	calc_wall_height(t_draw *draw, t_cub3d *data)
{
	draw->line_height = (int)(HEIGHT / draw->wall_dist);
	draw->line_start = -draw->line_height / 2 + HEIGHT / 2;
	draw->line_end = draw->line_height / 2 + HEIGHT / 2;
	draw->line_end += data->pitch;
	draw->line_start += data->pitch;
	if (data->crouching == true)
	{
		draw->line_end += 1;
		draw->line_start += 1;
	}
	if (draw->line_start < 0)
		draw->line_start = 0;
	if (draw->line_end >= HEIGHT)
		draw->line_end = HEIGHT -1;
}

void	tex_setup(t_draw *draw, t_cub3d *data)
{
	if (draw->side == 0)
		draw->wall_x = data->player->y + draw->wall_dist * draw->ray_dir_y;
	else
		draw->wall_x = data->player->x + draw->wall_dist * draw->ray_dir_x;
	draw->wall_x -= floor((draw->wall_x));
	draw->tex_x = (int)(draw->wall_x * data->tex->width);
	if (draw->side == 0 && draw->ray_dir_x > 0)
		draw->tex_x = data->tex->width - draw->tex_x - 1;
	if (draw->side == 1 && draw->ray_dir_y < 0)
		draw->tex_x = data->tex->width - draw->tex_x - 1;
	if (draw->side == 1 && draw->ray_dir_y < 0)
		draw->side = 2;
	if (draw->side == 0 && draw->ray_dir_x < 0)
		draw->side = 3;
	draw->step = 1.0 * data->tex->height / draw->line_height;
	draw->tex_pos = (draw->line_start - data->pitch - HEIGHT / 2 \
					+ draw->line_height / 2) * draw->step;
}

t_col	get_col(t_draw *draw, t_cub3d *data)
{
	t_col		colour;

	draw->tex_y = (int)draw->tex_pos & (int)(data->tex->pixels - 1); //dunno what this for tbh, might need?
	draw->tex_pos += draw->step;
	colour.r = (int)data->tex[draw->side].pixels[(data->tex->height
			* draw->tex_y + draw->tex_x) * data->tex->bytes_per_pixel];
	colour.g = (int)data->tex[draw->side].pixels[(data->tex->height
			* draw->tex_y + draw->tex_x) * data->tex->bytes_per_pixel + 1];
	colour.b = (int)data->tex[draw->side].pixels[(data->tex->height
			* draw->tex_y + draw->tex_x) * data->tex->bytes_per_pixel + 2];
	colour.a = (int)data->tex[draw->side].pixels[(data->tex->height
			* draw->tex_y + draw->tex_x) * data->tex->bytes_per_pixel + 3];
	if (draw->wall_dist > 1)
	{
		colour.r -= draw->wall_dist * 5;
		colour.g -= draw->wall_dist * 5;
		colour.b -= draw->wall_dist * 5;
		colour.a -= draw->wall_dist * 5;
	}
	return (colour);
}

void	draw_3d(t_cub3d *data)
{
	t_player	*player;
	t_col		buffer[HEIGHT + 1];
	t_draw		draw;
	int			x;
	int			y;

	// ft_bzero(data->img->pixels, WIDTH * HEIGHT * sizeof(unsigned int));
	player = data->player;
	x = 0;
	while (x < WIDTH)
	{
		setup_ray(&draw, x, data);
		get_step(&draw, data);
		hit_wall(&draw, data);
		calc_wall_height(&draw, data);
		tex_setup(&draw, data);
		draw.buffer_idx = -1;
		y = draw.line_start;
		while (y < draw.line_end)
		{
			buffer[++draw.buffer_idx] = get_col(&draw, data);
			y++;
		}
		draw_buffer(buffer, draw.line_start, draw.line_end, draw.buffer_idx, x, data);
		x++;
	}
	draw_cursor(data);
}
