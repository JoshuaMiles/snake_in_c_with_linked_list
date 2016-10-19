//
// Created by Joshua on 21/09/2016.
//

#include "./a2_n7176244.h"

#define LTHRES 500
#define RTHRES 500


/**
   _________   ______________    ___________________________________ _____________
  /  _  \   \ /   /\______   \  /   _____/\_   _____/\__    ___/    |   \______   \
 /  /_\  \   Y   /  |       _/  \_____  \  |    __)_   |    |  |    |   /|     ___/
/    |    \     /   |    |   \  /        \ |        \  |    |  |    |  / |    |
\____|__  /\___/    |____|_  / /_______  //_______  /  |____|  |______/  |____|
        \/                 \/          \/         \/
 */


void adc_init() {
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}


uint16_t adc_read(uint8_t ch) {

    ch &= 0b00000111;  // AND operation with 7
    ADMUX = (ADMUX & 0xF8) | ch;     // clears the bottom 3 bits before ORing

    ADCSRA |= (1 << ADSC);

    while (ADCSRA & (1 << ADSC));

    return (ADC);
}


void setup_inputs_and_outputs() {

    DDRD |= (1 << PIND6);//|(1<<PINB3);	//Teensy LEDLED0 and LED1 as outputs
    PORTD = 0x00;    // Turn everything off to start with


    SET_INPUT(DDRD, 1);
    SET_INPUT(DDRB, 1);
    SET_INPUT(DDRB, 7);
    SET_INPUT(DDRD, 0);
    SET_INPUT(DDRB, 0);
    // Right switch
    SET_INPUT(DDRF, 5);
    // Left Switch
    SET_INPUT(DDRF, 6);
    // ADC
//    SET_INPUT(ADCSRA, 6);

    SET_OUTPUT(DDRB, 2);
    SET_OUTPUT(DDRB, 3);

    //Back light
    SET_OUTPUT(DDRC, 7);

};


/*
   ___ ___  ____ ___________      ___ ___                    .___.__
 /   |   \|    |   \______ \    /   |   \_____    ____    __| _/|  |   ___________  ______
/    ~    \    |   /|    |  \  /    ~    \__  \  /    \  / __ | |  | _/ __ \_  __ \/  ___/
\    Y    /    |  / |    `   \ \    Y    // __ \|   |  \/ /_/ | |  |_\  ___/|  | \/\___ \
 \___|_  /|______/ /_______  /  \___|_  /(____  /___|  /\____ | |____/\___  >__|  /____  >
       \/                  \/         \/      \/     \/      \/           \/           \/
 */

void build_hud() {
    if (score > 9) {
        draw_char(0, 0, score / 10 + '0');
        draw_char(6, 0, score % 10 + '0');
    } else {
        draw_char(0, 0, score + '0');
//        sprintf(d(0, 0, head->y + '0');
    }
    draw_char(15, 0, '(');
    draw_char(20, 0, lives + '0');
    draw_char(26, 0, ')');
};


/*
   ___________________ _______  _____________________    _____  .____        ___ ______________.____   _______________________________  _________
  /  _____/\_   _____/ \      \ \_   _____/\______   \  /  _  \ |    |      /   |   \_   _____/|    |  \______   \_   _____/\______   \/   _____/
 /   \  ___ |    __)_  /   |   \ |    __)_  |       _/ /  /_\  \|    |     /    ~    \    __)_ |    |   |     ___/|    __)_  |       _/\_____  \
 \    \_\  \|        \/    |    \|        \ |    |   \/    |    \    |___  \    Y    /        \|    |___|    |    |        \ |    |   \/        \
  \______  /_______  /\____|__  /_______  / |____|_  /\____|__  /_______ \  \___|_  /_______  /|_______ \____|   /_______  / |____|_  /_______  /
 */

void name_and_student_number() {
    clear_screen();
    draw_string(LCD_X / 2 - 29, LCD_Y / 2 - 8, "Joshua Miles");
    draw_string(LCD_X / 2 - 20, LCD_Y / 2 + 9, "n7176244");
    show_screen();

    _delay_ms(2000);
    clear_screen();
    lcd_clear();
}

void update_random_seed() {
    seed++;
    if (seed > 10000) {
        // In case the seed gets too big
        seed = 0;
    }
    srand(seed);
}

int sprites_collided(Sprite *p, Sprite *q) {
    int p_top = p->y,
            p_bottom = p_top + p->height - 1,
            p_left = p->x,
            p_right = p_left + p->width - 1;

    int q_top = q->y,
            q_bottom = q_top + q->height - 1,
            q_left = q->x,
            q_right = q_left + q->width - 1;

    return !(
            p_bottom < q_top
            || p_top > q_bottom
            || p_right < q_left
            || p_left > q_right
    );
}


/**
  _________              __              ___ ___                    .___.__
 /   _____/ ____ _____  |  | __ ____    /   |   \_____    ____    __| _/|  |   ___________
 \_____  \ /    \\__  \ |  |/ // __ \  /    ~    \__  \  /    \  / __ | |  | _/ __ \_  __ \
 /        \   |  \/ __ \|    <\  ___/  \    Y    // __ \|   |  \/ /_/ | |  |_\  ___/|  | \/
/_______  /___|  (____  /__|_ \\___  >  \___|_  /(____  /___|  /\____ | |____/\___  >__|
        \/     \/     \/     \/    \/         \/      \/     \/      \/           \/
 */

void assign_data() {
    head->x = LCD_X / 2;
    head->y = LCD_Y / 2;
    head->dy = 0;
    head->dx = 3;
    head->next = tail;
    head->previous = NULL;

    tail->x = LCD_X / 2 - 3;
    tail->y = LCD_Y / 2;
    tail->previous = head;
    tail->next = NULL;

}

void ouroboros() {
    struct Snake_node *new_tail = tail->previous;
    new_tail->next = NULL;
    tail->previous = NULL;

    tail->x = head->x + head->dx;
    tail->y = head->y + head->dy;

    tail->dx = head->dx;
    tail->dy = head->dy;

    tail->next = head;
    head->previous = tail;

    head = tail;
    tail = new_tail;

}

void head_direction() {
    if (INPUT_READ(PIND, 1)) {
        if (head->dy == 3) {
            snake_loses_life();
        } else {
            SNAKE_WAIT = 0;
            head->dx = 0;
            head->dy = -3;
            OUTPUT_HIGH(PORTB, 2); // LED0 ON
        }
    }
        // Check if Left is Pressed
    else if (INPUT_READ(PINB, 1)) {
        if (head->dx == 3) {
            snake_loses_life();
        } else {

            SNAKE_WAIT = 0;
            head->dy = 0;
            head->dx = -3;
            OUTPUT_HIGH(PORTB, 2); // LED0 ON
        }
    }
        // Check if Down is Pressed
    else if (INPUT_READ(PINB, 7)) {
        if (head->dy == -3) {
            snake_loses_life();
        } else {
            SNAKE_WAIT = 0;
            head->dx = 0;
            head->dy = 3;

            OUTPUT_HIGH(PORTB, 3);  // LED1 ON
        }
    }
        // Check if Right is Pressed
    else if (INPUT_READ(PIND, 0)) {
        if (head->dx == -3) {
            snake_loses_life();
        } else {

            SNAKE_WAIT = 0;
//            _delay_ms(100);
            head->dy = 0;
            head->dx = 3;
        }

        OUTPUT_HIGH(PORTB, 3);  // LED1 ON
    }
// Check if Center is Pressed
    else if (INPUT_READ(PINB, 0)) {
// Toggle using XOR
        PORTB ^= (1 << 3);
        PORTB ^= (1 << 2);
    } else {
        OUTPUT_LOW(PORTB, 3);
        OUTPUT_LOW(PORTB, 2);
    }
}

void handle_wrap() {
    if (head->y < 0 || head->y > LCD_Y + 3) {
        head->y = LCD_Y - 3;
    } else if (head->y > LCD_Y) {
        head->y = 3;
    }

    if (head->x < 0 || head->x > LCD_X + 3) {
        head->x = LCD_X - 3;
    } else if (head->x < -3 || head->x > LCD_X) {
        head->x = 3;
    }
}

// Collisions

void handle_collisions(Sprite *food, Sprite *wall1, Sprite *wall2, Sprite *wall3) {

    if (self_collision()) {
//        snake_loses_life();
    }

    if (sprite_collision(food)) {
        eat_food(food);
        food_shifter(food, wall1, wall2, wall3);
        if (show_walls) {
            score += 2;
        }
        score++;
        PORTB ^= (1 << 3);
        PORTB ^= (1 << 2);
        OUTPUT_HIGH(PORTC, 7);
    }
}

int self_collision() {

    struct Snake_node *current_node = head->next;

    while (1) {
        if (single_body_collision(*current_node)) {
            return 1;
        }
        if (current_node->next != NULL) {
            current_node = current_node->next;
        } else {
            break;
        }
    }
    return 0;
}

int single_body_collision(struct Snake_node body) {
    int body_top = body.y,
            body_bottom = body_top + 2,
            body_left = body.x,
            body_right = body_left + 2;

    int head_top = head->y,
            head_bottom = head_top + 2,
            head_left = head->x,
            head_right = head_left + 2;

    return !(
            body_bottom < head_top
            || body_top > head_bottom
            || body_right < head_left
            || body_left > head_right
    );
}


void snake_maker() {
    head = (struct Snake_node *) malloc(sizeof(struct Snake_node));
    tail = (struct Snake_node *) malloc(sizeof(struct Snake_node));
}

void reset_snake() {
    struct Snake_node *current_node = head->next;
    free(head);
    while (1) {
        if (current_node->next != NULL) {
            current_node = current_node->next;
            free(current_node);
        } else {
            break;
        }
    }
}

void snake_speed() {
    max_speed = (adc_result0 * (100)) / 1023.0;
//    itoa(max_speed, int_buffer, 10);
//    sprintf(disp_buffer, "%s", int_buffer);
//    draw_string(0, 35, disp_buffer);

    while (--max_speed > 0) {
        _delay_ms(1);
    }
}

void eat_food(Sprite *food) {
    struct Snake_node *old_head = head;
    head = (struct Snake_node *) malloc(sizeof(struct Snake_node));
    old_head->previous = head;
    head->x = food->x;
    head->y = food->y;
    head->dx = old_head->dx;
    head->dy = old_head->dy;
    head->next = old_head;
}

void draw_snake_node(unsigned char top_left_x, unsigned char top_left_y) {
    unsigned char byte_width = ceil(3 / 8.0f);
    for (unsigned char i = 0; i < 3; i++) {
        for (unsigned char j = 0; j < 3; j++) {
            set_pixel(top_left_x + i, top_left_y + j,
                      (snake_byte[(int) (i * byte_width + floor(j / 8.0f))] >> (7 - j % 8)) & 1);
        }
    }
}

void snake_loses_life() {
    lives--;
    reset_snake();
    snake_maker();
    assign_data();
    SNAKE_WAIT = 1;
}

void draw_snek() {
    struct Snake_node *current_node = head;

    while (1) {
        draw_snake_node(current_node->x, current_node->y);
        if (current_node->next != NULL) {
            current_node = current_node->next;
        } else {
            break;
        }
    }
}

/*
___________               .___   ___ ___                    .___.__
\_   _____/___   ____   __| _/  /   |   \_____    ____    __| _/|  |   ___________
 |    __)/  _ \ /  _ \ / __ |  /    ~    \__  \  /    \  / __ | |  | _/ __ \_  __ \
 |     \(  <_> |  <_> ) /_/ |  \    Y    // __ \|   |  \/ /_/ | |  |_\  ___/|  | \/
 \___  / \____/ \____/\____ |   \___|_  /(____  /___|  /\____ | |____/\___  >__|
     \/                    \/         \/      \/     \/      \/           \/

 */


void food_shifter(Sprite *food_sprite, Sprite *wall1, Sprite *wall2, Sprite *wall3) {
//    int new_food_x = rand() % LCD_X - 3;
//    int new_food_y = rand() % LCD_Y - 3;

//    int new_food_x = LCD_X / 2;
//    int new_food_y = LCD_Y / 2 + 1;

//    if (new_food_y > 4 && new_food_x > 30) {
    food_sprite->x = rand() % LCD_X - 3;
    food_sprite->y = rand() % LCD_Y - 3;
    if (food_sprite->y <= 4 && food_sprite->x <= 30) {
        food_shifter(food_sprite, wall1, wall2, wall3);
    } else if (show_walls &&
               (sprites_collided(food_sprite, wall1) ||
                sprites_collided(food_sprite, wall2) ||
                sprites_collided(food_sprite, wall3))) {
        food_shifter(food_sprite, wall1, wall2, wall3);
    }
}


int sprite_collision(Sprite *sprite) {
    int sprite_top = sprite->y,
            sprite_bottom = sprite_top + sprite->height - 1,
            sprite_left = sprite->x,
            sprite_right = sprite_left + sprite->width - 1;

    int head_top = head->y,
            head_bottom = head_top + 3,
            head_left = head->x,
            head_right = head_left + 2;

    return !(
            sprite_bottom < head_top
            || sprite_top > head_bottom
            || sprite_right < head_left
            || sprite_left > head_right
    );
}

/**
 __      __        .__  .__      ___ ___                    .___.__
/  \    /  \_____  |  | |  |    /   |   \_____    ____    __| _/|  |   ___________  ______
\   \/\/   /\__  \ |  | |  |   /    ~    \__  \  /    \  / __ | |  | _/ __ \_  __ \/  ___/
 \        /  / __ \|  |_|  |__ \    Y    // __ \|   |  \/ /_/ | |  |_\  ___/|  | \/\___ \
  \__/\  /  (____  /____/____/  \___|_  /(____  /___|  /\____ | |____/\___  >__|  /____  >
       \/        \/                   \/      \/     \/      \/           \/           \/
 */

int wall_collision(int x1, int y1, int x2, int y2) {
    int line_top = y1,
            line_bottom = 1,
            line_left = x1,
            line_right = y2;

    int head_top = head->y,
            head_bottom = head_top + 2,
            head_left = head->x,
            head_right = head_left + 2;

    return !(
            line_bottom < head_top
            || line_top > head_bottom
            || line_right < head_left
            || line_left > head_right
    );
}


void handle_game() {

    Sprite food_sprite;
    Sprite wall1;
    Sprite wall2;
    Sprite wall3;
    snake_maker();

    int food_location_x = rand() % LCD_X;
    int food_location_y = rand() % LCD_Y;

    show_screen();
    assign_data();
    init_sprite(&food_sprite, (float) food_location_x, (float) food_location_y, 3, 3, food);
    init_sprite(&wall1, LCD_X * 0.75, 0, 1, 12, byte_wall1);
    init_sprite(&wall2, LCD_X / 2, LCD_Y - 12, 1, 12, byte_wall1);
    init_sprite(&wall3, LCD_X / 3, LCD_Y - 12, 1, 12, byte_wall1);


    adc_init();
    while (lives > 0) {
        clear_screen();
        build_hud();
        update_random_seed();
        adc_result0 = adc_read(0);


        snake_speed();
        if (SNAKE_WAIT != 1) {
            ouroboros();
        }

        head_direction();
        handle_wrap();
        handle_collisions(&food_sprite, &wall1, &wall2, &wall3);
        draw_sprite(&food_sprite);

        draw_snek(&head);

        if (INPUT_READ(PINF, 5)) {
            show_walls = 0;
        }
        if (INPUT_READ(PINF, 6)) {
            show_walls = 1;
        }


        if (show_walls) {

            draw_sprite(&wall1);
            draw_sprite(&wall2);
            draw_sprite(&wall3);

            if (sprite_collision(&wall1) || sprite_collision(&wall2) || sprite_collision(&wall3)) {
                snake_loses_life();
            }
        }
        show_screen();

        OUTPUT_LOW(PORTC, 7);
    }

    _delay_ms(75);
    clear_screen();
    draw_string((LCD_X / 2) - 5, LCD_Y / 2 - 4, "Game Over");
    show_screen();
}


/*
   _____      _____  .___ _______
  /     \    /  _  \ |   |\      \
 /  \ /  \  /  /_\  \|   |/   |   \
/    Y    \/    |    \   /    |    \
\____|__  /\____|__  /___\____|__  /
        \/         \/            \/
 */


int main() {
            set_clock_speed(CPU_2MHz);
    lcd_init(LCD_DEFAULT_CONTRAST);

    name_and_student_number();
    handle_game();

    return 0;
}