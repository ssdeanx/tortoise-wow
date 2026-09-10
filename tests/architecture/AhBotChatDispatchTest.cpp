// Execute both production forwarding functions. The terminal AHBot service
// records requests, so a dummy chat handler fails even when the module builds.
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
class ChatHandler { public: bool HandleAhBotCommand(char* args); };
namespace ahbot {
class AhBot {
public:
    static bool HandleAhBotCommand(ChatHandler*, char const*);
    ChatHandler* caller = nullptr;
    std::string received;
    int calls = 0;
    bool result = true;
    bool HandleCommand(ChatHandler* handler, std::string command) {
        caller = handler; received = command; ++calls; return result;
    }
};
}
ahbot::AhBot service;
#define auctionbot service
using ahbot::AhBot;
#include "NativeAhBotModuleDispatch.inc"
#include "NativeAhBotChatDispatch.inc"
#define CHECK(x) do { if (!(x)) { std::cerr << "Failed: " #x << '\n'; std::exit(1); } } while (0)
int main() {
    ChatHandler chat;
    for (std::string command : {"reload", "rebuild", "rebuild all", "status", "status all", "item 117", "item 117 reset", "unknown", ""}) {
        std::vector<char> args(command.begin(), command.end()); args.push_back(0);
        for (bool result : {false, true}) {
            service.result = result;
            int before = service.calls;
            CHECK(chat.HandleAhBotCommand(args.data()) == result);
            CHECK(service.calls == before + 1);
            CHECK(service.caller == &chat);
            CHECK(service.received == command);
        }
    }
    CHECK(chat.HandleAhBotCommand(nullptr));
    CHECK(service.received.empty());
    std::cout << "AHBot native chat/module forwarding passed\n";
}
