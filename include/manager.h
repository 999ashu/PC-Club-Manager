#pragma once

#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class Manager {
    std::unordered_map<std::string, std::pair<bool, int>> map_;
    std::queue<std::string> queue_;
    std::vector<bool> places_;

    std::pair<int, int> uptime_;
    int computers_;
    int cost_;

    int occupied_;

    static void printTime(int time);

    static void printLog(int time, int code, const std::string& payload = "");

public:
    Manager() = delete;

    Manager(int computers, std::pair<int, int> uptime, int cost);

    ~Manager();

    bool processEvent(int time, int id, const std::string& clientId, int place = -1);

    void close() const;
};