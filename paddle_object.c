#include "paddle_object.h"


struct paddle_object_t{
    game_object_t o;
    screen_t* screen;
    int cur_x;
    int cur_y;
    int width;  // width for paddle
    int height;  // height for paddle
    rectangle_t bounding_box;
    point_t p1;
    point_t p2;
};

rectangle_t* paddle_get_bounding_box(game_object_t* g){
    paddle_object_t* p = (paddle_object_t*)g;
    paddle_update_bounding_box(p);
    return &p->bounding_box;
}

void paddle_update_bounding_box(paddle_object_t* p){
    p->p1 = point_create(p->cur_x, p->cur_y);
    p->p2 = point_create(p->cur_x + p->width, p->cur_y + p->height);
    p->bounding_box = rectangle_create(p->p1, p->p2);
}

void paddle_draw_object(game_object_t* g){
    paddle_object_t* p = (paddle_object_t*)g;
    for (int i = p->cur_x; i < p->cur_x + p->width; i++){
        for (int j = p->cur_y; j < p->cur_y + p->height; j++){
            screen_set(p->screen, i, j, '=');
        }
    }
}

// set the current position for paddle, with consideration for both vertical and horizontal
void paddle_set_position(paddle_object_t* p, bool left, bool right, bool up, bool down){
    if (left){
        p->cur_x -= 2;
    }
    else if (right)
    {
        p->cur_x += 2;
    }
    else if (up){
        p->cur_y -= 1;
    }
    else if (down){
        p->cur_y += 1;
    }
}

int paddle_object_collision(game_object_t* g, si_handle s, int* max_y_each_column, int* hit_block_each_column, int max_y){
    return 0;
}

void paddle_timer_tick(game_object_t* g){
    return;
}

void paddle_inserting(si_handle s, game_object_t* g){
    si_insert(s, g);
}

void paddle_destroy(game_object_t* g){
    paddle_object_t* p = (paddle_object_t*)g;
    free(p);
}

int paddle_get_type(){
    return 1;
}

int paddle_get_x(game_object_t* g){
    paddle_object_t* p = (paddle_object_t*)g;
    return p->cur_x;
}

int paddle_get_y(game_object_t* g){
    paddle_object_t* p = (paddle_object_t*)g;
    return p->cur_y;
}

static game_object_vtable_t paddle_vtable = {
    .draw_object = paddle_draw_object,
    .object_collision = paddle_object_collision,
    .timer_tick = paddle_timer_tick,
    .inserting = paddle_inserting,
    .destroy = paddle_destroy,
    .get_type = paddle_get_type,
    .get_bounding_box = paddle_get_bounding_box,
    .get_x = paddle_get_x,
    .get_y = paddle_get_y
};


paddle_object_t* create_paddle(screen_t* s, int x, int y, int width, int height){
    paddle_object_t* paddle = malloc(sizeof(*paddle));
    paddle->o.vtable = &paddle_vtable;
    paddle->cur_x = x;
    paddle->cur_y = y;
    paddle->width = width;
    paddle->height = height;
    paddle->screen = s;

    paddle->p1 = point_create(x, y);
    paddle->p2 = point_create(x + width, y + height);
    paddle->bounding_box = rectangle_create(paddle->p1, paddle->p2);
    return paddle;
}
