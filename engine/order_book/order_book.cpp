#include <order_book/order_book.h>

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

    co_return;
}

void OrderBook::OnOrderbookWs(std::string data)
{
    Json order_book = Json::parse(data);
    // std::cout << "order book: " << order_book << std::endl;
}

void OrderBook::OnOrderbookRest(std::string data)
{
    Json snapshot = Json::parse(data);
    std::cout << "order book snapshot: " << snapshot << std::endl;

    uint64_t snapshot_id = snapshot["lastUpdateId"];

    // Check dedupe m_last_update_id
    if (m_snapshot_last_update_id != snapshot_id)
    {
        m_snapshot_last_update_id = snapshot_id;
        apply_snapshot(snapshot); 
    }
}

void OrderBook::apply_snapshot(Json& snapshsot)
{
    // Update Ask
    m_asks.clear();
    snapshsot["asks"].for_each([this](Json& level)
    {
        double price = std::stod((std::string)level[0]);
        double quantity = std::stod((std::string)level[1]);
        m_asks.insert(std::make_pair(price, quantity));
    });
    
    // Update Bids
    m_bids.clear();
    snapshsot["bids"].for_each([this](Json& level)
    {
        double price = std::stod((std::string)level[0]);
        double quantity = std::stod((std::string)level[1]);
        m_bids.insert(std::make_pair(price, quantity));
    });

    std::cout << "asks: ";
    for (auto& [price, quantity] : m_asks)
    {
        std::cout << "[" << price << " - " << quantity << "], ";
    }
    std::cout << std::endl;

    std::cout << "bids: ";
    for (auto& [price, quantity] : m_bids)
    {
        std::cout << "[" << price << " - " << quantity << "], ";
    }
    std::cout << std::endl;
}