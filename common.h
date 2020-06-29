/* 
 * File:   common.h
 * Author: Alexander Zhirov
 * Connection with me (telegram messanger): @alexanderzhirov
 *
 * Created on 2020.06.14
 */

#ifndef server_data
#define server_data

#include "lib/common.h"
#include "users/users.h"
#include "games/games.h"

typedef struct srv_data
{
    SERVER_ENET enet;
    SERVER_TIMER timer;
    SERVER_EVENT event;
    SERVER_USERS users;
    SERVER_GAMES games;
    bool done;
} SERVER_DATA;

SERVER_DATA* srv_create_data(SERVER_DATA* data, SERVER_PARAMETRS parametrs);
void srv_destroy_data(SERVER_DATA* data);
SERVER_DATA* srv_get_server_data(SERVER_DATA* data);

void srv_catch_signal(int signal, void (*handler)(int));
void srv_stop_server(int signal);

#endif
