#include "ball_object.h"
#include "math.h"


struct ball_object_t{
    game_object_t o;
    screen_t* screen;
    double dx;  // dx
    double dy;  // dy
    double x_true;  // the true x for ball
    double y_true;  // the true y for ball
    int x;  // the rounded x for ball
    int y;  // the rounded y for ball
    int x_prev;  // the previous x
    int y_prev;  // the previous y
    
    int collision_status;  // see below
    rectangle_t bounding_box;
    rectangle_t intersect_box;  // placeholder for rectangle_intersect
    point_t p1;
    point_t p2;
};

struct si_visit_collection{
    int collision_type;
    rectangle_t* ball_bounding_box;
    rectangle_t* for_intersect;
    int* max_y_each_column;
    int* hit_block_each_column;
    int block_row;
    int ball_x;
    int ball_y;
    int ball_y_prev;
};
typedef struct si_visit_collection si_visit_collection;


void ball_draw_object(game_object_t* g){
    ball_object_t* b = (ball_object_t*)g;
    b->x_prev = b->x;
    b->y_prev = b->y;
    b->x_true += b->dx;
    b->y_true += b->dy;
    b->x = round(b->x_true);
    b->y = round(b->y_true);
    screen_set(b->screen, b->x, b->y, 'O');
}

void ball_update_bounding_box(ball_object_t* b){
    b->p1 = point_create(b->x, b->y);
    b->p2 = point_create(b->x + 1, b->y + 1);
    b->bounding_box = rectangle_create(b->p1, b->p2);
}

rectangle_t* ball_get_bounding_box(game_object_t* g){
    ball_object_t* b = (ball_object_t*)g;
    ball_update_bounding_box(b);
    return &b->bounding_box;
}

bool si_visit_func_1(void * element, void * userdata){
    // userdata is si_visit_collection*
    game_object_t* g = (game_object_t*)element;

    if (get_type(g) == 0){
        return true;
    }

    rectangle_t* box_of_element = get_bounding_box(g);
    si_visit_collection* t = (si_visit_collection*)userdata;
    bool status = rectangle_intersect(box_of_element, t->ball_bounding_box, t->for_intersect);
    if (status){
        if (get_type(g) == 1)  // paddle
        {
            t->collision_type = 1;
            return false;
        }
        else if (get_type(g) == 2){  // blocks
            int row = get_block_row(g);
            int column = get_block_column(g);
            t->hit_block_each_column[column] += 1;  // record the number of hit blocks
            int block_x = get_x(g);
            int block_y = get_y(g);
            if (row >= t->max_y_each_column[column]){
                if (block_y == t->ball_y){
                    t->collision_type = 5;  // if the ball hits the lowest block of a column from side,
                                            // then it should have reversed dx and abs(dy)
                }
                else{
                    if (block_y < t->ball_y){
                        if (t->ball_y_prev > t->ball_y){
                            t->collision_type = 2;  // hit the lowest from bottom with up speed, it have reversed dy
                        }
                        else{
                            t->collision_type = 5;  // hit the lowest from bottom with down speed, reversed dx and abs(dy)
                        }
                    }
                    else{
                        t->collision_type = 2;  // hit the lowest from upper, reversed dy
                    }
                }
                t->max_y_each_column[column] = t->block_row - t->hit_block_each_column[column];
            }
            else{  // hit the block above (must be from side)
                if (block_x >= t->ball_x){
                    t->collision_type = 3;  // bounce to left
                }
                else{
                    t->collision_type = 4;  // bounce to right
                }
            }
            set_block_hit(g);
            return false;
        }
    }
    return true;
}

int ball_get_prev_y(game_object_t* g){
    ball_object_t* b = (ball_object_t*)g;
    return b->y_prev;
}

int ball_object_collision(game_object_t* g, si_handle s, int* max_y_each_column, int* hit_block_each_column, int max_y){  // int* hit_block_each_column, 
    rectangle_t* ball_bounding_box = ball_get_bounding_box(g);
    ball_object_t* b = (ball_object_t*)g;

    si_visit_collection t;
    t.collision_type = 0;
    t.ball_bounding_box = ball_bounding_box;
    t.for_intersect = &b->intersect_box;
    t.max_y_each_column = max_y_each_column;
    t.hit_block_each_column = hit_block_each_column;
    t.block_row = max_y;
    t.ball_x = get_x(g);
    t.ball_y = get_y(g);
    t.ball_y_prev = ball_get_prev_y(g);

    // get the collision type
    si_walk(s, si_visit_func_1, &t);

    b->collision_status = t.collision_type;    
    return 0;
    // return 0;
}

void ball_get_next_dxdy(game_object_t* g, int left, int right, int upper){
    ball_object_t* b = (ball_object_t*)g;
    double delta = (.99 + ((rand() % 2)/(double)100));
    // collision with paddle
    if (b->collision_status == 1){
        b->dy *= -1 * delta;
    }
    // collision with block
    else if (b->collision_status == 2){
        b->dy *= -1 * delta;
        // exit(1);
    }
    else if ((b->collision_status == 3) || (b->collision_status == 4))
    {
        b->dx *= -1 * delta;
    }
    else if (b->collision_status == 5){
        b->dy = fabs(b->dy);
        b->dx *= -1 * delta;
    }
    // collision with wall
    if ((b->x >= right) || (b->x <= left)){
        b->dx *= -1 * delta;
    }
    if (b->y <= upper){
        b->dy *= -1 * delta;
    }
}

bool ball_out_of_scope(game_object_t* g, int lowerbound){
    ball_object_t* b = (ball_object_t*)g;
    if (b->y >= lowerbound){
        return true;
    }
    return false;
}

void ball_timer_tick(game_object_t* g){
    return;
}

void ball_inserting(si_handle s, game_object_t* g){
    si_insert(s, g);
}

void ball_destroy(game_object_t* g){
    ball_object_t* b = (ball_object_t*)g;
    free(b);
}

int ball_get_type(){
    return 0;
}

int ball_get_x(game_object_t* g){
    ball_object_t* b = (ball_object_t*)g;
    return b->x;
}

int ball_get_y(game_object_t* g){
    ball_object_t* b = (ball_object_t*)g;
    return b->y;
}

static game_object_vtable_t ball_vtable = {
    .draw_object = ball_draw_object,
    .object_collision = ball_object_collision,
    .timer_tick = ball_timer_tick,
    .inserting = ball_inserting,
    .destroy = ball_destroy,
    .get_type = ball_get_type,
    .get_bounding_box = ball_get_bounding_box,
    .get_x = ball_get_x,
    .get_y = ball_get_y
};


ball_object_t* create_ball(screen_t* screen, double x, double y){
    ball_object_t* ball = malloc(sizeof(*ball));
    ball->o.vtable = &ball_vtable;
    ball->x = (int)x;
    ball->y = (int)y;
    ball->x_prev = ball->x;
    ball->y_prev = ball->y;
    ball->screen = screen;
    ball->dx = 1.0;
    ball->dy = -1.0;
    ball->x_true = x;
    ball->y_true = y;
    ball->collision_status = 0;

    ball->p1 = point_create(ball->x, ball->y);
    ball->p2 = point_create(ball->x + 1, ball->y + 1);
    ball->bounding_box = rectangle_create(ball->p1, ball->p2);

    ball->intersect_box = rectangle_create(ball->p1, ball->p2);

    return ball;
}
