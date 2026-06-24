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

    auto new_texture = std::make_unique<sf::Texture>();

    if (!new_texture->loadFromFile(textures_dir + "/" + name))
    {
        throw std::runtime_error("Failed to load texture: " + name);
    }

    sf::Texture &ref = *new_texture;
    textures[name] = std::move(new_texture);

    return ref;
}
sf::Font &ResourceManager::getFont(const std::string &name)
{
    auto it = fonts.find(name);
    if (it != fonts.end())
    {
        return *(it->second);
    }

    auto mew_font = std::make_unique<sf::Font>();

    if (!mew_font->openFromFile(fonts_dir + "/" + name))
    {
        throw std::runtime_error("Failed to load font: " + name);
    }

    sf::Font &ref = *mew_font;
    fonts[name] = std::move(mew_font);

    return ref;
}