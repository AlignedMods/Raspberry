#include "menu.hpp"
#include "core/log.hpp"
#include "core/types.hpp"
#include "game.hpp"
#include "registry.hpp"

void Execute(const std::string &str) {
    std::vector<std::string> tokens;
    std::string temp;

    u32 index = 0;

    while (index < str.size()) {
        if (str.at(index) != ' ') {
            temp += str.at(index);
        } else {
            tokens.push_back(temp);
            temp.clear();
        }

        index++;
    }

    // if there is something left over in the buffer we want to add it
    if (!temp.empty()) {
        tokens.push_back(temp);
        temp.clear();
    }

    // actual execution
    if (tokens.size() == 0) {
        Log(LogLevel::Error, "Expected command!");
        return;
    }

    if (tokens.at(0) == "switch") {
        if (tokens.size() < 2) {
            Log(LogLevel::Error, "Syntax: switch <menu>");
            return;
        } else {
            Game.SetCurrentMenu(tokens.at(1));
        }
    }

    if (tokens.at(0) == "exec-cpp") {
        if (tokens.size() < 2) {
            Log(LogLevel::Error, "Syntax: exec-cpp <function>");
            return;
        } else {
            Registry.CallFunction(tokens.at(1));
        }
    }
}
