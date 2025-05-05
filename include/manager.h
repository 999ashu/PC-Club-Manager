#pragma once

#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

class Manager {
    struct ClientInfo {
        bool active;
        int place;
    };

    std::unordered_map<std::string, int> id2uid_;
    std::vector<std::string> uid2id_;
    std::vector<ClientInfo> clients_;

    std::vector<int> places_, startTime_, totalTime_, revenue_;
    std::queue<int> queue_;

    std::pair<int, int> uptime_;
    int computers_, cost_, occupied_, users_;


    static void printTime(int time);

    static void printLog(int time, int code, const std::string& payload = "");

    void endSession(int place, int time);

public:
    Manager() = delete;

    Manager(int computers, std::pair<int, int> uptime, int cost);

    ~Manager();


    bool processEvent(int time, int id, const std::string& clientId, int place = -1);

    void close();
};
