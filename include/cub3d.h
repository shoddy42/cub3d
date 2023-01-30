/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub3d.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/16 15:54:39 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/30 17:03:21 by auzochuk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define WIDTH 2048
# define HEIGHT 1024
# define SCALE 64
# define VALID_TILES "01NSWE"
# define PLAYER_TILES "NSWE"

# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h> //todo: remove
# include <fcntl.h>
# include <math.h>
# include "../libft/include/libft.h"
# include "../mlx/include/MLX42/MLX42.h"

typedef	struct s_parse
{
	int x;
	int y;
	int	dx;
	int	dy;
}	t_parse;

typedef	struct s_player
{
	float	x;
	float	y;
	float	dir_x;
	float	dir_y;
}	t_player;

typedef struct s_map
{
	char	**map;
	//Floor color
	//Ceiling color
	//Wall color
	int		width;
	int		height;
}	t_map;

typedef struct s_cub3d
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	
	t_map		*level;
	t_player	*player;
	bool		has_player;
	
	char		*title;
}	t_cub3d;


void	error_exit(char *msg, int error_code);
bool	parse_map(char *file, t_cub3d *data);





//string colors
# define YELLOW	"\1\33[38;5;220m\2"
# define BLACK	"\1\33[38;5;232m\2"
# define ORANGE	"\1\33[38;5;202m\2"
# define RED	"\1\33[38;5;196m\2"
# define PINK	"\1\33[38;5;198m\2"
# define GREEN	"\1\33[38;5;28m\2"
# define BLUE	"\1\33[38;5;33m\2"
# define L_BLUE	"\1\33[38;5;69m\2"
# define D_BLUE	"\1\33[38;5;21m\2"
# define GREY	"\1\33[38;5;242m\2"
# define PURPLE	"\1\33[38;5;92m\2"
# define VIOLET	"\1\33[38;5;183m\2"
# define RESET	"\1\33[0m\2"
# define END	"\1\33[0m\2\3"

#endif