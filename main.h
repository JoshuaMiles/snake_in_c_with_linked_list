//
// Created by Joshua on 21/09/2016.
//

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <time.h>

#include "naughty_globals.h"

#include "./cab202_teensy/graphics.h"

#include "./cab202_teensy/cpu_speed.h"

#include "./cab202_teensy/sprite.h"

#include "./cab202_teensy/ascii_font.h"

#include "./cab202_teensy/macros.h"


#include <util/delay.h>


void debounce();
void setup_inputs_and_outputs();
void build_hud();
void initialise_sprites();
void handle_game();
void name_and_student_number();
void handle_wrap();
int sprite_collision(Sprite *sprite);
//int snake_collision();
//void reset_snake(*snake);
//void snake_direction();
//void snake_speed();
//void wrap_snake();
void snake_controls();
void ouroboros();
int wall_collision(int x1, int y1, int x2, int y2);

//void eat_something(Sprite * snake_sprite);
//void draw_snake_node(unsigned char top_left_x, unsigned char top_left_y);
int self_collision();
int single_body_collision(struct Snake_node body);
void snake_loses_life();
void draw_snek();
void draw_snake_node(unsigned char top_left_x, unsigned char top_left_y);
//Sprite feed_snake(Sprite *snake);
void build_snake(Sprite *snake);
void move_snake();
void snake_maker();
void food_shifter(Sprite * food);
void make_food();
void handle_game();
void build_a_wall();