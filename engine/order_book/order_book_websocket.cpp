#include <order_book/order_book_websocket.h>
#include <json/json.h>
#include <measure_time.h>

OrderBookWebsocket::OrderBookWebsocket(const std::string& symbol, net::io_context& ioc, EventBase* event_base)
    : m_symbol{symbol}, m_ioc{ioc}, m_event_base{event_base}
{
    std::string ws_path = "/ws/" + m_symbol + "@depth5@100ms";

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
            Json depth = Json::parse(buffer);

            std::cout << "depth: " << depth << std::endl;
 
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
}