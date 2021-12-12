#pragma once

#include "screen.h"
#include "spatial_index.h"
#include "paddle_object.h"
#include "vararray.h"
#include "util.h"

// Useful information for the objects to manipulate the game environment
// This will typically be passed to the objects during construction,
// and you should guarantee that this struct remains valid until all
// game objects are removed.
typedef struct game_data_t game_data_t;


// Ideally, this should be considered an opaque data structure, and accessor methods
// (below) should be used to access the data/influence the game,
// as this will isolate the game objects from changes in this structure.

struct game_data_t
{
    screen_t * screen;
    bool level_done;

    si_handle game_objects;   // All game objects (game objects are visible 
                               // on the screen.
    
    // quick access to ball and paddle
    game_object_t* ball;
    game_object_t* paddle;

    // walls and edges
    int left_wall;
    int right_wall;
    int lower_bound;

    // record the lowest block of each column
    int* max_y_each_column;
    // record the number of hit block for each column
    int* hit_block_each_column;
    // the score
    int score;
    // record the number of blocks in a row
    int block_num_in_row;
};


// Setup the gamedata structure
void game_initialize(game_data_t * game);

void game_destroy(game_data_t * game);

// Helper functions (these are called by the game objects to control the overall game)
//
// The idea is that these functions decouple the objects from the game_data_t structure.
// Ideally, you'd move the structure definition to game.c and only allow access via
// the functions in this header.
//
// Feel free to modify/add/remove as you see fit.

// Mark an object as moved
static inline void game_need_update(game_data_t * game, void * obj)
{
// COMPLETE ME
}

// Request than an object gets removed from the game
static inline void game_need_remove(game_data_t * game, void * obj)
{
// COMPLETE ME
}

// Indicate that the level should end because it was won/lost
static inline void game_finish_level(game_data_t * game, bool won)
{
  game->level_done = true;
}

// Return rectangle representing game screen area; Just a convenience
// function for the game objects, so they don't need to interrogate the screen
// every time the dimensions are needed.
// static inline const rectangle_t *  game_get_area(game_data_t * game)
// {
//   // TODO
// }

// Return a game object intersecting with the specified rectangle;
// returns NULL if no such object exists
void * game_intersect(game_data_t * game, const rectangle_t * rect);
