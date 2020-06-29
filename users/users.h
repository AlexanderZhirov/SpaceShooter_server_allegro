/* 
 * File:   users.h
 * Author: Alexander Zhirov
 * Connection with me (telegram messanger): @alexanderzhirov
 *
 * Created on 2020.06.14
 */

#ifndef server_users
#define server_users

#include <stdbool.h>
#include <enet/enet.h>

typedef enum srv_state
{
    STATE_NONE,
    STATE_LIST,
    STATE_PARTY,
    STATE_GAME,
    STATE_DISCONNECT
} SERVER_STATE;

typedef struct srv_game_command
{
    int dx;
    int dy;
    bool fire;
} SERVER_GAME_COMMAND;

typedef struct srv_user
{
    int id;
    int game_id;
    char nickname[40];
    int display_width;
    int display_height;
    float scale_factor;
    SERVER_STATE state;
    bool in_game;
    bool is_creator; // game creator
    bool is_ready;
    bool active;
    ENetPeer* peer;
    SERVER_GAME_COMMAND command;
} SERVER_USER;

typedef struct srv_users
{
    SERVER_USER** user;
    int user_size;
} SERVER_USERS;

SERVER_USERS* srv_create_users(SERVER_USERS* users, int user_size);
void srv_free_users(SERVER_USERS* users);
void srv_destroy_users(SERVER_USERS* users);

void srv_clear_user(SERVER_USER* user);
SERVER_USER* srv_get_free_user(SERVER_USERS* users);

#endif



