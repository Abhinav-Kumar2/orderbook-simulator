#include "engine.h"

void MatchEngine::newOrder(const Order& order) { // Add new order to ticker's order book
    orderbooks[order.ticker].addOrder(order);
    orderIDtoTicker[order.id] = order.ticker;
}

bool MatchEngine::cancel(const Ticker& ticker, OrderID id) { // Remove order from ticker's order book
    auto i = orderbooks.find(ticker);
    if (i != orderbooks.end()) {
        if (i->second.cancelOrder(id)) {
            orderIDtoTicker.erase(id);
            return true;
        }
    }
    return false;
}

bool MatchEngine::cancel(OrderID id) { // Remove order using just order ID
    auto i = orderIDtoTicker.find(id);
    if (i != orderIDtoTicker.end()) {
        const Ticker& t = i->second;
        auto bit = orderbooks.find(t);
        if (bit != orderbooks.end()) {
            bool ok = bit->second.cancelOrder(id);
            if (ok) orderIDtoTicker.erase(i);
            return ok;
        }
    }

    for (auto& ob : orderbooks) { // Fallback: search all order books
        if (ob.second.cancelOrder(id)) {
            orderIDtoTicker.erase(id);
            return true;
        }
    }
    return false;
}

void MatchEngine::match() { // Match and execute orders
    for (auto& ob : orderbooks) {
        ob.second.match();
    }
}