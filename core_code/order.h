// #pragma once
// #include "../data_handling/types.h"

// enum class Side : uint8_t { BUY = 0, SELL = 1 };

// struct Order {
//     OrderID id;
//     Ticker ticker;
//     Price price;
//     Qty qty;
//     Side side;
//     Timestamp ts;

//     Order(OrderID id_, const Ticker& t, Price p, Qty q, Side s, Timestamp time)
//         : id(id_), ticker(t), price(p), qty(q), side(s), ts(time) {}
// };

#pragma once
#include "../data_handling/types.h"

// Order side: Buy or Sell
enum class Side : uint8_t { BUY = 0, SELL = 1 };

// Order type: Limit, Market, Stop
enum class OrderType : uint8_t { LIMIT = 0, MARKET = 1, STOP = 2 };

// Basic order structure
struct Order {
    OrderID id;
    Ticker ticker;
    Price price;       // For LIMIT orders. For MARKET orders, price may be ignored.
    Qty qty;
    Side side;
    OrderType type;
    Price stopPrice;   // For STOP orders: the trigger price
    Timestamp ts;

    Order() = default;

    // Constructor for typical orders
    Order(OrderID id_,
          const Ticker& t,
          Price p,
          Qty q,
          Side s,
          Timestamp time,
          OrderType type_ = OrderType::LIMIT,
          Price stopP = 0.0)
        : id(id_), ticker(t), price(p), qty(q),
          side(s), type(type_), stopPrice(stopP), ts(time) {}
};
