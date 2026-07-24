#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "entities/Player.hpp"
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
    enum class GameState { Menu, Playing, GameOver };

    sf::RenderWindow window;
    sf::Clock clock;
    Player player;

    std::vector<Platform*> platforms;

    GameState state;
    int highScore;

    void processEvents();
    void update(float dt);
    void render();

    void genPlatforms();

    // Platform generation that guarantees a reachable climb path.
    sf::Vector2f reachablePlatformPos(float fromX, float fromY);
    Platform* makePlatform(float x, float y);
    Platform* highestSolidPlatform();
    bool isPositionClear(float x, float y, Platform* exclude = nullptr);

    void handle_scroll();
    void scrollPlatforms(float heightDiff);
    void regenPlatform(Platform* plat, std::vector<Platform*>& platforms);
    sf::Vector2f findTheHighestPlatPos(std::vector<Platform*>& platforms);

    void checkGameOver();

    // Session control
    void startGame();        // (re)start a fresh play session
    void clearPlatforms();

    // Rendering
    sf::Font font;
    sf::Text scoreText;      // in-game HUD score
    sf::Text uiText;         // reused for menu / game-over lines

    void drawCenteredText(const std::string& msg, unsigned size,
                          sf::Color color, float y);
    void renderMenu();
    void renderGameOver();
    void UIRender();
    void textsInit();

    // Clickable button images shown on the menu / game-over screens.
    sf::Texture startBtnTexture;
    sf::Texture restartBtnTexture;
    sf::Texture menuBtnTexture;
    sf::Sprite startButton;     // menu: begin a new game
    sf::Sprite restartButton;   // game over: play again
    sf::Sprite menuButton;      // game over: back to main menu

    void buttonsInit();
    void placeButtonCentered(sf::Sprite& button, float y);
    bool isButtonClicked(const sf::Sprite& button, sf::Vector2f point) const;
    void handleMenuClick(sf::Vector2f point);
    void handleGameOverClick(sf::Vector2f point);

    // High-score persistence
    void loadHighScore();
    void saveHighScore();

    sf::Texture backgroundTexture;
    sf::Sprite background;

    float distanceTraveled = 0;

    std::mt19937 gen;
};

#endif
