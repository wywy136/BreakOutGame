default:test

test:util_test.c util.h util.c spatial_index_test.c spatial_index.h spatial_index.c
	gcc -std=c11 -Wall -Werror -pedantic util_test.c util.c -o util_test -lcunit
	valgrind --show-reachable=yes --leak-check=full ./util_test
	gcc -std=c11 -Wall -Werror -pedantic spatial_index_test.c spatial_index.c util.c -o spatial_index_test -lcunit
	valgrind --show-reachable=yes --leak-check=full ./spatial_index_test

util_test:util_test.c util.h util.c
	gcc -std=c11 -Wall -Werror -pedantic util_test.c util.c -o util_test -lcunit
	gcc -std=c11 -Wall -Werror -pedantic spatial_index_test.c spatial_index.c util.c -o spatial_index_test -lcunit

clean:
	rm -rf util_test
	rm -rf spatial_index_test
	rm -rf game

vararray:vararray_test.c vararray.h vararray_test.c util.c spatial_index.c
	gcc -std=c11 -Wall -Werror -pedantic vararray_test.c vararray.c util.c spatial_index.c -o vararray_test -lcunit
	valgrind --show-reachable=yes --leak-check=full ./vararray_test

screen_demo:screen_demo.c
	gcc screen_demo.c ../screen.c -o screen_demo -lncurses -lm

game:game.h game.c screen.c spatial_index.c util.c ball_object.c paddle_object.c block_object.c wall_object.c
	gcc -std=c11 -Wall -Werror -pedantic game.c screen.c spatial_index.c util.c ball_object.c paddle_object.c block_object.c wall_object.c -o game -lncurses -lm
	valgrind --show-reachable=yes --leak-check=full ./game