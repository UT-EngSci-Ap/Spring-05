#ifndef __BUTTON__
#define __BUTTON__
#include "GObj.h"
#include <string>
class Button : public GObj
{
private:
    sf::Sprite sprite;
    sf::Text text;

public:
    Button(const sf::Texture &t, const sf::Font &f, sf::Color, const std::string &);
    bool isHovered(const sf::Vector2i &) const;
    void draw(sf::RenderWindow &) override;
    void setPosition(sf::Vector2f) override;
    void setScale(sf::Vector2f) override;
    sf::FloatRect getGlobalBounds() const override;
};

#endif