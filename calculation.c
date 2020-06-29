#include "calculation.h"
#include <stdbool.h>
#include <stdio.h>

/************************************************************
 * SHIP
 ***********************************************************/

void srv_move_ship(SERVER_DATA* data, SERVER_GAME* game, SERVER_MESSAGE_UPDATE* message)
{
    SERVER_OBJECTS* objects = &game->objects;
    
    for(int i = 0; i < objects->ship_size; i++)
    {
        SERVER_SHIP* ship = objects->ships[i];
        
        if(ship->active)
        {
            SERVER_USER* user = NULL;
        
            for(int j = 0; j < game->user_size; j++)
                if(game->user[j] && game->user[j]->id == ship->ID)
                    user = game->user[j];

            ship->y += user->command.dy * ship->speed * data->timer.elapsed_time;
            if(ship->y < 0)
                ship->y = 0;
            else if(ship->y > game->display_height)
                ship->y = game->display_height;

            ship->x += user->command.dx * ship->speed * data->timer.elapsed_time;
            if(ship->x < 0)
                ship->x = 0;
            else if(ship->x > 900 * game->scale_factor)
                ship->x = 900 * game->scale_factor;            
            
            message->ship[i].dx = user->command.dx;
            message->ship[i].dy = user->command.dy;            
        }
        
        message->ship[i].ID = ship->ID;
        message->ship[i].x = ship->x;
        message->ship[i].y = ship->y;
        message->ship[i].lives = ship->lives;
        message->ship[i].score = ship->score;
        message->ship[i].active = ship->active;
    }
}


/************************************************************
 * BULLET
 ***********************************************************/

void srv_fire_bullet(SERVER_GAME* game)
{
    for(int i = 0; i < game->user_size; i++)
    {
        if(game->user[i] && game->user[i]->command.fire)
        {
            SERVER_OBJECTS* objects = &game->objects;
            SERVER_SHIP* ship = NULL;
        
            for(int j = 0; j < objects->ship_size; j++)
                if(objects->ships[j]->ID == game->user[i]->id)
                    ship = objects->ships[j];

            if(!ship->active) continue;
    
            for(int j = 0; j < objects->bullet_size; j++)
            {
                SERVER_BULLET* bullet = objects->bullets[j];
                
                if(!bullet->live && bullet->ID == game->user[i]->id)
                {
                    bullet->x = ship->x + 17;
                    bullet->y = ship->y;
                    bullet->live = true;
                    
                    break;
                }
            }
        }
    }
}

void srv_update_bullet(SERVER_DATA* data, SERVER_GAME* game, SERVER_MESSAGE_UPDATE* message)
{
    SERVER_OBJECTS* objects = &game->objects;
    
    for(int i = 0; i < objects->bullet_size; i++)
    {
        SERVER_BULLET* bullet = objects->bullets[i];
        
        if(bullet->live)
        {
            bullet->x += bullet->speed * data->timer.elapsed_time;
            if(bullet->x > game->display_width)
                bullet->live = false;
        }
        
        message->bullet[i].ID = bullet->ID;
        message->bullet[i].active = bullet->active;
        message->bullet[i].live = bullet->live;
        message->bullet[i].x = bullet->x;
        message->bullet[i].y = bullet->y;
    }
}


void srv_collide_bullet(SERVER_GAME* game, SERVER_MESSAGE_UPDATE* message)
{
    SERVER_OBJECTS* objects = &game->objects;
    
    for(int i = 0; i < objects->bullet_size; i++)
    {
        SERVER_BULLET* bullet = objects->bullets[i];
        
        if(bullet->live)
        {            
            for(int j = 0; j < objects->comet_size; j++)
            {
                SERVER_COMET* comet = objects->comets[j];
                
                if(comet->live)
                {                    
                    if(bullet->x > (comet->x - comet->boundx) &&
                        bullet->x < (comet->x + comet->boundx) &&
                        bullet->y > (comet->y - comet->boundy) &&
                        bullet->y < (comet->y + comet->boundy))
                    {
                        bullet->live = false;
                        comet->live = false;
                        
                        for(int k = 0; k < objects->ship_size; k++)
                            if(bullet->ID == objects->ships[k]->ID)
                                objects->ships[k]->score++;
                        
                        srv_start_explosions(game, message, bullet->x, bullet->y);
                    }
                }
            }
        }
    }
}

/************************************************************
 * COMET
 ***********************************************************/

void srv_start_comet(SERVER_GAME* game)
{
    SERVER_OBJECTS* objects = &game->objects;
    
    for(int i = 0; i < objects->comet_size; i++)
    {
        SERVER_COMET* comet = objects->comets[i];
        
        if(!comet->live)
        {
            if(rand() % 500 == 0)
            {
                comet->live = true;
                comet->x = game->display_width;
                comet->y = 30 + rand() % (game->display_height - 60);
                break;
            }
        }
    }
    
}

void srv_update_comet(SERVER_DATA* data, SERVER_GAME* game, SERVER_MESSAGE_UPDATE* message)
{
    SERVER_OBJECTS* objects = &game->objects;
    
    for(int i = 0; i < objects->comet_size; i++)
    {
        SERVER_COMET* comet = objects->comets[i];
        
        if(comet->live)
            comet->x -= comet->speed * data->timer.elapsed_time;
        
        message->comet[i].live = comet->live;
        message->comet[i].x = comet->x;
        message->comet[i].y = comet->y;
    }
}

void srv_collide_comet(SERVER_GAME* game, SERVER_MESSAGE_UPDATE* message)
{
    SERVER_OBJECTS* objects = &game->objects;
    
    for(int i = 0; i < objects->comet_size; i++)
    {
        SERVER_COMET* comet = objects->comets[i];
        
        if(!comet->live) continue;
        
        for(int j = 0; j < objects->ship_size; j++)
        {
            SERVER_SHIP* ship = objects->ships[j];

            if(!ship->active) continue;

            if((comet->x - comet->boundx) < (ship->x + ship->boundx) &&
                (comet->x + comet->boundx) > (ship->x - ship->boundx) &&
                (comet->y - comet->boundy) < (ship->y + ship->boundy) &&
                (comet->y + comet->boundy) > (ship->y - ship->boundy))
            {
                ship->lives--;
                comet->live = false;

                srv_start_explosions(game, message, ship->x, ship->y);
            }
            else if(comet->x < 0)
            {
                comet->live = false;
                ship->lives--;
            }
            
            if(ship->lives <= 0)
                ship->active = false;
        }
    }
}

/************************************************************
 * EXPLOSION
 ***********************************************************/

static void srv_start_explosions(SERVER_GAME* game, SERVER_MESSAGE_UPDATE* message, int x, int y)
{
    SERVER_OBJECTS* objects = &game->objects;
    
    for(int i = 0; i < objects->explosion_size; i++)
    {
        SERVER_EXPLOSION* explosion = objects->explosions[i];
        
        if(!explosion->live)
        {
            explosion->live = true;
            explosion->x = x;
            explosion->y = y;
            
            message->explosion[i].live = explosion->live;
            message->explosion[i].x = explosion->x;
            message->explosion[i].y = explosion->y;
            
            break;
        }
    }
}

void srv_update_explosions(SERVER_DATA* data, SERVER_GAME* game)
{
    SERVER_OBJECTS* objects = &game->objects;
    
    for(int i = 0; i < objects->explosion_size; i++)
    {
        SERVER_EXPLOSION* explosion = objects->explosions[i];
        
        if(explosion->live)
        {
            explosion->lifetime += data->timer.elapsed_time;
            
            if(explosion->lifetime >= 1.5)
            {
                explosion->lifetime = 0;
                explosion->live = false;
            }
        }
    }
}

