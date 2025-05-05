#include "manager.h"

#include <iostream>
#include <iomanip>

Manager::Manager(const int computers, std::pair<int, int> uptime, const int cost)
    : uptime_(std::move(uptime)), computers_(computers), cost_(cost), occupied_(0) {
    places_.reserve(computers_ + 1);
    printTime(uptime_.first);
    std::cout << std::endl;
}

Manager::~Manager() {
    close();
}

void Manager::printTime(const int time) {
    std::cout
        << std::setfill('0') << std::setw(2) << (time / 60) << ":"
        << std::setfill('0') << std::setw(2) << (time % 60);
}

void Manager::printLog(const int time, const int code, const std::string& payload) {
    printTime(time);
    std::cout << ' ' << code;
    if (!payload.empty()) {
        std::cout << ' ' << payload;
    }
    std::cout << std::endl;
}

bool Manager::processEvent(const int time, const int id, const std::string& clientId, const int place) {
    printLog(time, id, id == 2 ? clientId + " " + std::to_string(place) : clientId);

    if (time < uptime_.first || time > uptime_.second) {
        if (id == 1) {
            printLog(time, 13, "NotOpenYet");
            return true;
        }
        return false;
    }

    switch (id) {
        case 1: {
            if (map_[clientId].first) {
                printLog(time, 13, "YouShallNotPass");
                return true;
            }
            map_[clientId] = {true, -1};
            break;
        }

        case 2: {
            if (!map_.contains(clientId)) {
                printLog(time, 13, "ClientUnknown");
                return true;
            }

            if (place < 1 || place > computers_) {
                return false;
            }

            if (places_[place]) {
                printLog(time, 13, "PlaceIsBusy");
                return true;
            }

            places_[map_[clientId].second] = false;
            map_[clientId].second = place;
            places_[place] = true;
            ++occupied_;
            break;
        }

        case 3: {
            if (!map_.contains(clientId)) {
                printLog(time, 13, "ClientUnknown");
                return true;
            }

            if (map_[clientId].second != -1) {
                --occupied_;
                places_[map_[clientId].second] = false;
                map_[clientId].second = -1;
            }

            if (queue_.size() > computers_) {
                printLog(time, 11, clientId);
            }

            if (occupied_ < computers_) {
                printLog(time, 13, "ICanWaitNoLonger!");
                queue_.push(clientId);
                return true;
            }

            break;
        }

        case 4: {
            if (!map_.contains(clientId)) {
                printLog(time, 13, "ClientUnknown");
                return true;
            }

            if (map_[clientId].first != 0) {
                --occupied_;
                places_[map_[clientId].second] = false;
            }

            printLog(time, 11, clientId);

            if (!queue_.empty()) {
                std::string client = queue_.front();
                while (!map_[client].first) {
                    queue_.pop();
                    client = queue_.front();
                }
                map_[client].second = map_[clientId].second;
                map_[clientId].second = -1;
                places_[map_[clientId].second] = true;
                ++occupied_;
                printLog(time, 12, client);
            }

            break;
        }

        default: {
            return false;
        }
    }

    return true;
}

void Manager::close() const {
    printTime(uptime_.second);
    std::cout << std::endl;
}