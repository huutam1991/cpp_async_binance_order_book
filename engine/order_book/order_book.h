#pragma once

#include <order_book/order_book_websocket.h>

class OrderBook
{
public:
    OrderBook(const std::string& symbol, net::io_context& ioc, EventBase* event_base);

private:
    OrderBookWebsocket m_order_book_websocket;
};