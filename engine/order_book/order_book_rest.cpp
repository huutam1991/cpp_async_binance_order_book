#include <order_book/order_book_rest.h>

OrderBookRest::OrderBookRest(net::io_context& ioc)
    : m_https_client_async{std::make_shared<HttpsClientAsync>(ioc, "fapi.binance.com", "443")}
{
}

Task<std::string> OrderBookRest::get_order_book(const std::string& symbol, size_t depth)
{
    std::string endpoint = "/fapi/v1/depth?symbol=" + symbol + "&limit=" + std::to_string(depth);
    co_return co_await m_https_client_async->get(endpoint);
}