#include <iostream>

#include <time/timer.h>
#include <ioc_pool.h>
#include <json/json.h>
#include <coroutine/task.h>
#include <coroutine/event_base_manager.h>

#include <engine_constants.h>
#include <order_book/order_book.h>

Task<int> async_main()
{
    std::vector<std::unique_ptr<OrderBook>> order_book_list;

    // BTCUSDT
    order_book_list.push_back(std::make_unique<OrderBook>(
        "btcusdt",                                                     // symbol
        10,                                                            // depth levels
        IOCPool::get_ioc_by_id(IOCId::MARKET_DATA),                        // io context (to use websocket, rest on boost, 1 io context running on 1 thread)
        EventBaseManager::get_event_base_by_id(EventBaseID::MARKET_MAKER_STRATEGY) // event base for coroutine (1 event base running on 1 thread)
    ));

    // ETHUSDT
    order_book_list.push_back(std::make_unique<OrderBook>(
        "ethusdt",                                                     // symbol
        10,                                                            // depth levels
        IOCPool::get_ioc_by_id(IOCId::MARKET_DATA),                        // io context (to use websocket, rest on boost, 1 io context running on 1 thread)
        EventBaseManager::get_event_base_by_id(EventBaseID::MARKET_MAKER_STRATEGY) // event base for coroutine (1 event base running on 1 thread)
    ));

    // BNBUSDT
    order_book_list.push_back(std::make_unique<OrderBook>(
        "bnbusdt",                                                     // symbol
        10,                                                            // depth levels
        IOCPool::get_ioc_by_id(IOCId::MARKET_DATA),                        // io context (to use websocket, rest on boost, 1 io context running on 1 thread)
        EventBaseManager::get_event_base_by_id(EventBaseID::MARKET_MAKER_STRATEGY) // event base for coroutine (1 event base running on 1 thread)
    ));

    // Loop to send REST request to query orderbook (full) at every 5 seconds, if the orderbook is not synced yet
    while (true)
    {
        for (auto& order_book : order_book_list)
        {
            if (order_book->is_not_synced())
            {
                co_await order_book->send_request_get_full_order_book();
            }
        }

        co_await Timer::sleep_for(5000);
    }

    co_return 0;
}

int main(int argc, char **argv)
{
    // Run async_main()
    auto task = async_main();
    auto async_run = task.start_running_on(EventBaseManager::get_event_base_by_id(EventBaseID::MARKET_MAKER_STRATEGY));
    int result = async_run.get();

    return result;
}
