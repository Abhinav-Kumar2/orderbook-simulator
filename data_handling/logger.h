#pragma once
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime> 

using namespace std;

void log(const string& msg) { // Log messages with timestamp
    using namespace chrono;
    auto now = system_clock::now();
    auto t = system_clock::to_time_t(now);
    auto us = duration_cast<microseconds>(now.time_since_epoch()).count() % 1'000'000;
    cout << put_time(localtime(&t), "%H:%M:%S")
              << "." << setfill('0') << setw(6) << us
              << " | " << msg << endl;
}