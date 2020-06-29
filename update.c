#include "update.h"
#include "connection.h"
#include <stdio.h>

/************************************************************
 * UPDATE GAMES
 ***********************************************************/

void srv_game_update(SERVER_DATA* data)
{
    for(int i = 0; i < data->games.game_size; i++)
    {
        if(!data->games.game[i]->active) continue;
        
        SERVER_GAME* game = data->games.game[i];
        
        if(game->game_started)
            srv_handle_game(data, game);
        else
            srv_handle_party(data, game);
    }
}

static void srv_handle_party(SERVER_DATA* data, SERVER_GAME* game)
{
    ENetPacket *packet;
    SERVER_MESSAGE_PARTY message;
    
    int iterator = 0;
    
    for(int i = 0; i < game->user_size; i++)
        if(game->user[i])
        {
            message.users[iterator].id = game->user[i]->id;
            message.users[iterator].is_ready = game->user[i]->is_ready;
            strncpy(message.users[iterator].nickname, game->user[i]->nickname, 40);
            iterator++;
        }
    
    message.count_connected = game->count_connected;
    message.game_id = game->id;
    message.active = game->active;
    message.game_started = game->game_started;
    strncpy(message.game_name, game->game_name, 40);
    
    for(int i = 0; i < game->user_size; i++)
        if(game->user[i])
        {
            message.is_creator = game->user[i]->is_creator;
            message.is_ready = game->user[i]->is_ready;
            message.user_id = game->user[i]->id;
            packet = enet_packet_create(&message, sizeof(message), ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(game->user[i]->peer, 0, packet);
        }
}

static void srv_handle_game(SERVER_DATA* data, SERVER_GAME* game)
{
    ENetPacket *packet;
    SERVER_MESSAGE_UPDATE message;
    
    srv_clear_message(&message);
    
    srv_update_explosions(data, game);
    srv_move_ship(data, game, &message);
    srv_fire_bullet(game);
    srv_update_bullet(data, game, &message);
    srv_start_comet(game);
    srv_update_comet(data, game, &message);
    srv_collide_bullet(game, &message);
    srv_collide_comet(game, &message);
    
    for(int i = 0; i < game->user_size; i++)
        if(game->user[i])
        {
            packet = enet_packet_create(&message, sizeof(message), ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(game->user[i]->peer, 0, packet);
        }
}

static void srv_clear_message(SERVER_MESSAGE_UPDATE* message)
{
    for(int i = 0; i < 15; i++)
    {
        message->explosion[i].live = false;
        message->explosion[i].x = 0;
        message->explosion[i].y = 0;
    }
    for(int i = 0; i < 15; i++)
    {
        message->comet[i].live = false;
        message->comet[i].x = 0;
        message->comet[i].y = 0;
    }
    for(int i = 0; i < 15; i++)
    {
        message->bullet[i].ID = -1;
        message->bullet[i].active = false;
        message->bullet[i].live = false;
        message->bullet[i].x = 0;
        message->bullet[i].y = 0;
    }
    for(int i = 0; i < 3; i++)
    {
        message->ship[i].ID = -1;
        message->ship[i].active = false;
        message->ship[i].dx = 0;
        message->ship[i].dy = 0;
        message->ship[i].lives = 0;
        message->ship[i].score = 0;
        message->ship[i].x = 0;
        message->ship[i].y = 0;
    }
}

/************************************************************
 * UPDATE USERS
 ***********************************************************/

void srv_client_update(SERVER_DATA* data)
{
    for(int i = 0; i < data->users.user_size; i++)
    {
        if(!data->users.user[i]->active) continue;
        
        SERVER_USER* user = data->users.user[i];
        
        switch(user->state)
        {
            case STATE_LIST:
                srv_handle_userlist(data, user);
                break;
            case STATE_PARTY:
                break;
            case STATE_GAME:
                break;
        }
    }
}

static void srv_handle_userlist(SERVER_DATA* data, SERVER_USER* user)
{
    ENetPacket *packet;
    SERVER_MESSAGE_LIST message;
    
    message.count_game = 0;
    
    int iterator = 0;
    
    for(int i = 0; i < 3; i++)
        if(data->games.game[i]->active && !data->games.game[i]->game_started)
        {
            message.games[iterator].id = data->games.game[i]->id;
            message.games[iterator].count_connected = data->games.game[i]->count_connected;
            strncpy(message.games[iterator].game_name, data->games.game[i]->game_name, 40);
            message.count_game++;
            iterator++;
        }
    
    message.state = STATE_LIST;
    packet = enet_packet_create(&message, sizeof(message), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(user->peer, 0, packet);
}

