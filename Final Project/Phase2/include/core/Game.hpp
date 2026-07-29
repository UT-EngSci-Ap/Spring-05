#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "entities/Player.hpp"
#include "entities/Monster.hpp"
#include "entities/Bullet.hpp"
#include "entities/Hole.hpp"
#include "platforms/Platform.hpp"
#include "systems/CollisionSystem.hpp"
#include "config/Constants.hpp"
#include <vector>
#include <string>
#include <random>

class Game {
public:
    Game();
    ~Game();
    void run();
private:
    // The game is always in exactly one of these states.
    enum class GameState { Menu, Playing, GameOver, Settings };

    sf::RenderWindow window;
    sf::Clock clock;
    Player player;

    std::vector<Platform*> platforms;
    std::vector<Monster*> monsters;
    std::vector<Bullet*> bullets;
    std::vector<Hole*> holes;

    GameState state;
    // One independent high score per difficulty (index with
    // static_cast<int>(Constants::Difficulty)) so Easy/Medium/Hard never
    // overwrite each other's best score.
    int highScores[3];

    void processEvents();
    void update(float dt);
    void render();

    void genPlatforms();

    // Platform generation that guarantees a reachable climb path.
    sf::Vector2f reachablePlatformPos(float fromX, float fromY);
    Platform* makePlatform(float x, float y);
    Platform* highestSolidPlatform();
    bool isPositionClear(float x, float y, Platform* exclude = nullptr);
    // General clearance check reused by platforms, monsters and holes so
    // that no two world objects ever overlap or touch.
    bool isAreaClear(float x, float y, float width, float height,
                      Platform* excludePlat = nullptr,
                      Monster* excludeMonster = nullptr,
                      Hole* excludeHole = nullptr);

    void handle_scroll();
    void scrollWorld(float heightDiff);
    void regenPlatform(Platform* plat, std::vector<Platform*>& platforms);
    sf::Vector2f findTheHighestPlatPos(std::vector<Platform*>& platforms);

    // Monsters
    void trySpawnMonster(float nearX, float nearY);
    void regenMonster(Monster* monster);
    void respawnDeadMonsters();
    void clearMonsters();

    // Bullets
    void handleShooting(float dt);
    void cleanupBullets();
    void clearBullets();
    float currentShootCooldown() const;

    // Holes (Hard mode)
    void trySpawnHole(float nearX, float nearY);
    void regenHole(Hole* hole);
    void clearHoles();
    bool holesEnabled() const;

    // Difficulty-dependent tuning
    int currentMonsterHealth() const;
    float movingPlatformSpeedMultiplier() const;

    void checkGameOver();
    void triggerGameOver();

    // Session control
    void startGame();        // (re)start a fresh play session
    void clearPlatforms();

    // Rendering
    sf::Font font;
    sf::Text scoreText;      // in-game HUD score
    sf::Text uiText;         // reused for menu / game-over / settings lines

    void drawCenteredText(const std::string& msg, unsigned size,
                          sf::Color color, float y);
    void drawTextAt(const std::string& msg, unsigned size,
                     sf::Color color, float x, float y);
    void renderMenu();
    void renderGameOver();
    void renderSettings();
    void UIRender();
    void textsInit();

    // Clickable button images shown on the menu / game-over screens.
    sf::Texture startBtnTexture;
    sf::Texture restartBtnTexture;
    sf::Texture menuBtnTexture;
    sf::Texture settingsBtnTexture;
    sf::Texture backBtnTexture;
    sf::Sprite startButton;     // menu: begin a new game
    sf::Sprite restartButton;   // game over: play again
    sf::Sprite menuButton;      // game over: back to main menu
    sf::Sprite settingsButton;  // menu: open the settings screen
    sf::Sprite backButton;      // settings: back to main menu

    void buttonsInit();
    void placeButtonCentered(sf::Sprite& button, float y);
    bool isButtonClicked(const sf::Sprite& button, sf::Vector2f point) const;
    void handleMenuClick(sf::Vector2f point);
    void handleGameOverClick(sf::Vector2f point);

    // Settings screen: volume slider + difficulty selector
    sf::RectangleShape volumeTrack;
    sf::CircleShape volumeHandle;
    bool draggingVolumeHandle = false;
    sf::RectangleShape easyBox, mediumBox, hardBox;

    void settingsInit();
    void handleSettingsClick(sf::Vector2f point);
    void updateVolumeFromMouseX(float mouseX);
    void applyVolumeFromHandle();
    void positionVolumeHandle();

    int volume = Constants::DEFAULT_VOLUME;                  // 0-100
    Constants::Difficulty difficulty = Constants::Difficulty::Easy;
    void loadSettings();
    void saveSettings();
    void applyVolume();

    // High-score persistence (separate score per difficulty)
    void loadHighScore();
    void saveHighScore();
    int currentHighScore() const;   // score for the currently active difficulty

    // Audio
    sf::Music menuMusic;
    sf::SoundBuffer jumpBuffer, loseBuffer, shootBuffer;
    sf::Sound jumpSound, loseSound, shootSound;
    void audioInit();
    void updateMenuMusic();

    sf::Texture backgroundTexture;
    sf::Sprite background;

    float distanceTraveled = 0;

    std::mt19937 gen;
};

#endif
