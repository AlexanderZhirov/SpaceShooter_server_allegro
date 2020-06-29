/* 
 * File:   update.h
 * Author: Alexander Zhirov
 * Connection with me (telegram messanger): @alexanderzhirov
 *
 * Created on 2020.06.15
 */

#ifndef server_update
#define server_update

#include "common.h"
#include "calculation.h"

/************************************************************
 * UPDATE USERS
 ***********************************************************/
void srv_client_update(SERVER_DATA* data);
static void srv_handle_userlist(SERVER_DATA* data, SERVER_USER* user);
/************************************************************
 * UPDATE GAMES
 ***********************************************************/
void srv_game_update(SERVER_DATA* data);
static void srv_handle_party(SERVER_DATA* data, SERVER_GAME* game);
static void srv_handle_game(SERVER_DATA* data, SERVER_GAME* game);
static void srv_clear_message(SERVER_MESSAGE_UPDATE* message);

#endif



