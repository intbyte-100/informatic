#pragma once
#include "GameObject.h"
#include <SFML/Graphics/RenderWindow.hpp>

class Obstacle : public GameObject {
public:
    Obstacle(const std::string& texturePath, const sf::Vector2f& position, const sf::Vector2f& scale);
    void update(float deltaTime);
    void draw(sf::RenderWindow &render) override;
};
