#include <catch2/catch_all.hpp>
#include "DataStore.hpp"

TEST_CASE("DB schema creation works") {
    DataStore db(":memory:");
    REQUIRE_NOTHROW(db.initSchema());
}

TEST_CASE("Insert and fetch candles") {
    DataStore db(":memory:");
    db.initSchema();
    Candle c{"2020-01-01", 100, 110, 90, 105, 1000};
    db.storeCandle("AAPL", c);

    auto candles = db.fetchCandles("AAPL", "2020-01-01", "2020-01-01");
    REQUIRE(candles.size() == 1);
    REQUIRE(candles[0].close == 105);
}
