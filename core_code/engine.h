#pragma once
#include "orderbook.h"
#include "../data_handling/logger.h"
#include "../data_handling/types.h"
#include <unordered_map>
using namespace std;

class MatchEngine {
public:
    void onNewOrder(const Order& o);
    void onCancel(const Ticker& t, OrderID id);
    bool onCancel(OrderID id);
    void runMatching();

private:
    unordered_map<Ticker, OrderBook> books;
};
