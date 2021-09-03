#ifndef CONSTANTS_H
#define CONSTANTS_H

const float SQUARE_PIXELS = 40.0;
const int Y_PIXELS = 1080;
const int X_PIXELS = 1080;
const int MAX_FOOD_ITEMS = 6;
const int FOOD_TIMER = 15;

enum movement_dir {UP = 0, DOWN, LEFT, RIGHT, NUMBER_OF_DIR, NONE};
typedef enum movement_dir Movement_dir;

enum game_type {SINGLE_PLAYER, TWO_PLAYER, EXIT };
typedef enum game_type Game_Type;

#endif