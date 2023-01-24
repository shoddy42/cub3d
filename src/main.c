/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/08 19:11:59 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/18 20:51:03 by auzochuk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	error_exit(char *msg, int error_code)
{
	printf ("%s\n", msg);
	exit (error_code);
}

int		open_map(char *file)
{
	int fd;

	if (ft_strlen(file) < 4)
		error_exit(RED "filename too short noob\n", 6);
	if (ft_strncmp(file + ft_strlen(file) - 4, ".cub", 4) != 0)
		error_exit(RED "not .cub noob", 7);
	fd = open(file, O_RDONLY);
	if (fd < 0)
		error_exit (RED "not real file, noob\n", 8);
	return (fd);
}

void	alloc_map(char *file, t_cub3d *data)
{
	int		fd;
	char	*line;
	int		alloc;

	data->level= calloc(1, sizeof(t_map));
	if(!data->level)
		error_exit("allocation failed\n", 69);
	fd = open_map(file);
	while (get_next_line(fd, &line) > 0)
	{
		data->level->height++;
		free(line);
	}
	close(fd);
	data->level->map = ft_calloc(data->level->height + 1, sizeof(char *));
	fd = open_map(file);
	while (get_next_line(fd, &line))
		if (ft_strlen(line) > data->level->width)
			data->level->width = ft_strlen(line);
	alloc = -1;
	while(++alloc < data->level->height)
	{
		data->level->map[alloc] = calloc(data->level->width + 1, sizeof(char));
		if(!data->level->map[alloc])
			error_exit("allocation failed\n", 69);
	}
	close(fd);
}

bool	check_direction(t_parse parse, t_map *level)
{
	while (parse.x < level->width && parse.y < level->height && parse.x >= 0 && parse.y >= 0)
	{
		// if ()
		if (level->map[parse.y][parse.x] == '1')
		{
			// printf ("[%i][%i] found wall [%i][%i]  ?: [%c]\n", parse.dx, parse.dy, parse.x + 1, parse.y + 1, level->map[parse.y][parse.x]);
			return (true);
		}
		// printf ("path [%i][%i] ?: [%c]\n", parse.x + 1, parse.y + 1, level->map[parse.y][parse.x]);

		parse.x += parse.dx;
		parse.y += parse.dy;
	}
	// printf ("false2\n");
	// printf ("[%i][%i] ended at [%i][%i]  ?: [%c]\n", parse.dx, parse.dy, parse.x + 1, parse.y + 1, level->map[parse.y][parse.x]);
	return (false);
}

bool	check_cell(int x, int y, t_map *level)
{
	if (!check_direction((t_parse){x, y, 1, 0}, level)) //right
		return (false);
	if (!check_direction((t_parse){x, y, -1, 0}, level))	//left
		return (false);
	if (!check_direction((t_parse){x, y, 0, -1}, level))	//up
		return (false);
	if (!check_direction((t_parse){x, y, 0, 1}, level))	//down
		return (false);
	if (!check_direction((t_parse){x, y, 1, -1}, level)) //up right
		return (false);
	if (!check_direction((t_parse){x, y, 1, 1}, level)) //down right
		return (false);
	if (!check_direction((t_parse){x, y, -1, 1}, level)) //down left
		return (false);
	if (!check_direction((t_parse){x, y, -1, -1}, level)) //up left
		return (false);
	return (true);
}

bool	parse_map(char *file, t_cub3d *data)
{
	char	*line;
	int		fd;
	int		i;

	i = 0;
	alloc_map(file, data);
	fd = open_map(file);
	while (get_next_line(fd, &line) > 0)
	{
		ft_strlcpy(data->level->map[i], line, data->level->width + 1);
		free(line);
		i++;
	}
	int e = -1;
	int z = -1;
	while (data->level->map[++e])
	{
		z = -1;
		while (data->level->map[e][++z])
			printf ("%c", data->level->map[e][z]);
		printf ("\n");
	}
	int x = 0;
	int y = -1;
	while (data->level->map[++y])
	{
		x = -1;
		while (data->level->map[y][++x])
			if (data->level->map[y][x] == '0')
				if (check_cell(x, y, data->level) == false)
					error_exit("Invalid map noob\n", 92);
	}
	printf ("pog\n");
	return (true);
}

void	bad_draw(t_cub3d *data)
{
	int x = -1;
	int y = -1;

	ft_bzero(data->img->pixels, WIDTH * HEIGHT * sizeof(unsigned int));
	while (++y <= 500)
	{
		x = -1;
		while (++x <= 500)
		{
			if (x == 500 || y == 500 || x == 0 || y == 0 || (x == data->player->x && y == data->player->y))
				mlx_put_pixel(data->img, x, y, 0xFFFFFFFF);
		}
	}
}

void	keyhook(mlx_key_data_t keydata, void *param)
{
	t_cub3d *data;

	data = param;

	if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(data->mlx);
	if (keydata.key == MLX_KEY_D)
		data->player->x += 3;
	if (keydata.key == MLX_KEY_A)
		data->player->x -= 3;
	if (keydata.key == MLX_KEY_W)
		data->player->y -= 3;
	if (keydata.key == MLX_KEY_S)
		data->player->y += 3;
	bad_draw(data);
}

bool	init(char **av, t_cub3d *data)
{
	data->mlx = mlx_init(WIDTH, HEIGHT, av[1], false);
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->player = ft_calloc(1, sizeof(t_player));
	if (!data->player)
		return (false);
	mlx_image_to_window(data->mlx, data->img, 0, 0);
	mlx_key_hook(data->mlx, &keyhook, data);
	data->player->x = 250;
	data->player->y = 250;
	return (true);
}

int	main(int ac, char **av)
{
	t_cub3d data;

	if (ac != 2)
	{
		printf ("bad xD\n");
		exit(0);
	}
	if (!parse_map(av[1], &data))
		return (0);
	// init(av, &data);
	// bad_draw(&data);
	// mlx_loop(data.mlx);
	// mlx_delete_image(data.mlx, data.img);
	// mlx_terminate(data.mlx);
	return (0);
}
