/* 
 * File:   common.h
 * Author: Alexander Zhirov
 * Connection with me (telegram messanger): @alexanderzhirov
 *
 * Created on 2020.06.13
 */

#ifndef server_common
#define server_common

#include "enet.h"
#include "timer.h"
#include "event.h"

typedef struct srv_resource
{
    SERVER_ENET enet;
    SERVER_TIMER timer;
    SERVER_EVENT event;
} SERVER_RESOURCE;

typedef struct cl_parametrs
{
    int port;
    int FPS;
    int users;
    int games;
} SERVER_PARAMETRS;

void srv_load_resources();

#endif
