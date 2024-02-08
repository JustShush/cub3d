/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddiniz-m <ddiniz-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 12:16:48 by ddiniz-m          #+#    #+#             */
/*   Updated: 2024/02/08 11:52:21 by ddiniz-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int sizeofmap_x(t_general *gen)
{
	int i;
	int j;
	int res;

	res = 0;
	i = 0;
	while(gen->map->tilemap[i])
	{
		j = 0;
		while(gen->map->tilemap[i][j])
			j++;
		if(j > res)
			res = j;
		i++;
	}
	return (i);
}

int sizeofmap_y(t_general *gen)
{
	int i = 0;

	while(gen->map->tilemap[i])
	{
		i++;
	}
	return (i - 1);
}

void	put_square(t_general *gen, int y, int x, int color)
{
	int	j = 0;
	int	i = 0;

	while (j < SCALE)
	{
		i = 0;
		while (i < SCALE)
		{
			if (j == 0 || i == 0)
				my_mlx_pixel_put(gen->img, x + i, y + j, 0x000000);
			else
				my_mlx_pixel_put(gen->img, x + i, y + j, color);
			i++;
		}
		j++;
	}
}

//Takes player position and angle and ray lenght. Shoots ray
void	put_rays(t_general *gen, int angle, int py, int px, int len)
{
	int	i;
	int	rx;
	int	ry;
	
	i = 0;
	while (i < len)
	{
		rx = round(px - (cos(toRad(angle)) * i));
		ry = round(py - (sin(toRad(angle)) * i));
		mlx_pixel_put(gen->mlx, gen->win, rx, ry, GREEN_PIXEL);
		i++;
	}
}

void	draw_player(t_general *gen)
{
	int	i;
	int	j;
	int	x;
	int	y;

	j = gen->player->an + 30;
	x = gen->player->x * SCALE / 64;
	y = (gen->player->y * SCALE / 64);
	while (j > gen->player->an - 30)
	{
		i = 0;
		while (i < SCALE)
		{
			my_mlx_pixel_put(gen->img, x - (cos(toRad(j)) * i), y - (sin(toRad(j) ) * i), GREEN_PIXEL);
			i++;
		}
		j--;
	}
	my_mlx_pixel_put(gen->img, x, y, ORANGE_PIXEL);
}

//Takes player position and radius. Draws circle
void	put_player(t_general *gen, int py, int px, int r)
{
	int	i;
	int	j;
	int	x;
	int	y;
	
	i = 0;
	mlx_pixel_put(gen->mlx, gen->win, px, py, ORANGE_PIXEL);
	while (i <= 360)
	{
		j = r;
		while (j > 0)
		{
			x = round(px + (j * cos(toRad(i))));
			y = round(py + (j * sin(toRad(i))));
			mlx_pixel_put(gen->mlx, gen->win, x, y, ORANGE_PIXEL);
			j--;
		}
		i += 1;
	}

}

//Casts rays for minimap and puts player
void	raycast2d(t_general *gen)
{
	int	i;
	int	x;
	int	y;
	int	pradius;
	int	ray_len;

	pradius = 2;
	ray_len = 50;
	i = gen->player->an + (gen->pov / 2);
	x = gen->player->x * 20 / 64;
	y = (gen->player->y * 20 / 64) + 300;
	while (i > gen->player->an - (gen->pov / 2))
		put_rays(gen, i--, y, x, ray_len);
	put_player(gen, y, x, pradius);
}

int	minimap(t_general *gen)
{
	int y = 0;
	int x = 0;
	int map_x = 0;
	int map_y = 0;

	raycast2d(gen);
	while(gen->map->tilemap[y] && map_y <= gen->map_height && map_x <= gen->map_width)
	{
		while(gen->map->tilemap[y][x] && map_y <= gen->map_height && map_x <= gen->map_width && gen->map->tilemap[y][x] != '\n')
		{
			if(gen->map->tilemap[y][x] == '1')
				put_square(gen, map_y, map_x, 0x00000FFF);
			else if(gen->map->tilemap[y][x] == '0')
				put_square(gen, map_y, map_x, 0x00FFF000);
			else if(gen->map->tilemap[y][x] == 'W' || gen->map->tilemap[y][x] == 'E' || gen->map->tilemap[y][x] == 'N' || gen->map->tilemap[y][x] == 'S')
				put_square(gen, map_y, map_x, 0x00FFF000);
			else if(gen->map->tilemap[y][x] == ' ' || gen->map->tilemap[y][x] == '\t')
				put_square(gen, map_y, map_x, 0x000000FF);
			map_x += SCALE;
			x++;
		}
		map_y += SCALE;
		map_x = 0;
		x = 0;
		y++;
	}
	return (0);
}