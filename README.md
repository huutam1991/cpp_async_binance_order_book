# cpp_async_binance_order_book

This project implements a low-latency asynchronous order book synchronizer using WebSocket and REST for Binance Futures.
I use my own custom-built **coroutine model** and **json class** in this project:
- [`coroutine`](core/coroutine): lightweight, simple, and extremely fast — designed specifically for high-performance asynchronous tasks.
- [`json`](core/json) while not as fast as libraries like RapidJSON, nlohmann, simdjson, ... but I deliberately used it in this project (together with my coroutine model) to demonstrate that I am capable of building complex features from scratch

## Assignment 1a Requirements & How They Are Fulfilled

### 1. WebSocket Subscription (Asynchronous)
- **Implemented in:** [`order_book_websocket.cpp/`](engine/order_book/order_book_websocket/order_book_websocket.cpp) (`engine`), [`websocket_client_async.cpp`](core/websocket/websocket_client_async.cpp) (`core`)
- **Library used:** Boost.Asio + Boost.Beast (low-level, non-blocking).
- **Details:** Each symbol connects to Binance's partial depth WebSocket stream using its own I/O thread: [`main.cpp`](https://github.com/huutam1991/cpp_async_binance_order_book/blob/d9962cb1f6ea812caceb66eb965536f23835f3eb/engine/main.cpp#L19-L41)
- **Symbols:** BTCUSDT, ETHUSDT, BNBUSDT.

### 2. REST Polling (Asynchronous)
- **Implemented in:** [`order_book_rest.cpp`](engine/order_book/order_book_rest/order_book_rest.cpp) (`engine`), [`https_client_async.cpp`](core/https_client/https_client_async.cpp) (`core`)
- **Logic:** Periodic (5s) polling of full order book via REST when local order book is not yet synced: [`main.cpp`](https://github.com/huutam1991/cpp_async_binance_order_book/blob/d9962cb1f6ea812caceb66eb965536f23835f3eb/engine/main.cpp#L43-L55)
- **Asynchronous:** Custom coroutine framework + Boost.Asio.

### 3. Multiple Sessions & Efficient Thread Management
- **Implemented in:** [`ioc_pool.h`](core/ioc_pool.h) (`core`)
- **Design:** One `io_context` thread per symbol, handling both REST and WebSocket.
- **Benefit:** Clean horizontal scaling; thread-safe and parallel across symbols.

### 4. Queue/Buffer & Deduplication Mechanism
- **Deduplication:**: Optional hash-based [`dedupe_checker.h`](core/dedupe_checker.h) (`core`) for message-based deduplication.
- **Buffer:** Order book maintained in memory with map-based structures.

### 5. CPU Efficiency
- **Horizontal split:** Each symbol's I/O context runs on a dedicated thread.
- **Coroutines:** Avoids busy-waiting and blocking, all of handling logic run on 1 thread with `EventBaseID` is: [`EventBaseID::MAIN_FLOW`](https://github.com/huutam1991/cpp_async_binance_order_book/blob/d9962cb1f6ea812caceb66eb965536f23835f3eb/engine/main.cpp#L64)
- **Timer:** Non-blocking scheduling using [`sleep_for()`](https://github.com/huutam1991/cpp_async_binance_order_book/blob/d9962cb1f6ea812caceb66eb965536f23835f3eb/engine/main.cpp#L54) without spinning. 

### 6. Required Callback Names
- [`OnOrderbookWs(std::string data)`](https://github.com/huutam1991/cpp_async_binance_order_book/blob/d9962cb1f6ea812caceb66eb965536f23835f3eb/engine/order_book/order_book.cpp#L33) 
- [`OnOrderbookRest(std::string data)`](https://github.com/huutam1991/cpp_async_binance_order_book/blob/d9962cb1f6ea812caceb66eb965536f23835f3eb/engine/order_book/order_book.cpp#L116) 

Both functions are implemented in `order_book.cpp`, with deduplication and state update logic built-in.

---

## Build & Run Instructions

### Prerequisites
Run the setup script to install dependencies: `cmake`, `openssl`, `boost`

```bash
./install_library.sh
```

### Build source code

```bash
./build_bash.sh
```

### Run 

```bash
./run_bash.sh
```

## Assignment 1b
### Diagram
Each symbol runs on its own dedicated `Boost I/O context` thread for both `WebSocket` and `REST` operations.
However, the data returned is handled sequentially on a single thread (`EventBaseID::MAIN_FLOW`) using the `coroutine system`, so there is no blocking.

```
Y-axis: CPU thread per symbol
X-axis: Time ---->

CPU Thread 1 - IO context (BTCUSDT):
    ┌──────────────┬────────────┬────────────┬────────────┐
    │ WS read task │ REST poll  │ WS read    │ REST poll  │
    └──────────────┴────────────┴────────────┴────────────┘

CPU Thread 2 - IO context (ETHUSDT):
    ┌──────────────┬────────────┬────────────┐
    │ WS read task │ WS read    │ REST poll  │
    └──────────────┴────────────┴────────────┘

CPU Thread 3 - IO context (BNBUSDT):
    ┌──────────────┬────────────┬────────────┐
    │ REST poll    │ WS read    │ REST poll  │
    └──────────────┴────────────┴────────────┘

CPU Thread 4 - coroutine (EventBaseID::MAIN_FLOW):
    ┌─────────────────────────┬───────────────────────────┬─────────────────────────┐
    │ OnOrderbookWs (BTCUSDT) │ OnOrderbookRest (ETHUSDT) │ OnOrderbookWs (BNBUSDT) │
    └─────────────────────────┴───────────────────────────┴─────────────────────────┘
```
