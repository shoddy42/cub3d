/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bresenham.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/14 07:49:25 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/14 07:49:32 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	bresenham(t_point2d a, t_point2d b, t_cub3d *data, uint32_t color)
{
	float		max;
	t_point2d	step;

	step[X] = b[X] - a[X];
	step[Y] = b[Y] - a[Y];
	max = fmax(fabs(step[X]), fabs(step[Y]));
	step[X] /= max;
	step[Y] /= max;
	while ((int)(a[X] - b[X]) || (int)(a[Y] - b[Y]))
	{
		if (a[X] < WIDTH && a[X] > 0 && a[Y] < HEIGHT && a[Y] > 0)
			mlx_put_pixel(data->img, (int)(a[X]), (int)(a[Y]), color);
		a[X] += step[X];
		a[Y] += step[Y];
	}
}
