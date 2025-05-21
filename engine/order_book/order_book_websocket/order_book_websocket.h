#pragma once

#include <memory>
#include <string>

#include <websocket/websocket_client_async.h>
#include <ioc_pool.h>

class OrderBookWebsocket
{
public:
    OrderBookWebsocket(const std::string& symbol, net::io_context& ioc, EventBase* event_base);

private:
    std::string m_symbol;
    net::io_context& m_ioc;
    EventBase* m_event_base;

    std::shared_ptr<WebsocketClientAsync> m_websocket;

    TaskVoid keep_websocket_alive();
    
};