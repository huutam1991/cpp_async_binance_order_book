#include <order_book/order_book_websocket/order_book_websocket.h>
#include <json/json.h>
#include <timer.h>
#include <measure_time.h>

OrderBookWebsocket::OrderBookWebsocket(const std::string& symbol, size_t depth_level, net::io_context& ioc, EventBase* event_base, std::function<void(std::string)> on_order_book_ws)
    : m_symbol{symbol}, m_depth_level{depth_level}, m_ioc{ioc}, m_event_base{event_base}, m_on_order_book_ws{on_order_book_ws}
{
    std::string ws_path = "/ws/" + m_symbol + "@depth" + std::to_string(m_depth_level);

    m_websocket = std::make_shared<WebsocketClientAsync>(m_ioc, m_event_base);
    m_websocket->set_callbacks(
        // on_connect
        [this, ws_path]() -> TaskVoid
        {
            std::cout << "Websocket [ws_path] is connected: " << std::endl;
            co_return;
        },
        // on_message
        [this, symbol](std::string buffer) -> TaskVoid
        {
            MeasureTime t("depth handle", MeasureUnit::MICROSECOND);
            m_on_order_book_ws(std::move(buffer));
 
            co_return;
        },
        // on_disconnect
        [this, symbol]() -> TaskVoid
        {
            // Re-start
            // ADD_LOG("Disconnect, re-start BinanceMarketData");
            // this->start_websocket(symbol);

            co_return;
        },
        // on_close
        []() -> TaskVoid
        {
            // ADD_LOG("BinanceMarketData close");
            co_return;
        }
    );

    m_websocket->connect("fstream.binance.com", "443", ws_path);

    // Start task to keep websocket alive
    keep_websocket_alive().start_running_on(m_event_base);
}

TaskVoid OrderBookWebsocket::keep_websocket_alive()
{
    // Send ping at every 30 second to keep websocket alive
    while (true)
    {
        co_await Timer::sleep_for(30000);
        m_websocket->send_ping();
    }
    
}