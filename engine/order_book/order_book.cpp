#include <order_book/order_book.h>
#include <json/json.h>

OrderBook::OrderBook(const std::string& symbol, size_t depth_level, net::io_context& ioc, EventBase* event_base)
    : m_symbol{symbol},
      m_depth_level{depth_level},
      m_order_book_websocket{
          symbol, 
          depth_level,
          ioc, 
          event_base, 
          [this](std::string data) { this->OnOrderbookWs(std::move(data)); }
      },
      m_order_book_rest{ioc}
{}

TaskVoid OrderBook::send_request_get_full_order_book()
{
    std::string data = co_await m_order_book_rest.get_order_book(m_symbol, m_depth_level);

    // Update to method OnOrderbookRest
    OnOrderbookRest(data);
}

void OrderBook::OnOrderbookWs(std::string data)
{
    Json order_book = Json::parse(data);
    std::cout << "order book: " << order_book << std::endl;
}

void OrderBook::OnOrderbookRest(std::string data)
{
    Json order_book = Json::parse(data);
    std::cout << "order book snapshot: " << order_book << std::endl;
}