/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub3d.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/16 15:54:39 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/18 15:52:23 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define WIDTH 1920
# define HEIGHT 1080

# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h> //todo: remove
# include <fcntl.h>
# include "libft/include/libft.h"
# include "mlx/include/MLX42/MLX42.h"

typedef	struct s_player
{
	float	x;
	float	y;
	float	dir;

}	t_player;

typedef struct s_cub3d
{
	mlx_t		*mlx;
	mlx_image_t	*img;

	char		**map;
	t_player	*player;
	char		*title;
}	t_cub3d;

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