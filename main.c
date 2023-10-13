
#include <stdio.h>
#include <stdlib.h>

#include "term_manip.c"

#define DUNGEON_SIZE 100
#define TREE_DEPTH 4

typedef struct char2
{
    unsigned char x, y;
} char2;

typedef struct leaf
{
    unsigned char x, y, width, height;
    char2 key;
    struct leaf *child_1, *child_2;
} leaf;

int p0w(int base, int exponent)
{
    if (exponent == 0)
        return 1;
    int result = base;
    for (int i = 0; i < exponent; i++)
    {
        result *= base;
    }
    return result;
}

void dungeon_gen()
{
    leaf tree[p0w(2, TREE_DEPTH)];
    char leaf_pointer = 0;
    char depth_offset = 1;
    for (char l = 0; l < TREE_DEPTH; l++)
    {
        for (char i = 0; i < depth_offset; i++)
        {
            depth_offset--;
            depth_offset += i;
            tree[depth_offset].child_1 = &tree[depth_offset*2 + 1];
            tree[depth_offset].child_2 = &tree[depth_offset*2 + 2];
            printf("depth: %d, leaf: %d, child_1: %d, child_2: %d\n", l, depth_offset, depth_offset*2+1, depth_offset*2+2);
            depth_offset -= i;
            depth_offset++;
        }
        depth_offset *= 2;
    }

//     unsigned char decider = arc4random_uniform(DUNGEON_SIZE / 5) + (DUNGEON_SIZE / 5 * 2);
//     if (decider % 2)
//     {
//         tree[0].key.x = decider;
//         tree[0].key.y = 0;
//     }
//     else
//     {
//         tree[0].key.x = 0;
//         tree[0].key.y = decider;
//     }
}

int main()
{
    arc4random_uniform(8);
    unsigned char dungeon[DUNGEON_SIZE][DUNGEON_SIZE];
    for (int i = 0; i < DUNGEON_SIZE; i++)
    {
        for (int ii = 0; ii < DUNGEON_SIZE; ii++)
        {
            dungeon[i][ii] = 1;
        }
    }
    dungeon_gen();
    set_raw_term();
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q')
        ;
    return 0;
}