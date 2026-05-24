#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Obstacle.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Game");

    Player player("player.png", sf::Vector2f(400, 300), sf::Vector2f(0.1f, 0.1f)); 
    Obstacle obstacle("obstacle.png", sf::Vector2f(200, 100), sf::Vector2f(0.1f, 0.1f)); 

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();

        player.update(deltaTime, obstacle);
        obstacle.update(deltaTime);

        window.clear();
        player.draw(window);
        obstacle.draw(window);
        window.display();
    }

    return 0;
}