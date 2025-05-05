#include "manager.h"

#include <iomanip>
#include <iostream>

Manager::Manager(const int computers, std::pair<int, int> uptime, const int cost)
    : uptime_(std::move(uptime)), computers_(computers), cost_(cost), occupied_(0), users_(1) {
    places_.assign(computers_ + 1, 0);
    startTime_.assign(computers_ + 1, -1);
    totalTime_.assign(computers_ + 1, 0);
    revenue_.assign(computers_ + 1, 0);
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

void Manager::endSession(const int place, const int time) {
    int start = startTime_[place];
    if (start >= 0 && time > start) {
        const int duration = time - start;
        totalTime_[place] += duration;
        const int hours = (duration + 59) / 60;
        revenue_[place] += hours * cost_;
    }
    startTime_[place] = -1;
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
            const auto user = id2uid_.find(clientId);
            if (user != id2uid_.end()) {
                const int uid = user->second;
                if (clients_[uid - 1].active) {
                    printLog(time, 13, "YouShallNotPass");
                    return true;
                }
                clients_[uid - 1].active = true;
                clients_[uid - 1].place = -1;
            } else {
                const int uid = users_++;
                id2uid_[clientId] = uid;
                clients_.push_back({true, -1});
                uid2id_.push_back(clientId);
            }

            break;
        }

        case 2: {
            const auto user = id2uid_.find(clientId);

            if (user == id2uid_.end()) {
                printLog(time, 13, "ClientUnknown");
                return true;
            }
            if (place < 1 || place > computers_) {
                return false;
            }
            if (places_[place] != 0) {
                printLog(time, 13, "PlaceIsBusy");
                return true;
            }

            const int uid = user->second;
            const int old = clients_[uid - 1].place;
            if (old != -1) {
                endSession(old, time);
                places_[old] = 0;
            } else {
                ++occupied_;
            }

            clients_[uid - 1].place = place;
            places_[place] = uid;
            startTime_[place] = time;
            break;
        }

        case 3: {
            const auto user = id2uid_.find(clientId);
            if (user == id2uid_.end()) {
                printLog(time, 13, "ClientUnknown");
                return true;
            }

            const int uid = user->second;
            const int old = clients_[uid - 1].place;
            if (old != -1) {
                endSession(old, time);
                --occupied_;
                places_[old] = 0;
                clients_[uid - 1].place = -1;
            }
            if (queue_.size() > computers_) {
                printLog(time, 11, clientId);
            }
            if (occupied_ < computers_) {
                printLog(time, 13, "ICanWaitNoLonger!");
                return true;
            }

            queue_.push(uid);
            break;
        }

        case 4: {
            const auto user = id2uid_.find(clientId);
            if (user == id2uid_.end()) {
                printLog(time, 13, "ClientUnknown");
                return true;
            }

            const int uid = user->second;
            const int old = clients_[uid - 1].place;
            if (clients_[uid - 1].active && old != -1) {
                endSession(old, time);
                --occupied_;
                places_[old] = 0;
            }
            clients_[uid - 1] = {false, -1};

            if (old != -1 && !queue_.empty()) {
                int nUid = 0;
                while (!queue_.empty()) {
                    nUid = queue_.front();
                    queue_.pop();
                    if (clients_[nUid - 1].active) {
                        clients_[nUid - 1].place = old;
                        places_[old] = nUid;
                        ++occupied_;
                        startTime_[old] = time;
                        const std::string& nid = uid2id_[nUid - 1];
                        printLog(time, 12, nid + " " + std::to_string(old));
                        break;
                    }
                }
            }

            break;
        }

        default: {
            return false;
        }
    }

    return true;
}

void Manager::close() {
    printTime(uptime_.second);
    std::cout << "\n";
    for (int i = 1; i <= computers_; ++i) {
        if (startTime_[i] >= 0) {
            endSession(i, uptime_.second);
        }
        std::cout << i << " " << revenue_[i] << " ";
        printTime(totalTime_[i]);
        std::cout << '\n';
    }
}
