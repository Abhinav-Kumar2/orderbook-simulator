#include "DataStore.hpp"
#include <sqlite3.h>
#include <stdexcept>
#include <iostream>

DataStore::DataStore(const std::string& db_path) {
    if (sqlite3_open(db_path.c_str(), reinterpret_cast<sqlite3**>(&db)) != SQLITE_OK)
        throw std::runtime_error("Failed to open DB");
}

void DataStore::initSchema() {
    const char* sql = R"(
    CREATE TABLE IF NOT EXISTS historical_prices (
        symbol TEXT,
        datetime TEXT,
        open REAL,
        high REAL,
        low REAL,
        close REAL,
        volume INTEGER,
        PRIMARY KEY (symbol, datetime)
    );
    )";
    char* errMsg = nullptr;
    if (sqlite3_exec((sqlite3*)db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string err = errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error("SQL error: " + err);
    }
}

void DataStore::storeCandle(const std::string& symbol, const Candle& c) {
    const char* sql = "INSERT OR REPLACE INTO historical_prices VALUES (?,?,?,?,?,?,?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2((sqlite3*)db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, symbol.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, c.datetime.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, c.open);
    sqlite3_bind_double(stmt, 4, c.high);
    sqlite3_bind_double(stmt, 5, c.low);
    sqlite3_bind_double(stmt, 6, c.close);
    sqlite3_bind_int64(stmt, 7, c.volume);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

std::vector<Candle> DataStore::fetchCandles(const std::string& symbol,
                                            const std::string& start,
                                            const std::string& end) {
    const char* sql = "SELECT datetime, open, high, low, close, volume "
                      "FROM historical_prices WHERE symbol=? AND datetime>=? AND datetime<=?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2((sqlite3*)db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, symbol.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, start.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, end.c_str(), -1, SQLITE_STATIC);

    std::vector<Candle> candles;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Candle c{
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
            sqlite3_column_double(stmt, 1),
            sqlite3_column_double(stmt, 2),
            sqlite3_column_double(stmt, 3),
            sqlite3_column_double(stmt, 4),
            sqlite3_column_int64(stmt, 5)
        };
        candles.push_back(c);
    }
    sqlite3_finalize(stmt);
    return candles;
}
