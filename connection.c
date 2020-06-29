#include "connection.h"
#include <stdio.h>
#include "lib/message.h"

/************************************************************
 * CONNECTION HANDLING
 ***********************************************************/

void srv_send_receive(SERVER_DATA* data)
{
    ENetPacket *packet;
    ENetEvent event;
    SERVER_MESSAGE_NONE* message_none;
    SERVER_MESSAGE_JOINGAME* message_list;
    SERVER_MESSAGE_SENDPARTY* message_party;
    SERVER_MESSAGE_GAME* message_game;
    
    while (enet_host_service(data->enet.server, &event, 0) > 0)
    {
        switch(event.type)
        {
            case ENET_EVENT_TYPE_NONE:
                break;
            case ENET_EVENT_TYPE_CONNECT:
                printf("Сервер: новый пользователь из %x:%hu\n", event.peer->address.host, event.peer->address.port);
                
                SERVER_USER* user = srv_get_free_user(&data->users);
                show_message_error(user, "Failed to allocate free game");
                // Отсоединить пользователя
//                enet_peer_disconnect_now(event.peer, 0);
                user->peer = event.peer; // сохранить пир в пользователе
                
                event.peer->data = malloc(sizeof(int));
                *(int*)event.peer->data = user->id;
                printf("Сервер: пользователь присоединён #%d\n", user->id);
                break;
            case ENET_EVENT_TYPE_RECEIVE:
            {
                int user_id = *(int*)event.peer->data;
                
                switch(data->users.user[user_id]->state)
                {
                    case STATE_NONE:
                        message_none = (SERVER_MESSAGE_NONE*)event.packet->data;
                        srv_readmessage_none(data, user_id, message_none);
                        break;
                    case STATE_LIST:
                        message_list = (SERVER_MESSAGE_JOINGAME*)event.packet->data;
                        srv_readmessage_list(data, user_id, message_list);
                        break;
                    case STATE_PARTY:
                        message_party = (SERVER_MESSAGE_SENDPARTY*)event.packet->data;
                        srv_readmessage_party(data, user_id, message_party);
                        break;
                    case STATE_GAME:
                        message_game = (SERVER_MESSAGE_GAME*)event.packet->data;
                        srv_readmessage_game(data, user_id, message_game);
                        break;
                }
            
                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT:
            {
                int user_id = *(int*)event.peer->data;
                srv_removing_from_game(data, data->users.user[user_id]);
                srv_clear_user(data->users.user[user_id]);
                
                printf("Сервер: пользователь #%d был отсоединён\n", user_id);
                // сообщить пользователям об отсоединении клиента
                
                free(event.peer->data);
                event.peer->data = NULL;
                break;
            }
        }
    }
}

static void srv_readmessage_none(SERVER_DATA* data, int user_id, SERVER_MESSAGE_NONE* message)
{
    SERVER_USER* user = data->users.user[user_id];
    
    user->display_height = message->display_height;
    user->display_width = message->display_width;
    user->scale_factor = message->scale_factor;
    user->is_creator = message->is_creator;
    user->is_ready = message->is_ready;
    strncpy(user->nickname, message->nickname, 40);
    
    if(message->is_creator)
        srv_create_new_game(data, user, message->game_name);
        
    srv_change_state(data, user_id, message->state);
}

static void srv_readmessage_list(SERVER_DATA* data, int user_id, SERVER_MESSAGE_JOINGAME* message)
{
    SERVER_USER* user = data->users.user[user_id];
    SERVER_GAME* game = data->games.game[message->id];
    
    ENetPacket *packet;
    SERVER_MESSAGE_LIST answer;
    
    if(game->count_connected < 3)
    {
        game->count_connected++;
        
        srv_change_state(data, user_id, message->state);
        
        user->game_id = message->id;
        user->in_game = true;
        
        for(int i = 0; i < game->user_size; i++)
            if(!game->user[i])
            {
                game->user[i] = user;

                break;
            }
        
        answer.state = message->state;

        packet = enet_packet_create(&answer, sizeof(answer), ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(user->peer, 0, packet);
    }
}

static void srv_readmessage_party(SERVER_DATA* data, int user_id, SERVER_MESSAGE_SENDPARTY* message)
{
    SERVER_USER* user = data->users.user[user_id];
    
    switch(message->state)
    {
        case STATE_LIST:
            if(message->kick_user)
                user = data->users.user[message->user_id];
            
            srv_removing_from_game(data, user);
            user->state = message->state;
            user->in_game = false;
            user->is_ready = false;
            user->game_id = -1;
            user->is_creator = false;
            
            srv_send_change_state(user, message->state);
            break;
        case STATE_PARTY:
            user->is_ready = message->is_ready;
            break;
        case STATE_GAME:
            srv_start_game(data, user);
            break;
    }
}

static void srv_readmessage_game(SERVER_DATA* data, int user_id, SERVER_MESSAGE_GAME* message)
{
    SERVER_USER* user = data->users.user[user_id];
    
    switch(message->state)
    {
        case STATE_GAME:
        {
            user->command.dx = message->dx;
            user->command.dy = message->dy;
            user->command.fire = message->fire;
            
            break;
        }
    }
}

static void srv_change_state(SERVER_DATA* data, int user_id, SERVER_STATE state)
{
    SERVER_USER* user = data->users.user[user_id];
    
    user->state = state;
}

static void srv_create_new_game(SERVER_DATA* data, SERVER_USER* user, const char* game_name)
{
    // Creating a new game and placing the user there
    SERVER_GAME* game = srv_get_free_game(&data->games);
    // Отправить сообщение в случае, если нет места для создания новой игры
    show_message_error(game, "Failed to allocate free game");
    
    user->game_id = game->id;
    user->in_game = true;
    game->user[0] = user;
    game->count_connected++;
    strncpy(game->game_name, game_name, 40);
}

static void srv_removing_from_game(SERVER_DATA* data, SERVER_USER* user)
{
    if(user->in_game)
    {
        SERVER_GAME* game = data->games.game[user->game_id];

        for(int i = 0; i < game->user_size; i++)
            if(game->user[i] && game->user[i]->id == user->id)
            {
                game->user[i] = NULL;
                --game->count_connected;

                if(game->game_started)
                {
                    for(int j = 0; j < game->objects.ship_size; j++)
                        if(user->id == game->objects.ships[j]->ID)
                            game->objects.ships[j]->active = false;
                    
                    for(int j = 0; j < game->objects.bullet_size; j++)
                        if(user->id == game->objects.bullets[j]->ID)
                            game->objects.bullets[j]->active = false;
                }
                
                if(game->count_connected == 0)
                {
                    if(game->game_started)
                        srv_destroy_objects(&game->objects);
                    
                    srv_clear_games(game);                    
                }
                else if(user->is_creator)
                {
                    for(int j = 0; j < game->user_size; j++)
                        if(game->user[j])
                        {
                            game->user[j]->is_creator = true;
                            game->user[j]->is_ready = true;
                            
                            break;
                        }
                }
                
                break;
            }
    }
}

static void srv_start_game(SERVER_DATA* data, SERVER_USER* user)
{
    if(user->in_game)
    {
        bool users_ready = true;
        
        SERVER_GAME* game = data->games.game[user->game_id];
        
        for(int i = 0; i < game->user_size; i++)
            if(game->user[i])
                if(!game->user[i]->is_ready)
                {
                    users_ready = false;
                    break;
                }
        
        if(users_ready)
        {
            game->game_started = true;
            
            srv_init_objects(game);
            
            for(int i = 0; i < game->user_size; i++)
                if(game->user[i])
                {
                    game->user[i]->command.dx = 0;
                    game->user[i]->command.dy = 0;
                    game->user[i]->command.fire = false;
                    
                    game->user[i]->state = STATE_GAME;

                    srv_send_change_state(game->user[i], STATE_GAME);
                }
        }
    }
}

static void srv_send_change_state(SERVER_USER* user, SERVER_STATE state)
{
    ENetPacket *packet;
    SERVER_MESSAGE_PARTY message;
    
    message.state = state;
    packet = enet_packet_create(&message, sizeof(message), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(user->peer, 0, packet);
}


static void srv_init_objects(SERVER_GAME* game)
{
    SERVER_OBJECTS* objects = &game->objects;
    int count_users = game->count_connected;
    
    int start_position = 0;
    int position_ratio = 1; // part user position
    bool start_ratio = false;
    int iterator = 0;
    
    srv_create_objects(objects, count_users, 5 * count_users, 15, 5 * count_users);
    
    for(int i = 0; i < game->user_size; i++)
        if(game->user[i] && game->user[i]->is_creator)
        {
            game->display_width = game->user[i]->display_width;
            game->display_height = game->user[i]->display_height;
            game->scale_factor = game->user[i]->scale_factor;
            break;
        }
    
    start_position = game->display_height / (count_users * 2);
    
    for(int i = 0; i < game->user_size; i++)
        if(game->user[i])
        {
            if(start_ratio)
                position_ratio += 2;
            else
                start_ratio = true;
            
            srv_init_ships(objects, iterator, game->user[i]->id, 3, 1, 1, 10, 12, 0, 0, 50, 41, 46, 3, 3, 0, 250, 20, start_position * position_ratio);
            
            for(int j = iterator++ * 5; j < iterator * 5; j++)
                srv_init_bullets(objects, j, game->user[i]->id, 500, 0, 0);
        }
    
    for(int i = 0; i < objects->comet_size; i++)
        srv_init_comets(objects, i, 0, 21, rand() % 2 ? 1 : -1, 35, 35, 0, 0, 2, 96, 96, 143, 250, 0, 0);
    
    for(int i = 0; i < objects->explosion_size; i++)
        srv_init_explosions(objects, i, 8, 1, 0, 0, 1, 128, 128, 31, 0, 0);
}

