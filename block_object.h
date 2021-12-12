#include "game_object.h"
#include "util.h"
#include "screen.h"
#include "spatial_index.h"

#include "stdlib.h"

struct block_object_t;
typedef struct block_object_t block_object_t;


void block_draw_object(game_object_t* g);

int block_object_collision(game_object_t* g, si_handle s, int* max_y_each_column, int* hit_block_each_column, int max_y);

void block_timer_tick(game_object_t* g);

void block_inserting(si_handle s, game_object_t* g);

void block_destroy(game_object_t* g);

int block_get_type();

block_object_t* create_block(screen_t* s, int x, int y, int width, int height, int row, int column, int score);

rectangle_t* block_get_bounding_box(game_object_t* g);

void set_block_hit(game_object_t* g);

bool get_block_hit(game_object_t* g);

int get_block_row(game_object_t* g);
int get_block_column(game_object_t* g);
int block_get_x(game_object_t* g);
int block_get_y(game_object_t* g);
int get_block_score(game_object_t* g);
