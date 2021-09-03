#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "constants.h"
#include "helpers.h"
#include "food.h"
using namespace std;

class Snake {
public:
    Snake(sf::Color snake_color = sf::Color::Green);
    void display(sf::RenderWindow& window);
    sf::RectangleShape create_seg(sf::Vector2f coords, sf::Color color);
    void turn(Movement_dir dir);
    bool check_for_food(vector<Food>& f, int& f_index);
    bool check_for_food(const sf::Vector2f& coords);
    void increase_size();
    bool check_for_collisions();
    bool check_for_collisions(const vector<sf::RectangleShape> snakeTwoBody);
    int getScore() { return body.size(); }

    const vector<sf::RectangleShape>& get_body() { return body; }
private:
    vector<sf::RectangleShape> body;
    vector<Movement_dir> directions;    //corresponds to each segment of the snake

    sf::Color body_color;
    Movement_dir prev_dir;  //tracks the previous direction
};

#endif