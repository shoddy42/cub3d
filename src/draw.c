/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   draw.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/13 06:19:28 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/13 12:13:10 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	bresenham(t_point2d a, t_point2d b, t_cub3d *data, uint32_t color)
{
	float		max;
	t_point2d	step;

	step[X] = b[X] - a[X];
	step[Y] = b[Y] - a[Y];
	max = fmax(fabs(step[X]), fabs(step[Y]));
	step[X] /= max;
	step[Y] /= max;
	while ((int)(a[X] - b[X]) || (int)(a[Y] - b[Y]))
	{
		if (a[X] < WIDTH && a[X] > 0 && a[Y] < HEIGHT && a[Y] > 0)
			mlx_put_pixel(data->img, (int)(a[X]), (int)(a[Y]), color);
		a[X] += step[X];
		a[Y] += step[Y];
	}
}

void	draw_square(int x, int y, t_cub3d *data, uint32_t color)
{
	int	i;
	int	bx;
	int	newx;
	int	newy;

	i = -1;
	bx = (x + 1) * data->scale - 1;
	newy = HEIGHT - (data->level->height * data->scale) + (y * data->scale);
	newx = WIDTH - (data->level->width * data->scale) + (x * data->scale);
	while (++i < data->scale - 1)
	{
		bresenham((t_point2d){newx, newy + i},
			(t_point2d){newx + (1 * data->scale) - 1, newy + i},
			data, color);
	}
}

void	draw_player(t_player *player, t_cub3d *data)
{
	int	i;

	i = -data->scale / 4;
	while (i < data->scale / 4)
	{
		i++;
		bresenham((t_point2d){WIDTH - (data->level->width * data->scale) + (player->pos[X] * data->scale) - data->scale / 4,
			HEIGHT - (data->level->height * data->scale) + player->pos[Y] * data->scale + i},
			(t_point2d){WIDTH - (data->level->width * data->scale) + (player->pos[X] * data->scale) + (data->scale / 4),
			HEIGHT - (data->level->height * data->scale) + player->pos[Y] * data->scale + i},
			data, 0xFF0000FF);
	}
	bresenham((t_point2d){WIDTH - (data->level->width * data->scale) + (player->pos[X] * data->scale),
		HEIGHT - (data->level->height * data->scale) + player->pos[Y] * data->scale},
		(t_point2d){WIDTH - (data->level->width * data->scale) + (player->pos[X] * data->scale) + player->dir[X] * (data->scale * 2),
		HEIGHT - (data->level->height * data->scale) + player->pos[Y] * data->scale + player->dir[Y] * (data->scale * 2)},
		data, 0x0000FFFF);
}

void	draw_cursor(t_cub3d *data)
{
	int	half_wit;
	int	half_height;

	half_wit = (int)WIDTH / 2;
	half_height = (int)HEIGHT / 2;
	bresenham((t_point2d){half_wit - 20, half_height - 20},
		(t_point2d){half_wit + 20, half_height - 20}, data, 0xFF00FFFF);
	bresenham((t_point2d){half_wit - 20, half_height + 20},
		(t_point2d){half_wit + 20, half_height + 20}, data, 0xFF00FFFF);
	bresenham((t_point2d){half_wit - 20, half_height - 20},
		(t_point2d){half_wit - 20, half_height + 20}, data, 0xFF00FFFF);
	bresenham((t_point2d){half_wit + 20, half_height - 20},
		(t_point2d){half_wit + 20, half_height + 20}, data, 0xFF00FFFF);
	mlx_put_pixel(data->img, half_wit, half_height, 0xFF00FFFF);
	mlx_put_pixel(data->img, half_wit, half_height + 1, 0xFF00FFFF);
	mlx_put_pixel(data->img, half_wit + 1, half_height, 0xFF00FFFF);
	mlx_put_pixel(data->img, half_wit + 1, half_height + 1, 0xFF00FFFF);
}

//minimap
void	bad_draw(t_cub3d *data)
{
	int	x;
	int	y;

	x = -1;
	y = -1;
	while (++y < data->level->height)
	{
		x = -1;
		while (++x < data->level->width)
		{
			if (data->level->map[y][x] == '1')
				draw_square(x, y, data, 0xFFFFFF99);
			else if (data->level->map[y][x] == '0')
				draw_square(x, y, data, 0x99999999);
		}
	}
	draw_cursor(data);
	draw_player(data->player, data);
}

void	draw_buffer(t_col *buffer, int x, t_draw *draw, t_cub3d *data)
{
	int	idx;

	idx = draw->line_end;
	while (idx < HEIGHT)
	{
		mlx_put_pixel(data->img, x, idx, colour_to_uint(data->floor));
		idx++;
	}
	idx = 0;
	while (idx <= draw->line_start)
	{
		mlx_put_pixel(data->img, x, idx, colour_to_uint(data->ceiling));
		idx++;
	}
	while (draw->buffer_idx >= 0)
	{
		mlx_put_pixel(data->img, x, draw->buffer_idx + draw->line_start,
			colour_to_uint(buffer[draw->buffer_idx]));
		draw->buffer_idx--;
	}
}
