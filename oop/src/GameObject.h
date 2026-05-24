#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class GameObject {
public:
    GameObject(const std::string& texturePath, const sf::Vector2f& position, const sf::Vector2f& scale);
    virtual ~GameObject() = default; 
    void update(float deltaTime);
    sf::FloatRect getBounds() const;
    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;
    void move(float x, float y);
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    virtual void draw(sf::RenderWindow& window) = 0;
    
};