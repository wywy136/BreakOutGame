#include "spatial_index.h"
#include "util.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>


typedef struct
{
    point_t position;
    char * name;
} my_element_t;

static void bbox(const void * element, rectangle_t * rect, void * userdata)
{
    my_element_t * e = (my_element_t *) element;
    *rect = rectangle_create(e->position, e->position);
}

static bool contains(const rectangle_t * r, const void * element, void * userdata)
{
    const my_element_t * e = (const my_element_t *) element;
    return rectangle_contains(r, e->position);
}

static bool visit_print(void * element, void * userdata)
{
    const char * s = (const char *) userdata;
    const my_element_t * e = (const my_element_t *) element;
    printf("%s: %s at ", s, e->name);
    point_print(e->position, stdout);
    puts("");
    return true;
}

static bool visit_free(void * element, void * userdata)
{
    my_element_t * e = (my_element_t *) element;
    free(e->name);
    free(e);
    return true;
}

static char * my_strdup(const char * s)
{
    unsigned int len = strlen(s) + 1;
    char * ret = malloc(len);
    strcpy(ret, s);
    ret[len-1] = 0;
    return ret;
}

int main(int argc, char ** args)
{
    // not secure (predictable value), but good enough for us
    srand(time(NULL));

    si_handle h = si_create(&contains, &bbox, NULL);

    for (unsigned int i=0; i<10; ++i)
    {
        my_element_t * n = malloc(sizeof(*n));
        char buf[32];
        snprintf(buf, sizeof(buf), "%i", i);
        n->name = my_strdup(buf);
        n->position = point_create(rand() % 100, rand() % 100);
        si_insert(h, n);
    }

    // Number of points
    printf("Number of points in index: %u\n", si_size(h));

    rectangle_t box;
    si_bbox(h, &box);
    printf("Bounding box containing all points: ");
    rectangle_print(box, stdout);
    putchar('\n');

    // Print all the elements
    // note the usage of userdata to pass extra information to the
    // visit_print function.
    si_walk(h, visit_print, "Element: ");

    // Free user memory associated with the elements
    si_walk(h, visit_free, NULL);

    si_destroy(h);

    return EXIT_SUCCESS;
}
