# Final Project
*ywang27@uchicago.edu*

This file briefly describe my final project for this course, including the modifications to the provided files and the optional features.

## Code Modifications
- `game.h` - add some fields to `game_data_t`, with their meanings in comment.
- `game_object.h` - there are in total 9 functions pointers in `game_object_vtable_t`, with their meanings as their names.
- `ball_object.h` - the declarations of `ball_object_t`, `create_ball()` and all other functions specific to this class, same for
	- `paddle_object.h`
	- `block_object.h`
	- `wall_object.h`
- `ball_object.c` - implementation details of functions in `ball_object.h`.
	- See comments for fields in `ball_object_t`
	- With other functions pretty forward, the most intricate function here is `ball_object_collision()`
		- This function detects potential collision with paddle, blocks and walls and determine the next position of ball
		- See comments for `si_visit_func_1` for a detailed documentation of determining the next position of ball
		- Also create a struct `si_visit_collection` for carrying a couple of fields into `si_walk()`
	- `ball_get_next_dxdy()` set the dx,dy accordingly
- `paddle_object.c` - comments for fields in `paddle_object_t` and `paddle_set_position()`.
- `block_object.c` and `wall_object.c` - quite straightforward to understand, without functions specific to these two classes.
- `game.c` - see comments for functions in this file, mainly `si_visit_func_2()`, `si_visit_destroy()`, `game_initialize()` and `game_destroy()`.

## Optional Features
I have added two optional features for this game.
- A vertically controllable paddle, which could be controlled to move in vertical direction. This brings more flexibility to this game since the player can select either to keep their paddle far away from the blocks or closer to blocks and change the position anytime.
- Distinguishable blocks, which assigns higher scores to upper blocks and lower scores to lower blocks.

## Running
Running `make game` will build the executable under strict C11 complier flages and run it under valgrind. 
```
LEAK SUMMARY:
==1888146==    definitely lost: 0 bytes in 0 blocks
==1888146==    indirectly lost: 0 bytes in 0 blocks
==1888146==      possibly lost: 0 bytes in 0 blocks
==1888146==    still reachable: 52,139 bytes in 300 blocks
==1888146==         suppressed: 0 bytes in 0 blocks
==1888146== 
==1888146== For lists of detected and suppressed errors, rerun with: -s
==1888146== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
Besides the "still reachable" leaks lead by `screen_XXX`, the program is able to run with no warnings or errors.

Run `make clean` to delete the executable.
