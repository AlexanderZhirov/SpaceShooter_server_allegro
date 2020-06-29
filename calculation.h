/* 
 * File:   calculation.h
 * Author: Alexander Zhirov
 * Connection with me (telegram messanger): @alexanderzhirov
 *
 * Created on 2020.06.26
 */

#ifndef server_calculation
#define server_calculation

#include "games/games.h"
#include "common.h"
#include "connection.h"

/************************************************************
 * SHIP
 ***********************************************************/
void srv_move_ship(SERVER_DATA* data, SERVER_GAME* game, SERVER_MESSAGE_UPDATE* message);
/************************************************************
 * BULLET
 ***********************************************************/
void srv_fire_bullet(SERVER_GAME* game);
void srv_update_bullet(SERVER_DATA* data, SERVER_GAME* game, SERVER_MESSAGE_UPDATE* message);
void srv_collide_bullet(SERVER_GAME* game, SERVER_MESSAGE_UPDATE* message);
/************************************************************
 * COMET
 ***********************************************************/
void srv_start_comet(SERVER_GAME* game);
void srv_update_comet(SERVER_DATA* data, SERVER_GAME* game, SERVER_MESSAGE_UPDATE* message);
void srv_collide_comet(SERVER_GAME* game, SERVER_MESSAGE_UPDATE* message);
/************************************************************
 * EXPLOSION
 ***********************************************************/
static void srv_start_explosions(SERVER_GAME* game, SERVER_MESSAGE_UPDATE* message, int x, int y);
void srv_update_explosions(SERVER_DATA* data, SERVER_GAME* game);

#endif



