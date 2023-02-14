/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/13 06:10:36 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/14 09:40:05 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	copy_map(char *file, t_cub3d *data)
{
	char	*line;
	int		fd;
	int		i;

	fd = open_map(file);
	i = -1;
	while (++i < data->start_of_map && get_next_line(fd, &line))
		if (line)
			free(line);
	while (i <= data->end_of_map && get_next_line(fd, &line))
	{
		ft_strlcpy(data->level->map[i - data->start_of_map],
			line, data->level->width + 2);
		free(line);
		i++;
	}
	data->level->height = data->end_of_map - data->start_of_map + 1;
}

bool	allocate_map(char *file, t_cub3d *data)
{
	int		i;

	data->level->map = ft_calloc(data->end_of_map - data->start_of_map + 2,
			sizeof(char *));
	if (!data->level->map)
		error_exit("Error: Failed to allocate level->map", 1);
	i = -1;
	while (++i <= data->end_of_map - data->start_of_map)
	{
		data->level->map[i] = ft_calloc(data->level->width + 2, sizeof(char));
		if (!data->level->map[i])
			error_exit("Error: map[x] allocation failed\n", 1);
	}
	copy_map(file, data);
	return (true);
}

void	get_map_size(char *line, int fd, t_cub3d *data)
{
	if (ft_strlen(line) > (size_t)data->level->width)
		data->level->width = ft_strlen(line);
	if (line)
		free(line);
	while (get_next_line(fd, &line) && ft_strlen(line) > 0)
	{
		data->end_of_map++;
		if (ft_strlen(line) > (size_t)data->level->width)
			data->level->width = ft_strlen(line);
		if (line)
			free(line);
	}
	data->end_of_map += data->start_of_map;
	if (line)
		free(line);
}

bool	init_map(char **av, t_cub3d *data)
{
	int		fd;
	char	*line;

	fd = open_map(av[1]);
	data->level = calloc(1, sizeof(t_map));
	if (!data->level)
		error_exit("Error: allocation failed", 1);
	while (get_next_line(fd, &line) && fill_element(line, data))
	{
		data->start_of_map++;
		if (line)
			free(line);
	}
	if (!line)
		error_exit("Error: No map!", 25);
	if (data->has_tex[0] == false || data->has_tex[1] == false
		|| data->has_tex[2] == false || data->has_tex[3] == false
		|| data->has_tex[4] == false || data->has_tex[5] == false)
		error_exit("Error: Missing a texture or ceiling/floor!", 1);
	get_map_size(line, fd, data);
	allocate_map(av[1], data);
	return (true);
}
