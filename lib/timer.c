#include "timer.h"
#include "message.h"

SERVER_TIMER* srv_create_timer(SERVER_TIMER* timer, int FPS)
{
    if(!timer)
    {
        timer = (SERVER_TIMER*)malloc(sizeof(SERVER_TIMER));
        show_message_error(timer, "Failed to allocate space for SERVER_TIMER");
    }
    
    timer->al_timer = al_create_timer(1.0 / FPS);
    show_message_error(timer->al_timer, "Failed to create Allegro timer");
    
    timer->last_time = 0.0;
    timer->current_time = 0.0;
    timer->elapsed_time = 0.0;
    
    return timer;
}

void srv_free_timer(SERVER_TIMER* timer)
{
    srv_destroy_timer(timer);
    free(timer);
}

void srv_destroy_timer(SERVER_TIMER* timer)
{
    al_destroy_timer(timer->al_timer);
}

void srv_update_time(SERVER_TIMER* timer)
{
    timer->last_time = timer->current_time;
    timer->current_time = al_get_time();
    timer->elapsed_time = timer->current_time - timer->last_time;
}
