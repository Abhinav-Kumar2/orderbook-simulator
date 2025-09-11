#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <iostream>

class CommandHandler {
public:
    CommandHandler();
    void run();
private:
    std::unordered_map<std::string, std::function<void()>> commands;
    void registerCommands();
};
