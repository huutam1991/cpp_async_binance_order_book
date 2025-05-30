#pragma once

#include <https_client/https_client_async.h>
#include <coroutine/task.h>

class OrderBookRest
{
public:
    OrderBookRest(net::io_context& ioc);
    Task<std::string> get_order_book(const std::string& symbol, size_t depth);

private:
    net::io_context& m_ioc;
};