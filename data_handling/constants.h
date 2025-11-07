#pragma once
#include <limits>
#include "types.h"
using namespace std;

struct Constants { 
    static constexpr Price InvalidPrice = numeric_limits<Price>::quiet_NaN();
    static constexpr Qty InvalidQty = 0;
};
