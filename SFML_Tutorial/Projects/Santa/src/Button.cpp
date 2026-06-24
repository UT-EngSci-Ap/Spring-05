#include "Button.h"
#include "LayOut.h"

constexpr float TEXT_BUTTON_SCALE = 0.6;
constexpr float BUTTON_TEXT_VERTICAL_OFFSET = -7.f;

Button::Button(sf::Texture &texture, const sf::Font &f, sf::Color clr, const std::string &str)
    : sprite(texture), text(f)
{
    texture.setSmooth(true);
    text.setString(str);
    sprite.setColor(clr);
    updateLayout();
}

bool Button::isHovered(const sf::Vector2i &mouse_position) const
{
    return sprite.getGlobalBounds().contains(sf::Vector2f(mouse_position));
}

void Button::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
    window.draw(text);
}

void Button::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
    updateLayout();
}

void Button::setScale(sf::Vector2f scale)
{
    sprite.setScale(scale);
    updateLayout();
}

sf::FloatRect Button::getGlobalBounds() const
{
    return sprite.getGlobalBounds();
}

sf::FloatRect Button::getLocalBounds() const
{
    return sprite.getLocalBounds();
}

void Button::updateLayout()
{
    LayOut::fitText(text, sprite.getGlobalBounds().size, TEXT_BUTTON_SCALE);
    LayOut::centerVertical<sf::Text>({text}, sprite.getGlobalBounds());
    text.move({0, BUTTON_TEXT_VERTICAL_OFFSET});
}