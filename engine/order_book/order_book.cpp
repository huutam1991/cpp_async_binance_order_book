#include <order_book/order_book.h>

OrderBook::OrderBook(const std::string& symbol, net::io_context& ioc, EventBase* event_base)
    : m_order_book_websocket(symbol, ioc, event_base)
{}