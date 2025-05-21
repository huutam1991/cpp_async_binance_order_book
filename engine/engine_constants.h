#pragma once

enum IOCId
{
    TIMER,
    MARKET_DATA,
    ORDER_ENTRY, 
};

enum EventBaseID
{
    ORDERBOOK_ENGINE,
    MARKET_DATA_EVB,
    ORDER,                   // OrderManager
    ORDER_DATA_MODEL_HELPER, // Help to save Order
    GATEWAY,                 // Gateway
    STRATEGY                 // Strategy
};
