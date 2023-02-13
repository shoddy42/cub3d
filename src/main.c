/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/08 19:11:59 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/13 12:00:36 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	error_exit(char *msg, int error_code)
{
	printf ("%s\n", msg);
	exit (error_code);
}

void	init(char **av, t_cub3d *data)
{
	data->tex = ft_calloc(4, sizeof(mlx_texture_t));
	if (!data->tex)
		error_exit("Error: Failed to allocate tex\n", 1);
	init_map(av, data);
	parse_map(av[1], data);
	data->scale = WIDTH / data->level->width / 4;
	if (HEIGHT / data->level->height / 2 < data->scale)
		data->scale = HEIGHT / data->level->height / 2;
	if (data->scale < 4)
		data->scale = 4;
	printf ("scale? %i\n", data->scale);
	data->mlx = mlx_init(WIDTH, HEIGHT, av[1], false);
	if (!data->mlx)
		error_exit("Error: Failed to allocate mlx instance.\n", 1);
	mlx_set_cursor_mode(data->mlx, MLX_MOUSE_HIDDEN);
	mlx_set_mouse_pos(data->mlx, WIDTH / 2, HEIGHT / 2);
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	if (!data->img)
		error_exit("Error: Failed to allocate mlx image\n", 1);
	if (mlx_image_to_window(data->mlx, data->img, 0, 0) == -1)
		error_exit("Error: Failed to push image to window\n", 1);
	mlx_key_hook(data->mlx, &keyhook, data);
	mlx_mouse_hook(data->mlx, &mousehook, data);
	mlx_cursor_hook(data->mlx, &cursorhook, data);
	if (mlx_loop_hook(data->mlx, &loophook, data) == false)
		error_exit("Error: Failed to initialise loop hook\n", 1);
	return ;
}

int	main(int ac, char **av)
{
	t_cub3d	data;

	if (ac != 2)
		error_exit("Error: Usage ./cub3d <map location>.cub\n", 1);
	ft_bzero(&data, sizeof(t_cub3d));
	init(av, &data);
	mlx_loop(data.mlx);
	mlx_delete_image(data.mlx, data.img);
	mlx_terminate(data.mlx);
	return (0);
}
