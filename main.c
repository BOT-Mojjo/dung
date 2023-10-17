
#include <stdio.h>
#include <stdlib.h>

#include "term_manip.c"

#define DUNGEON_SIZE 100
#define TREE_DEPTH 5

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
    if (exponent == 0)
        return 1;
    int result = base;
    for (int i = 0; i < exponent; i++)
    {
        result *= base;
    }
    return result;
}

void split_room(leaf *leaf, char depth)
{
    printf("depth %d\n", depth);
    if (depth >= TREE_DEPTH)
    {
        leaf->child_1 = NULL;
        leaf->child_2 = NULL;
        return;
    }
    float split_line = arc4random_uniform(300); // 1/5 of 1000
    split_line += 350;
    split_line /= 1000;
    unsigned char split_point = leaf->width * split_line;
    if (leaf->width > leaf->height)
    { // split the x-axis
        leaf->child_2->key.x = leaf->key.x + split_point;
        leaf->child_2->key.y = leaf->key.y;
        leaf->child_2->width = leaf->width - split_point;
        leaf->child_2->height = leaf->height;

        leaf->child_1->key.x = leaf->key.x;
        leaf->child_1->key.y = leaf->key.y;
        
        leaf->child_1->width = split_point;
        leaf->child_1->height = leaf->height;
        for (int i = 0; i < leaf->height; i++)
        {
            dungeon[leaf->key.x + split_point][leaf->key.y + i] = '#' + depth;
        }
    }
    else
    { // split the y-axis
        leaf->child_2->key.x = leaf->key.x;
        leaf->child_2->key.y = leaf->key.y + split_point;
        leaf->child_2->width = leaf->width;
        leaf->child_2->height = leaf->height - split_point;

        leaf->child_1->key.x = leaf->key.x;
        leaf->child_1->key.y = leaf->key.y;

        leaf->child_1->width = leaf->width;
        leaf->child_1->height = split_point;
        for (int i = 0; i < leaf->width; i++)
        {
            dungeon[leaf->key.x + i][leaf->key.y + split_point] = '#' + depth;
        }
    }
    split_room(leaf->child_1, depth + 1);
    split_room(leaf->child_2, depth + 1);
}

void dungeon_gen()
{
    leaf tree[p0w(2, TREE_DEPTH)];
    char leaf_pointer = 0;
    char depth_offset = 1;
    // root preperations, remember to initialize your values kids!
    tree[0].width = DUNGEON_SIZE;
    tree[0].height = DUNGEON_SIZE;
    tree[0].key.x = 0;
    tree[0].key.y = 0;

    for (char l = 0; l < TREE_DEPTH; l++)
    {
        for (char i = 0; i < depth_offset; i++)
        {
            depth_offset--;
            depth_offset += i;
            tree[depth_offset].child_1 = &tree[depth_offset * 2 + 1];
            tree[depth_offset].child_2 = &tree[depth_offset * 2 + 2];
            // printf("depth: %d, leaf: %d, child_1: %d, child_2: %d\n", l, depth_offset, depth_offset * 2 + 1, depth_offset * 2 + 2);
            depth_offset -= i;
            depth_offset++;
        }
        depth_offset *= 2;
    }
    // for (int i = 0; i < 31; i++)
    // {
    //     printf("%p\n", &tree[i]);
    // }
    depth_offset = 4;
    char reminder = 0b0000;
    split_room(&tree[0], 0);
}

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
            dungeon_print[ii] = dungeon[i][ii / 2];
            dungeon_print[ii + 1] = ' ';
        }

        printf("%s%c", dungeon_print, '\n');
    }
    return 0;
    char c;

    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q')
        ;
    return 0;
}