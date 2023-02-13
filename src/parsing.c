/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/30 15:25:30 by auzochuk      #+#    #+#                 */
/*   Updated: 2023/02/13 07:59:14 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

bool	check_direction(t_parse parse, t_map *level)
{
	while (parse.x < level->width && parse.y < level->height
		&& parse.x >= 0 && parse.y >= 0)
	{
		if (level->map[parse.y][parse.x] == '1')
			return (true);
		else if (!ft_charinstr(level->map[parse.y][parse.x], VALID_TILES))
			return (false);
		parse.x += parse.dx;
		parse.y += parse.dy;
	}
	return (false);
}

void	check_cell(int x, int y, t_map *level)
{
	if (!check_direction((t_parse){x, y, 1, 0}, level))
		error_exit("Error: Right side wall not closed.", 1);
	if (!check_direction((t_parse){x, y, -1, 0}, level))
		error_exit("Error: Left side wall not closed.", 1);
	if (!check_direction((t_parse){x, y, 0, -1}, level))
		error_exit("Error: Top side wall not closed.", 1);
	if (!check_direction((t_parse){x, y, 0, 1}, level))
		error_exit("Error: Bottom side wall not closed.", 1);
	if (!check_direction((t_parse){x, y, 1, -1}, level))
		error_exit("Error: Diagonal up right wall not closed.", 1);
	if (!check_direction((t_parse){x, y, 1, 1}, level))
		error_exit("Error: Diagonal down right wall not closed.", 1);
	if (!check_direction((t_parse){x, y, -1, 1}, level))
		error_exit("Error: Diagonal down left wall not closed.", 1);
	if (!check_direction((t_parse){x, y, -1, -1}, level))
		error_exit("Error: Diagonal up left wall not closed.", 1);
}

//todo: set direction of player depending on NSWE
bool	set_player(t_cub3d *data, int x, int y)
{
	//definitely change
	data->player->dir[X] = -1;
	data->player->dir[Y] = 0;
	data->player->side_dir[X] = 0;
	data->player->side_dir[Y] = 1;
	//maybe changes
	data->player->plane_x = 0;
	data->player->plane_y = 0.66;
	//doesnt change
	data->player->pos[X] = (float)x + 0.5f;
	data->player->pos[Y] = (float)y + 0.5f;
	data->level->map[y][x] = '0';
	return (true);
}

bool	init_player(t_cub3d *data)
{
	bool	has_player;
	t_map	*level;
	int		x;
	int		y;

	data->player = ft_calloc(1, sizeof(t_player));
	if (!data->player)
		error_exit("Error: Failed to allocate player\n", 1);
	level = data->level;
	has_player = false;
	y = -1;
	while (++y < level->height)
	{
		x = -1;
		while (++x < level->width)
		{
			if (ft_charinstr(level->map[y][x], PLAYER_TILES) == true
				&& has_player == false)
				has_player = set_player(data, x, y);
			else if (ft_charinstr(level->map[y][x], PLAYER_TILES) == true
				&& has_player == true)
				error_exit("Error: Map has more than one player.\n", 1);
		}
	}
	return (has_player);
}

void	parse_map(char *file, t_cub3d *data)
{
	char	*line;
	int		fd;
	int		x;
	int		y;

	if (!init_player(data))
		error_exit("Error: Map has no player.\n", 1);
	y = -1;
	while (data->level->map[++y])
	{
		x = -1;
		while (data->level->map[y][++x])
			if (data->level->map[y][x] == '0')
				check_cell(x, y, data->level);
	}
}
