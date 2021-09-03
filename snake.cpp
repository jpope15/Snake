#include "snake.h"

Snake::Snake(sf::Color snake_color) {
    int x = Helpers::roundUp(rand() % X_PIXELS-SQUARE_PIXELS, SQUARE_PIXELS);
    int y = Helpers::roundUp(rand() % Y_PIXELS-SQUARE_PIXELS, SQUARE_PIXELS);
    sf::RectangleShape newSeg = create_seg(sf::Vector2f(x, y), snake_color);
    body.push_back(newSeg);
    directions.push_back(NONE);
    prev_dir = NONE;
    body_color = snake_color;
}

void Snake::display(sf::RenderWindow& window) {
    for (int i = 0; i < body.size(); i++) {
        window.draw(body[i]);
    }
}

sf::RectangleShape Snake::create_seg(sf::Vector2f coords, sf::Color color){
    sf::RectangleShape s;
    s.setSize(sf::Vector2f(SQUARE_PIXELS, SQUARE_PIXELS));
    s.setFillColor(color);
    s.setPosition(coords);
    return s;
}

void Snake::turn(Movement_dir dir) {

    directions.insert(directions.begin(), dir);
    prev_dir = directions[directions.size()-1];
    directions.pop_back();

    for (int i = 0; i < body.size() && i < directions.size(); i++) {
        int x_factor = 0, y_factor = 0;
        switch(directions[i]) {
            case UP:
                y_factor = -SQUARE_PIXELS;
                break;
            case DOWN:
                y_factor = SQUARE_PIXELS;
                break;
            case LEFT:
                x_factor = -SQUARE_PIXELS;
                break;
            case RIGHT:
                x_factor = SQUARE_PIXELS;
                break;
            default:
                break;
        }
        body[i].move(sf::Vector2f(x_factor, y_factor));
    }
}

bool Snake::check_for_food(vector<Food>& f, int& f_index) {
    for (int j = 0; j < f.size(); j++) {
        if (body[0].getGlobalBounds().intersects(f[j].get_shape().getGlobalBounds())) {
            f_index = j;
            return true;
        }
    }
    return false;
}
bool Snake::check_for_food(const sf::Vector2f& coords) {
    for (int i = 0; i < body.size(); i++) 
        if (body[i].getGlobalBounds().contains(coords))
            return true;
    return false;
}

void Snake::increase_size() {
    sf::Vector2f coords;
    sf::RectangleShape last_seg = body[body.size()-1];
    
    switch (directions[directions.size()-1]) {
        case UP:
            coords.x = last_seg.getPosition().x;
            coords.y = last_seg.getPosition().y + SQUARE_PIXELS;
            break;
        case DOWN:
            coords.x = last_seg.getPosition().x;
            coords.y = last_seg.getPosition().y - SQUARE_PIXELS;
            break;
        case RIGHT:
            coords.x = last_seg.getPosition().x - SQUARE_PIXELS;
            coords.y = last_seg.getPosition().y;
            break;
        case LEFT:
            coords.x = last_seg.getPosition().x + SQUARE_PIXELS;
            coords.y = last_seg.getPosition().y;
            break;
        default:
            break;
    }

    sf::RectangleShape new_seg = create_seg(coords, body_color);
    body.push_back(new_seg);
    directions.push_back(prev_dir);
}

bool Snake::check_for_collisions(){ 
    sf::Vector2f head_coords = body[0].getPosition();
    if (head_coords.x > X_PIXELS-SQUARE_PIXELS || head_coords.x < 0) return true;
    if (head_coords.y > Y_PIXELS-SQUARE_PIXELS || head_coords.y < 0) return true;
    for (int i = 1; i < body.size(); i++)
        if (body[0].getGlobalBounds().intersects(body[i].getGlobalBounds()))
            return true;
    return false;
}

bool Snake::check_for_collisions(const vector<sf::RectangleShape> snakeTwoBody) {
    sf::Vector2f snakeOneHeadCoords = body[0].getPosition();
    sf::Vector2f snakeTwoHeadCoords = snakeTwoBody[0].getPosition();
    for (int i = 0; i < snakeTwoBody.size(); i++)      
        if (body[0].getGlobalBounds().intersects(snakeTwoBody[i].getGlobalBounds()))
            return true;
    
    for (int i = 0; i < body.size(); i++) 
        if (snakeTwoBody[0].getGlobalBounds().intersects(body[i].getGlobalBounds()))
            return true;

    return false;
}
