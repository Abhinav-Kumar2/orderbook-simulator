#pragma once
#include <string>
#include <vector>

struct Candle {
    std::string datetime;
    double open, high, low, close;
    long long volume;
};

class DataStore {
public:
    explicit DataStore(const std::string& db_path = "market.db");
    void initSchema();
    void storeCandle(const std::string& symbol, const Candle& c);
    std::vector<Candle> fetchCandles(const std::string& symbol,
                                     const std::string& start,
                                     const std::string& end);
private:
    void* db; // sqlite3*
};
