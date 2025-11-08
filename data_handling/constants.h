#pragma once
#include <limits>
#include "types.h"
using namespace std;

struct Constants { 
    static constexpr Price InvalidPrice = numeric_limits<Price>::quiet_NaN();
    static constexpr Qty InvalidQty = 0;
};

struct Trade { // Struct of a trade
    OrderID buyId = 0;
    OrderID sellId = 0;
    Ticker ticker;
    Price price = 0.0;
    Qty qty = 0;
    Timestamp ts = 0;
};