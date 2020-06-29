#include "enet.h"
#include "message.h"

SERVER_ENET* srv_create_enet(SERVER_ENET* enet, int port)
{
    if(!enet)
    {
        enet = (SERVER_ENET*)malloc(sizeof(SERVER_ENET));
        show_message_error(enet, "Failed to allocate space for SERVER_ENET");
    }
    
    enet->address.host = ENET_HOST_ANY;
    enet->address.port = port;
    
    enet->server = enet_host_create(&enet->address, 32, 2, 0, 0);
    show_message_error(enet->server, "Failed to enet host create");

    return enet;
}

void srv_free_enet(SERVER_ENET* enet)
{
    srv_destroy_enet(enet);
    free(enet);
}

void srv_destroy_enet(SERVER_ENET* enet)
{
    enet_host_destroy(enet->server);
    enet_deinitialize();
}

bool srv_check_init_enet(int result)
{
    return result == 0;
}
