/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/30 15:25:30 by auzochuk      #+#    #+#                 */
/*   Updated: 2023/01/30 17:38:33 by auzochuk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

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
	if (line)
		free(line);
	close(fd);
	data->level->map = ft_calloc(data->level->height + 1, sizeof(char *));
	fd = open_map(file);
	while (get_next_line(fd, &line))
	{
		if (ft_strlen(line) > data->level->width)
			data->level->width = ft_strlen(line);
		free(line);
	}
	if (line)
		free(line);
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
		else if (!ft_charinstr(level->map[parse.y][parse.x], VALID_TILES))
			return (false);
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

// E
bool	init_bob(t_cub3d *data)
{
	bool has_player;
	t_map *level;
	int	x;
	int	y;

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
		// printf ("hello %i [%s]\n", level->width, level->map[y]);
		while (x < level->width)
		{
			if (ft_charinstr(level->map[y][x], PLAYER_TILES) == true && has_player == false)
			{
				//todo: set direction of player depending on NSWE
				printf ("Player data set\n");
				data->player->dir_x = -1;
				data->player->dir_y = 0;
				data->player->plane_x = 0;
				data->player->plane_y = 0.66;
				data->player->x = (float)x * SCALE;
				data->player->y = (float)y * SCALE;
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
	int		i;

	i = 0;
	alloc_map(file, data);
	fd = open_map(file);
	//this copies unto the map
	while (get_next_line(fd, &line) > 0)
	{
		ft_strlcpy(data->level->map[i], line, data->level->width + 1);
		free(line);
		i++;
	}
	if (line)
		free(line);
	int e = -1;
	int z = -1;
	//this just prints
	while (data->level->map[++e])
	{
		z = -1;
		while (data->level->map[e][++z])
			printf ("%c", data->level->map[e][z]);
		printf ("\n");
	}
	if (!init_bob(data))
		return (false);
	int x = 0;
	int y = -1;
	// actual parsing of the map
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
