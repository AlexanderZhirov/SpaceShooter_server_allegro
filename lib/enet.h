/* 
 * File:   enet.h
 * Author: Alexander Zhirov
 * Connection with me (telegram messanger): @alexanderzhirov
 *
 * Created on 2020.06.13
 */

#ifndef server_enet
#define server_enet

#include <enet/enet.h>
#include <stdbool.h>

typedef struct srv_enet
{
    ENetHost* server;
    ENetAddress address;
} SERVER_ENET;

SERVER_ENET* srv_create_enet(SERVER_ENET* enet, int port);
void srv_free_enet(SERVER_ENET* enet);
void srv_destroy_enet(SERVER_ENET* enet);

bool srv_check_init_enet(int result);

#endif
