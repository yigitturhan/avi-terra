#ifndef UNTITLED_JSON_HELPER_H
#define UNTITLED_JSON_HELPER_H
#include "board.h"
#include <nlohmann/json.hpp>
class json_helper {
public:
    static void deserialize(Table &table, const std::string& path);
    static void serialize(const std::vector<std::vector<Position>> positions, const std::string& path);
};

#endif
