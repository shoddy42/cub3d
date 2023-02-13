/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/13 06:10:36 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/13 12:28:39 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

bool	fill_map(char *file, t_cub3d *data)
{
	int		i;
	int		fd;
	char	*line;

	data->level->map = ft_calloc(data->end_of_map - data->start_of_map + 2,
			sizeof(char *));
	if (!data->level->map)
		error_exit("Failed to allocate level->map\n", 88);
	i = -1;
	while (++i <= data->end_of_map - data->start_of_map)
	{
		data->level->map[i] = ft_calloc(data->level->width + 2, sizeof(char));
		if (!data->level->map[i])
			error_exit("map[x] allocation failed\n", 69);
	}
	fd = open_map(file, data);
	i = -1;
	//skip not map
	while (++i < data->start_of_map && get_next_line(fd, &line))
		if (line)
			free(line);
	while (i <= data->end_of_map && get_next_line(fd, &line))
	{
		ft_strlcpy(data->level->map[i - data->start_of_map],
			line, data->level->width + 2);
		printf ("?: %s\n", data->level->map[i - data->start_of_map]);
		free(line);
		i++;
	}
	data->level->height = data->end_of_map - data->start_of_map + 1;
	return (true);
}

bool	init_map(char **av, t_cub3d *data)
{
	int		fd;
	char	*line;

	fd = open_map(av[1], data);
	data->level = calloc(1, sizeof(t_map));
	if (!data->level)
		error_exit("allocation failed\n", 69);
	while (get_next_line(fd, &line) && fill_element(line, data))
	{
		data->start_of_map++;
		if (line)
			free(line);
	}
	if (!line)
		error_exit("no map!\n", 25);
	if (ft_strlen(line) > data->level->width)
		data->level->width = ft_strlen(line);
	if (line)
		free(line);
	while (get_next_line(fd, &line) && ft_strlen(line) > 0)
	{
		data->end_of_map++;
		if (ft_strlen(line) > data->level->width)
			data->level->width = ft_strlen(line);
		if (line)
			free(line);
	}
	data->end_of_map += data->start_of_map;
	if (line)
		free(line);
	fill_map(av[1], data);
	return (true);
}
