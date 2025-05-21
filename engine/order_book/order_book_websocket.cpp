#include <order_book/order_book_websocket.h>
#include <json/json.h>
#include <measure_time.h>

OrderBookWebsocket::OrderBookWebsocket(const std::string& symbol, net::io_context& ioc, EventBase* event_base)
    : m_symbol{symbol}, m_ioc{ioc}, m_event_base{event_base}
{
    m_websocket = std::make_shared<WebsocketClientAsync>(m_ioc, m_event_base);

    m_websocket->set_callbacks(
        // on_connect
        [this, symbol, websocket = m_websocket]() -> TaskVoid
        {
            // ADD_LOG("Binance websocket depth connected");

            // Subcribe for depth
            // size_t stream_id = get_stream_id_count();
            // std::string lower_case_symbol = symbol;
            // STRING_LOWER_CASE(lower_case_symbol);
            // Json params;
            // params[0] = "btcusdt@depth5@1000ms";

            // // <symbol>@depth<levels>@100ms

            // Json subcribe;
            // subcribe["method"] = "SUBSCRIBE";
            // subcribe["params"] = params;
            // subcribe["id"] = 1;

            // // ADD_LOG("subcribe = " << subcribe);

            // websocket->send(subcribe.get_string_value());

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

    m_websocket->connect("fstream.binance.com", "443", "/ws/btcusdt@depth5@100ms");
}