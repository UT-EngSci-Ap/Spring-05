#ifndef __RESOURCE_MANAGER__
#define __RESOURCE_MANAGER__
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <memory>

class ResourceManager
{
private:
    std::string textures_dir;
    std::string fonts_dir;
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts;

public:
    ResourceManager(const std::string &t, const std::string &f);
    sf::Font &getFont(const std::string &);
    sf::Texture &getTexture(const std::string &);
};

#endif