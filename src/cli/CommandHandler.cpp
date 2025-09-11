#include "CommandHandler.hpp"
#include <cstdlib>

CommandHandler::CommandHandler() { registerCommands(); }

void CommandHandler::registerCommands() {
    commands["help"] = []() { std::cout << "Commands: help, quit, load-data\n"; };
    commands["quit"] = []() { std::cout << "Goodbye!\n"; std::exit(0); };
    commands["load-data"] = []() { std::cout << "Stub: load data\n"; };
}

void CommandHandler::run() {
    std::string input;
    std::cout << "Algorithmic Trading Engine CLI\n";
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (commands.count(input)) commands[input]();
        else std::cout << "Unknown command\n";
    }
}
