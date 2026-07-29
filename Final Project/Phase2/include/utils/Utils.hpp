#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>


namespace Utils {
    void centerTextOrigin(sf::Text& text);
    void scaleSprite(sf::Sprite& sprite, float width, float height);

    // Maximum horizontal distance the player can cover while travelling up to
    // a platform that sits `verticalGap` pixels above the launch point. Used by
    // platform generation to guarantee every platform is actually reachable.
    float maxHorizontalReach(float verticalGap);

    // Scans a texture top-to-bottom and returns, as a fraction of its height
    // (0..1), how far down the first non-transparent pixel row is. Used to
    // find the true visual top of a sprite when its source image has
    // transparent padding above the actual artwork (e.g. a shooting-pose
    // sprite drawn with empty space above a raised arm) - the sprite's
    // bounding box alone can't tell you that.
    float opaqueTopRatio(const sf::Texture& texture);

    // Asset loaders that fail fast with a clear message instead of silently
    // producing blank sprites/text when a file is missing.
    void loadTexture(sf::Texture& texture, const std::string& path);
    void loadImage(sf::Image& image, const std::string& path);
    void loadFont(sf::Font& font, const std::string& path);
    void loadSoundBuffer(sf::SoundBuffer& buffer, const std::string& path);
    void openMusic(sf::Music& music, const std::string& path);
}
