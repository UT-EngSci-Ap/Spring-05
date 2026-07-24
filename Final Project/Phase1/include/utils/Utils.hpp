#pragma once

#include <SFML/Graphics.hpp>
#include <string>


namespace Utils {
    void centerTextOrigin(sf::Text& text);
    void scaleSprite(sf::Sprite& sprite, float width, float height);

    // Maximum horizontal distance the player can cover while travelling up to
    // a platform that sits `verticalGap` pixels above the launch point. Used by
    // platform generation to guarantee every platform is actually reachable.
    float maxHorizontalReach(float verticalGap);

    // Asset loaders that fail fast with a clear message instead of silently
    // producing blank sprites/text when a file is missing.
    void loadTexture(sf::Texture& texture, const std::string& path);
    void loadImage(sf::Image& image, const std::string& path);
    void loadFont(sf::Font& font, const std::string& path);
}
