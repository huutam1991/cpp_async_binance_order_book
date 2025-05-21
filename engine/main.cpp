#include <iostream>

#include <timer_new.h>
#include <ioc_pool.h>
#include <json/json.h>
#include <coroutine/task.h>
#include <coroutine/event_base_manager.h>

#include <engine_constants.h>
#include <order_book/order_book.h>
#include <order_book/order_book_rest.h>

Task<int> async_main()
{
    // Init Timer with ioc TIMER
    TimerNew::init(IOCPool::get_ioc_by_id(IOCId::TIMER));

    // 
    OrderBook order_book(
        "btcusdt", 
        IOCPool::get_ioc_by_id(IOCId::MARKET_DATA), 
        EventBaseManager::get_event_base_by_id(EventBaseID::MAIN_FLOW)
    );

    // OrderBookRest
    OrderBookRest order_book_rest(IOCPool::get_ioc_by_id(IOCId::MARKET_DATA), "fapi.binance.com", "443");

    // Loop to send REST request to query orderbook at every 5 seconds
    while (true) 
    {
        co_await TimerNew::sleep_for(5000);

        std::string str = co_await order_book_rest.get_order_book("btcusdt", 100);
        Json order_book_data = Json::parse(str);

        std::cout << "order_book_data: " << order_book_data << std::endl;
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
