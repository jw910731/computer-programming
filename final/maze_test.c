#include <stdint.h>
#include "maze.h"
/*
+---+---+---+
| S |   | D |
+---+---+---+

Of course, TA will not give you such a simple maze.
TA will use a two-dimensional array and some blocks of this array is not walkable.
*/

static int32_t row = 1;
static int32_t column = 3;

static int32_t cur_row = 0;
static int32_t cur_column = 0;

void setup ()
{
    cur_row = 0;
    cur_column = 0;
    return;
}

int32_t move_up ()
{
    return 0;
}

int32_t move_down ()
{
    return 0;
}

int32_t move_left ()
{
    if( cur_column == 0 )
        return 0;
    else
    {
        cur_column -= 1;
        return 1;
    }
}

int32_t move_right ()
{
    if( cur_column == column - 1 )
        return 0;
    else
    {
        cur_column += 1;
        return 1;
    }
}

int32_t is_palace ()
{
    if( cur_row == 0 && cur_column == 2 )
        return 1;
    else
        return 0;
}

