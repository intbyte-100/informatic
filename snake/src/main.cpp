#include <SFML/Graphics.hpp>
#include <cstdlib> // Для rand()
#include <ctime>   // Для time()
#include "Snake.h"

class Game {
public:
    Game() : window(sf::VideoMode(WIDTH, HEIGHT), "Snake Game"), snake(), clock() {
        food.setPosition(rand() % NUM_X * SIZE, rand() % NUM_Y * SIZE);
    }

    void run() {
        sf::Time timeSinceLastMove = sf::Time::Zero;

        while (window.isOpen()) {
            sf::Time deltaTime = clock.restart();
            timeSinceLastMove += deltaTime;

            render();
            handleEvents();
            if (timeSinceLastMove.asSeconds() >= MOVE_INTERVAL) {
                timeSinceLastMove = sf::Time::Zero;
                update();
            }
            
        }
    }

private:
    sf::RenderWindow window;
    Snake snake;
    sf::RectangleShape food;
    sf::Clock clock;

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W) snake.changeDirection(UP);
                if (event.key.code == sf::Keyboard::S) snake.changeDirection(DOWN);
                if (event.key.code == sf::Keyboard::A) snake.changeDirection(LEFT);
                if (event.key.code == sf::Keyboard::D) snake.changeDirection(RIGHT);
            }
        }
    }

    void update() {
        snake.move();

        
        if (snake.getSegments().front().x * SIZE == food.getPosition().x &&
            snake.getSegments().front().y * SIZE == food.getPosition().y) {
            snake.grow();
            food.setPosition(rand() % NUM_X * SIZE, rand() % NUM_Y * SIZE);
        }
    }

    void render() {
        window.clear();

        
        for (const auto& segment : snake.getSegments()) {
            sf::RectangleShape rectangle(sf::Vector2f(SIZE, SIZE));
            rectangle.setPosition(segment.x * SIZE, segment.y * SIZE);
            window.draw(rectangle);
        }
        
        
        for (const auto& segment: snake.getSegments()) {
            for (const auto &anoter_segment: snake.getSegments()) {
                if (segment.id == anoter_segment.id) {
                    continue;
                }
                
                if (segment.x == anoter_segment.x && segment.y == anoter_segment.y && segment.id != anoter_segment.id) {
                    snake = Snake();
                    break;
                }
                    
                    
            }
        }

       
        food.setSize(sf::Vector2f(SIZE, SIZE));
        food.setFillColor(sf::Color::Red);
        window.draw(food);

        window.display();
    }
};

int main() {
    srand(static_cast<unsigned>(time(0))); 
    Game game;
    game.run();
    return 0;
}