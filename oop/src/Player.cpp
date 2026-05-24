#include "Player.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cstdlib>

Player::Player(const std::string& texturePath, const sf::Vector2f& position, const sf::Vector2f& scale)
    : GameObject(texturePath, position, scale) {
}

void Player::update(float deltaTime, Obstacle& obstacle) {
    int speed = 200;
    
    if (sprite.getGlobalBounds().intersects(obstacle.getBounds())) {
        handleCollision(obstacle.getBounds());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        move(0, -speed * deltaTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) // Move left
        move(-speed * deltaTime, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) // Move down
        move(0, speed * deltaTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) // Move right
        move(speed * deltaTime, 0);
}

void Player::handleCollision(const sf::FloatRect& obstacleBounds) {
    auto playerBounds = sprite.getGlobalBounds();

    
    float playerCenterX = playerBounds.left + playerBounds.width / 2.f;
    float playerCenterY = playerBounds.top + playerBounds.height / 2.f;
    float obstacleCenterX = obstacleBounds.left + obstacleBounds.width / 2.f;
    float obstacleCenterY = obstacleBounds.top + obstacleBounds.height / 2.f;

   
    float deltaX = playerCenterX - obstacleCenterX;
    float deltaY = playerCenterY - obstacleCenterY;

  
    float intersectX = std::abs(deltaX) - (playerBounds.width / 2.f + obstacleBounds.width / 2.f);
    float intersectY = std::abs(deltaY) - (playerBounds.height / 2.f + obstacleBounds.height / 2.f);

   
    if (intersectX < 0.f && intersectY < 0.f) {
       
        if (intersectX > intersectY) {
            
            if (deltaX > 0)
                sprite.move(-intersectX, 0.f);
            else
                sprite.move(intersectX, 0.f);
        } else {
           
            if (deltaY > 0)
                sprite.move(0.f, -intersectY);
            else
                sprite.move(0.f, intersectY);
        }
    }
}



void Player::draw(sf::RenderWindow& render) {
    render.draw(sprite);
}