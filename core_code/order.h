#pragma once
#include "../data_handling/types.h"

enum class Side : uint8_t { BUY = 0, SELL = 1 };

struct Order { // Struct of an order
    OrderID id;
    Ticker ticker;
    Price price;
    Qty qty;
    Side side;
    Timestamp ts;

    Order(OrderID id_, const Ticker& t, Price p, Qty q, Side s, Timestamp time)
        : id(id_), ticker(t), price(p), qty(q), side(s), ts(time) {}
};
