#include "game_object.h"
#include "util.h"
#include "screen.h"
#include "spatial_index.h"
#include "block_object.h"

#include "stdlib.h"


struct ball_object_t;
typedef struct ball_object_t ball_object_t;

void ball_draw_object(game_object_t* g);
int ball_object_collision(game_object_t* g, si_handle s, int* max_y_each_column, int* hit_block_each_column, int max_y);
void ball_timer_tick(game_object_t* g);
void ball_inserting(si_handle s, game_object_t* g);
void ball_destroy(game_object_t* g);
int ball_get_type();
void ball_update_bounding_box(ball_object_t* g);
rectangle_t* ball_get_bounding_box(game_object_t* g);

ball_object_t* create_ball(screen_t* screen, double x, double y);

void ball_get_next_dxdy(game_object_t* g, int left, int right, int upper);  // specific to ball_object_t
bool ball_out_of_scope(game_object_t* g, int lowerbound);  // specific to ball_object_t
int ball_get_x(game_object_t* g); 
int ball_get_y(game_object_t* g);  
int ball_get_prev_y(game_object_t* g);  // specific to ball_object_t