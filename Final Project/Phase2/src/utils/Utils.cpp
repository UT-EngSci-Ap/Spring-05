#include "utils/Utils.hpp"
#include "config/Constants.hpp"
#include <stdexcept>
#include <cmath>

void Utils::centerTextOrigin(sf::Text& text) {
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({
        textBounds.position.x + textBounds.size.x / 2.f,
        textBounds.position.y + textBounds.size.y / 2.f
    });
}

void Utils::scaleSprite(sf::Sprite& sprite, float width, float /*height*/) {
    float scaleX = width / sprite.getGlobalBounds().size.x;
    sprite.setScale({scaleX, scaleX});
}

void Utils::loadTexture(sf::Texture& texture, const std::string& path) {
    if (!texture.loadFromFile(path))
        throw std::runtime_error("Failed to load texture: " + path);
}

void Utils::loadImage(sf::Image& image, const std::string& path) {
    if (!image.loadFromFile(path))
        throw std::runtime_error("Failed to load image: " + path);
}

void Utils::loadFont(sf::Font& font, const std::string& path) {
    if (!font.openFromFile(path))
        throw std::runtime_error("Failed to load font: " + path);
}

void Utils::loadSoundBuffer(sf::SoundBuffer& buffer, const std::string& path) {
    if (!buffer.loadFromFile(path))
        throw std::runtime_error("Failed to load sound: " + path);
}

void Utils::openMusic(sf::Music& music, const std::string& path) {
    if (!music.openFromFile(path))
        throw std::runtime_error("Failed to load music: " + path);
}

float Utils::opaqueTopRatio(const sf::Texture& texture) {
    sf::Image image = texture.copyToImage();
    sf::Vector2u size = image.getSize();
    if (size.y == 0)
        return 0.f;

    for (unsigned int y = 0; y < size.y; y++) {
        for (unsigned int x = 0; x < size.x; x++) {
            if (image.getPixel({x, y}).a > 0)
                return static_cast<float>(y) / static_cast<float>(size.y);
        }
    }
    return 0.f; // fully transparent image - no padding to correct for
}

float Utils::maxHorizontalReach(float verticalGap) {
    const float launchSpeed = -Constants::JUMP_SPEED;   // magnitude of the jump
    const float g = Constants::GRAVITY;

    // disc < 0 means the gap is higher than the jump apex and the platform can
    // never be reached at all; clamp so we fall back to apex-only reach.
    float disc = launchSpeed * launchSpeed - 2.f * g * verticalGap;
    if (disc < 0.f)
        disc = 0.f;

    // Time to fall back down to the target height (the later of the two times
    // the player passes through it). The player lands while descending, so this
    // is the real window available for horizontal travel.
    float descendTime = (launchSpeed + std::sqrt(disc)) / g;
    return Constants::HORIZONTAL_SPEED * descendTime;
}
