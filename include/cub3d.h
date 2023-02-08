/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub3d.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/16 15:54:39 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/08 01:37:57 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define WIDTH 2048
# define HEIGHT 1024
# define SCALE 16
# define texWidth 64
# define texHeight 64
# define VALID_TILES "01NSWE "
# define WALL_TILES "1"
# define PLAYER_TILES "NSWE"

# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h> //todo: remove
# include <fcntl.h>
# include <math.h>
# include "../libft/include/libft.h"
# include "../mlx/include/MLX42/MLX42.h"

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
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	side_dir_x;
	double	side_dir_y;
	double	plane_x;
	double	plane_y;
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
	int		buffer_idx;

	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;

	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	wall_dist;

	int		step_x;
	int		step_y;

	int		line_height;
	int		line_start;
	int		line_end;

	double	wall_x;
	int		tex_x;
	int		tex_y;
	double	tex_pos;
	double	step;

	int		side;
}	t_draw;

typedef struct s_map
{
	char	**map;
	//Floor color
	//Ceiling color
	//Wall color
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
	t_texture		*textures;
	mlx_texture_t	*tex;

	// mlx_texture_t	*north;
	// mlx_texture_t	*south;
	// mlx_texture_t	*east;
	// mlx_texture_t	*west;
	t_col			ceiling;
	t_col			floor;


	// char **map;
	// int scale;
	int			start_of_map;
	int			end_of_map;


	int			pitch;
	double		mouse_x;
	double		mouse_y;
	char		*title;
}	t_cub3d;


void	error_exit(char *msg, int error_code);
bool	parse_map(char *file, t_cub3d *data);

int		open_map(char *file, t_cub3d *data);

bool	fill_element(char *str, t_cub3d *data);


//hooks
void	keyhook(mlx_key_data_t keydata, void *param);
void	mousehook(mouse_key_t button, action_t action, modifier_key_t mods, void *param);
void	cursorhook(double xpos, double ypos, void *param);
void	loophook(void *param);

//draw
void	draw_3d(t_cub3d *data);
void	draw_cursor(t_cub3d *data);
void	draw_player(t_player *player, t_cub3d *data);
void	draw_square(int	x, int y, t_cub3d *data, uint32_t color);
void	bad_draw(t_cub3d *data);
void	draw_buffer(t_col *buffer, int start, int end, int i, int x, t_cub3d *data);

// ray_setup
void	setup_ray(t_draw *draw, int x, t_cub3d *data);
void	get_step(t_draw *draw, t_cub3d *data);


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