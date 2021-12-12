#include "game_object.h"
#include "util.h"
#include "screen.h"
#include "spatial_index.h"

#include "stdlib.h"

struct paddle_object_t;
typedef struct paddle_object_t paddle_object_t;

// typedef struct{
//     game_object_t o;
//     point_t start_pos;
//     rectangle_t rec;
// } paddle_object_t;


void paddle_draw_object(game_object_t* g);

int paddle_object_collision(game_object_t* g, si_handle s, int* max_y_each_column, int* hit_block_each_column, int max_y);
void paddle_timer_tick(game_object_t* g);

void paddle_inserting(si_handle s, game_object_t* g);

void paddle_destroy(game_object_t* g);

void paddle_set_position(paddle_object_t* p, bool left, bool right, bool up, bool down);

int paddle_get_type();
rectangle_t* paddle_get_bounding_box(game_object_t* g);
void paddle_update_bounding_box(paddle_object_t* p);
int paddle_get_x(game_object_t* g);
int paddle_get_y(game_object_t* g);
// static game_object_vtable_t paddle_vtable;
// = {
//     .draw_object = paddle_draw_object,
//     .object_collision = paddle_object_collision,
//     .timer_tick = paddle_timer_tick,
//     .inserting = paddle_intersecting,
//     .destroy = paddle_destroy
// };


paddle_object_t* create_paddle(screen_t* s, int x, int y, int width, int height);
// {
//     paddle_object_t* paddle = malloc(sizeof(*paddle));
//     paddle->o.vtable = &paddle_vtable;
//     point_t newp1 = point_create(x, y);
//     paddle->start_pos = newp1;
//     point_t newp2 = point_create(x + width, y + height);
//     paddle->rec = rectangle_create(newp1, newp2);
//     return paddle;
// }
