#include "Button.h"
#include "LayOut.h"
constexpr float TEXT_BUTTON_SCALE = 0.8;

bool Button::isHovered(const sf::Vector2i &mouse_position) const
{
    return sprite.getGlobalBounds().contains(sf::Vector2f(mouse_position));
}

Button::Button(const sf::Texture &t, const sf::Font &f, sf::Color clr, const std::string &str) : sprite(t), text(f)
{
    text.setString(str);
    LayOut::fill(text, sprite.getGlobalBounds().size, TEXT_BUTTON_SCALE);
    LayOut::centerVertical<sf::Text>(
        {text},
        sprite.getGlobalBounds().size);
}

void Button::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
    window.draw(text);
}

void Button::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
    LayOut::centerVertical<sf::Text>(
        {text},
        sprite.getGlobalBounds().size);
}

void Button::setScale(sf::Vector2f scale)
{
    sprite.setScale(scale);
    LayOut::fill(
        text,
        sprite.getGlobalBounds().size,
        TEXT_BUTTON_SCALE);
}
sf::FloatRect Button::getGlobalBounds() const
{
    return sprite.getGlobalBounds();
}