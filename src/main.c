/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/08 19:11:59 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/30 16:49:08 by auzochuk      ########   odam.nl         */
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
	float max;
	float step_x = bx - ax;
	float step_y = by - ay;
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
	int i = 0;

	while (i < SCALE - 1)
	{
		i++;
		bresenham(x * SCALE, y * SCALE + i, (x + 1) * SCALE - 1, y * SCALE + i, data, color);
	}
}

void	draw_player(t_player *player, t_cub3d *data)
{
	int i;

	i = 0;
	// printf("lpayer x = %f\n lpayer y = %f \n", player->x, player->y);
	while (i < SCALE / 4)
	{
		i++;
		bresenham(player->x, player->y + i, player->x + (SCALE / 4), player->y + i, data, 0xFF0000FF);
		// bresenham(player->x * SCALE / 2, player->y * SCALE / 2 + i, (player->x + 1) * SCALE / 2, player->y * SCALE / 2 + i, data, 0xFF0000FF);
	}

}

void	bad_draw(t_cub3d *data)
{
	int x = -1;
	int y = -1;

	ft_bzero(data->img->pixels, WIDTH * HEIGHT * sizeof(unsigned int));
	while (++y < data->level->height)
	{
		x = -1;
		while (++x < data->level->width)
		{
			if (data->level->map[y][x] == '1')
				draw_square(x, y, data, 0xFFFFFFFF);
			else if (data->level->map[y][x] == '0')
				draw_square(x, y, data, 0x999999FF);
		}
	}
	draw_player(data->player, data);
}

//todo: switch to a system that logs keydown and key release instead of is_key_down bs
void	keyhook(mlx_key_data_t keydata, void *param)
{
	t_cub3d *data;
	float	magnitude;

	data = param;
	magnitude = 1;
	if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(data->mlx);
	// if (keydata.action == MLX_RELEASE)
	// 	return ;
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT_SHIFT))
		magnitude = 1.5;
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
		data->player->x += 3 * magnitude;
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
		data->player->x -= 3 * magnitude;
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
		data->player->y -= 3 * magnitude;
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
		data->player->y += 3 * magnitude;
	bad_draw(data);
}

bool	init(char **av, t_cub3d *data)
{
	data->mlx = mlx_init(WIDTH, HEIGHT, av[1], false);
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);

	mlx_image_to_window(data->mlx, data->img, 0, 0);
	mlx_key_hook(data->mlx, &keyhook, data);
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
	{
		printf ("Parsley not good\n");
		printf ("player x: %f player y: %f\n", data.player->x, data.player->y);
		return (0);
	}
	init(av, &data);
	bad_draw(&data);
	mlx_loop(data.mlx);
	mlx_delete_image(data.mlx, data.img);
	mlx_terminate(data.mlx);
	return (0);
}
