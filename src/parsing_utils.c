/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/13 07:21:23 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/14 10:18:06 by wkonings      ########   odam.nl         */
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
	if (data->has_tex[type] == true)
		error_exit("Duplicate texture in .cub.", 1);
	data->has_tex[type] = true;
	data->tex[type] = *tex;
	free(tex);
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
		error_exit("Error: illegal characters in colour!", 99);
	if (has_num(str) == false)
		error_exit("Error: No red value!", -2);
	col->r = ft_atoi(str);
	if (col->r > 255 || col->r < 0)
		error_exit("Error: Red colour must be in range [0 - 255]!", 8);
	str = ft_strchr(str, ',');
	if (!str || str[1] == '\0' || has_num(str + 1) == false)
		error_exit("Error: No green value!", 12);
	col->g = ft_atoi(str + 1);
	if (col->g > 255 || col->g < 0)
		error_exit("Error: Green colour must be in range [0 - 255]!", 9);
	str = ft_strchr(str + 1, ',');
	if (!str || str[1] == '\0' || has_num(str + 1) == false)
		error_exit("Error: No blue value!", 10);
	col->b = ft_atoi(str + 1);
	if (col->b > 255 || col->b < 0)
		error_exit ("Error: Blue colour must be in range [0 - 255]!", 11);
	col->a = 255;
	return (true);
}

bool	fill_element(char *str, t_cub3d *data, bool ret)
{
	if (str && ft_strlen(str) == 0)
		return (true);
	if (ft_strncmp(str, "SO ", 3) == 0)
		ret = set_texture(str + 3, data, SOUTH);
	else if (ft_strncmp(str, "NO ", 3) == 0)
		ret = set_texture(str + 3, data, NORTH);
	else if (ft_strncmp(str, "WE ", 3) == 0)
		ret = set_texture(str + 3, data, WEST);
	else if (ft_strncmp(str, "EA ", 3) == 0)
		ret = set_texture(str + 3, data, EAST);
	else if (ft_strncmp(str, "C ", 2) == 0)
		ret = set_colour(str + 2, &data->ceiling);
	else if (ft_strncmp(str, "F ", 2) == 0)
		ret = set_colour(str + 2, &data->floor);
	else if (ft_strcomply(str, VALID_TILES))
		return (false);
	else
		error_exit("Illegal string in .cub", 1);
	if (ft_strncmp(str, "C ", 2) == 0)
		data->has_tex[CEILING] = true;
	if (ft_strncmp(str, "F ", 2) == 0)
		data->has_tex[FLOOR] = true;
	return (ret);
}

int	open_map(char *file)
{
	int	fd;

	if (ft_strlen(file) < 4)
		error_exit(RED "Error: Filename too short!", 6);
	if (ft_strncmp(file + ft_strlen(file) - 4, ".cub", 4) != 0)
		error_exit(RED "Error: map is not of .cub type", 7);
	fd = open(file, O_RDONLY);
	if (fd < 0)
		error_exit (RED "Error: Unable to open file.", 8);
	return (fd);
}
