#include <iostream>

#include <timer_new.h>
#include <ioc_pool.h>
#include <coroutine/event_base_manager.h>

#include <engine_constants.h>
#include <order_book/order_book.h>

int main(int argc, char **argv) 
{
    // Init Timer with ioc TIMER
    TimerNew::init(IOCPool::get_ioc_by_id(IOCId::TIMER));

    OrderBook order_book(
        "btcusdt", 
        IOCPool::get_ioc_by_id(IOCId::MARKET_DATA), 
        EventBaseManager::get_event_base_by_id(EventBaseID::MARKET_DATA_EVB)
    );

    while (true)
    {
        /* code */
    }
    

    return EXIT_SUCCESS;
}
