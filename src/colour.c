/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   colour.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/13 06:12:48 by wkonings      #+#    #+#                 */
/*   Updated: 2023/02/13 08:28:46 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

uint32_t	colour_to_uint(t_col col)
{
	uint32_t	ret;

	if (col.r < 0)
		col.r = 0;
	if (col.g < 0)
		col.g = 0;
	if (col.b < 0)
		col.b = 0;
	if (col.a < 0)
		col.a = 0;
	ret = col.a;
	ret += col.r << 24;
	ret += col.g << 16;
	ret += col.b << 8;
	return (ret);
}
