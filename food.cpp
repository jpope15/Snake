#include "food.h"

sf::CircleShape Food::create_food(sf::Vector2f coords, sf::Color color){
    sf::CircleShape temp;
    temp.setRadius(SQUARE_PIXELS/2);
    temp.setPointCount(30);
    temp.setFillColor(color);
    temp.setPosition(coords);
    return temp;
}
