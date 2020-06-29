#include "event.h"
#include "message.h"

SERVER_EVENT* srv_create_event(SERVER_EVENT* event, SERVER_TIMER* timer)
{
    if(!event)
    {
        event = (SERVER_EVENT*)malloc(sizeof(SERVER_EVENT));
        show_message_error(event, "Failed to allocate space for SERVER_EVENT");
    }
    
    event->queue = al_create_event_queue();
    show_message_error(event->queue, "Failed to create Allegro event");
    
    if(timer)
        al_register_event_source(event->queue, al_get_timer_event_source(timer->al_timer));
    
    return event;
}

void srv_free_event(SERVER_EVENT* event)
{
    srv_destroy_event(event);
    free(event);
};

void srv_destroy_event(SERVER_EVENT* event)
{
    al_destroy_event_queue(event->queue);
}
