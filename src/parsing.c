/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/30 15:25:30 by auzochuk      #+#    #+#                 */
/*   Updated: 2023/02/08 00:47:32 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

bool	set_texture(char *str, t_cub3d *data, t_tex_type type)
{
	mlx_texture_t	*tex;

	while (*str && *str == ' ')
		str++;
	tex = mlx_load_png(str);
	if (!tex)
	{
		printf (RED "Error. " RESET "Failed to load texture [%s]\n", str);
		exit (1);
	}
	data->tex[type] = *tex;
	return (true);
}

bool	has_num(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ',' || str[i] == '\0')
			return (false);
		else if (ft_isdigit(str[i]))
			return (true);
		i++;
	}
	return (false);
}

bool	set_colour(char *str, t_col *col)
{
	if (ft_strcomply(str, " ,0123456789") == false)
		error_exit("illegal characters!\n", 99);
	if (has_num(str) == false)
		error_exit("No red value!\n", -2);
	col->r = ft_atoi(str);
	if (col->r > 255 || col->r < 0)
		error_exit("Red colour must be in range [0 - 255]!\n", 8);
	str = ft_strchr(str, ',');
	if (!str || str[1] == '\0' || has_num(str + 1) == false)
		error_exit("No green value!\n", 12);
	col->g = ft_atoi(str + 1);
	if (col->g > 255 || col->g < 0)
		error_exit("Green colour must be in range [0 - 255]!\n", 9);
	str = ft_strchr(str + 1, ',');
	if (!str || str[1] == '\0' || has_num(str + 1) == false)
		error_exit("No blue value!\n", 10);
	col->b = ft_atoi(str + 1);
	if (col->b > 255 || col->b < 0)
		error_exit ("Blue colour must be in range [0 - 255]!\n", 11);
	col->a = 255;
	return (true);
}

bool	fill_element(char *str, t_cub3d *data)
{
	bool	test;

	test = false;
	if (str && ft_strlen(str) == 0)
		return (true);
	if (ft_strncmp(str, "SO ", 3) == 0)
		test = set_texture(str + 3, data, SOUTH);
	if (ft_strncmp(str, "NO ", 3) == 0)
		test = set_texture(str + 3, data, NORTH);
	if (ft_strncmp(str, "WE ", 3) == 0)
		test = set_texture(str + 3, data, WEST);
	if (ft_strncmp(str, "EA ", 3) == 0)
		test = set_texture(str + 3, data, EAST);
	if (ft_strncmp(str, "C ", 2) == 0)
		test = set_colour(str + 2, &data->ceiling);
	if (ft_strncmp(str, "F ", 2) == 0)
		test = set_colour(str + 2, &data->floor);
	return (test);
}

int	open_map(char *file, t_cub3d *data)
{
	int		fd;

	if (ft_strlen(file) < 4)
		error_exit(RED "filename too short noob\n", 6);
	if (ft_strncmp(file + ft_strlen(file) - 4, ".cub", 4) != 0)
		error_exit(RED "not .cub noob", 7);
	fd = open(file, O_RDONLY);
	if (fd < 0)
		error_exit (RED "not real file, noob\n", 8);
	return (fd);
}

bool	check_direction(t_parse parse, t_map *level)
{
	while (parse.x < level->width && parse.y < level->height && parse.x >= 0 && parse.y >= 0)
	{
		if (level->map[parse.y][parse.x] == '1')
			return (true);
		else if (!ft_charinstr(level->map[parse.y][parse.x], VALID_TILES))
		{
			printf ("no wall found!\n");
			return (false);
		}
		parse.x += parse.dx;
		parse.y += parse.dy;
	}
	// printf ("[%i][%i] ended at [%i][%i]  ?: [%c]\n", parse.dx, parse.dy, parse.x + 1, parse.y + 1, level->map[parse.y][parse.x]);
	return (false);
}

bool	check_cell(int x, int y, t_map *level)
{
	if (!check_direction((t_parse){x, y, 1, 0}, level)) //right
		return (false);
	if (!check_direction((t_parse){x, y, -1, 0}, level))//left
		return (false);
	if (!check_direction((t_parse){x, y, 0, -1}, level))//up
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

// E
bool	init_bob(t_cub3d *data)
{
	bool	has_player;
	t_map	*level;
	int		x;
	int		y;

	data->player = ft_calloc(1, sizeof(t_player));
	if (!data->player)
		return (false);
	level = data->level;
	has_player = false;
	x = 0;
	y = 0;
	while (y < level->height)
	{
		x = 0;
		while (x < level->width)
		{
			if (ft_charinstr(level->map[y][x], PLAYER_TILES) == true && has_player == false)
			{
				//todo: set direction of player depending on NSWE
				printf ("Player data set\n");
				data->player->dir_x = -1;
				data->player->dir_y = 0;
				data->player->side_dir_x = 0;
				data->player->side_dir_y = 1;
				data->player->plane_x = 0;
				data->player->plane_y = 0.66;
				data->player->x = (float)x + 0.5f;
				data->player->y = (float)y + 0.5f;
				has_player = true;
				level->map[y][x] = '0';
			}
			else if (ft_charinstr(level->map[y][x], PLAYER_TILES) == true && has_player == true)
			{
				printf ("failed because pleyer already ritard\n");
				return (false);
			}
			x++;
		}
		y++;
	}
	if (has_player == false)
		printf ("no player found noob\n");
	return (has_player);
}

bool	parse_map(char *file, t_cub3d *data)
{
	char	*line;
	int		fd;
	int		x;
	int		y;

	if (!init_bob(data))
		return (false);
	// actual parsing of the map
	y = -1;
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
