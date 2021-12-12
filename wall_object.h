#include "game_object.h"
#include "util.h"
#include "screen.h"

#include "stdlib.h"


struct wall_object_t;
typedef struct wall_object_t wall_object_t;
// typedef struct{
//     game_object_t o;
//     int width;
//     int height;
// } wall_object_t;


void wall_draw_object(game_object_t* g);

int wall_object_collision(game_object_t* g, si_handle s, int* max_y_each_column, int* hit_block_each_column, int max_y);

void wall_timer_tick(game_object_t* g);

void wall_inserting(si_handle s, game_object_t* g);

void wall_destroy(game_object_t* g);

int wall_get_type();

rectangle_t* wall_get_bounding_box(game_object_t* g);

int wall_get_x(game_object_t* g);

int wall_get_y(game_object_t* g);

// static game_object_vtable_t wall_vtable;
// {
//     .draw_object = wall_draw_object,
//     .object_collision = wall_object_collision,
//     .timer_tick = wall_timer_tick,
//     .inserting = wall_intersecting,
//     .destroy = wall_destroy
// };


wall_object_t* create_wall(screen_t* s, int x, int y);
// {
//     wall_object_t* wall = malloc(sizeof(*wall));
//     wall->o.vtable = &wall_vtable;
//     wall->width = x;
//     wall->height = y;
//     return wall;
// }
