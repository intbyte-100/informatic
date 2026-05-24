#pragma once
#include "GameObject.h"
#include "Obstacle.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

class Player : public GameObject {
public:
    Player(const std::string& texturePath, const sf::Vector2f& position, const sf::Vector2f& scale);
    void update(float deltaTime, Obstacle& obstacle);
    void draw(sf::RenderWindow &render) override;
private:
    void handleCollision(const sf::FloatRect& obstacleBounds);
};