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
	bool hit;
	int side; //was a NS or an EW wall hit?

	for(int x = 0; x < SCREEN_WIDTH; x++)
	{
		/* Calculate ray position and direction */
		cameraX = 2 * x / (double)(SCREEN_WIDTH) - 1;  /* x-coordinate in camera space */
		ray_dir.x = dir.x + plane.x * cameraX;
		ray_dir.y = dir.y + plane.y * cameraX;

		/* Set the grid coordinates on the maze */
		map.x = (int)(pos.x);
		map.y = (int)(pos.y);

		/* Compute distance to next X or Y intersection. */
		delta_dist.x = sqrt(1 + (pow(ray_dir.y, 2)) / pow(ray_dir.x, 2));
		delta_dist.y = sqrt(1 + (pow(ray_dir.x, 2)) / pow(ray_dir.y, 2));

		/* Calculate step and initial side_dist from pos to first X or Y */
		if (ray_dir.x < 0)
		{
			step.x = -1;
			side_dist.x = (pos.x - map.x) * delta_dist.x;
		}
		else
		{
			step.x = 1;
			side_dist.x = (map.x + 1.0 - pos.x) * delta_dist.x;
		}

		if (ray_dir.y < 0)
		{
			step.y = -1;
			side_dist.y = (pos.y - map.y) * delta_dist.y;
		}
		else
		{
			step.y = 1;
			side_dist.y = (map.y + 1.0 - pos.y) * delta_dist.y;
		}

		hit = false;  /* was there wall hit? */
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
		draw_stripe(instance, x, side, side_dist, delta_dist, map);
	}
  SDL_RenderPresent(instance->renderer);
}

/**
 * draw_stripe - Draws a stripe of the wall on the window
 * @instance: a pointer to a struct containing the window and renderer as members
 * @pos_x: the horizontal position of the wall stripe be drawn on the window
 * @wall_direction: indicates whether the wall is North-South or East-West facing
 * @side_dist: length of ray from current position to next x or y-side
 * @delta_dist: length of ray from one x or y-side to next x or y-side
 */
void draw_stripe(
	SDL_Instance *instance, int pos_x, int wall_direction,
	point_double_t side_dist, point_double_t delta_dist, point_int_t map)
{
	int line_height, draw_start, draw_end;
	SDL_Color color;
	SDL_Color RGB_Red = {255, 0, 0, 255};
	SDL_Color RGB_Green = {0, 255, 0, 255};
	SDL_Color RGB_Blue = {0, 0, 255, 255};
	SDL_Color RGB_White = {255, 255, 255, 255};
	SDL_Color RGB_Yellow = {255, 255, 0, 255};

	double wall_distance;  /* Perpendicular wall distance */

	/* Calculate distance projected on camera direction. */
	if (wall_direction == 0)
		wall_distance = (side_dist.x - delta_dist.x);
	else
		wall_distance = (side_dist.y - delta_dist.y);

	/* Correct fish bowl effect */
	wall_distance = wall_distance * cos(plane.x);

	/* Calculate height of line to draw on screen */
	line_height = (int)(SCREEN_HEIGHT / wall_distance);

	/* calculate lowest and highest pixel to fill in current stripe */
	draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
	if (draw_start < 0)
		draw_start = 0;

	draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
	if (draw_end >= SCREEN_HEIGHT)
		draw_end = SCREEN_HEIGHT - 1;

	/* Choose wall color */
  switch (WORLD_MAP[map.x][map.y])
  {
    case 1: color = RGB_Red;
    case 2: color = RGB_Green;
    case 3: color = RGB_Green;
    case 4: color = RGB_White;
    default: color = RGB_Yellow;
  }
	/* give x and y sides different brightness */
	if (wall_direction == 1)
	{
		color.r /= 2;
		color.g /= 2;
		color.b /= 2;
	}

	//draw the pixels of the stripe as a vertical line
	// verLine(x, drawStart, drawEnd, color);
	SDL_SetRenderDrawColor(
		instance->renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(
		instance->renderer, pos_x, draw_start, pos_x, draw_end);
}