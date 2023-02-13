/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   wasd.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/13 12:29:35 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/13 12:47:13 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	move_forward(t_cub3d *data, t_player *player,
	char **map, float movespeed)
{
	if (map[(int)(player->pos[Y] + player->dir[Y]
			* movespeed)][(int)player->pos[X]] != '1')
		player->pos[Y] += player->dir[Y] * movespeed;
	if (map[(int)player->pos[Y]][(int)(player->pos[X]
		+ player->dir[X] * movespeed)] != '1')
		player->pos[X] += player->dir[X] * movespeed;
}

void	move_back(t_cub3d *data, t_player *player, char **map, float movespeed)
{
	if (map[(int)(player->pos[Y] - player->dir[Y]
			* movespeed)][(int)player->pos[X]] != '1')
		player->pos[Y] -= player->dir[Y] * movespeed;
	if (map[(int)player->pos[Y]][(int)(player->pos[X]
		- player->dir[X] * movespeed)] != '1')
		player->pos[X] -= player->dir[X] * movespeed;
}

void	move_left(t_cub3d *data, t_player *player, char **map, float movespeed)
{
	if (map[(int)(player->pos[Y] - player->side_dir[Y]
			* movespeed)][(int)player->pos[X]] != '1')
		player->pos[Y] -= player->side_dir[Y] * movespeed;
	if (map[(int)player->pos[Y]][(int)(player->pos[X]
		- player->side_dir[X] * movespeed)] != '1')
		player->pos[X] -= player->side_dir[X] * movespeed;
}


void	move_right(t_cub3d *data, t_player *player, char **map, float movespeed)
{
	if (map[(int)(player->pos[Y] + player->side_dir[Y]
			* movespeed)][(int)player->pos[X]] != '1')
		player->pos[Y] += player->side_dir[Y] * movespeed;
	if (map[(int)player->pos[Y]][(int)(player->pos[X]
		+ player->side_dir[X] * movespeed)] != '1')
		player->pos[X] += player->side_dir[X] * movespeed;
}
