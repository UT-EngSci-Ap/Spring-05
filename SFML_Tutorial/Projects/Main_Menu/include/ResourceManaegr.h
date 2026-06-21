#ifndef __RESOURCE_MANAGER__
#define __RESOURCE_MANAGER__
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <memory>

class ResourceManager
{
private:
    std::string src_dir;
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;

public:
    ResourceManager(const std::string &s) : src_dir(s) {}
    sf::Texture &getTexture(const std::string &);
};

#endif