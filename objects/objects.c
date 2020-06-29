#include "objects.h"
#include "../lib/message.h"

SERVER_OBJECTS* srv_create_objects(SERVER_OBJECTS* objects, int ship_size, int bullet_size, int comet_size, int explosion_size)
{
    if(!objects)
    {
        SERVER_OBJECTS* objects = (SERVER_OBJECTS*)malloc(sizeof(SERVER_OBJECTS));
        show_message_error(objects, "Failed to allocate space for SERVER_OBJECTS");
    }
    
    objects->ship_size = ship_size;
    objects->bullet_size = bullet_size;
    objects->comet_size = comet_size;
    objects->explosion_size = explosion_size;
    
    objects->ships = srv_create_ships(ship_size);
    objects->bullets = srv_create_bullets(bullet_size);
    objects->comets = srv_create_comets(comet_size);
    objects->explosions = srv_create_explosions(explosion_size);
    
    return objects;
}

void srv_free_objects(SERVER_OBJECTS* objects)
{
    srv_destroy_objects(objects);
    free(objects);
}

void srv_destroy_objects(SERVER_OBJECTS* objects)
{
    if(objects->ship_size > 0)
    {
        for(int i = 0; i < objects->ship_size; i++)
            free(objects->ships[i]);
        free(objects->ships);
    }
    
    if(objects->bullet_size > 0)
    {
        for(int i = 0; i < objects->bullet_size; i++)
            free(objects->bullets[i]);
        free(objects->bullets);
    }
    
    if(objects->comet_size > 0)
    {
        for(int i = 0; i < objects->comet_size; i++)
            free(objects->comets[i]);
        free(objects->comets);
    }
    
    if(objects->explosion_size > 0)
    {
        for(int i = 0; i < objects->explosion_size; i++)
            free(objects->explosions[i]);
        free(objects->explosions);
    }
}

static SERVER_SHIP** srv_create_ships(int size)
{
    SERVER_SHIP** ships = (SERVER_SHIP**)malloc(sizeof(SERVER_SHIP*) * size);
    show_message_error(ships, "Failed to allocate space for SERVER_SHIP collection");
    
    for(int i = 0; i < size; i++)
    {
        ships[i] = (SERVER_SHIP*)malloc(sizeof(SERVER_SHIP));
        show_message_error(ships[i], "Failed to allocate space for SERVER_SHIP");
    }
    
    return ships;
}

static SERVER_BULLET** srv_create_bullets(int size)
{
    SERVER_BULLET** bullets = (SERVER_BULLET**)malloc(sizeof(SERVER_BULLET*) * size);
    show_message_error(bullets, "Failed to allocate space for SERVER_BULLET collection");
    
    for(int i = 0; i < size; i++)
    {
        bullets[i] = (SERVER_BULLET*)malloc(sizeof(SERVER_BULLET));
        show_message_error(bullets[i], "Failed to allocate space for SERVER_BULLET");
    }
    
    return bullets;
}

static SERVER_COMET** srv_create_comets(int size)
{
    SERVER_COMET** comets = (SERVER_COMET**)malloc(sizeof(SERVER_COMET*) * size);
    show_message_error(comets, "Failed to allocate space for SERVER_COMET collection");
    
    for(int i = 0; i < size; i++)
    {
        comets[i] = (SERVER_COMET*)malloc(sizeof(SERVER_COMET));
        show_message_error(comets[i], "Failed to allocate space for SERVER_COMET");
    }
    
    return comets;
}

static SERVER_EXPLOSION** srv_create_explosions(int size)
{
    SERVER_EXPLOSION** explosions = (SERVER_EXPLOSION**)malloc(sizeof(SERVER_EXPLOSION*) * size);
    show_message_error(explosions, "Failed to allocate space for SERVER_EXPLOSION collection");
    
    for(int i = 0; i < size; i++)
    {
        explosions[i] = (SERVER_EXPLOSION*)malloc(sizeof(SERVER_EXPLOSION));
        show_message_error(explosions[i], "Failed to allocate space for SERVER_EXPLOSION");
    }
    
    return explosions;
}

void srv_init_ships(SERVER_OBJECTS* objects, int key_ship, int ID, int animation_columns, int animation_direction, int animation_row, int boundx, int boundy, int cur_frame,
        int frame_count, int frame_delay, int frame_height, int frame_width, int lives, int max_frame, int score, int speed, int x, int y)
{
    SERVER_SHIP* ship = objects->ships[key_ship];
    
    ship->ID = ID;
    ship->animation_columns = animation_columns;
    ship->animation_direction = animation_direction;
    ship->animation_row = animation_row;
    ship->boundx = boundx;
    ship->boundy = boundy;
    ship->cur_frame = cur_frame;
    ship->frame_count = frame_count;
    ship->frame_delay = frame_delay;
    ship->frame_height = frame_height;
    ship->frame_width = frame_width;
    ship->lives = lives;
    ship->max_frame = max_frame;
    ship->score = score;
    ship->speed = speed;
    ship->x = x;
    ship->y = y;
    ship->active = true;
}

void srv_init_bullets(SERVER_OBJECTS* objects, int key_bullet, int ID, int speed, int x, int y)
{
    SERVER_BULLET* bullet = objects->bullets[key_bullet];
    
    bullet->ID = ID;
    bullet->live = false;
    bullet->speed = speed;
    bullet->x = x;
    bullet->y = y;
    bullet->active = true;
}

void srv_init_comets(SERVER_OBJECTS* objects, int key_comet, int ID, int animation_columns, int animation_direction, int boundx, int boundy, int cur_frame,
        int frame_count, int frame_delay, int frame_height, int frame_width, int max_frame, int speed, int x, int y)
{
    SERVER_COMET* comet = objects->comets[key_comet];
    
    comet->ID = ID;
    comet->animation_columns = animation_columns;
    comet->animation_direction = animation_direction;
    comet->boundx = boundx;
    comet->boundy = boundy;
    comet->cur_frame = cur_frame;
    comet->frame_count = frame_count;
    comet->frame_delay = frame_delay;
    comet->frame_height = frame_height;
    comet->frame_width = frame_width;
    comet->max_frame = max_frame;
    comet->speed = speed;
    comet->live = false;
    comet->x = x;
    comet->y = y;
}

void srv_init_explosions(SERVER_OBJECTS* objects, int key_explosion, int animation_columns, int animation_direction, int cur_frame,
        int frame_count, int frame_delay, int frame_height, int frame_width, int max_frame, int x, int y)
{
    SERVER_EXPLOSION* explosion = objects->explosions[key_explosion];
    
    explosion->animation_columns = animation_columns;
    explosion->animation_direction = animation_direction;
    explosion->cur_frame = cur_frame;
    explosion->frame_count = frame_count;
    explosion->frame_delay = frame_delay;
    explosion->frame_height = frame_height;
    explosion->frame_width = frame_width;
    explosion->live = false;
    explosion->max_frame = max_frame;
    explosion->x = x;
    explosion->y = y;
    explosion->lifetime = 0;
}
