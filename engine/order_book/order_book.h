#pragma once

#include <map>

#include <coroutine/task_void.h>
#include <json/json.h>
#include <order_book/order_book_websocket/order_book_websocket.h>
#include <order_book/order_book_rest/order_book_rest.h>

class OrderBook
{
public:
    OrderBook(const std::string& symbol, size_t depth_level, net::io_context& ioc, EventBase* event_base);
    TaskVoid send_request_get_full_order_book();

private:
    std::string m_symbol;
    size_t m_depth_level;
    OrderBookWebsocket m_order_book_websocket;
    OrderBookRest m_order_book_rest;

    // Bid, Ask
    std::map<double, double> m_bids;
    std::map<double, double> m_asks;

    // Use for dedupe logic
    size_t m_snapshot_last_update_id = 0;

    void OnOrderbookWs(std::string data);
    void OnOrderbookRest(std::string data);

    void apply_snapshot(Json& snapshsot);
};