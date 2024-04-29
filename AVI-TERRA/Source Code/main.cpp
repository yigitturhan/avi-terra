#include <iostream>
#include <thread>
#include <chrono>
#include "helper.h"
#include "json_helper.h"
#include <SFML/Graphics.hpp>
#include <fstream>
void check_collisions(Table &table){
    for (int i = 0; i < table.balls.size(); i++){
        for (int j = i+1; j < table.balls.size(); j++){
            helper::adjust_velocity_after_collision(table.balls[i],table.balls[j], table);
        }
    }
}
std::chrono::microseconds calculate_sleeping_time(double timestep, double dec){
    std::chrono::microseconds timestep_microseconds(static_cast<long long>(timestep));
    std::chrono::microseconds dec_time(static_cast<long long>(dec));
    auto ratio = static_cast<double>(dec_time.count()) / static_cast<double>(timestep_microseconds.count());
    std::chrono::microseconds time_between(static_cast<long long>(ratio));
    return time_between;
}
std::vector<int> get_snapshot_times(const std::string &path){
    std::vector<int> snapshot_times;
    std::string line;
    std::ifstream file(path);
    while (getline(file, line)) {
        int time = atoi(line.c_str());
        snapshot_times.push_back(time); // Add integer to vector
    }
    file.close();
    return snapshot_times;
}
int main() {
    std::string input_path = "../initial-state.json"; //path of initial-state.json
    std::string output_path = "../output.json"; //path of output.json
    std::string snapshot_path = "../snapshot-times.txt"; //path of snapshot-times.txt

    Table table = *(new Table());
    json_helper::deserialize(table, input_path); //this function gets the path then fills the table object
    std::vector<int> snapshot_times = get_snapshot_times(snapshot_path);
    std::vector<std::vector<Position>> positions;
    double time_counter = 0;
    int index = 0, last_index = snapshot_times.size()-1;
    double timestep = 0.0005; //a value to calculate smallest possible step
    bool printed = false;
    std::chrono::microseconds sleeping_time = calculate_sleeping_time(timestep,table.data.deceleration);

    sf::RenderWindow window(sf::VideoMode(1000, 500), "AVITERRA");
    sf::RectangleShape rectangle(sf::Vector2f(table.width*5, table.height*5));
    rectangle.setFillColor(sf::Color::White);
    rectangle.setPosition(0, 0);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(rectangle);
        if(index <= last_index){
            if (time_counter - snapshot_times[index] < timestep && time_counter - snapshot_times[index] >= 0){
                std::vector<Position> temp;
                for(auto & ball : table.balls) temp.push_back(ball.position);
                positions.push_back(temp);
                index++;
            }
        }
        else{
            if(!printed){
                printed = true;
                json_helper::serialize(positions,output_path);
            }
        }
        for(int i = 0; i < table.balls.size(); i++){ //setting different colors on different balls
            sf::CircleShape circle(table.data.radius*5);
            switch (i%6) {
                case 1:
                    circle.setFillColor(sf::Color::Red);
                    break;
                case 2:
                    circle.setFillColor(sf::Color::Blue);
                    break;
                case 3:
                    circle.setFillColor(sf::Color::Green);
                    break;
                case 4:
                    circle.setFillColor(sf::Color::Magenta);
                    break;
                case 5:
                    circle.setFillColor(sf::Color::Cyan);
                    break;
                default:
                    circle.setFillColor(sf::Color::Yellow);
                    break;
            }
            circle.setPosition((table.balls[i].position.x-table.data.radius)*5,(table.balls[i].position.y-table.data.radius)*5);
            window.draw(circle);
        }
        window.display();
        std::this_thread::sleep_for(std::chrono::microseconds(sleeping_time));//to visualize correctly
        for (int j = 0; j < table.balls.size(); j++){
            helper::step(table.balls[j], table, timestep); //this functions calculates the position and velocity at next step
        }
        time_counter += timestep;
        check_collisions(table); //checks ball to ball collisions and adjusts the velocity
    }
}

