#include <iostream>

#include <timer_new.h>
#include <ioc_pool.h>
#include <json/json.h>
#include <coroutine/task.h>
#include <coroutine/event_base_manager.h>

#include <engine_constants.h>
#include <order_book/order_book.h>

Task<int> async_main()
{
    // Init Timer with ioc TIMER
    TimerNew::init(IOCPool::get_ioc_by_id(IOCId::TIMER));

    std::vector<std::unique_ptr<OrderBook>> order_book_list;

    // BTCUSDT
    order_book_list.push_back(std::make_unique<OrderBook>(
        "btcusdt", 
        1000,
        IOCPool::get_ioc_by_id(IOCId::BTCUSDT), 
        EventBaseManager::get_event_base_by_id(EventBaseID::MAIN_FLOW)
    ));

    // ETHUSDT
    order_book_list.push_back(std::make_unique<OrderBook>(
        "ethusdt", 
        1000,
        IOCPool::get_ioc_by_id(IOCId::ETHUSDT), 
        EventBaseManager::get_event_base_by_id(EventBaseID::MAIN_FLOW)
    ));

    // BNBUSDT
    order_book_list.push_back(std::make_unique<OrderBook>(
        "bnbusdt", 
        1000,
        IOCPool::get_ioc_by_id(IOCId::BNBUSDT), 
        EventBaseManager::get_event_base_by_id(EventBaseID::MAIN_FLOW)
    ));

    // Loop to send REST request to query orderbook (full) at every 5 seconds
    while (true) 
    {
        co_await TimerNew::sleep_for(5000);

        for (auto& order_book : order_book_list)
        {
            order_book->send_request_get_full_order_book();
        }
    }

    co_return 0;
}

int main(int argc, char **argv) 
{
    // Run async_main()
    auto task = async_main();
    auto async_run = task.start_running_on(EventBaseManager::get_event_base_by_id(EventBaseID::MAIN_FLOW));
    int result = async_run.get();

    return result;
}
