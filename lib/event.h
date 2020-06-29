/* 
 * File:   event.h
 * Author: Alexander Zhirov
 * Connection with me (telegram messanger): @alexanderzhirov
 *
 * Created on 2020.06.13
 */

#ifndef server_event
#define server_event

#include <allegro5/allegro5.h>
#include "timer.h"

typedef struct srv_event
{
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_EVENT current;
} SERVER_EVENT;

SERVER_EVENT* srv_create_event(SERVER_EVENT* event, SERVER_TIMER* timer);
void srv_free_event(SERVER_EVENT* event);
void srv_destroy_event(SERVER_EVENT* event);

#endif



