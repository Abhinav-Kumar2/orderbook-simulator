#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
using namespace std;

int main() {
    unordered_map<string, function<void()>>commands{
        {"help", [](){cout<<"Available commands: help, quit\n";}},
        {"quit", [](){cout<<"Goodbye!\n"; exit(0);}}
    };

    cout<<"Algorithmic Trading Engine CLI Iteration 1\n";
    string input;
    while(true){
        cout<<"> ";
        getline(cin, input);
        if(commands.count(input)){
            commands[input]();
        }else{
            cout<<"Unknown command\n";
        }
    }
}
