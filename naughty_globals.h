#include "byte.h"

static byte food [] = {
        BYTE( 11100000 ),
        BYTE( 10100000 ),
        BYTE( 11100000 )
};
byte body[100];
byte length = 10;

int seed = 1;
int score = 0;
int lives = 5;

struct Snake_node *head;

struct Snake_node *tail;

struct Snake_node {
    int x, y, dy, dx;
    struct Snake_node *previous;
    struct Snake_node *next;
} Snake_node;

static byte snake_byte [] = {
        BYTE( 11100000 ),
        BYTE( 11100000 ),
        BYTE( 11100000 )
};


//byte *bitmaps[3] = { food  };
