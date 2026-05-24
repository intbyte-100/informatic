#include "GameObject.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

GameObject::GameObject(const std::string& texturePath, const sf::Vector2f& position, const sf::Vector2f& scale)
{
    if (!texture.loadFromFile(texturePath)) {
        std::cout << "Failed to load texture: " << texturePath << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setPosition(position);
    sprite.setScale(scale);
}

void GameObject::update(float deltaTime) {
}

sf::FloatRect GameObject::getBounds() const {
    return sprite.getGlobalBounds();
}

void GameObject::setPosition(const sf::Vector2f& position) {
    sprite.setPosition(position);
}

sf::Vector2f GameObject::getPosition() const {
    return sprite.getPosition();
}


void GameObject::move(float x, float y) {
    this->sprite.move(x, y);
}

void GameObject::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}