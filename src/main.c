/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/08 19:11:59 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/31 20:22:55 by wkonings      ########   odam.nl         */
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

	ft_bzero(data->img->pixels, WIDTH * HEIGHT * sizeof(unsigned int));
	// while (++y < data->level->height)
	// {
	// 	x = -1;
	// 	while (++x < data->level->width)
	// 	{
	// 		// if (data->level->map[y][x] == '1')
	// 		// 	draw_square(x, y, data, 0xFFFFFFFF);
	// 		// else if (data->level->map[y][x] == '0')
	// 		// 	draw_square(x, y, data, 0x999999FF);
	// 	}
	// }
	draw_cursor(data);
	// bresenham(WIDTH / 2, 0, WIDTH / 2, HEIGHT, data, 0xFFFFFFFF);
	// draw_player(data->player, data);
}

void	draw_buffer(uint32_t *buffer, int start, int end, int i, int x, t_cub3d *data)
{
	while (i > 0)
	{
		mlx_put_pixel(data->img, x, i + start, buffer[i]);
		i--;
	}
}

void	draw_3d(t_cub3d *data)
{
	t_player	*player = data->player;
	uint32_t	buffer[HEIGHT];

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
		int draw_end = line_height / 2 + HEIGHT / 2;

		draw_end += data->pitch;
		draw_start += data->pitch;
		if (data->crouching == true)
		{
			draw_end -= 100;
			draw_start -= 100;
		}
		if (draw_start < 0)
			draw_start = 0;
		if (draw_end >= HEIGHT)
			draw_end = HEIGHT -1;


		double wall_x;
		if(side == 0)
			wall_x = player->y + wall_dist * ray_dir_y;
		else     
			wall_x = player->x + wall_dist * ray_dir_x;
		wall_x -= floor((wall_x));

		int tex_x = (int)(wall_x * texWidth);
		if (side == 0 && ray_dir_x > 0)
			tex_x = texWidth - tex_x - 1;
		if (side == 1 && ray_dir_y < 0)
			tex_x = texWidth - tex_x - 1;

		double step = 1.0 * texHeight / line_height;
		double tex_pos = (draw_start - data->pitch - HEIGHT / 2 + line_height / 2) * step;


		// if (side == 0)
		// 	bresenham(x, draw_start, x, draw_end, data, 0x990000FF);
		// else
		// 	bresenham(x, draw_start, x, draw_end, data, 0xFF0000FF);
		uint32_t col;
		if (side == 0)
			col = 0x990000FF;
		else
			col = 0xFF0000FF;

		// col = data->textures->tex[1][texHeight * ]
		// if (wall_dist > 4)
		// {
		// 	uint32_t red;
		// 	red = col >> 24;
		// 	red -= (int)wall_dist * 5;
		// 	col = red << 24;
		// 	col += 255;
		// 	// printf ("adj col?: %u\n", col);
		// 	// col = col >> 16;
		// 	// col -= (wall_dist / 10);
		// 	// col = col << 16;
		// }
		int buffer_idx = -1;
		for (int y = draw_start; y < draw_end; y++)
		{
			int tex_y = (int)tex_pos & (texHeight - 1);
			tex_pos += step;
			col = data->textures[5].tex[texHeight * tex_y + tex_x];
			buffer[++buffer_idx] = col;
			
		}
		mlx_tex
		draw_buffer(buffer, draw_start, draw_end, buffer_idx, x, data);
		// bresenham(x, draw_start, x, draw_end, data, col);
		x++;
	}
	draw_cursor(data);
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
	float movespeed = 0.1f;
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
	{
		if (data->level->map[(int)(data->player->y + data->player->side_dir_y * movespeed)][(int)data->player->x] != '1')
			data->player->y += data->player->side_dir_y * movespeed;
		if (data->level->map[(int)data->player->y][(int)(data->player->x + data->player->side_dir_x * movespeed)] != '1')
			data->player->x += data->player->side_dir_x * movespeed;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
	{
		if (data->level->map[(int)(data->player->y - data->player->side_dir_y * movespeed)][(int)data->player->x] != '1')
			data->player->y -= data->player->side_dir_y * movespeed;
		if (data->level->map[(int)data->player->y][(int)(data->player->x - data->player->side_dir_x * movespeed)] != '1')
			data->player->x -= data->player->side_dir_x * movespeed;
	}
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
		data->crouching = !data->crouching;
	}
	// bad_draw(data);
	// draw_3d(data);
}

void	mousehook(mouse_key_t button, action_t action, modifier_key_t mods, void *param)
{
	t_cub3d *data;

	data = param;
}

void	cursorhook(double xpos, double ypos, void *param)
{
	t_cub3d *data;
	data = param;

	if (xpos != WIDTH / 2)
	{
		double turn = (WIDTH / 2) - xpos;
		turn /= 50;
		if (turn > 10)
			printf ("BIG SIDE TURN %f\n", turn);
		// printf ("xpos: %f,  mouseX: %f turn: %f\n", xpos, data->mouse_x, turn);

		double oldDirX = data->player->dir_x;
		data->player->dir_x = data->player->dir_x * cos(turn) - data->player->dir_y * sin(turn);
		data->player->dir_y = oldDirX * sin(turn) + data->player->dir_y * cos(turn);

		double old_side_dir_x = data->player->side_dir_x;
		data->player->side_dir_x = data->player->side_dir_x * cos(turn) - data->player->side_dir_y * sin(turn);
		data->player->side_dir_y = old_side_dir_x * sin(turn) + data->player->side_dir_y * cos(turn);

		double oldPlaneX = data->player->plane_x;
		data->player->plane_x = data->player->plane_x * cos(turn) - data->player->plane_y * sin(turn);
		data->player->plane_y = oldPlaneX * sin(turn) + data->player->plane_y * cos(turn);
	}
	if (ypos != HEIGHT / 2)
	{
		
		double turn = (HEIGHT / 2) - ypos;
		// printf ("pitch?: %i, turn?: %f\n", data->pitch, turn);
		// turn /= 5;

		data->pitch += turn;
		// if (turn > 50)
		// 	printf ("%i BIG VERT TURN %f\n", data->pitch, turn);
		if (data->pitch >  (HEIGHT / 3))
			data->pitch =  (HEIGHT / 3);
		if (data->pitch < (-HEIGHT / 3))
			data->pitch = (-HEIGHT / 3);
	}
	mlx_set_mouse_pos(data->mlx, WIDTH / 2, HEIGHT / 2);
	// data->mouse_x = xpos;
	// bad_draw(data);
	// draw_3d(data);
}

void	loophook(void *param)
{
	t_cub3d *data;

	data = param;
	bad_draw(data);
	draw_3d(data);
}

void	generate_textures(t_cub3d *data)
{
	for(int x = 0; x < texWidth; x++)
	{
		for(int y = 0; y < texHeight; y++)
		{
			int xorcolor = (x * 256 / texWidth) ^ (y * 256 / texHeight);
			//int xcolor = x * 256 / texWidth;
			int ycolor = y * 256 / texHeight;
			int xycolor = y * 128 / texHeight + x * 128 / texWidth;
			data->textures[0].tex[texWidth * y + x] = ((65536 * 254 * (x != y && x != texWidth - y))<<8) + 255; //flat red texture with black cross
			data->textures[1].tex[texWidth * y + x] = ((xycolor + 256 * xycolor + 65536 * xycolor)<<8) + 255; //sloped greyscale
			data->textures[2].tex[texWidth * y + x] = ((256 * xycolor + 65536 * xycolor)<<8) + 255; //sloped yellow gradient
			data->textures[3].tex[texWidth * y + x] = ((xorcolor + 256 * xorcolor + 65536 * xorcolor)<<8) + 255; //xor greyscale
			data->textures[4].tex[texWidth * y + x] = ((256 * xorcolor)<<8) + 255; //xor green
			data->textures[5].tex[texWidth * y + x] = ((65536 * 192 * (x % 16 && y % 16))<<8) + 255; //red bricks
			data->textures[6].tex[texWidth * y + x] = ((65536 * ycolor)<<8) + 255; //red gradient
			data->textures[7].tex[texWidth * y + x] = ((128 + 256 * 128 + 65536 * 128)<<8) + 255; //flat grey texture
		}
	}
}

bool	init(char **av, t_cub3d *data)
{
	data->mlx = mlx_init(WIDTH, HEIGHT, av[1], false);
	mlx_set_cursor_mode(data->mlx, MLX_MOUSE_HIDDEN);
	// mlx_set_mouse_pos(data->mlx, WIDTH / 2, HEIGHT / 2);
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->textures = ft_calloc(9, sizeof(t_texture));
	generate_textures(data);
	mlx_image_to_window(data->mlx, data->img, 0, 0);
	mlx_key_hook(data->mlx, &keyhook, data);
	mlx_mouse_hook(data->mlx, &mousehook, data);
	mlx_cursor_hook(data->mlx, &cursorhook, data);
	mlx_loop_hook(data->mlx, &loophook, data);
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
	draw_3d(&data);
	mlx_loop(data.mlx);
	mlx_delete_image(data.mlx, data.img);
	mlx_terminate(data.mlx);
	return (0);
}
