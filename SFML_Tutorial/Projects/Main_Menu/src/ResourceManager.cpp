#include "ResourceManager.h"

sf::Texture &ResourceManager::getTexture(const std::string &name)
{
    auto it = textures.find(name);
    if (it != textures.end())
    {
        return *(it->second);
    }

    auto new_texture = std::make_unique<sf::Texture>();

    if (!new_texture->loadFromFile(src_dir + "/" + name))
    {
        throw std::runtime_error("Failed to load texture: " + name);
    }

    sf::Texture &ref = *new_texture;
    textures[name] = std::move(new_texture);

    return ref;
}