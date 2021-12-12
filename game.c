
// This indicates that we want the POSIX-2001 (and before) functions as
// well.
#define _POSIX_C_SOURCE 200112L

#include "screen.h"
#include "game_object.h"
#include "game.h"
#include "util.h"
#include "spatial_index.h"
#include "paddle_object.h"
#include "ball_object.h"
#include "wall_object.h"
#include "block_object.h"

#include <assert.h>
#include <time.h> //  nanosleep, POSIX.1-2008
#include <sys/time.h> // gettimeofday
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NANOSEC_PER_SEC ((uint64_t) 1000000000ull)
#define MICROSEC_PER_SEC ((uint64_t) 1000000ull)
#define TIMER_TICK_PER_SEC 10

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

// COMPLETE ME
// COMPLETE ME
#define PADDLE_WID 9
#define PADDLE_HEI 1
#define BLOCK_WID 6
#define BLOCK_HEI 1
#define BLOCK_ROW 4


bool si_intersects_func_func(const rectangle_t * p, const void * element, void * userdata){
    return true;
}

void si_bounding_box_func(const void * element, rectangle_t * rect, void * userdata){
    return;
}

bool si_visit_func(void * element, void * userdata){
    game_object_draw_object(element);
    return true;
}

bool si_visit_destroy(void * element, void * userdata){
    game_object_t* g = (game_object_t*)element;
    game_object_destroy(g);  // free the objects in spatial index, possibly a ball, a paddle, a block or a wall
    return true;
}

bool si_visit_func_2(void * element, void * userdata){
    game_object_t* g = (game_object_t*)element;
    game_data_t* d = (game_data_t*)userdata;
    if ((get_type(g) == 2) && (get_block_hit(g))){
        d->score += get_block_score(g);  // calculate the score
        si_remove(d->game_objects, g);  // remove the block from spatial index
        game_object_destroy(g);  // free the block
        return false;
    }
    return true;
}

void game_initialize(game_data_t * game){
    game->screen = screen_init();
    game->level_done = false;
    // spatial index for storing all the objects
    game->game_objects = si_create(
        si_intersects_func_func,
        si_bounding_box_func,
        NULL
    );
    
    game->score = 0;

    // get screen size
    const int maxx = screen_getmaxx(game->screen)-1;
    const int maxy = screen_getmaxy(game->screen)-1;

    game->lower_bound = maxy;
    double ball_x = ((double) maxx)/2;
    double ball_y = ((double) maxy) - 2; 

    int block_num = maxx / BLOCK_WID;
    if (block_num % 2 == 1){
        block_num -= 4;
    }
    else{
        block_num -= 5;
    }
    game->block_num_in_row = block_num;
    // malloc the space for the two fields
    game->max_y_each_column = (int*)malloc(sizeof(int)*game->block_num_in_row);
    for (int i = 0; i < game->block_num_in_row; i++){
        game->max_y_each_column[i] = BLOCK_ROW - 1;
    }
    game->hit_block_each_column = (int*)malloc(sizeof(int)*game->block_num_in_row);
    for (int i = 0; i < game->block_num_in_row; i++){
        game->hit_block_each_column[i] = 0;
    }

    // create game objects
    // ball
    game_object_t* ball = (game_object_t*)create_ball(game->screen, ball_x, ball_y);
    game->ball = ball;
    game_object_inserting(game->game_objects, ball);
    
    // paddle
    double paddle_x = ball_x - PADDLE_WID / 2;
    double paddle_y = ((double) maxy); 
    game_object_t* paddle = (game_object_t*)create_paddle(game->screen, paddle_x, paddle_y, PADDLE_WID, PADDLE_HEI);
    game->paddle = paddle;
    game_object_inserting(game->game_objects, paddle);

    // block
    double block_x, block_y, block_x_left, block_x_right;
    int row, column, column_left, column_right;
    for (int i = BLOCK_ROW; i > 0; i--){
        block_x = ball_x - BLOCK_WID / 2;
        block_y = i * BLOCK_HEI;
        row = i - 1;
        column = (int)(game->block_num_in_row / 2);
        game_object_t* block = (game_object_t*)create_block(game->screen, block_x, block_y, BLOCK_WID, BLOCK_HEI, row, column, BLOCK_ROW-i+1);
        game_object_inserting(game->game_objects, block);
        
        for (int j = 0; j < game->block_num_in_row / 2; j++){
            block_x_left = block_x - (j + 1) * BLOCK_WID;
            column_left = column - (j + 1);
            game_object_t* block_1 = (game_object_t*)create_block(game->screen, block_x_left, block_y, BLOCK_WID, BLOCK_HEI, row, column_left, BLOCK_ROW-i+1);
            game_object_inserting(game->game_objects, block_1);
            
            block_x_right = block_x + (j + 1) * BLOCK_WID;
            column_right = column + (j + 1);
            game_object_t* block_2 = (game_object_t*)create_block(game->screen, block_x_right, block_y, BLOCK_WID, BLOCK_HEI, row, column_right, BLOCK_ROW-i+1);
            game_object_inserting(game->game_objects, block_2);
        }
    }
    game->left_wall = block_x_left - 1;
    game->right_wall = block_x_right + BLOCK_WID;

    // wall
    for (int i = 0; i < maxy; i++){
        game_object_t* left_wall = (game_object_t*)create_wall(game->screen, game->left_wall, i);
        game_object_inserting(game->game_objects, left_wall);
    }
    for (int i = 0; i < maxy; i++){
        game_object_t* right_wall = (game_object_t*)create_wall(game->screen, game->right_wall, i);
        game_object_inserting(game->game_objects, right_wall);
    }
    for (int i = game->left_wall; i < game->right_wall; i++){
        game_object_t* upper_wall = (game_object_t*)create_wall(game->screen, i, 0);
        game_object_inserting(game->game_objects, upper_wall);
    }
}


void game_destroy(game_data_t * data)
{
    si_walk(data->game_objects, si_visit_destroy, NULL);  // free all remaining the objects in spatial index
    si_destroy(data->game_objects);  // free the spatial index itself
    free(data->max_y_each_column);  // free the malloc'd space
    free(data->hit_block_each_column);  // same
    screen_done(data->screen);
}

// Adds the game objects to the index
// Also check the the screen is the large enough (you can assume the screen
// won't change in size during the game)
static void create_level(game_data_t * data)
{
    return;
}


// move paddle left or right
static void move_paddle(game_data_t * data, bool left, bool right, bool up, bool down)
{
    paddle_object_t* p = (paddle_object_t*)data->paddle;
    paddle_set_position(p, left, right, up, down);
}

// Returns the current system time in microseconds since the UNIX epoch
// (only use for relative comparisons)
static uint64_t get_time_in_us()
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) < 0)
    {
        perror("Problem getting time!");
        exit(1);
    }
    return ((uint64_t) tv.tv_sec * MICROSEC_PER_SEC) + tv.tv_usec;
}

// Suspend execution for the specified number of microseconds
static void sleep_for_us(uint64_t us)
{
    struct timespec t;
    t.tv_sec = us / MICROSEC_PER_SEC;
    t.tv_nsec = (NANOSEC_PER_SEC/MICROSEC_PER_SEC)*us;
    nanosleep(&t, NULL);
}

// Cause all objects to draw themselves on the screen.
static void draw_objects(game_data_t * data)
{
    screen_clear(data->screen, data->score);
    si_walk(data->game_objects, si_visit_func, NULL);
    screen_refresh(data->screen);
}

// Let all game objects know that there was a timer tick
static void do_timer_tick(game_data_t * data)
{
    // game_object_timer_tick(data->ball);
    game_object_object_collision(data->ball, data->game_objects, data->max_y_each_column, data->hit_block_each_column, BLOCK_ROW-1);
    if (ball_out_of_scope(data->ball, data->lower_bound)){
        data->level_done = true;
    }
}

// Remove all the objects from data->to_remove from the spatial index.
// Don't forget to empty data->to_remove when done.
static void possibly_remove_objects(game_data_t * data)
{
    // if(!si_walk(data->game_objects, si_visit_func_2, data)){
    //     data->score += 1;
    // }
    si_walk(data->game_objects, si_visit_func_2, data);
}

// Do what is needed to let the spatial index know that the objects
// in to_update could have been moved.
// Clear to_update when done.
static void possibly_update_objects(game_data_t * data)
{
    ball_get_next_dxdy(data->ball, data->left_wall+1, data->right_wall-1, 1);
}

int main(int argc, char ** args)
{
    // Initialize game data
    game_data_t data;

    game_initialize(&data);

    // Optional: add a level system,
    // generating a different (more difficult?) board for every level.

    create_level(&data);

    // Number of microseconds since unix epoch.
    uint64_t next_timer_tick = get_time_in_us();

    bool stop = false;

    while (!data.level_done && !stop)
    {
       // Check if a key was pressed
        const int key = getch();
        switch (key)
        {
            case 'q':
                stop = true;
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_UP:
            case KEY_DOWN:
                move_paddle(&data, key == KEY_LEFT, key == KEY_RIGHT, key == KEY_UP, key == KEY_DOWN);
                break;
        }
        // draw_objects(&data);
        // Check if we need to issue a timer tick to the objects.
        if (get_time_in_us() > next_timer_tick)
        {
            next_timer_tick += (MICROSEC_PER_SEC/TIMER_TICK_PER_SEC);

            // Tell all the objects some time has passed
            // They can respond by moving (and possibly colliding!),
            // updating their internal parameters, ...
            //
            // NOTE: See the discussion in the assigment regarding modifying
            // the spatial index while it is being enumerated.
            do_timer_tick(&data);

            // Check if we need to remove or move any game objects
            possibly_remove_objects(&data);
            possibly_update_objects(&data);

            // redraw the game objects
            draw_objects(&data);
        }


        // sleep 1ms (or until the next timer tick)
        // (to avoid spending 100% CPU polling the keyboard)
        // NOTE that if we somehow fell behind (next_timer_tick <
        // get_time_in_us) we won't sleep but process the pending timer ticks
        // until we're back caught up.
        if (next_timer_tick > get_time_in_us())
            sleep_for_us(MIN(1000, next_timer_tick - get_time_in_us()));
    }

    game_destroy(&data);

    return EXIT_SUCCESS;
}
