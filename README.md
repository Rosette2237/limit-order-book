# Limit Order Book Service

Single-symbol, in-memory limit order book service in C++20 with Drogon.

## Features

- Price-time priority
- Partial fills and orders that cross several price levels
- Unique ID for every accepted order
- Thread-safe matching engine
- Exchange core has no Drogon dependency and is unit-tested without a server.

## API

- `POST /orders`: submit a limit order
- `DELETE /orders/{id}`: cancel a resting order
- `GET /book`: best 5 bid and ask levels
- `WS /marketdata`: pushes `{"trades", "bids", "asks"}` after every accepted order or cancel

## Build

Requires g++ with C++20, CMake 3.28+, and Drogon.

```bash
cmake -S . -B build
cmake --build build -j
```

## Test

```bash
./build/order_book_test
```

Test on price priority, FIFO within a price, partial fills, crossing multiple levels, cancellation, and invalid cancellation.

## Run

```bash
cd build && ./limit_order_book   # serves http://localhost:8080
```


