/* 
 * File:   games.h
 * Author: Alexander Zhirov
 * Connection with me (telegram messanger): @alexanderzhirov
 *
 * Created on 2020.06.15
 */

#ifndef server_games
#define server_games

#include "../users/users.h"
#include "../objects/objects.h"

typedef struct srv_game
{
    int id;
    int display_width;
    int display_height;
    float scale_factor;
    char game_name[40];
    SERVER_USER** user;
    int user_size;
    int count_connected;
    bool game_started;
    bool active;
    SERVER_OBJECTS objects;
} SERVER_GAME;

typedef struct srv_games
{
    SERVER_GAME** game;
    int game_size;
} SERVER_GAMES;

SERVER_GAMES* srv_create_games(SERVER_GAMES* games, int game_size);
void srv_free_games(SERVER_GAMES* games);
void srv_destroy_games(SERVER_GAMES* games);

void srv_clear_games(SERVER_GAME* game);
SERVER_GAME* srv_get_free_game(SERVER_GAMES* games);

#endif



