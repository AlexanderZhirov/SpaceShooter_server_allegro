#include <signal.h>

#include "common.h"
#include "lib/message.h"

SERVER_DATA* srv_create_data(SERVER_DATA* data, SERVER_PARAMETRS parametrs)
{
    if(!data)
    {
        data = (SERVER_DATA*)malloc(sizeof(SERVER_DATA));
        show_message_error(data, "Failed to allocate space for SERVER_DATA");
    }
    
    srv_create_enet(&data->enet, parametrs.port);
    puts("Enet создан!");
    srv_create_timer(&data->timer, parametrs.FPS);
    puts("Таймер загружен. Готов к запуску!");
    srv_create_event(&data->event, &data->timer);
    puts("События ждут указаний!");
    srv_create_users(&data->users, parametrs.users);
    puts("Место для игроков выделено!");
    srv_create_games(&data->games, parametrs.games);
    puts("Место для игр выделено!");
    puts("Все системы в норме! Запускаю сервер!");
    puts("--------------------------------------------------");
    
    data->done = false;
    
    srv_get_server_data(data);
    // Server Stop Event Logging
    srv_catch_signal(SIGINT, srv_stop_server);
    
    return data;
}

void srv_destroy_data(SERVER_DATA* data)
{
    srv_destroy_enet(&data->enet);
    puts("Почистил Enet!");
    srv_destroy_event(&data->event);
    puts("С событиями тоже разобрался!");
    srv_destroy_timer(&data->timer);
    puts("Кто говорил, что временем нельзя управлять? Таймер остановлен!");
    srv_destroy_users(&data->users);
    puts("Освободил выделенное место для игроков!");
    srv_destroy_games(&data->games);
    puts("Освободил выделенное место для игр!");
    free(data);
    puts("Сервер остановлен! Порядок!");
}

SERVER_DATA* srv_get_server_data(SERVER_DATA* data)
{
    static SERVER_DATA* server;
    
    if(data)
        server = data;
    
    return server;
}
