#define _DEFAULT_SOURCE
#include <signal.h>
#include <stdio.h>

#include "common.h"
#include "lib/message.h"

void srv_catch_signal(int signal, void (*handler)(int))
{
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    
    show_message_error(sigaction(signal, &action, NULL) == 0, "Failed to register signal");
}

void srv_stop_server(int signal)
{
    SERVER_DATA* data = srv_get_server_data(NULL);
    
    data->done = true;
    
    puts("--------------------------------------------------");
    puts("Останавливаю сервер! Без паники!");
}
