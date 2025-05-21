#include <order_book/order_book.h>
#include <json/json.h>

OrderBook::OrderBook(const std::string& symbol, net::io_context& ioc, EventBase* event_base)
    : m_order_book_websocket(
        symbol, 
        ioc, 
        event_base, 
        [this](std::string msg) { this->OnOrderbookWs(std::move(msg)); }
    )
{}

void OrderBook::OnOrderbookWs(std::string data)
{
    Json order_book = Json::parse(data);
    std::cout << "order book: " << order_book << std::endl;
}