#include "block_object.h"

struct block_object_t{
    game_object_t o;
    screen_t* screen;
    int x;
    int y;
    int width;
    int height;
    bool hit;
    int score;
    point_t p1;
    point_t p2;
    rectangle_t bounding_box;
    int row;
    int column;
};

void block_draw_object(game_object_t* g){
    block_object_t* b = (block_object_t*)g;
    for (int i = b->x; i < b->x + b->width; i++){
        for (int j = b->y; j < b->y + b->height; j++){
            screen_set(b->screen, i, j, '#');
        }
    }
}

int block_object_collision(game_object_t* g, si_handle s, int* max_y_each_column, int* hit_block_each_column, int max_y){
    return 0;
}

void block_timer_tick(game_object_t* g){
    return;
}

void block_inserting(si_handle s, game_object_t* g){
    si_insert(s, g);
}

void block_destroy(game_object_t* g){
    block_object_t* b = (block_object_t*)g;
    free(b);
}

int block_get_type(){
    return 2;
}

rectangle_t* block_get_bounding_box(game_object_t* g){
    block_object_t* b = (block_object_t*)g;
    return &b->bounding_box;
}

void set_block_hit(game_object_t* g){
    block_object_t* b = (block_object_t*)g;
    b->hit = true;
}

bool get_block_hit(game_object_t* g){
    block_object_t* b = (block_object_t*)g;
    return b->hit;
}

int get_block_row(game_object_t* g){
    block_object_t* b = (block_object_t*)g;
    return b->row;
}

int get_block_column(game_object_t* g){
    block_object_t* b = (block_object_t*)g;
    return b->column;
}

int block_get_x(game_object_t* g){
    block_object_t* b = (block_object_t*)g;
    return b->x;
}

int block_get_y(game_object_t* g){
    block_object_t* b = (block_object_t*)g;
    return b->y;
}

int get_block_score(game_object_t* g){
    block_object_t* b = (block_object_t*)g;
    return b->score;
}

static game_object_vtable_t block_vtable = {
    .draw_object = block_draw_object,
    .object_collision = block_object_collision,
    .timer_tick = block_timer_tick,
    .inserting = block_inserting,
    .destroy = block_destroy,
    .get_type = block_get_type,
    .get_bounding_box = block_get_bounding_box,
    .get_x = block_get_x,
    .get_y = block_get_y
};

block_object_t* create_block(screen_t* s, int x, int y, int width, int height, int row, int column, int score){
    block_object_t* block = malloc(sizeof(*block));
    block->o.vtable = &block_vtable;
    block->screen = s;
    block->width = width;
    block->height = height;
    block->x = x;
    block->y = y;
    block->hit = false;
    block->score = score;

    block->p1 = point_create(x, y);
    block->p2 = point_create(x + width, y + height);
    block->bounding_box = rectangle_create(block->p1, block->p2);

    block->row = row;
    block->column = column;

    return block;
}
