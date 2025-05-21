#include <iostream>

#include <timer_new.h>
#include <ioc_pool.h>
#include <coroutine/task.h>
#include <coroutine/event_base_manager.h>

#include <engine_constants.h>
#include <order_book/order_book.h>

Task<int> async_main()
{
    // Init Timer with ioc TIMER
    TimerNew::init(IOCPool::get_ioc_by_id(IOCId::TIMER));

    // 
    OrderBook order_book(
        "btcusdt", 
        IOCPool::get_ioc_by_id(IOCId::MARKET_DATA), 
        EventBaseManager::get_event_base_by_id(EventBaseID::MAIN_THREAD)
    );

    while (true) 
    {

    }

    co_return 0;
}

int main(int argc, char **argv) 
{
    // Run async_main()
    auto task = async_main();
    auto async_run = task.start_running_on(EventBaseManager::get_event_base_by_id(EventBaseID::MAIN_THREAD));
    int result = async_run.get();

    return result;
}
