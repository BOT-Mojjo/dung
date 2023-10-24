

void generate_room(leaf *leaf)
{
    unsigned char x_offset, y_offset, r_width, r_height;
    float room_width = leaf->width / 5;
    float room_height = leaf->height / 5;
    // offset of the room from leaf boudry
    x_offset = r0nd(room_width, room_width * 1.5);
    r_width = r0nd(room_width * 3, leaf->width - x_offset);
    if (r_width <= 3)
        r_width = 4;
    y_offset = r0nd(room_height, room_height * 1.5);
    r_height = r0nd(room_height * 3, leaf->height - y_offset);
    if (r_height <= 3)
        r_height = 4;
    x_offset += leaf->key.x;
    y_offset += leaf->key.y;
    // printf("%d, %d, %d, %d\n", x_offset, y_offset, r_width, r_height);
    // fyller rummet med ROOM_CHAR
    leaf->room_size = r_width * r_height;
    for (char i = 0; i < r_width; i++)
    {
        for (char ii = 0; ii < r_height; ii++)
        {
            dungeon[i + x_offset][ii + y_offset] = ROOM_CHAR;
        }
    }
    // gör väggar runt rummet
    for (char x = 0; x < r_width; x++)
    {
        for (char y = 0; y < r_height; y++)
        {
            if (x == 0 || x == r_width - 1)
                dungeon[x + x_offset][y + y_offset] = '-';
            if (y == 0 || y == r_height - 1)
                dungeon[x + x_offset][y + y_offset] = '|';
        }
    }
}

void generate_corridor(leaf *leaf, char split_point)
{
    // dont change order of chars, doing pointer manipulation
    // to get the right variables in corridor_step()
    char offset = 1, negative = 0, positive = 0;
    if (leaf->width > leaf->height)
    {
        char height_offset = (leaf->height) / 2;
        height_offset += leaf->key.y;

        // the corridor room can spawn on a wall
        // and takes a step into the room before stopping.
        // check to stop that
        if (dungeon[leaf->key.x + split_point][height_offset] == '-')
        {
            dungeon[leaf->key.x + split_point][height_offset] = '+';
            if (dungeon[leaf->key.x + split_point + 1][height_offset] == ROOM_CHAR)
                negative = 1;
            else
                positive = 1;
        }
        else
            dungeon[leaf->key.x + split_point][height_offset] = '#';
        while (1)
        {
            if (!positive)
            {
                char split_offset = leaf->key.x + split_point + offset;
                if (dungeon[split_offset][height_offset] == ' ')
                    dungeon[split_offset][height_offset] = MAP_CHAR;
                else
                { // Corner checks :)), god i wish i knew how to make this into a generic funktion
                    if (dungeon[split_offset][height_offset + 1] == ' ')
                    {
                        dungeon[split_offset][height_offset] = '|';
                        dungeon[split_offset - 1][height_offset] = ' ';
                        dungeon[split_offset - 2][height_offset - 1] = MAP_CHAR;
                        dungeon[split_offset - 1][height_offset - 1] = MAP_CHAR;
                        dungeon[split_offset][height_offset - 1] = '+';
                    }
                    else if (dungeon[split_offset][height_offset - 1] == ' ')
                    {
                        dungeon[split_offset][height_offset] = '|';
                        dungeon[split_offset - 1][height_offset] = ' ';
                        dungeon[split_offset - 2][height_offset + 1] = MAP_CHAR;
                        dungeon[split_offset - 1][height_offset + 1] = MAP_CHAR;
                        dungeon[split_offset][height_offset + 1] = '+';
                    }
                    else
                        dungeon[split_offset][height_offset] = '+';
                    positive = 1;
                }
            }

            if (!negative)
            {
                char split_offset = leaf->key.x + split_point - offset;
                if (dungeon[split_offset][height_offset] == ' ')
                    dungeon[split_offset][height_offset] = MAP_CHAR;
                else
                {
                    if (dungeon[split_offset][height_offset + 1] == ' ')
                    {
                        dungeon[split_offset][height_offset] = '|';
                        dungeon[split_offset + 1][height_offset] = ' ';
                        dungeon[split_offset + 2][height_offset - 1] = MAP_CHAR;
                        dungeon[split_offset + 1][height_offset - 1] = MAP_CHAR;
                        dungeon[split_offset][height_offset - 1] = '+';
                    }
                    else if (dungeon[split_offset][height_offset - 1] == ' ')
                    {
                        dungeon[split_offset][height_offset] = '|';
                        dungeon[split_offset + 1][height_offset] = ' ';
                        dungeon[split_offset + 2][height_offset + 1] = MAP_CHAR;
                        dungeon[split_offset + 1][height_offset + 1] = MAP_CHAR;
                        dungeon[split_offset][height_offset + 1] = '+';
                    }
                    else
                        dungeon[split_offset][height_offset] = '+';
                    negative = 1;
                }
            }
            if (positive && negative)
                break;
            offset++;
        }
    }
    else
    {
        // identical to code above but flipped for the horizontal axis
        char width_offset = (leaf->width) / 2;
        width_offset += leaf->key.x;
        if (dungeon[width_offset][leaf->key.y + split_point] == '|')
        {
            dungeon[width_offset][leaf->key.y + split_point] = '+';
            if (dungeon[width_offset][leaf->key.y + split_point + 1] == ROOM_CHAR)
                positive = 1;
            else
                negative = 1;
        }
        else
            dungeon[width_offset][leaf->key.y + split_point] = '#';
        while (1)
        {
            if (!positive)
            {
                char split_offset = leaf->key.y + split_point + offset;
                if (dungeon[width_offset][split_offset] == ' ')
                    dungeon[width_offset][split_offset] = MAP_CHAR;
                else
                {
                    if (dungeon[width_offset + 1][split_offset] == ' ')
                    {
                        dungeon[width_offset][split_offset] = '|';
                        dungeon[width_offset][split_offset - 1] = ' ';
                        dungeon[width_offset - 1][split_offset - 2] = MAP_CHAR;
                        dungeon[width_offset - 1][split_offset - 1] = MAP_CHAR;
                        dungeon[width_offset - 1][split_offset] = '+';
                    }
                    else if (dungeon[width_offset - 1][split_offset] == ' ')
                    {
                        dungeon[width_offset][split_offset] = '|';
                        dungeon[width_offset][split_offset - 1] = ' ';
                        dungeon[width_offset + 1][split_offset - 2] = MAP_CHAR;
                        dungeon[width_offset + 1][split_offset - 1] = MAP_CHAR;
                        dungeon[width_offset + 1][split_offset] = '+';
                    }
                    else
                        dungeon[width_offset][split_offset] = '+';
                    positive = 1;
                }
            }

            if (!negative)
            {
                char split_offset = leaf->key.y + split_point - offset;
                if (dungeon[width_offset][split_offset] == ' ')
                    dungeon[width_offset][split_offset] = MAP_CHAR;
                else
                {
                    if (dungeon[width_offset + 1][split_offset] == ' ')
                    {
                        dungeon[width_offset][split_offset] = '|';
                        dungeon[width_offset][split_offset + 1] = ' ';
                        dungeon[width_offset - 1][split_offset + 2] = MAP_CHAR;
                        dungeon[width_offset - 1][split_offset + 1] = MAP_CHAR;
                        dungeon[width_offset - 1][split_offset] = '+';
                    }
                    else if (dungeon[width_offset - 1][split_offset] == ' ')
                    {
                        dungeon[width_offset][split_offset] = '|';
                        dungeon[width_offset][split_offset + 1] = ' ';
                        dungeon[width_offset + 1][split_offset + 2] = MAP_CHAR;
                        dungeon[width_offset + 1][split_offset + 1] = MAP_CHAR;
                        dungeon[width_offset + 1][split_offset] = '+';
                    }
                    dungeon[width_offset][split_offset] = '+';
                    negative = 1;
                }
            }
            if (positive && negative)
                break;
            offset++;
        }
    }
}

void split_room(leaf *leaf, char depth)
{
    if (depth >= TREE_DEPTH || arc4random_uniform(20) + 1 < depth)
    {
        generate_room(leaf);
        leaf->child_1 = NULL;
        leaf->child_2 = NULL;
        return;
    }
    float split_line = arc4random_uniform(300); // 1/5 of 1000
    split_line += 350;
    split_line /= 1000;
    unsigned char split_point;
    if (leaf->width > leaf->height)
    { // split the x-axis
        split_point = leaf->width * split_line;
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
            // dungeon[leaf->key.x + split_point][leaf->key.y + i] = '#' + depth;
        }
    }
    else
    { // split the y-axis
        split_point = leaf->height * split_line;
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
            // dungeon[leaf->key.x + i][leaf->key.y + split_point] = '#' + depth;
        }
    }
    split_room(leaf->child_1, depth + 1);
    split_room(leaf->child_2, depth + 1);

    generate_corridor(leaf, split_point);
}

void dungeon_gen()
{
    leaf tree[p0w(2, TREE_DEPTH + 1)];
    char leaf_pointer = 0;
    char depth_offset = 1;
    // root preperations, remember to initialize your values, kids!
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
    split_room(&tree[0], 0);
}