#pragma once

#include "util.h"
#include "spatial_index.h"


// Forward declaration for game_data_t
// (otherwise there would be a circular dependency between game_object.h and
//  paddle_object.h)
struct game_data_t;
typedef struct game_data_t game_data_t;

struct ball_object_t;
typedef struct ball_object_t ball_object_t;

// Interface for all game objects.
// Game objects are visible on the screen.
//
// Feel free to add more methods to the interface, as you see fit.
//
// For a complete example of this technique, see the examples directory.
//

typedef struct game_object_t game_object_t;

typedef struct game_object_vtable_t game_object_vtable_t;
struct game_object_vtable_t
{
    // Methods for the game object
    //   You definitely want a method to:
    //      - draw the object on the screen
    //      - tell the object another object collided with it
    //          (best to pass the other object as well to the method)
    //      - A 'timer tick', i.e. a method which gets called
    //        every X milliseconds, so that object can perform time
    //        based actions (such as a block temporarily becoming
    //        invisible), move across the playing field, etc.
    //      - A method to allow inserting these objects in a spatial index
    //      (i.e. intersect, bounding box)
    //
    //    and of course you need a method to destroy the object as well.
    void (*draw_object)(game_object_t* object);
    int (*object_collision)(game_object_t* object, si_handle s, int* max_y_each_column, int* hit_block_each_column, int max_y);
    void (*timer_tick)(game_object_t* object);
    void (*inserting)(si_handle s, game_object_t* object);
    void (*destroy)(game_object_t* object);
    int (*get_type)();
    rectangle_t* (*get_bounding_box)(game_object_t* object);
    int (*get_x)(game_object_t* object);
    int (*get_y)(game_object_t* object);
};


struct game_object_t{
    game_object_vtable_t* vtable;
};


static inline void game_object_draw_object(game_object_t* g){
    g->vtable->draw_object(g);
}


static inline int game_object_object_collision(game_object_t* g, si_handle s, int* max_y_each_column, int* hit_block_each_column, int max_y){
    return g->vtable->object_collision(g, s, max_y_each_column, hit_block_each_column, max_y);
}


static inline void game_object_timer_tick(game_object_t* g){
    g->vtable->timer_tick(g);
}


static inline void game_object_inserting(si_handle s, game_object_t* g){
    g->vtable->inserting(s, g);
}


static inline void game_object_destroy(game_object_t* g){
    g->vtable->destroy(g);
}


static inline int get_type(game_object_t* g){
    return g->vtable->get_type();
}


static inline rectangle_t* get_bounding_box(game_object_t* g){
    return g->vtable->get_bounding_box(g);
}


static inline int get_x(game_object_t* g){
    return g->vtable->get_x(g);
}


static inline int get_y(game_object_t* g){
    return g->vtable->get_y(g);
}
