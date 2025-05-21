#include <order_book/order_book_rest.h>

OrderBookRest::OrderBookRest(net::io_context& ioc, const std::string& host, const std::string port)
    : m_ioc{ioc}, m_host{host}, m_port{port}
{
}

Task<std::string> OrderBookRest::get_order_book(const std::string& symbol, size_t depth)
{
    std::string endpoint = "/fapi/v1/depth?symbol=" + symbol + "&limit=" + std::to_string(depth);
    auto http_client_async = std::make_shared<HttpsClientAsync>(m_ioc, m_host, m_port);
    co_return co_await http_client_async->get(endpoint);
}