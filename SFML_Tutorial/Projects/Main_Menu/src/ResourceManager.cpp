#include "ResourceManager.h"

sf::Texture &ResourceManager::getTexture(const std::string &name)
{
    auto it = textures.find(name);
    if (it != textures.end())
    {
        return *(it->second);
    }

    auto newTexture = std::make_unique<sf::Texture>();

    if (!newTexture->loadFromFile(src_dir + "/" + name))
    {
        throw std::runtime_error("Failed to load texture: " + name);
    }

    sf::Texture &ref = *newTexture;
    textures[name] = std::move(newTexture);

    return ref;
}