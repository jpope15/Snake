#ifndef FOOD_H
#define FOOD_H

#include <SFML/Graphics.hpp>
#include "constants.h"

class Food {
public:
    Food(sf::Vector2f coordinates) : coords(coordinates) {
        item = create_food(coords, sf::Color::Red);
    }
    
    void display(sf::RenderWindow& window) {
        window.draw(item);
    }
    
    sf::CircleShape create_food(sf::Vector2f coords, sf::Color color);
    sf::CircleShape get_shape() { return item; }

private:
    sf::Vector2f coords;
    sf::CircleShape item;

};

#endif