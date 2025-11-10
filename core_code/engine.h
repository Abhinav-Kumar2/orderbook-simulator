#pragma once
#include "orderbook.h"
#include "../data_handling/logger.h"
#include "../data_handling/types.h"
#include <unordered_map>
using namespace std;

class MatchEngine { // Class for order placing, cancellation and execution
public:
    void newOrder(const Order& o);
    bool cancel(const Ticker& t, OrderID id);
    bool cancel(OrderID id);
    void match();

private:
    unordered_map<Ticker, OrderBook> orderbooks; // Map of ticker to its order book
    unordered_map<OrderID, Ticker> orderIDtoTicker; // Map of order ID to its ticker
};