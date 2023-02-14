/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   player.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/14 08:23:10 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/14 08:27:32 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	set_player_west(t_player *player)
{
	player->dir[X] = -1;
	player->dir[Y] = 0;
	player->side_dir[X] = 0;
	player->side_dir[Y] = 1;
	player->plane_x = 0;
	player->plane_y = 0.66;
}

void	set_player_east(t_player *player)
{
	player->dir[X] = 1;
	player->dir[Y] = 0;
	player->side_dir[X] = 0;
	player->side_dir[Y] = -1;
	player->plane_x = 0;
	player->plane_y = -0.66;
}

void	set_player_north(t_player *player)
{
	player->dir[X] = 0;
	player->dir[Y] = -1;
	player->side_dir[X] = -1;
	player->side_dir[Y] = 0;
	player->plane_x = -0.66;
	player->plane_y = 0;
}

void	set_player_south(t_player *player)
{
	player->dir[X] = 0;
	player->dir[Y] = 1;
	player->side_dir[X] = 1;
	player->side_dir[Y] = 0;
	player->plane_x = 0.66;
	player->plane_y = 0;
}

bool	set_player(t_cub3d *data, int x, int y, char dir)
{
	if (dir == 'W')
		set_player_west(data->player);
	if (dir == 'E')
		set_player_east(data->player);
	if (dir == 'N')
		set_player_north(data->player);
	if (dir == 'S')
		set_player_south(data->player);
	data->player->pos[X] = (float)x + 0.5f;
	data->player->pos[Y] = (float)y + 0.5f;
	data->level->map[y][x] = '0';
	return (true);
}
