#pragma once

#include <ncurses.h>
#include <stdbool.h>

typedef char screen_char_t;

#define SCREEN_CHAR_SNAKE 1

struct screen_t;

typedef struct screen_t screen_t;

/// Show a message on the screen, user needs to press a key to continue
/// (and function doesn't return until a key has been pressed)
void screen_message (screen_t * screen, const char * s);

/// Update the score on the screen
void screen_update_score (screen_t * s,
                          unsigned int score, unsigned int highscore);

/// Create the screen area
/// Also clears the screen (see screen_clear)
///
/// The screen is made up of the playing field, and a information(score)
/// area.
///
/// The playing field has coordinates (0,0) for the !!!upper left corner!!!,
/// and (getmaxx()-1, getmaxy()-1) for the lower right corner.
///
///
screen_t * screen_init ();

/// (Re)draw the borders of the playing field, and empty the inside of the
/// screen.
/// (i.e. remove any traces of the snake, message or other content)
/// Clears the (high)score area.
void screen_clear (screen_t * scr, int score);

/// Draw a character on the playing field at the specified location
void screen_set (screen_t * scr,
                 unsigned int x, unsigned int y, screen_char_t c);

/// Return the character at the specified location in the playing field.
/// Note: empty space is made of ' '
screen_char_t screen_get (const screen_t * scr,
                          unsigned int x, unsigned int y);

/// Return the number of horizontal characters for the playing field
unsigned int screen_getmaxx (const screen_t * scr);

/// Return the number of vertical characters for the playing field
unsigned int screen_getmaxy (const screen_t * scr);

/// Called when the screen can be destroyed.
void screen_done (screen_t * scr);


// Draw a horizontal line
void screen_draw_hline(screen_t * scr, int x, int y, unsigned int len);

// Draw a vertical line
void screen_draw_vline(screen_t * scre, int x, int y, unsigned int len);

/// This function needs to be called after changing the screen
/// (adding characters, clearing the screen, ...)
///
/// (This is an optimization to allow making all changes to the screen in
///  one operation)
void screen_refresh (screen_t * scr);



/// In case you want to go beyond the simple functions provided above,
/// use this function to get direct access to the underlying (*in this
/// implementation* ncurses object)
WINDOW * screen_get_mainwin (screen_t * scr);

