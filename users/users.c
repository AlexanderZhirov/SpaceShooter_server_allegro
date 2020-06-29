#include "users.h"
#include "../lib/message.h"

SERVER_USERS* srv_create_users(SERVER_USERS* users, int user_size)
{
    if(!users)
    {
        users = (SERVER_USERS*)malloc(sizeof(SERVER_USERS));
        show_message_error(users, "Failed to allocate space for SERVER_USERS");
    }
    
    users->user_size = user_size;
    
    if(user_size)
    {
        users->user = (SERVER_USER**)malloc(sizeof(SERVER_USER*) * user_size);
        show_message_error(users->user, "Failed to allocate space for SERVER_USER collection");
        
        for(int i = 0; i < user_size; i++)
        {
            users->user[i] = (SERVER_USER*)malloc(sizeof(SERVER_USER));
            show_message_error(users->user[i], "Failed to allocate space for SERVER_USER");
            
            users->user[i]->id = -1;
            users->user[i]->active = false;
            users->user[i]->is_creator = false;
            users->user[i]->is_ready = false;
            users->user[i]->in_game = false;
            users->user[i]->game_id = -1;
            users->user[i]->state = STATE_NONE;
            users->user[i]->nickname[0] = '\0';
            users->user[i]->display_width = 0;
            users->user[i]->display_height = 0;
            users->user[i]->scale_factor = 0;
            users->user[i]->command.dx = 0;
            users->user[i]->command.dy = 0;
            users->user[i]->command.fire = false;
        }
    }
    else
        users->user = NULL;
    
    return users;
}

void srv_free_users(SERVER_USERS* users)
{
    srv_destroy_users(users);
    free(users);
}

void srv_destroy_users(SERVER_USERS* users)
{
    for(int i = 0; i < users->user_size; i++)
        free(users->user[i]);
    
    free(users->user);
}

SERVER_USER* srv_get_free_user(SERVER_USERS* users)
{
    for(int i = 0; i < users->user_size; i++)
        if(!users->user[i]->active)
        {
            users->user[i]->id = i;
            users->user[i]->active = true;
            return users->user[i];
        }
    
    return NULL;
}

void srv_clear_user(SERVER_USER* user)
{
    user->id = -1;
    user->game_id = -1;
    user->active = false;
    user->is_ready = false;
    user->in_game = false;
    user->state = STATE_NONE;
    user->nickname[0] = '\0';
    user->display_width = 0;
    user->display_height = 0;
    user->command.dx = 0;
    user->command.dy = 0;
    user->command.fire = false;
    user->scale_factor = 0;
}
