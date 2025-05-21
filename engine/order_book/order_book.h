#pragma once

#include <coroutine/task_void.h>

#include <order_book/order_book_websocket/order_book_websocket.h>
#include <order_book/order_book_rest/order_book_rest.h>

class OrderBook
{
public:
    OrderBook(const std::string& symbol, size_t depth_level, net::io_context& ioc, EventBase* event_base);
    TaskVoid send_request_get_order_book();

private:
    std::string m_symbol;
    size_t m_depth_level;
    OrderBookWebsocket m_order_book_websocket;
    OrderBookRest m_order_book_rest;

    void OnOrderbookWs(std::string data);
    void OnOrderbookRest(std::string data);
};