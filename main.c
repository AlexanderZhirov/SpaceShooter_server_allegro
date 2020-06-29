#include <stdio.h>
#include "server.h"

int main(int argc, char** argv)
{
    srv_load_resources();
    
    SERVER_PARAMETRS parametrs =
    {
        .port = 9234,
        .FPS = 60,
        .users = 30,
        .games = 3
    };
    
    SERVER_DATA* data = srv_create_data(NULL, parametrs);
    
    al_start_timer(data->timer.al_timer);
    data->timer.current_time = al_current_time();
    
    srand(time(NULL));
    
    while(!data->done)
    {
        al_wait_for_event(data->event.queue, &data->event.current);
        
        if(data->event.current.type == ALLEGRO_EVENT_TIMER)
        {
            srv_update_time(&data->timer);
            // возможно тут нужно сделать потоки
            srv_game_update(data);
            srv_client_update(data);
            
            srv_send_receive(data);
        }
    }
    
    srv_destroy_data(data);
    
    return EXIT_SUCCESS;
}
