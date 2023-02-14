/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   draw.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/13 06:19:28 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/14 08:06:15 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

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

void	draw_player(t_player *player, t_cub3d *data, int width, int height)
{
	int	offset;
	int	scale;

	scale = data->scale;
	offset = -data->scale / 4;
	while (offset < scale / 4)
	{
		offset++;
		bresenham((t_point2d){WIDTH - (width * scale)
			+ (player->pos[X] * scale) - scale / 4,
			HEIGHT - (height * scale) + (player->pos[Y] * scale) - offset},
			(t_point2d){WIDTH - (width * scale)
			+ (player->pos[X] * scale) + (scale / 4),
			HEIGHT - (height * scale) + (player->pos[Y] * scale) - offset},
			data, 0xFF0000FF);
	}
	bresenham((t_point2d){WIDTH - (width * scale) + (player->pos[X] * scale),
		HEIGHT - (height * scale) + player->pos[Y] * scale},
		(t_point2d){WIDTH - (width * scale) + (player->pos[X] * scale)
		+ player->dir[X] * (scale * 2),
		HEIGHT - (height * scale) + player->pos[Y] * scale
		+ player->dir[Y] * (scale * 2)},
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
void	draw_minimap(t_cub3d *data)
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
	draw_player(data->player, data, data->level->width, data->level->height);
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
