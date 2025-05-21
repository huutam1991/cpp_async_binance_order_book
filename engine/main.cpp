#include <iostream>

#include <websocket/websocket_client_async.h>
#include <timer_new.h>
#include <external_request/https_client_async.h>
#include <ioc_pool.h>

#include <engine_constants.h>

int main(int argc, char **argv) 
{
    // Init Timer with ioc TIMER
    TimerNew::init(IOCPool::get_ioc_by_id(IOCId::TIMER));

    while (true)
    {
        /* code */
    }
    

    return EXIT_SUCCESS;
}
