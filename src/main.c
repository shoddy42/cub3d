/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/08 19:11:59 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/30 22:34:51 by wkonings      ########   odam.nl         */
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

	int i = -1;
	int bx = (x + 1) * SCALE - 1;

	if (bx >= WIDTH / 2)
		bx = WIDTH / 2 - 1;
	while (++i < SCALE - 1)
	{
		bresenham(x * SCALE, y * SCALE + i, bx, y * SCALE + i, data, color);
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
		bresenham(player->x * SCALE, player->y * SCALE + i, player->x * SCALE + (SCALE / 4), player->y * SCALE + i, data, 0xFF0000FF);
	}
	bresenham(player->x * SCALE, player->y * SCALE, player->x * SCALE + player->dir_x * 50, player->y * SCALE + player->dir_y * 50, data, 0x0000FFFF);
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
			// if (data->level->map[y][x] == '1')
			// 	draw_square(x, y, data, 0xFFFFFFFF);
			// else if (data->level->map[y][x] == '0')
			// 	draw_square(x, y, data, 0x999999FF);
		}
	}
	// bresenham(WIDTH / 2, 0, WIDTH / 2, HEIGHT, data, 0xFFFFFFFF);
	// draw_player(data->player, data);
}

void	draw_3d(t_cub3d *data)
{
	t_player *player = data->player;

	int x = 0;
	while (x < WIDTH)
	{
		double camera_x = 2 * x / (double)WIDTH - 1;
		double ray_dir_x = player->dir_x + player->plane_x * camera_x;
		double ray_dir_y = player->dir_y + player->plane_y * camera_x;

		
		int map_x = (int)player->x;
		int map_y = (int)player->y;
		// printf ("START: map[%i][%i]\n", map_y, map_x);

		double side_dist_x;
		double side_dist_y;
		double delta_dist_x = (ray_dir_x == 0) ? 1e30 : fabs(1 / ray_dir_x);
		double delta_dist_y = (ray_dir_y == 0) ? 1e30 : fabs(1 / ray_dir_y);
		double wall_dist;


		// printf ("vars px: %f py: %f pdx: %f pdy: %f\n", player->x, player->y, player->dir_x, player->dir_y);
		// printf ("ray dir x: %f ray dir y: %f\n", ray_dir_x, ray_dir_y);
		// printf ("delta dist x: %f delta dist y: %f\n", delta_dist_x, delta_dist_y);
		
		int step_x;
		int step_y;
		bool hit = false;
		int side;

		if (ray_dir_x < 0)
		{
			step_x = -1;
			side_dist_x = (player->x - map_x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			side_dist_x = (map_x + 1.0 - player->x) * delta_dist_x;
		}
		if (ray_dir_y < 0)
		{
			step_y = -1;
			side_dist_y = (player->y - map_y) * delta_dist_y;
		}
		else
		{
			step_y = 1;
			side_dist_y = (map_y + 1.0 - player->y) * delta_dist_y;
		}
		//DDA
		while (hit == false)
		{
			if (side_dist_x < side_dist_y)
			{
				side_dist_x += delta_dist_x;
				map_x += step_x;
				side = 0;
			}
			else
			{
				side_dist_y += delta_dist_y;
				map_y += step_y;
				side = 1;
			}
			if (data->level->map[map_y][map_x] == '1')
				hit = true;
		}

		if (side == 0)
			wall_dist = (side_dist_x - delta_dist_x);
		else
			wall_dist = (side_dist_y - delta_dist_y);
		
		// printf ("dist = %f", wall_dist);
		int line_height = (int)(HEIGHT / wall_dist);
		// printf ("line height = %i\n", line_height);
		int draw_start = -line_height / 2 + HEIGHT / 2;
		if (draw_start < 0)
			draw_start = 0;
		int draw_end = line_height / 2 + HEIGHT / 2;
		if (draw_end >= HEIGHT)
			draw_end = HEIGHT -1;

		if (side == 0)
			bresenham(x, draw_start, x, draw_end, data, 0x990000FF);
		else
			bresenham(x, draw_start, x, draw_end, data, 0xFF0000FF);
		x++;
	}
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
	float rot_speed = 0.1;
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
	{
      double oldDirX = data->player->dir_x;
      data->player->dir_x = data->player->dir_x * cos(-rot_speed) - data->player->dir_y * sin(-rot_speed);
      data->player->dir_y = oldDirX * sin(-rot_speed) + data->player->dir_y * cos(-rot_speed);
      double oldPlaneX = data->player->plane_x;
      data->player->plane_x = data->player->plane_x * cos(-rot_speed) - data->player->plane_y * sin(-rot_speed);
      data->player->plane_y = oldPlaneX * sin(-rot_speed) + data->player->plane_y * cos(-rot_speed);
		
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
	{
      double oldDirX = data->player->dir_x;
      data->player->dir_x = data->player->dir_x * cos(rot_speed) - data->player->dir_y * sin(rot_speed);
      data->player->dir_y = oldDirX * sin(rot_speed) + data->player->dir_y * cos(rot_speed);
      double oldPlaneX = data->player->plane_x;
      data->player->plane_x = data->player->plane_x * cos(rot_speed) - data->player->plane_y * sin(rot_speed);
      data->player->plane_y = oldPlaneX * sin(rot_speed) + data->player->plane_y * cos(rot_speed);
	}
	float movespeed = 0.1f;
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
	{
		if (data->level->map[(int)(data->player->y + data->player->dir_y * movespeed)][(int)data->player->x] != '1')
			data->player->y += data->player->dir_y * movespeed;
		if (data->level->map[(int)data->player->y][(int)(data->player->x + data->player->dir_x * movespeed)] != '1')
			data->player->x += data->player->dir_x * movespeed;

	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
	{
		if (data->level->map[(int)(data->player->y - data->player->dir_y * movespeed)][(int)data->player->x] != '1')
			data->player->y -= data->player->dir_y * movespeed;
		if (data->level->map[(int)data->player->y][(int)(data->player->x - data->player->dir_x * movespeed)] != '1')
			data->player->x -= data->player->dir_x * movespeed;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT_CONTROL))
	{
		
	}
	bad_draw(data);
	draw_3d(data);
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
