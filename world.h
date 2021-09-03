


#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>

#include "snake.h"
#include "constants.h"
#include "food.h"
#include "helpers.h"

using namespace std;

class World {
public:
    World() {}
    void runGame();
    void get_event_res(const sf::Event& event, sf::RenderWindow& window, Movement_dir& dir);
    void get_event_res(
        const sf::Event& event, 
        sf::RenderWindow& window, 
        Movement_dir& snakeOneDir, 
        Movement_dir& SnakeTwoDir,
        int& snakeOneCount,
        int& snakeTwoCount,
        bool& snake_one_moved,
        bool& snake_two_moved);
    Game_Type intro_gui();
    void run_single_player(int& score);    //returns the score when the game ends
    void run_two_player(int& p1Score, int& p2Score);
    sf::Vector2f get_food_pos(Snake s1);
    sf::Vector2f get_food_pos(Snake s1, Snake s2);


    bool handle_game(
        sf::RenderWindow& window,
        Snake& s,
        vector<Food>& food_arr,
        vector<int>& food_counter,
        int& count,
        bool& moved,
        Movement_dir dir
    ); //returns true if we need to stop the game

    bool display_death_screen(int score);   //single player version
    bool display_death_screen(int p1Score, int p2Score); //two player version
    void display_direction_screen();
private:
};


#endif