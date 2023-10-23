
#include <stdio.h>
#include <stdlib.h>

#include "term_manip.c"

#define DUNGEON_SIZE 80
#define TREE_DEPTH 5

#define MAP_CHAR '.'
#define ROOM_CHAR '.'

char dungeon[DUNGEON_SIZE][DUNGEON_SIZE];


typedef struct char2
{
    unsigned char x, y;
} char2;

typedef struct leaf
{
    unsigned char width, height;
    char2 key;
    struct leaf *child_1, *child_2;
    // maybe just save the arr index instead? since you save it on the stack.
} leaf;

int p0w(int base, int exponent)
{
    int result = 1;
    for (int i = 0; i < exponent; i++)
    {
        result *= base;
    }
    return result;
}

int r0nd(int min, int max)
{
    int rand_product = min;

    rand_product += arc4random_uniform(max - min);

    return rand_product;
}


#include "generation.c"

int main()
{
    arc4random_uniform(8);
    for (int i = 0; i < DUNGEON_SIZE; i++)
    {
        for (int ii = 0; ii < DUNGEON_SIZE; ii++)
        {
            dungeon[i][ii] = ' ';
        }
    }

    char dungeon_print[DUNGEON_SIZE * 2 + 1];
    dungeon_gen();
    set_raw_term();

    for (int i = 0; i < DUNGEON_SIZE; i++)
    {
        for (int ii = 0; ii < DUNGEON_SIZE * 2; ii += 2)
        {
            // printf("%c%c", dungeon[i][ii/2], ' ');
            switch (dungeon[i][ii / 2])
            {
            case ('-'):
                dungeon_print[ii] = dungeon[i][ii / 2];
                dungeon_print[ii + 1] = '-';
                dungeon_print[ii - 1] = '-';
                break;
            default:
                dungeon_print[ii] = dungeon[i][ii / 2];
                dungeon_print[ii + 1] = ' ';
            }
        }

        printf("%s%c", dungeon_print, '\n');
    }

    return 0;
    char c;

    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q')
        ;
    return 0;
}