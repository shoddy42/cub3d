/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/08 19:11:59 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/30 19:14:02 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

double	deg_to_rad(const double a)
{
	return (a / 360.0f * 2.0f * M_PI);
}

double	rad_to_deg(const double a)
{
	return (a / (2.0f * M_PI) * 360.0f);
}

void	error_exit(char *msg, int error_code)
{
	printf ("%s\n", msg);
	exit (error_code);
}

// char 	**cub_hardcoded_map(void)
// {
// 	char **map;
// 	map = ft_calloc(20 + 1, sizeof(char *));
// 	map[0] =  ft_strdup("11111111111111111111");
// 	map[1] =  ft_strdup("10000000000000000001");
// 	map[2] =  ft_strdup("10000000000000000001");
// 	map[3] =  ft_strdup("10000000000000000001");
// 	map[4] =  ft_strdup("10000000000000000001");
// 	map[5] =  ft_strdup("10000000000000000001");
// 	map[6] =  ft_strdup("10000000001000000001");
// 	map[7] =  ft_strdup("10000000001000000001");
// 	map[8] =  ft_strdup("10000000001000000001");
// 	map[9] =  ft_strdup("10000000001000000001");
// 	map[10] = ft_strdup("10000000001000000001");
// 	map[11] = ft_strdup("10000000000000000001");
// 	map[12] = ft_strdup("10000000000000000001");
// 	map[13] = ft_strdup("10000000000000000001");
// 	map[14] = ft_strdup("10000000000000000001");
// 	map[15] = ft_strdup("10000000000000000001");
// 	map[16] = ft_strdup("10000000000000000001");
// 	map[17] = ft_strdup("10000000000000000001");
// 	map[18] = ft_strdup("10000000000000000001");
// 	map[19] = ft_strdup("11111111111111111111");
// 	return (map);
// }

char 	**cub_hardcoded_map(void)
{
	char **map;
	map = ft_calloc(8 + 1, sizeof(char *));
	map[0] =  ft_strdup("11111111");
	map[1] =  ft_strdup("10000001");
	map[2] =  ft_strdup("10000001");
	map[3] =  ft_strdup("10000001");
	map[4] =  ft_strdup("10001001");
	map[5] =  ft_strdup("10000001");
	map[6] =  ft_strdup("10000001");
	map[7] =  ft_strdup("11111111");
	map[8] =  ft_strdup("");
	return (map);
}

bool	is_wall(int x, int y, t_cub3d *data)
{
	x /= data->scale;
	y /= data->scale;
	if ((x < 8 && y < 8) && data->map[y][x] == '1')	
		return (true);
	return (false);
}

bool	is_inbounds(int x, int y, t_cub3d *data)
{
	if (x > 0 && y > 0)
		if (x < WIDTH && y < HEIGHT)
			return (true);
	return (false);
}

int fix_angle(int angle)
{
	if (angle > 359)
		angle -= 360;
	if (angle < 0)
		angle += 360;
	return (angle);
}

void	bresenham(float ax, float ay, float bx, float by, t_cub3d *data, uint32_t color)
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

void	ray(t_cub3d *data)
{
	int x = -1;

	while (++x < WIDTH / 2)
	{
		double camera_x = 2 * x / (double)WIDTH - 1;

		double ray_dir_x = data->player->dir_x + data->player->plane_x * camera_x;
		double ray_dir_y = data->player->dir_y + data->player->plane_y * camera_x;
		int map_x = (int)data->player->x;
		int map_y = (int)data->player->y;

		double	side_dist_x;
		double	side_dist_y;
		double	deltaDistX = (ray_dir_x == 0) ? 1e30 : fabs(1 / ray_dir_x);
		double	deltaDistY = (ray_dir_y == 0) ? 1e30 : fabs(1 / ray_dir_y);
		double	perp_wall_dist;
		
		int	step_x;
		int	step_y;
		int hit = 0;
		int side;

		if (ray_dir_x < 0)
		{
			step_x = -1;
			side_dist_x = (data->player->x - map_x) * deltaDistX;
		}
		else
		{
			step_x = 1;
			side_dist_x = (map_x + 1.0 - data->player->x) * deltaDistX;
		}
		if (ray_dir_y < 0)
		{
			step_y = -1;
			side_dist_y = (data->player->y - map_y) * deltaDistY;
		}
		else
		{
			step_y = 1;
			side_dist_y = (map_y + 1.0 - data->player->y) * deltaDistY;
		}
		// DDA time
		while (hit == 0)
		{
			if (side_dist_x < side_dist_y)
			{
				side_dist_x += deltaDistX;
				map_x += step_x;
				side = 0;
			}
			else
			{	
				side_dist_y += deltaDistY;
				map_y += step_y;
				side = 1;
			}
			if (map_y >= 0 && map_y < 8 && map_x >= 0 && map_x < 8)
				if (data->map[map_y][map_x] == '1')
					hit = 1;
		}
		if (side == 0)
			perp_wall_dist = (side_dist_x - deltaDistX);
		else
			perp_wall_dist = (side_dist_y - deltaDistY);
		int line_height = (int)(HEIGHT / perp_wall_dist);
		printf ("height: %i dist: %f\n", line_height, perp_wall_dist);
		int draw_start = -line_height / 2 + HEIGHT / 2;
		if (draw_start < 0)
			draw_start = 0;
		int draw_end = line_height / 2 + HEIGHT / 2;
		if (draw_end >= HEIGHT)
			draw_end = HEIGHT - 1;

		uint32_t col;
			col = 0xFF0000FF;
		if (side == 1)
			col = 0x990000FF;
		bresenham(x + (WIDTH / 2), draw_start, x + (WIDTH / 2), draw_end, data, col);
	}
	
}

void	draw_player(t_cub3d *data)
{
	int x;
	int y;

	y = (data->player->y * data->scale) - (data->scale / 8);
	while (y < (data->player->y * data->scale) + (data->scale / 8))
	{
		x = (data->player->x * data->scale) - (data->scale / 8);
		while (x < (data->player->x * data->scale) + (data->scale / 8))
		{
			if (is_inbounds(x, y, data))
				mlx_put_pixel(data->img, x, y, 0x999999FF);
			x++;
		}
		y++;
	}
	ray(data);
	bresenham(data->player->x * data->scale, data->player->y * data->scale, \
				data->player->x * data->scale + (data->player->dir_x * 100), \
				data->player->y * data->scale + (data->player->dir_y * 100), data, 0xFF0000FF);
}

void	bad_draw(t_cub3d *data)
{
	int x = -1;
	int y = -1;

	ft_bzero(data->img->pixels, WIDTH * HEIGHT * sizeof(unsigned int));
	while (++y < HEIGHT)
	{
		x = -1;
		while (++x < WIDTH)
		{
			if ((x % data->scale == 0 || y % data->scale == 0) && x < WIDTH / 2)
				mlx_put_pixel(data->img, x, y, 0x010101FF);
			if (is_wall(x, y, data) && x % data->scale != 0 && y % data->scale != 0)
				mlx_put_pixel(data->img, x, y, 0xFFFFFFFF);
		}
	}
	bresenham(512, 512, 512, 0, data, 0x999999FF);
	draw_player(data);
}

void	keyhook(mlx_key_data_t keydata, void *param)
{
	t_cub3d *data;

	data = param;

	if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(data->mlx);
	if (keydata.action == MLX_RELEASE)
		return ;
	float rotSpeed = 0.1;
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
	{
		double oldDirX = data->player->dir_x;
		data->player->dir_x = data->player->dir_x * cos(rotSpeed) - data->player->dir_y * sin(rotSpeed);
		data->player->dir_y = oldDirX * sin(rotSpeed) + data->player->dir_y * cos(rotSpeed);
		double oldPlaneX = data->player->plane_x;
		data->player->plane_x = data->player->plane_x * cos(rotSpeed) - data->player->plane_y * sin(rotSpeed);
		data->player->plane_y = oldPlaneX * sin(rotSpeed) + data->player->plane_y * cos(rotSpeed);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
	{
		double oldDirX = data->player->dir_x;
		data->player->dir_x = data->player->dir_x * cos(-rotSpeed) - data->player->dir_y * sin(-rotSpeed);
		data->player->dir_y = oldDirX * sin(-rotSpeed) + data->player->dir_y * cos(-rotSpeed);
		double oldPlaneX = data->player->plane_x;
		data->player->plane_x = data->player->plane_x * cos(-rotSpeed) - data->player->plane_y * sin(-rotSpeed);
		data->player->plane_y = oldPlaneX * sin(-rotSpeed) + data->player->plane_y * cos(-rotSpeed);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
	{
		printf ("pos now? [%f][%f]\n", data->player->x, data->player->y);
		if (data->player->x + data->player->dir_x * 0.1 < 6.99 && data->player->x + data->player->dir_x * 0.1 > 1.01)
		{
			data->player->x += data->player->dir_x * 0.1;
			data->player->y += data->player->dir_y * 0.1;
		}
		// if (data->player->x < 8 && data->player->x > 0 && data->player->y < 8 && data->player->y > 0)
			// if (data->map[(int)(data->player->x + data->player->dir_x * 5.0)][(int)data->player->y] != '0')
		printf ("pos after? [%f][%f]\n", data->player->x, data->player->y);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
	{
		printf ("pos now? [%f][%f]\n", data->player->x, data->player->y);
		if (data->player->x + data->player->dir_x * 0.1 < 8 && data->player->x + data->player->dir_x * 0.1 > 0)
		{
			data->player->x -= data->player->dir_x * 0.1;
			data->player->y -= data->player->dir_y * 0.1;
		}
		printf ("pos after? [%f][%f]\n", data->player->x, data->player->y);
		// data->player->y -= data->player->dy;
		// data->player->x -= data->player->dx;
	}
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
	data->player->x = 3;
	data->player->y = 6;
	data->scale = 64;
	data->player->dir_x = -1;
	data->player->dir_y = 0;
	data->player->plane_x = 0;
	data->player->plane_y = 0.66;
	data->map = cub_hardcoded_map();
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
	init(av, &data);
	bad_draw(&data);
	mlx_loop(data.mlx);
	mlx_delete_image(data.mlx, data.img);
	mlx_terminate(data.mlx);
	return (0);
}
