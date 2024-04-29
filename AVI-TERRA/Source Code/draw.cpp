#include <SFML/Graphics.hpp>
#include "board.h"
void draw(Table &table){
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Rectangle");
    sf::RectangleShape rectangle(sf::Vector2f(table.width*5, table.height*5));
    rectangle.setFillColor(sf::Color::White);
    rectangle.setPosition(30.f, 20.f);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(rectangle);
        window.display();
    }
}