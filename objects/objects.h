/* 
 * File:   objects.h
 * Author: Alexander Zhirov
 * Connection with me (telegram messanger): @alexanderzhirov
 *
 * Created on 2020.06.03
 */

#ifndef server_objects
#define server_objects

#include <stdbool.h>

typedef struct srv_ship
{
    int ID;
    int x;
    int y;
    int lives;
    int speed;
    int boundx;
    int boundy;
    int score;
    bool active;
    
    int max_frame;
    int cur_frame;
    int frame_count;
    int frame_delay;
    int frame_width;
    int frame_height;
    int animation_columns;
    int animation_direction;
    
    int animation_row;
} SERVER_SHIP;

typedef struct srv_bullet
{
    int ID;
    int x;
    int y;
    bool live;
    int speed;
    bool active;
} SERVER_BULLET;

typedef struct srv_comet
{
    int ID;
    int x;
    int y;
    bool live;
    int speed;
    int boundx;
    int boundy;
    
    int max_frame;
    int cur_frame;
    int frame_count;
    int frame_delay;
    int frame_width;
    int frame_height;
    int animation_columns;
    int animation_direction;
} SERVER_COMET;

typedef struct srv_explosion
{
    int x;
    int y;
    bool live;
    float lifetime;
    
    int max_frame;
    int cur_frame;
    int frame_count;
    int frame_delay;
    int frame_width;
    int frame_height;
    int animation_columns;
    int animation_direction;
} SERVER_EXPLOSION;

typedef struct srv_objects
{
    SERVER_SHIP** ships;
    int ship_size;
    SERVER_BULLET** bullets;
    int bullet_size;
    SERVER_COMET** comets;
    int comet_size;
    SERVER_EXPLOSION** explosions;
    int explosion_size;
} SERVER_OBJECTS;

SERVER_OBJECTS* srv_create_objects(SERVER_OBJECTS* data, int ship_size, int bullet_size, int comet_size, int explosion_size);
void srv_free_objects(SERVER_OBJECTS* objects);
void srv_destroy_objects(SERVER_OBJECTS* objects);

static SERVER_SHIP** srv_create_ships(int size);
static SERVER_BULLET** srv_create_bullets(int size);
static SERVER_COMET** srv_create_comets(int size);
static SERVER_EXPLOSION** srv_create_explosions(int size);

void srv_init_ships(SERVER_OBJECTS* objects, int key_ship, int ID, int animation_columns, int animation_direction, int animation_row, int boundx, int boundy, int cur_frame,
        int frame_count, int frame_delay, int frame_height, int frame_width, int lives, int max_frame, int score, int speed, int x, int y);
void srv_init_bullets(SERVER_OBJECTS* objects, int key_bullet, int ID, int speed, int x, int y);
void srv_init_comets(SERVER_OBJECTS* objects, int key_comet, int ID, int animation_columns, int animation_direction, int boundx, int boundy, int cur_frame,
        int frame_count, int frame_delay, int frame_height, int frame_width, int max_frame, int speed, int x, int y);
void srv_init_explosions(SERVER_OBJECTS* objects, int key_explosion, int animation_columns, int animation_direction, int cur_frame,
        int frame_count, int frame_delay, int frame_height, int frame_width, int max_frame, int x, int y);

#endif
