/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/08 19:11:59 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/08 01:50:38 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	error_exit(char *msg, int error_code)
{
	printf ("%s\n", msg);
	exit (error_code);
}

void    bresenham(float ax, float ay, float bx, float by, t_cub3d *data, uint32_t color)
{
	float	max;
	float	step_x;
	float	step_y;

	step_x = bx - ax;
	step_y = by - ay;
	max = fmax(fabs(step_x), fabs(step_y));
	step_x /= max;
	step_y /= max;
	while ((int)(ax - bx) || (int)(ay - by))
	{
		if (ax < WIDTH && ax > 0 && ay < HEIGHT && ay > 0)
			mlx_put_pixel(data->img, (int)(ax), (int)(ay), color);
		ax += step_x;
		ay += step_y;
	}
}

void	draw_square(int	x, int y, t_cub3d *data, uint32_t color)
{
	int i = -1;
	int bx = (x + 1) * SCALE - 1;

	if (bx >= WIDTH / 2)
		bx = WIDTH / 2 - 1;
	while (++i < SCALE - 1)
	{
		bresenham(WIDTH - x * SCALE, HEIGHT - y * SCALE + i, WIDTH - bx, HEIGHT - y * SCALE + i, data, color);
	}
}

void	draw_player(t_player *player, t_cub3d *data)
{
	int i;

	i = 0;

	//todo: actually offsetting the player. currently the ray is accurate but the player model is shifted.
	while (i < SCALE / 4)
	{
		i++;
		bresenham((WIDTH - player->x * SCALE) - SCALE / 4, HEIGHT - (player->y - 1) * SCALE + i,  WIDTH - player->x * SCALE + (SCALE / 4), HEIGHT - (player->y - 1) * SCALE + i, data, 0xFF0000FF);
	}
	bresenham(WIDTH - player->x * SCALE, HEIGHT - (player->y - 1) * SCALE,  WIDTH - player->x * SCALE - player->dir_x * 50, HEIGHT - (player->y - 1) * SCALE - player->dir_y * 50, data, 0x0000FFFF);
}

void	draw_cursor(t_cub3d *data)
{
	int half_wit = WIDTH / 2;
	int half_height = HEIGHT / 2;

	bresenham(half_wit - 20, half_height - 20, half_wit + 20, half_height - 20, data, 0x0000FFFF);
	bresenham(half_wit - 20, half_height + 20, half_wit + 20, half_height + 20, data, 0x0000FFFF);
	bresenham(half_wit - 20, half_height - 20, half_wit - 20, half_height + 20, data, 0x0000FFFF);
	bresenham(half_wit + 20, half_height - 20, half_wit + 20, half_height + 20, data, 0x0000FFFF);
	mlx_put_pixel(data->img, half_wit, half_height, 0x0000FFFF);
	mlx_put_pixel(data->img, half_wit, half_height + 1, 0x0000FFFF);
	mlx_put_pixel(data->img, half_wit + 1, half_height, 0x0000FFFF);
	mlx_put_pixel(data->img, half_wit + 1, half_height + 1, 0x0000FFFF);
}

void	bad_draw(t_cub3d *data)
{
	int x = -1;
	int y = -1;

	
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

uint32_t	colour_to_uint(t_col col)
{
	uint32_t	ret;

	if (col.r < 0)
		col.r = 0;
	if (col.g < 0)
		col.g = 0;
	if (col.b < 0)
		col.b = 0;
	if (col.a < 0)
		col.a = 0;
	ret = col.a;
	ret += col.r << 24;
	ret += col.g << 16;
	ret += col.b << 8;
	return (ret);
}

void	draw_buffer(t_col *buffer, int start, int end, int i, int x, t_cub3d *data)
{
	int	idx;

	idx = end;
	//floor
	while (idx < HEIGHT)
	{
		mlx_put_pixel(data->img, x, idx, colour_to_uint(data->floor));
		idx++;
	}
	//draws ceiling
	idx = 0;
	while (idx <= start)
	{
		mlx_put_pixel(data->img, x, idx, colour_to_uint(data->ceiling));
		idx++;
	}
	while (i >= 0)
	{
		mlx_put_pixel(data->img, x, i + start, colour_to_uint(buffer[i]));
		i--;
	}
}



bool	init(char **av, t_cub3d *data)
{
	data->mlx = mlx_init(WIDTH, HEIGHT, av[1], false);
	mlx_set_cursor_mode(data->mlx, MLX_MOUSE_HIDDEN);

	mlx_set_mouse_pos(data->mlx, WIDTH / 2, HEIGHT / 2);
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->textures = ft_calloc(9, sizeof(t_texture));
	mlx_image_to_window(data->mlx, data->img, 0, 0);
	mlx_key_hook(data->mlx, &keyhook, data);
	mlx_mouse_hook(data->mlx, &mousehook, data);
	mlx_cursor_hook(data->mlx, &cursorhook, data);
	mlx_loop_hook(data->mlx, &loophook, data);
	return (true);
}

bool	fill_map(char *file, t_cub3d *data)
{
	int		i;
	int		fd;
	char	*line;

	// allocate map now todo: change + 10
	data->level->map = ft_calloc(data->end_of_map - data->start_of_map + 2, sizeof(char *));
	if (!data->level->map)
		error_exit("Failed to allocate level->map\n", 88);
	printf ("allocated this big: [%i]\n", data->end_of_map - data->start_of_map + 2);
	i = -1;
	while (++i <= data->end_of_map - data->start_of_map)
	{
		//2?
		data->level->map[i] = ft_calloc(data->level->width + 2, sizeof(char));
		if (!data->level->map[i])
			error_exit("map[x] allocation failed\n", 69);
	}
	fd = open_map(file, data);
	i = 0;
	//skip not map
	while (i < data->start_of_map && get_next_line(fd, &line))
	{
		i++;
		if (line)
			free(line);
	}
	while (i <= data->end_of_map && get_next_line(fd, &line))
	{
		//2?
		ft_strlcpy(data->level->map[i - data->start_of_map], line, data->level->width + 2);
		printf ("?: %s\n", data->level->map[i - data->start_of_map]);
		free(line);
		i++;
	}
	data->level->height = data->end_of_map - data->start_of_map + 1;
	return (true);
}

//todo: change **av to *file
bool	init_map(char **av, t_cub3d *data)
{
	int		fd;
	char	*line;

	fd = open_map(av[1], data);
	data->level = calloc(1, sizeof(t_map));
	if (!data->level)
		error_exit("allocation failed\n", 69);
	while (get_next_line(fd, &line) && fill_element(line, data))
	{
		data->start_of_map++;
		if (line)
			free(line);
	}
	if (!line)
		error_exit("no map!\n", 25);
	if (ft_strlen(line) > data->level->width)
		data->level->width = ft_strlen(line);
	if (line)
		free(line);
	while (get_next_line(fd, &line) && ft_strlen(line) > 0)
	{
		data->end_of_map++;
		if (ft_strlen(line) > data->level->width)
			data->level->width = ft_strlen(line);
		if (line)
			free(line);
	}
	data->end_of_map += data->start_of_map;
	if (line)
		free(line);
	fill_map(av[1], data);
	return (true);
}

int	main(int ac, char **av)
{
	t_cub3d	data;

	if (ac != 2)
	{
		printf ("bad xD\n");
		exit(0);
	}
	ft_bzero(&data, sizeof(t_cub3d));
	//todo: reorder init steps. init memory needed to init map > init map > init mlx
	data.tex = ft_calloc(5, sizeof(mlx_texture_t));
	init_map(av, &data);
	init(av, &data);
	if (!parse_map(av[1], &data))
	{
		printf ("Parsley not good\n");
		printf ("player x: %f player y: %f\n", data.player->x, data.player->y);
		return (0);
	}
	draw_3d(&data);
	bad_draw(&data);
	mlx_loop(data.mlx);
	mlx_delete_image(data.mlx, data.img);
	mlx_terminate(data.mlx);
	return (0);
}
