/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/08 19:11:59 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/17 14:54:07 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "include/cub3d.h"

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

bool	alloc_map(char *file, t_cub3d *data)
{
	int		fd;
	char	*line;
	int		alloc;

	alloc = 0;
	fd = open_map(file);
	while (get_next_line(fd, &line) > 0)
		alloc++;
	close(fd);
	data->map = ft_calloc(alloc + 1, sizeof(char *));

	fd = open_map(file);
	int i = -1;
	while (alloc > 0)
	{
		alloc = get_next_line(fd, &line);
		
	}
	return (true);
}

bool	parse_map(char *file, t_cub3d *data)
{
	int		fd;
	char	*line;

	alloc_map(file, data);
	fd = open_map(file);
	while (get_next_line(fd, &line) > 0)
	{
		
		
	}
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
	init(av, &data);
	bad_draw(&data);
	mlx_loop(data.mlx);
	mlx_delete_image(data.mlx, data.img);
	mlx_terminate(data.mlx);
	return (0);
}
