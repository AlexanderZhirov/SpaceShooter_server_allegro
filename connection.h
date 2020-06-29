/* 
 * File:   connection.h
 * Author: Alexander Zhirov
 * Connection with me (telegram messanger): @alexanderzhirov
 *
 * Created on 2020.06.14
 */

#ifndef server_connection
#define server_connection

#include "common.h"

typedef struct srv_usercommand
{
    int dx;
    int dy;
    bool fire;
} SERVER_USER_COMMAND;

typedef struct srv_userdata
{
    int id;
    char nickname[40];
    bool is_ready;
    SERVER_USER_COMMAND command;
} SERVER_USER_DATA;

typedef struct srv_gamedata
{
    int id;
    char game_name[40];
    int count_connected;
} SERVER_GAME_DATA;

typedef struct srv_message_none
{
    int display_width;
    int display_height;
    float scale_factor;
    SERVER_STATE state;
    char game_name[40];
    char nickname[40];
    bool is_creator;
    bool is_ready;
} SERVER_MESSAGE_NONE;

typedef struct srv_message_list
{
    SERVER_GAME_DATA games[3];
    SERVER_STATE state;
    int count_game;
} SERVER_MESSAGE_LIST;

typedef struct srv_message_party
{
    int game_id;
    int user_id;
    SERVER_USER_DATA users[3];
    char game_name[40];
    int count_connected;
    bool game_started;
    bool active;
    bool is_creator;
    bool is_ready;
    SERVER_STATE state;
} SERVER_MESSAGE_PARTY;

typedef struct srv_message_sendparty
{
    SERVER_STATE state;
    bool is_creator;
    bool is_ready;
    bool kick_user;
    int user_id;
} SERVER_MESSAGE_SENDPARTY;

typedef struct srv_message_joingame
{
    int id;
    SERVER_STATE state;
} SERVER_MESSAGE_JOINGAME;

typedef struct srv_message_game
{
    int dx;
    int dy;
    bool fire;
    SERVER_STATE state;
} SERVER_MESSAGE_GAME;

// GAME

typedef struct srv_update_ship
{
    int ID;
    int x;
    int y;
    int dx;
    int dy;
    int lives;
    int score;
    bool active;
} SERVER_UPDATE_SHIP;

typedef struct srv_update_bullet
{
    int ID;
    int x;
    int y;
    bool live;
    bool active;
} SERVER_UPDATE_BULLET;

typedef struct srv_update_comet
{
    int x;
    int y;
    bool live;
} SERVER_UPDATE_COMET;

typedef struct srv_update_explosion
{
    int x;
    int y;
    bool live;
} SERVER_UPDATE_EXPLOSION;

typedef struct srv_message_update
{
    SERVER_UPDATE_SHIP ship[3];
    SERVER_UPDATE_BULLET bullet[15];
    SERVER_UPDATE_COMET comet[15];
    SERVER_UPDATE_EXPLOSION explosion[15];
} SERVER_MESSAGE_UPDATE;

void srv_send_receive(SERVER_DATA* data);
static void srv_readmessage_none(SERVER_DATA* data, int user_id, SERVER_MESSAGE_NONE* message);
static void srv_readmessage_list(SERVER_DATA* data, int user_id, SERVER_MESSAGE_JOINGAME* message);
static void srv_readmessage_party(SERVER_DATA* data, int user_id, SERVER_MESSAGE_SENDPARTY* message);
static void srv_readmessage_game(SERVER_DATA* data, int user_id, SERVER_MESSAGE_GAME* message);
static void srv_change_state(SERVER_DATA* data, int user_id, SERVER_STATE state);

static void srv_create_new_game(SERVER_DATA* data, SERVER_USER* user, const char* game_name);
static void srv_removing_from_game(SERVER_DATA* data, SERVER_USER* user);
static void srv_start_game(SERVER_DATA* data, SERVER_USER* user);

static void srv_send_change_state(SERVER_USER* user, SERVER_STATE state);

static void srv_init_objects(SERVER_GAME* game);

#endif



