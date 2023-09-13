#include "headers/maze.h"

/**
 * move - Handles movement events
 * Return: Nothing
 * 
*/
void move()
{
	double old_time = 0; /* time of previous frame */
	double frame_time;
	double move_speed;
	double rotation_speed;
	const uint8_t *keystate;
	point_double_t old_dir;
	point_double_t old_plane;

	/* timing for input and FPS counter */
	old_time = time;
	time = SDL_GetTicks();
	frame_time = (time - old_time) / 1000.0;  /* frame_time is the time this frame has taken, in seconds */
	keystate = SDL_GetKeyboardState(NULL);

	/* speed modifiers */
	move_speed = frame_time * 5.0;  /* the constant value is in squares/second */
	rotation_speed = frame_time * 3.0;  /* the constant value is in radians/second */

   /* move forward if no wall in front of you */
	if (keystate[SDL_SCANCODE_W])
	{
		if (WORLD_MAP[(int)(pos.x + dir.x * move_speed)][(int)(pos.y)] == false)
			pos.x += dir.x * move_speed;
		if (WORLD_MAP[(int)(pos.x)][(int)(pos.y + dir.y * move_speed)] == false)
			pos.y += dir.y * move_speed;
	}
	/* move backwards if no wall behind you */
	if (keystate[SDL_SCANCODE_S])
	{
		if (WORLD_MAP[(int)(pos.x - dir.x * move_speed)][(int)(pos.y)] == false)
			pos.x -= dir.x * move_speed;
		if (WORLD_MAP[(int)(pos.x)][(int)(pos.y - dir.y * move_speed)] == false)
			pos.y -= dir.y * move_speed;
	}
	/* strafe left */
	if (keystate[SDL_SCANCODE_Q])
	{
		if (!*(WORLD_MAP + (int)(pos.x - plane.x * move_speed) * MAP_WIDTH + (int)(pos.y)))
			pos.x -= plane.x * move_speed;
		if (!*(WORLD_MAP + (int)(pos.x) * MAP_WIDTH + (int)(pos.y - plane.y * move_speed)))
			pos.y -= plane.y * move_speed;
	}

	/* strafe right */
	if (keystate[SDL_SCANCODE_E])
	{
		if (!*(WORLD_MAP + (int)(pos.x + plane.x * move_speed) * MAP_WIDTH + (int)(pos.y)))
			pos.x += plane.x * move_speed;
		if (!*(WORLD_MAP + (int)(pos.x) * MAP_WIDTH + (int)(pos.y + plane.y * move_speed)))
			pos.y += plane.y * move_speed;
	}
	/* rotate to the right */
	if (keystate[SDL_SCANCODE_RIGHT])
	{
		/* both camera direction and camera plane must be rotated */
		old_dir.x = dir.x;
		dir.x = dir.x * cos(-rotation_speed) - dir.y * sin(-rotation_speed);
		dir.y = old_dir.x * sin(-rotation_speed) + dir.y * cos(-rotation_speed);
		old_plane.x = plane.x;
		plane.x = plane.x * cos(-rotation_speed) - plane.y * sin(-rotation_speed);
		plane.y = old_plane.x * sin(-rotation_speed) + plane.y * cos(-rotation_speed);
	}
	/* rotate to the left */
	if (keystate[SDL_SCANCODE_LEFT])
	{
		/* both camera direction and camera plane must be rotated */
		old_dir.x = dir.x;
		dir.x = dir.x * cos(rotation_speed) - dir.y * sin(rotation_speed);
		dir.y = old_dir.x * sin(rotation_speed) + dir.y * cos(rotation_speed);
		old_plane.x = plane.x;
		plane.x = plane.x * cos(rotation_speed) - plane.y * sin(rotation_speed);
		plane.y = old_plane.x * sin(rotation_speed) + plane.y * cos(rotation_speed);
	}
}
