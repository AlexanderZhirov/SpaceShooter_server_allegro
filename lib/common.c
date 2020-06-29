#include <allegro5/allegro.h>

#include "common.h"
#include "message.h"

void srv_load_resources()
{
    show_message_error(al_init(), "Failed to initialize Allegro");
    show_message_error(srv_check_init_enet(enet_initialize()), "Failed to enet initialize");
}
