#include <fstream>
#include "json_helper.h"
#include <nlohmann/json.hpp>
#include <iostream>
using json = nlohmann::json;
void json_helper::deserialize(Table &table, const std::string& path) {
    std::ifstream file(path);
    json jsonData;
    file >> jsonData;
    table.data.deceleration = jsonData["table"]["deacceleration"];
    table.data.radius = jsonData["ball"]["radius"];
    table.width = jsonData["table"]["width"];
    table.height = jsonData["table"]["height"];
    for (const auto& ballData : jsonData["balls"]) {
        Ball ball = *(new Ball());
        ball.position.x = ballData["position"]["x"];
        ball.position.y = ballData["position"]["y"];
        ball.velocity.x = ballData["velocity"]["x"];
        ball.velocity.y = ballData["velocity"]["y"];
        table.balls.push_back(ball);
    }
}
void json_helper::serialize(const std::vector<std::vector<Position>> positions, const std::string& path) {
    json jsonData;
    for (const auto& position_vector : positions) {
        json timeData;
        for (const auto & position : position_vector){
            json ballData;
            ballData["x"] = position.x;
            ballData["y"] = position.y;
            timeData.push_back(ballData);
        }
        jsonData["positions"].push_back(timeData);
    }
    std::ofstream file(path);
    file << std::setw(4) << jsonData << std::endl;
    file.close();
}
