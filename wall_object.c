#include "wall_object.h"


struct wall_object_t{
    game_object_t o;
    int x1;
    int y1;
    screen_t* screen;
    rectangle_t bounding_box;
};


void wall_draw_object(game_object_t* g){
    wall_object_t* w = (wall_object_t*)g;
    screen_set(w->screen, w->x1, w->y1, '+');
    // screen_refresh(w->screen);
}

int wall_object_collision(game_object_t* g, si_handle s, int* max_y_each_column, int* hit_block_each_column, int max_y){
    return 0;
}

void wall_timer_tick(game_object_t* g){
    return;
}

void wall_inserting(si_handle s, game_object_t* g){
    si_insert(s, g);
}

void wall_destroy(game_object_t* g){
    wall_object_t* w = (wall_object_t*)g;
    free(w);
}

int wall_get_type(){
    return 3;
}

int wall_get_x(game_object_t* g){
    wall_object_t* w = (wall_object_t*)g;
    return w->x1;
}

int wall_get_y(game_object_t* g){
    wall_object_t* w = (wall_object_t*)g;
    return w->y1;
}

rectangle_t* wall_get_bounding_box(game_object_t* g){
    wall_object_t* w = (wall_object_t*)g;
    return &w->bounding_box;
}

static game_object_vtable_t wall_vtable = {
    .draw_object = wall_draw_object,
    .object_collision = wall_object_collision,
    .timer_tick = wall_timer_tick,
    .inserting = wall_inserting,
    .destroy = wall_destroy,
    .get_type = wall_get_type,
    .get_bounding_box = wall_get_bounding_box,
    .get_x = wall_get_x,
    .get_y = wall_get_y
};


wall_object_t* create_wall(screen_t* s, int x, int y){
    wall_object_t* wall = malloc(sizeof(*wall));
    wall->o.vtable = &wall_vtable;
    wall->x1 = x;
    wall->y1 = y;
    // wall->x2 = x1;
    // wall->y2 = y1;
    wall->screen = s;
    point_t p1 = point_create(x, y);
    point_t p2 = point_create(x+1, y+1);
    wall->bounding_box = rectangle_create(p1, p2);

    return wall;
}
