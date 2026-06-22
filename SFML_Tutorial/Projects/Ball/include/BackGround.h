#ifndef __BACK_GROUND__
#define __BACK_GROUND__
#include <SFML/Graphics.hpp>
#include "GObj.h"
#include "ResourceManager.h"
class BackGround : GObj
{
private:
    sf::Sprite sprite;

public:
    BackGround(const sf::Texture &);
    void draw(sf::RenderWindow &) override;
    void setPosition(sf::Vector2f) override;
    void setScale(sf::Vector2f) override;
    sf::FloatRect getGlobalBounds() const override;
    sf::FloatRect getLocalBounds() const override;
};

#endif