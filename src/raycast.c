#include "headers/maze.h"

/**
 * cast_ray - Cast an imaginary ray from the player's position
 * @instance: pointer to a struct with the window and renderer members
 * Return: Nothing
*/
void cast_ray(SDL_Instance *instance)
{
	point_double_t ray_dir;
	point_double_t side_dist;  //length of ray from current position to next x or y-side
	point_double_t delta_dist;

	point_int_t map;
	point_int_t step;      // what direction to step in x or y-direction (either +1 or -1)

	double cameraX;
	double perp_wall_dist;  // Perpendicular wall distance
	bool hit;
	int side; //was a NS or an EW wall hit?

	for(int x = 0; x < SCREEN_WIDTH; x++)
	{
		//calculate ray position and direction
		cameraX = 2 * x / (double)(SCREEN_WIDTH - 1); //x-coordinate in camera space
		ray_dir.x = dir.x + plane.x * cameraX;
		ray_dir.y = dir.y + plane.y * cameraX;

		map.x = (int)(pos.x);  /* which box of the map we're in */
		map.y = (int)(pos.y);

		/*
		* length of ray from one x or y-side to next x or y-side. These are derived as:
		* delta_dist.x = sqrt(1 + (ray_dir.y * ray_dir.y) / (ray_dir.x * ray_dir.x))
		* delta_dist.y = sqrt(1 + (ray_dir.x * ray_dir.x) / (ray_dir.y * ray_dir.y))
		* which can be simplified to abs(|ray_dir| / ray_dir.x) and abs(|ray_dir| / ray_dir.y)
		* where |ray_dir| is the length of the vector (ray_dir.x, ray_dir.y). Its length,
		* unlike (dir.x, dir.y) is not 1, however this does not matter, only the
		* ratio between delta_dist.x and delta_dist.y matters, due to the way the DDA
		* stepping further below works. So the values can be computed as below.
		* Division through zero is prevented, even though technically that's not
		* needed in C++ with IEEE 754 floating point values.
		*/
		delta_dist.x = (ray_dir.x == 0) ? 1e30 : abs(1 / ray_dir.x);
		delta_dist.y = (ray_dir.y == 0) ? 1e30 : abs(1 / ray_dir.y);

		hit = false;  /* was there wall hit? */

		if(ray_dir.x < 0)  /* calculate step and initial side_dist */
		{
			step.x = -1;
			side_dist.x = (pos.x - map.x) * delta_dist.x;
		}
		else
		{
			step.x = 1;
			side_dist.x = (map.x + 1.0 - pos.x) * delta_dist.x;
		}

		if(ray_dir.y < 0)
		{
			step.y = -1;
			side_dist.y = (pos.y - map.y) * delta_dist.y;
		}
		else
		{
			step.y = 1;
			side_dist.y = (map.y + 1.0 - pos.y) * delta_dist.y;
		}

		while (hit == false)  /* perform DDA */
		{
			if (side_dist.x < side_dist.y)  /* jump to next map square, either in x-direction, or in y-direction */
			{
				side_dist.x += delta_dist.x;
				map.x += step.x;
				side = 0;
			}
			else
			{
				side_dist.y += delta_dist.y;
				map.y += step.y;
				side = 1;
			}

			if (WORLD_MAP[map.x][map.y] > 0)  /* Check if ray has hit a wall */
				hit = true;
		}
		draw_stripe(instance, x, perp_wall_dist, side, side_dist, delta_dist);
	}
  SDL_RenderPresent(instance->renderer);
}

/**
 * draw_stripe - Draws a stripe of the wall on the window
 * @instance: a pointer to a struct containing the window and renderer as members
 * @pos_x: the horizontal position of the wall stripe be drawn on the window
 * @wall_distance: the perpendicular distance of the wall from the player
 * @wall_direction: indicates whether the wall is North-South or East-West facing
 * @side_dist: length of ray from current position to next x or y-side
 * @delta_dist: length of ray from one x or y-side to next x or y-side
 */
void draw_stripe(SDL_Instance *instance, int pos_x, double wall_distance,
			int wall_direction, point_double_t side_dist, point_double_t delta_dist)
{
	int line_height, draw_start, draw_end;
	SDL_Color color;

	/*
		* Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
		* hit to the camera plane. Euclidean to center camera point would give fisheye effect!
		* This can be computed as (map.x - pos.x + (1 - step.x) / 2) / ray_dir.x for side == 0, or same formula with Y
		* for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
		* because they were left scaled to |ray_dir|. sideDist is the entire length of the ray above after the multiple
		* steps, but we subtract deltaDist once because one step more into the wall was taken above.
	*/
	if (wall_direction == 0)
		wall_distance = (side_dist.x - delta_dist.x);
	else
		wall_distance = (side_dist.y - delta_dist.y);

	/* Calculate height of line to draw on screen */
	line_height = (int)(SCREEN_HEIGHT / wall_distance);

	/* calculate lowest and highest pixel to fill in current stripe */
	draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
	if (draw_start < 0)
		draw_start = 0;

	draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
	if (draw_end >= SCREEN_HEIGHT)
		draw_end = SCREEN_HEIGHT - 1;

	/* choose wall color */
	color.r = 0, color.g = 255, color.b = 0, color.a = 255;

	/* give x and y sides different brightness */
	if (wall_direction == 1)
	{
		color.r /= 2;
		color.g /= 2;
		color.b /= 2;
	}

	//draw the pixels of the stripe as a vertical line
	// verLine(x, drawStart, drawEnd, color);
	SDL_SetRenderDrawColor(instance->renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(instance->renderer, pos_x, draw_start, pos_x, draw_end);
}