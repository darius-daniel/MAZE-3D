#ifndef _MAZE_H_
#define _MAZE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* ==================================================================
 * Structs
 * ==================================================================
*/

/**
 * struct SDL_Instance - holds the addresses of the
 * @window: a running game window instance
 * @renderer: the maze renderer
*/
typedef struct SDL_Instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
} SDL_Instance;

/**
 * struct point_int_s - defines a two-dimensional point with integer values
 * for the x- and y-coordinates
 * @x: the x-coordinate
 * @y: the y-coodinate
*/
typedef struct point_int_s
{
	int x;
	int y;
} point_int_t;


/**
 * struct point_double_s - defines a two-dimensional point with floating point
 * values for the x- and y-coordinates
 * @x: the x-coordinate
 * @y: the y-coodinate
*/
typedef struct point_double_s
{
	double x;
	double y;
} point_double_t;

/* ==================================================================
 * Macros
 * ==================================================================
*/
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define SCREEN_WIDTH 1260
#define SCREEN_HEIGHT 720

/* ==================================================================
 * Global Variables
 * ==================================================================
*/
extern SDL_Instance instance;
extern int WORLD_MAP[MAP_HEIGHT][MAP_WIDTH];

extern point_double_t plane;
extern point_double_t pos;
extern point_double_t dir;

extern double time;


/* ==================================================================
 * Prototypes
 * ==================================================================
*/
int init_instance(SDL_Instance *);
int done();
void cast_ray(SDL_Instance *instance);
void draw_stripe(
	SDL_Instance *instance, int pos_x, int wall_direction,
	point_double_t side_dist, point_double_t delta_dist,
	point_int_t map, point_double_t ray_direction);
void move();

#endif
