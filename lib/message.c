#include "message.h"

void show_message_error(bool result, const char* message)
{
    if(!result)
    {
        printf("%s\n", message);
        exit(EXIT_FAILURE);
    }
}
