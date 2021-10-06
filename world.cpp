#include "world.h"

void World::runGame() {
    srand(time(0));
    bool play_again;
    do {
        play_again = false;
        Game_Type game_style = intro_gui();
        if (game_style == SINGLE_PLAYER){
            int score;
            run_single_player(score);
            play_again = display_death_screen(score);
        } 
        if (game_style == TWO_PLAYER) {
            int p1Score, p2Score;
            display_direction_screen();
            run_two_player(p1Score, p2Score);
            play_again = display_death_screen(p1Score, p2Score);
        }
    } while (play_again);
}

void World::run_single_player(int& score) {
    Snake s;
    vector<Food> food_arr;
    vector<int> food_counter(MAX_FOOD_ITEMS);   //keeps track if a food was eaten and when to spawn a new one
    bool moved = false;            //tells us if a turn has passed
    Movement_dir dir = NONE;           //for the turns with no keyboard input

    sf::Texture background;
    sf::Sprite backgroundImage;
    if (!background.loadFromFile("assets/snake.png")) 
        cout << "error opening background file" << endl;
    backgroundImage.setTexture(background);

    for (int i = 0; i < MAX_FOOD_ITEMS; i++) {  //spawing food on the board
        Food f(get_food_pos(s));
        food_arr.push_back(f);
    }
    int count = 0; 

    sf::RenderWindow window(sf::VideoMode(X_PIXELS, Y_PIXELS), "Snake");
    while (window.isOpen()){
        moved = false;
        sf::Event event;
        while (window.pollEvent(event)){
            get_event_res(event, window, dir);
            s.turn(dir);
            count = 0;  //so it doesnt move twice on one turn
            moved = true;
        }
        
        if (handle_game(window, s, food_arr, food_counter, count, moved, dir)) break;

        window.clear();
        window.draw(backgroundImage);
        s.display(window);
        for (int i = 0; i < food_arr.size(); i++) {
            food_arr[i].display(window);
        }
        window.display();
    }
    score =  s.getScore();
}

void World::run_two_player(int& p1Score, int& p2Score) {
    Snake snakeOne;
    Snake snakeTwo(sf::Color::Blue);

    vector<Food> food_arr;
    vector<int> food_counter(MAX_FOOD_ITEMS);   //keeps track if a food was eaten and when to spawn a new one
    bool moved = false;            //tells us if a turn has passed
    bool snake_one_moved = false;
    bool snake_two_moved = false;   //eliminates the bug where the snake with no input would still move
    Movement_dir snakeOneDir = NONE;           //for the turns with no keyboard input
    Movement_dir snakeTwoDir = NONE;

    sf::Texture background;
    sf::Sprite backgroundImage;
    if (!background.loadFromFile("assets/snake.png")) 
        cout << "error opening background file" << endl;
    backgroundImage.setTexture(background);

    for (int i = 0; i < MAX_FOOD_ITEMS; i++) {  //spawing food on the board
        Food f(get_food_pos(snakeOne, snakeTwo));
        food_arr.push_back(f);
    }
    int snakeOneCount = 0; 
    int snakeTwoCount = 0;

    sf::RenderWindow window(sf::VideoMode(X_PIXELS, Y_PIXELS), "Snake");
    while (window.isOpen()){
        moved = false;
        snake_one_moved = false;
        snake_two_moved = false;
        sf::Event event;
        while (window.pollEvent(event)){
            
            get_event_res(event, window, snakeOneDir, snakeTwoDir, snakeOneCount, snakeTwoCount, snake_one_moved, snake_two_moved);
            if (snake_one_moved) snakeOne.turn(snakeOneDir);   
            if (snake_two_moved) snakeTwo.turn(snakeTwoDir);
            moved = true;
        }
        
        if (handle_game(window, snakeOne, food_arr, food_counter, snakeOneCount, moved, snakeOneDir)) break;
        if (handle_game(window, snakeTwo, food_arr, food_counter, snakeTwoCount, moved, snakeTwoDir)) break;
        if (snakeOne.check_for_collisions(snakeTwo.get_body())) break;

        window.clear();
        window.draw(backgroundImage);
        snakeOne.display(window);
        snakeTwo.display(window);
        for (int i = 0; i < food_arr.size(); i++) {
            food_arr[i].display(window);
        }
        window.display();
    }
    p1Score = snakeOne.getScore();
    p2Score = snakeTwo.getScore();
}

//event handling for single player
void World::get_event_res(const sf::Event& event, sf::RenderWindow& window, Movement_dir& dir) {
    switch(event.type) {
        case sf::Event::Closed: 
            window.close();
            break;
        case sf::Event::KeyReleased:  
            switch(event.key.code){         //setting the direction
                case sf::Keyboard::W:
                case sf::Keyboard::Up: 
                    dir = UP;
                    break;
                case sf::Keyboard::S:
                case sf::Keyboard::Down:
                    dir = DOWN;
                    break;
                case sf::Keyboard::A:
                case sf::Keyboard::Left:
                    dir = LEFT;
                    break;
                case sf::Keyboard::D:
                case sf::Keyboard::Right:
                    dir = RIGHT;
                    break;
                default:
                    break;
            }
        break;
        default:
            break;
    }
}

//handling events for two player
void World::get_event_res(const sf::Event& event, 
    sf::RenderWindow& window, 
    Movement_dir& snakeOneDir, 
    Movement_dir& snakeTwoDir,
    int& snakeOneCount,
    int& snakeTwoCount,
    bool& snake_one_moved,
    bool& snake_two_moved)
{
    switch(event.type) {
        case sf::Event::Closed: 
            window.close();
            break;
        case sf::Event::KeyReleased:  
            switch(event.key.code){         //setting the direction
                case sf::Keyboard::W:
                    snakeTwoDir = UP;
                    snakeTwoCount = 0;
                    snake_two_moved = true;
                    break;
                case sf::Keyboard::Up: 
                    snakeOneDir = UP;
                    snakeOneCount = 0;
                    snake_one_moved = true;
                    break;
                case sf::Keyboard::S:
                    snakeTwoDir = DOWN;
                    snakeTwoCount = 0;
                    snake_two_moved = true;
                    break;
                case sf::Keyboard::Down:
                    snakeOneDir = DOWN;
                    snakeOneCount = 0;
                    snake_one_moved = true;
                    break;
                case sf::Keyboard::A:
                    snakeTwoDir = LEFT;
                    snakeTwoCount = 0;
                    snake_two_moved = true;
                    break;
                case sf::Keyboard::Left:
                    snakeOneDir = LEFT;
                    snakeOneCount = 0;
                    snake_one_moved = true;
                    break;
                case sf::Keyboard::D:
                    snakeTwoDir = RIGHT;
                    snakeTwoCount = 0;
                    snake_two_moved = true;
                    break;
                case sf::Keyboard::Right:
                    snakeOneDir = RIGHT;
                    snakeOneCount = 0;
                    snake_one_moved = true;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}


bool World::handle_game(
    sf::RenderWindow& window,
    Snake& s,
    vector<Food>& food_arr,
    vector<int>& food_counter,
    int& count,
    bool& moved,
    Movement_dir dir
) {
    count++;
    if (count == 400) {         //turn with no keyboard input
        s.turn(dir);
        moved = true;
        count = 0;
    }
    if (s.check_for_collisions()) { return true; }//checking if the snake hit itself or the walls

    int f_index;
    if (s.check_for_food(food_arr, f_index)) {  //seeing if the snake found food
        food_arr.erase(food_arr.begin() + f_index); //deleting the food at the correct index
        s.increase_size();
        food_counter[f_index]++;    //starting the counter
    }
    if (food_arr.size() == 0) { //if there is no food spawned, spawn one piece
        Food f(get_food_pos(s));
        food_arr.push_back(f);
        food_counter[rand()%food_counter.size()] = 0;    
    }
    for (int i = 0; i < food_counter.size(); i++) {
        if (food_counter[i] == FOOD_TIMER+1) {    //after n turns, spawn new food
            food_counter[i] = 0;
            Food f(get_food_pos(s));
            food_arr.push_back(f);
        } else if (food_counter[i] != 0) {
            if (moved)
                food_counter[i]++;  //if we moved and this timer is started, increment it
        }
    }
    return false;
}

sf::Vector2f World::get_food_pos(Snake s1) {
    sf::Vector2f coords;
    coords.x = Helpers::roundUp(rand()%X_PIXELS-SQUARE_PIXELS, SQUARE_PIXELS);
    coords.y = Helpers::roundUp(rand()%Y_PIXELS-SQUARE_PIXELS, SQUARE_PIXELS);
    while (s1.check_for_food(coords)) {
        coords.x = Helpers::roundUp(rand()%X_PIXELS-SQUARE_PIXELS, SQUARE_PIXELS);
        coords.y = Helpers::roundUp(rand()%Y_PIXELS-SQUARE_PIXELS, SQUARE_PIXELS);
    }
    return coords;
}
sf::Vector2f World::get_food_pos(Snake s1, Snake s2) {
    sf::Vector2f coords;
    coords.x = Helpers::roundUp(rand()%X_PIXELS-SQUARE_PIXELS, SQUARE_PIXELS);
    coords.y = Helpers::roundUp(rand()%Y_PIXELS-SQUARE_PIXELS, SQUARE_PIXELS);
    while (s1.check_for_food(coords) || s2.check_for_food(coords)) {
        coords.x = Helpers::roundUp(rand()%X_PIXELS-SQUARE_PIXELS, SQUARE_PIXELS);
        coords.y = Helpers::roundUp(rand()%Y_PIXELS-SQUARE_PIXELS, SQUARE_PIXELS);
    }
    return coords;
}

Game_Type World::intro_gui() {
    sf::RenderWindow window( sf::VideoMode( X_PIXELS, Y_PIXELS ), "Please Choose an Option");

    sf::Font font;
    if ( !font.loadFromFile( "assets/Tourney-Thin.ttf" ) )
        std::cout << "Can't find the font file" << std::endl;

    sf::Text singlePlayer;
    singlePlayer.setFont( font );
    singlePlayer.setStyle( sf::Text::Bold );
    singlePlayer.setString( "Single Player" );
    singlePlayer.setFillColor( sf::Color::White );
    singlePlayer.setCharacterSize( 48 );
    singlePlayer.setPosition( X_PIXELS/2-200, Y_PIXELS/2-96 );

    sf::Text twoPlayer;
    twoPlayer.setFont( font );
    twoPlayer.setStyle( sf::Text::Bold );
    twoPlayer.setString( "Two Player" );
    twoPlayer.setFillColor( sf::Color::White );
    twoPlayer.setCharacterSize( 48 );
    twoPlayer.setPosition( X_PIXELS/2-160, Y_PIXELS/2-48 );

    while (window.isOpen()) {
        sf::Event Event;
        
        while ( window.pollEvent( Event )) {
            sf::Vector2i mousePos = sf::Mouse::getPosition( window );
            sf::Vector2f mousePosF( mousePos.x, mousePos.y );
            switch ( Event.type ) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseMoved:
                    if ( singlePlayer.getGlobalBounds().contains(mousePosF)) {
                        singlePlayer.setFillColor( sf::Color::Red );
                    } else {
                        singlePlayer.setFillColor( sf::Color::White );
                    }

                    if (twoPlayer.getGlobalBounds().contains(mousePosF)) {
                        twoPlayer.setFillColor(sf::Color::Red);
                    } else {
                        twoPlayer.setFillColor(sf::Color::White);
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    if ( singlePlayer.getGlobalBounds().contains( mousePosF ) ) {
                        return SINGLE_PLAYER;
                    }
                    if (twoPlayer.getGlobalBounds().contains(mousePosF)) {
                        return TWO_PLAYER;
                    }
                    break;
                default:
                    break;
            }
        }

        window.clear();
        window.draw( singlePlayer );
        window.draw( twoPlayer );
        window.display();
    }
    return EXIT;
}

bool World::display_death_screen(int score) {
    sf::RenderWindow window( sf::VideoMode( X_PIXELS, Y_PIXELS ), "GAME OVER");

    sf::Font font;
    if ( !font.loadFromFile( "assets/Tourney-Thin.ttf" ) )
        std::cout << "Can't find the font file" << std::endl;

    sf::Text deathMessage;
    deathMessage.setFont( font );
    deathMessage.setStyle( sf::Text::Bold );
    deathMessage.setString( "GAME OVER!!!" );
    deathMessage.setFillColor( sf::Color::White );
    deathMessage.setCharacterSize( 48 );
    deathMessage.setPosition( X_PIXELS/2-200, Y_PIXELS/2-96 );

    sf::Text scoreMessage;
    scoreMessage.setFont(font);
    scoreMessage.setStyle(sf::Text::Bold);
    scoreMessage.setString("Final Score: " + to_string(score));
    scoreMessage.setFillColor(sf::Color::White);
    scoreMessage.setCharacterSize(48);
    scoreMessage.setPosition(X_PIXELS/2-220, Y_PIXELS/2-48);

    sf::Text playAgainButton("Play Again", font, 48);
    playAgainButton.setStyle(sf::Text::Bold);
    playAgainButton.setFillColor(sf::Color::White);
    playAgainButton.setPosition(X_PIXELS/2-180, Y_PIXELS-96);


    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);\
            sf::Vector2f mousePosF(mousePos.x, mousePos.y); 
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    return false;
                    break;
                case sf::Event::MouseMoved: 
                    if (playAgainButton.getGlobalBounds().contains(mousePosF))
                        playAgainButton.setFillColor(sf::Color::Red);
                    else 
                        playAgainButton.setFillColor(sf::Color::White);
                    break;
                case sf::Event::MouseButtonPressed:
                    if (playAgainButton.getGlobalBounds().contains(mousePosF))
                        return true;
                    break;
                default:
                    break;
            }
            window.clear();
            window.draw(deathMessage);
            window.draw(scoreMessage);
            window.draw(playAgainButton);
            window.display();
        }
    }
    return false;
}

bool World::display_death_screen(int p1Score, int p2Score) {
    sf::RenderWindow window( sf::VideoMode( X_PIXELS, Y_PIXELS ), "GAME OVER");

    sf::Font font;
    if ( !font.loadFromFile( "assets/Tourney-Thin.ttf" ) )
        std::cout << "Can't find the font file" << std::endl;

    sf::Text deathMessage;
    deathMessage.setFont( font );
    deathMessage.setStyle( sf::Text::Bold );
    deathMessage.setString( "GAME OVER!!!" );
    deathMessage.setFillColor( sf::Color::White );
    deathMessage.setCharacterSize( 48 );
    deathMessage.setPosition( X_PIXELS/2-200, Y_PIXELS/2-96 );

    sf::Text p1scoreMessage;
    p1scoreMessage.setFont(font);
    p1scoreMessage.setStyle(sf::Text::Bold);
    p1scoreMessage.setString("Player 1 Score: " + to_string(p2Score));
    p1scoreMessage.setFillColor(sf::Color::White);
    p1scoreMessage.setCharacterSize(48);
    p1scoreMessage.setPosition(X_PIXELS/2-260, Y_PIXELS/2-48);
    
    sf::Text p2scoreMessage;
    p2scoreMessage.setFont(font);
    p2scoreMessage.setStyle(sf::Text::Bold);
    p2scoreMessage.setString("Player 2 Score: " + to_string(p1Score));
    p2scoreMessage.setFillColor(sf::Color::White);
    p2scoreMessage.setCharacterSize(48);
    p2scoreMessage.setPosition(X_PIXELS/2-260, Y_PIXELS/2);
    
    sf::Text playAgainButton("Play Again", font, 48);
    playAgainButton.setStyle(sf::Text::Bold);
    playAgainButton.setFillColor(sf::Color::White);
    playAgainButton.setPosition(X_PIXELS/2-180, Y_PIXELS-96);

    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePosF(mousePos.x, mousePos.y); 
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    return false;
                    break;
                case sf::Event::MouseMoved: 
                    if (playAgainButton.getGlobalBounds().contains(mousePosF))
                        playAgainButton.setFillColor(sf::Color::Red);
                    else 
                        playAgainButton.setFillColor(sf::Color::White);
                    break;
                case sf::Event::MouseButtonPressed:
                    if (playAgainButton.getGlobalBounds().contains(mousePosF))
                        return true;
                    break;
                default:
                    break;
            }
            window.clear();
            window.draw(deathMessage);
            window.draw(p1scoreMessage);
            window.draw(p2scoreMessage);
            window.draw(playAgainButton);
            window.display();
        }
    }
    return false;
}

void World::display_direction_screen() {
    sf::RenderWindow window( sf::VideoMode( X_PIXELS, Y_PIXELS ), "Directions");

    sf::Font font;
    if ( !font.loadFromFile( "assets/Tourney-Thin.ttf" ) )
        std::cout << "Can't find the font file" << std::endl;

    sf::Text headerText;
    headerText.setFont(font);
    headerText.setStyle(sf::Text::Bold);
    headerText.setString("DIRECTIONS");
    headerText.setFillColor(sf::Color::White);
    headerText.setCharacterSize(48);
    headerText.setPosition(X_PIXELS/2-150, 48);

    sf::Text keyContinue;
    keyContinue.setFont(font);
    keyContinue.setStyle(sf::Text::Bold);
    keyContinue.setString("Press any key to continue...");
    keyContinue.setFillColor(sf::Color::White);
    keyContinue.setCharacterSize(48);
    keyContinue.setPosition(X_PIXELS/2-350, Y_PIXELS-96);

    sf::Text playerOneHeader("Player One:", font, 48);
    playerOneHeader.setStyle(sf::Text::Bold);
    playerOneHeader.setFillColor(sf::Color::White);
    playerOneHeader.setPosition(X_PIXELS/8, Y_PIXELS/3);

    sf::Text playerOneRuleOne("1. Blue Snake", font, 48);
    playerOneRuleOne.setStyle(sf::Text::Bold);
    playerOneRuleOne.setFillColor(sf::Color::White);
    playerOneRuleOne.setPosition(X_PIXELS/8+48, Y_PIXELS/3+60);
    
    sf::Text playerOneRuleTwo("2. Use keys W, A, S, D", font, 48);
    playerOneRuleTwo.setStyle(sf::Text::Bold);
    playerOneRuleTwo.setFillColor(sf::Color::White);
    playerOneRuleTwo.setPosition(X_PIXELS/8+48, Y_PIXELS/3+108);

    sf::Text playerTwoHeader("Player Two:", font, 48);
    playerTwoHeader.setStyle(sf::Text::Bold);
    playerTwoHeader.setFillColor(sf::Color::White);
    playerTwoHeader.setPosition(X_PIXELS/8, Y_PIXELS/2+48);

    sf::Text playerTwoRuleOne("1. Green Snake", font, 48);
    playerTwoRuleOne.setStyle(sf::Text::Bold);
    playerTwoRuleOne.setFillColor(sf::Color::White);
    playerTwoRuleOne.setPosition(X_PIXELS/8+48, Y_PIXELS/2+108);
    
    sf::Text playerTwoRuleTwo("2. Use arrow keys", font, 48);
    playerTwoRuleTwo.setStyle(sf::Text::Bold);
    playerTwoRuleTwo.setFillColor(sf::Color::White);
    playerTwoRuleTwo.setPosition(X_PIXELS/8+48, Y_PIXELS/2+156);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::KeyReleased:
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }
        window.clear();
        window.draw(headerText);
        window.draw(keyContinue);
        window.draw(playerOneHeader);
        window.draw(playerOneRuleOne);
        window.draw(playerOneRuleTwo);
        window.draw(playerTwoHeader);
        window.draw(playerTwoRuleOne);
        window.draw(playerTwoRuleTwo);
        window.display();
    }
}