#include "games.h"
#include "../lib/message.h"

SERVER_GAMES* srv_create_games(SERVER_GAMES* games, int game_size)
{
    if(!games)
    {
        games = (SERVER_GAMES*)malloc(sizeof(SERVER_GAMES));
        show_message_error(games, "Failed to allocate space for SERVER_GAMES");
    }
    
    games->game_size = game_size;
    
    if(game_size)
    {
        games->game = (SERVER_GAME**)malloc(sizeof(SERVER_GAME*) * game_size);
        show_message_error(games->game, "Failed to allocate space for SERVER_GAME collection");
        
        for(int i = 0; i < game_size; i++)
        {
            games->game[i] = (SERVER_GAME*)malloc(sizeof(SERVER_GAME));
            show_message_error(games->game[i], "Failed to allocate space for SERVER_USER");
            
            games->game[i]->user = (SERVER_USER**)malloc(sizeof(SERVER_USER*) * 3);
            show_message_error(games->game[i]->user, "Failed to allocate space for SERVER_USER collection");
            
            games->game[i]->active = false;
            games->game[i]->game_started = false;
            games->game[i]->count_connected = 0;
            games->game[i]->user_size = 3;
            games->game[i]->game_name[0] = '\0';
            games->game[i]->display_width = 0;
            games->game[i]->display_height = 0;
            games->game[i]->scale_factor = 0;
            
            for(int j = 0; j < 3; j++)
                games->game[i]->user[j] = NULL;
        }
    }
    else
        games->game = NULL;
    
    return games;
}

void srv_free_games(SERVER_GAMES* games)
{
    srv_destroy_games(games);
    free(games);
}

void srv_destroy_games(SERVER_GAMES* games)
{
    for(int i = 0; i < games->game_size; i++)
    {
        free(games->game[i]->user);
        free(games->game[i]);
    }
    
    free(games->game);
}

// return pointer
SERVER_GAME* srv_get_free_game(SERVER_GAMES* games)
{
    for(int i = 0; i < games->game_size; i++)
        if(!games->game[i]->active)
        {
            games->game[i]->active = true;
            games->game[i]->id = i;
            return games->game[i];
        }
    
    return NULL;
}

void srv_clear_games(SERVER_GAME* game)
{
    for(int i = 0; i < game->user_size; i++)
        game->user[i] = NULL;
    
    game->active = false;
    game->game_started = false;
    game->count_connected = 0;
    game->game_name[0] = '\0';
    game->display_width = 0;
    game->display_height = 0;
    game->scale_factor = 0;
}
