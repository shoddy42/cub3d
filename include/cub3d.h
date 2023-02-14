/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub3d.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/16 15:54:39 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/14 09:39:34 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define WIDTH 1920
# define HEIGHT 1080
# define SCALE 16
# define SHADING 1
# define VALID_TILES "01NSWE "
# define WALL_TILES "1"
# define PLAYER_TILES "NSWE"

# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <math.h>
# include "../libft/include/libft.h"
# include "../mlx/include/MLX42/MLX42.h"

typedef double	t_point2d __attribute__ ((vector_size (2 * sizeof(double))));

typedef enum e_direction2d
{
	X,
	Y,
}	t_dir2d;

typedef enum e_tex_type
{
	NORTH,
	SOUTH,
	EAST,
	WEST,
	CEILING,
	FLOOR,
}	t_tex_type;

typedef struct s_parse
{
	int	x;
	int	y;
	int	dx;
	int	dy;
}	t_parse;

typedef struct s_player
{
	t_point2d	pos;
	t_point2d	dir;
	t_point2d	side_dir;
	t_point2d	plane;
	double		plane_x;
	double		plane_y;
}	t_player;

typedef struct s_col
{
	int	r;
	int	g;
	int	b;
	int	a;
}	t_col;

typedef struct s_draw
{
	int			buffer_idx;

	double		camera_x;
	t_point2d	ray_dir;

	int			map_x;
	int			map_y;
	t_point2d	side_dist;
	t_point2d	delta_dist;
	double		wall_dist;

	int			step_x;
	int			step_y;

	int			line_height;
	int			line_start;
	int			line_end;

	double		wall_x;
	int			tex_x;
	int			tex_y;
	double		tex_pos;
	double		step;

	int			side;
}	t_draw;

typedef struct s_map
{
	char	**map;
	int		width;
	int		height;
}	t_map;

typedef struct s_texture
{
	int	tex[WIDTH * HEIGHT];

}	t_texture;

typedef struct s_cub3d
{
	mlx_t			*mlx;
	mlx_image_t		*img;

	t_map			*level;
	t_player		*player;
	bool			has_player;
	bool			crouching;
	mlx_texture_t	*tex;
	bool			has_tex[6];

	t_col			ceiling;
	t_col			floor;

	int				start_of_map;
	int				end_of_map;

	int				pitch;
	double			mouse_x;
	double			mouse_y;
	int				scale;
	char			*title;
}	t_cub3d;

void		bresenham(t_point2d a, t_point2d b, t_cub3d *data, uint32_t color);
bool		set_player(t_cub3d *data, int x, int y, char dir);

void		error_exit(char *msg, int error_code);

//colour
uint32_t	colour_to_uint(t_col col);

//map
bool		init_map(char **av, t_cub3d *data);
void		parse_map(t_cub3d *data);
int			open_map(char *file);
bool		fill_element(char *str, t_cub3d *data);

//wasd
void		move_back(t_player *player, char **map, float movespeed);
void		move_forward(t_player *player, char **map, float movespeed);
void		move_left(t_player *player, char **map, float movespeed);
void		move_right(t_player *player, char **map, float movespeed);

//hooks
void		keyhook(mlx_key_data_t keydata, void *param);
void		cursorhook(double xpos, double ypos, void *param);
void		kbm_turn_side(t_cub3d *data, int dir);
void		loophook(void *param);

//draw
void		draw_3d(t_cub3d *data);
void		draw_cursor(t_cub3d *data);
void		draw_player(t_player *player, t_cub3d *data, int width, int height);
void		draw_square(int x, int y, t_cub3d *data, uint32_t color);
void		draw_minimap(t_cub3d *data);
void		draw_buffer(t_col *buffer, int x, t_draw *draw, t_cub3d *data);

// ray_setup
void		setup_ray(t_draw *draw, int x, t_cub3d *data);
void		get_step(t_draw *draw, t_cub3d *data);

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