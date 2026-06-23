#include "ResourceManager.h"
#include <SFML/Graphics.hpp>

ResourceManager::ResourceManager(const std::string &t, const std::string &f) : textures_dir(t), fonts_dir(f) {}

sf::Texture &ResourceManager::getTexture(const std::string &name)
{
    auto it = textures.find(name);
    if (it != textures.end())
    {
        return *(it->second);
    }

    auto newTexture = std::make_unique<sf::Texture>();

    if (!newTexture->loadFromFile(textures_dir + "/" + name))
    {
        throw std::runtime_error("Failed to load texture: " + name);
    }

    sf::Texture &ref = *newTexture;
    textures[name] = std::move(newTexture);

    return ref;
}
sf::Font &ResourceManager::getFont(const std::string &name)
{
    auto it = fonts.find(name);
    if (it != fonts.end())
    {
        return *(it->second);
    }

    auto newFont = std::make_unique<sf::Font>();

    if (!newFont->openFromFile(fonts_dir + "/" + name))
    {
        throw std::runtime_error("Failed to load font: " + name);
    }

    sf::Font &ref = *newFont;
    fonts[name] = std::move(newFont);

    return ref;
}