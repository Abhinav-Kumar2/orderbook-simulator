#include "engine.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <chrono>

using namespace std;

static uint64_t insert_us = 0;
static uint64_t match_us = 0;
static uint64_t cancel_us = 0;

static uint64_t count_insert = 0;
static uint64_t count_cancel = 0;

using clock_type = std::chrono::steady_clock;

static void processCsvFile(const string& path, MatchEngine& engine) {
    ifstream in(path);
    if (!in.is_open()) {
        cerr << "Failed to open CSV: " << path << "\n";
        return;
    }

    string line;
    OrderID nextId = 1;
    size_t lineNo = 0;

    while (getline(in, line)) {
        ++lineNo;
        if (line.empty()) continue;

        if (lineNo == 1) {
            if (line.find("BUY") != string::npos ||
                line.find("SELL") != string::npos ||
                line.find("TICKER") != string::npos) {
                continue;
            }
        }

        istringstream ss(line);
        string token;
        if (!getline(ss, token, ',')) continue;

        string cmd = token;

        if (cmd == "BUY" || cmd == "SELL") {
            string ticker, priceStr, qtyStr;
            if (!getline(ss, ticker, ',')) continue;
            if (!getline(ss, priceStr, ',')) continue;
            if (!getline(ss, qtyStr, ',')) continue;

            double price = stod(priceStr);
            uint64_t qty = stoull(qtyStr);
            Side side = (cmd == "BUY") ? Side::BUY : Side::SELL;

            Timestamp ts = static_cast<Timestamp>(
                chrono::steady_clock::now().time_since_epoch().count());

            Order o(nextId++, ticker, price, qty, side, ts);

            auto t1 = clock_type::now();
            engine.newOrder(o);
            auto t2 = clock_type::now();
            insert_us += chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
            count_insert++;

            auto t3 = clock_type::now();
            engine.match();
            auto t4 = clock_type::now();
            match_us += chrono::duration_cast<chrono::microseconds>(t4 - t3).count();
        }
        else if (cmd == "CANCEL") {
            string ticker, idStr;
            if (!getline(ss, ticker, ',')) continue;
            if (!getline(ss, idStr, ',')) continue;

            OrderID id = stoull(idStr);

            auto t1 = clock_type::now();
            engine.cancel(ticker, id);
            auto t2 = clock_type::now();
            cancel_us += chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
            count_cancel++;

            auto t3 = clock_type::now();
            engine.match();
            auto t4 = clock_type::now();
            match_us += chrono::duration_cast<chrono::microseconds>(t4 - t3).count();
        }
    }

    in.close();
}

int main(int argc, char** argv) {
    MatchEngine engine;

    if (argc > 1) {
        string csvPath = argv[1];
        cout << "Processing CSV: " << csvPath << "\n";

        auto global_start = clock_type::now();

        processCsvFile(csvPath, engine);

        auto global_end = clock_type::now();
        uint64_t total_us =
            chrono::duration_cast<chrono::microseconds>(global_end - global_start).count();

        cout << "Total Orders Inserted:  " << count_insert << "\n";
        cout << "Total Orders Cancelled: " << count_cancel << "\n\n";

        cout << "Time spent inserting:   " << insert_us  << " us\n";
        cout << "Time spent matching:    " << match_us   << " us\n";
        cout << "Time spent cancelling:  " << cancel_us  << " us\n\n";

        cout << "TOTAL processing time:  " << total_us   << " us\n";
        cout << "TOTAL processing time:  " << total_us/1000.0 << " ms\n";

        return 0;
    }

    cout << "No CSV file provided.\n";
    return 0;
}

