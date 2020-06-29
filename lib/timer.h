/* 
 * File:   timer.h
 * Author: Alexander Zhirov
 * Connection with me (telegram messanger): @alexanderzhirov
 *
 * Created on 2020.06.13
 */

#ifndef server_timer
#define server_timer

#include <allegro5/allegro5.h>

typedef struct srv_timer
{
    ALLEGRO_TIMER* al_timer;
    double last_time;
    double current_time;
    double elapsed_time;
} SERVER_TIMER;

SERVER_TIMER* srv_create_timer(SERVER_TIMER* timer, int FPS);

void srv_free_timer(SERVER_TIMER* timer);
void srv_destroy_timer(SERVER_TIMER* timer);
void srv_update_time(SERVER_TIMER* timer);

#endif



