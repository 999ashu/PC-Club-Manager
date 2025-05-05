#include <manager.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#define CHECK_CONDITION(condition, message)     \
if (!(condition)) {                             \
    std::cout << (message) << std::endl;        \
    return 1;                                   \
}

int main(const int argc, char* argv[]) {
    CHECK_CONDITION(argc >= 2, "Usage: " + std::string(argv[0]) + " <input_file.txt>");

    int n, cost;
    std::string tmp;

    std::ifstream file(argv[1], std::ios::in);
    CHECK_CONDITION(file.is_open(), "Can't open file: " + std::string(argv[1]));

    file >> n >> std::ws;
    CHECK_CONDITION(n > 0, "Amount of computers should be positive value: " + std::to_string(n));

    std::getline(file, tmp);
    std::istringstream iss(tmp);
    std::tm start_tm{}, end_tm{};
    CHECK_CONDITION(iss >> std::get_time(&start_tm, "%R") >> std::ws >> std::get_time(&end_tm, "%R"),
                    "Failed to parse time from: " + tmp);

    int start_min = start_tm.tm_hour * 60 + start_tm.tm_min;
    int end_min = end_tm.tm_hour * 60 + end_tm.tm_min;

    auto uptime = std::make_pair(start_min, end_min);
    CHECK_CONDITION(uptime.second - uptime.first > 0, "Incorrect input time format: " + tmp);

    file >> cost >> std::ws;
    CHECK_CONDITION(cost > 0, "Cost should be positive value: " + std::to_string(cost));

    Manager manager(n, uptime, cost);

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::istringstream lineStream(line);
        std::string clientId;
        std::tm time{};
        int id;

        CHECK_CONDITION(lineStream >> std::get_time(&time, "%R") >> id >> clientId,
                        "Failed to parse event: " + line);

        int place = -1;
        if (id == 2) {
            CHECK_CONDITION(lineStream >> place, "Missing place number for seat event: " + line);
        }

        CHECK_CONDITION(manager.processEvent(time.tm_hour * 60 + time.tm_min, id, clientId, place),
                        "Incorrect input format: " + line);
    }

    return 0;
}
