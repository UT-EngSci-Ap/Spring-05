#include <random>
#include <optional>
#include <algorithm>
#include <fstream>
#include "core/Game.hpp"
#include "config/Constants.hpp"
#include "utils/Utils.hpp"
#include "platforms/MovingPlatform.hpp"
#include "platforms/BreakablePlatform.hpp"
#include "platforms/NormalPlatform.hpp"
using namespace std;

namespace {
    // UI palette (matches the assignment document theme).
    const sf::Color COLOR_PRIMARY(26, 58, 92);
    const sf::Color COLOR_TEXT(44, 62, 80);
    const char* HIGH_SCORE_FILE = "highscore.txt";

    // On-screen button image layout (the source PNGs are 400x100, a 4:1 ratio).
    const float BUTTON_WIDTH = 240.f;
    const float MENU_START_BUTTON_Y = 420.f;
    const float GAMEOVER_RESTART_BUTTON_Y = 440.f;
    const float GAMEOVER_MENU_BUTTON_Y = 530.f;
}

Game::Game() :
    window(sf::VideoMode({static_cast<unsigned int>(Constants::WINDOW_WIDTH),
                          static_cast<unsigned int>(Constants::WINDOW_HEIGHT)}), "doodle jump"),
    player(Constants::PLAYER_INITIAL_POS.x, Constants::PLAYER_INITIAL_POS.y),
    scoreText(font),
    uiText(font),
    startButton(startBtnTexture),
    restartButton(restartBtnTexture),
    menuButton(menuBtnTexture),
    background(backgroundTexture)
{
    gen.seed(std::random_device{}());
    Utils::loadTexture(backgroundTexture, "assets/background.png");
    background.setTexture(backgroundTexture, true);
    textsInit();
    buttonsInit();
    loadHighScore();
    state = GameState::Menu;
}

Game::~Game() {
    clearPlatforms();
}

void Game::clearPlatforms() {
    for (Platform* plat : platforms)
        delete plat;
    platforms.clear();
}

void Game::textsInit() {
    Utils::loadFont(font, "fonts/ariblk.ttf");

    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setPosition({10.f, 10.f});
}

void Game::buttonsInit() {
    Utils::loadTexture(startBtnTexture, "assets/start_button.png");
    Utils::loadTexture(restartBtnTexture, "assets/restart_button.png");
    Utils::loadTexture(menuBtnTexture, "assets/menu_button.png");

    startButton.setTexture(startBtnTexture, true);
    restartButton.setTexture(restartBtnTexture, true);
    menuButton.setTexture(menuBtnTexture, true);

    Utils::scaleSprite(startButton, BUTTON_WIDTH, 0.f);
    Utils::scaleSprite(restartButton, BUTTON_WIDTH, 0.f);
    Utils::scaleSprite(menuButton, BUTTON_WIDTH, 0.f);
}

void Game::placeButtonCentered(sf::Sprite& button, float y) {
    sf::Vector2f size = button.getGlobalBounds().size;
    button.setPosition({(Constants::WINDOW_WIDTH - size.x) / 2.f, y});
}

bool Game::isButtonClicked(const sf::Sprite& button, sf::Vector2f point) const {
    return button.getGlobalBounds().contains(point);
}

void Game::genPlatforms() {
    float playerX = player.getPosition().x;
    float playerY = player.getPosition().y;

    float startPlatformY = playerY + 30.f;
    platforms.push_back(new NormalPlatform(playerX, startPlatformY));

    // The "anchor" is the last solid platform the player can launch from. Each
    // new platform is generated to be reachable from it. Breakable platforms
    // never become the anchor, so the solid platforms always form an unbroken,
    // climbable chain - there is always a way to go higher.
    float anchorX = playerX;
    float anchorY = startPlatformY;

    for (int i = 0; i < Constants::INITIAL_PLATFORM_COUNT; i++) {
        // Pick a reachable position, then keep re-rolling until it also keeps the
        // minimum clear distance from every platform already placed.
        sf::Vector2f pos = reachablePlatformPos(anchorX, anchorY);
        for (int attempt = 0; attempt < Constants::PLATFORM_PLACEMENT_ATTEMPTS
                              && !isPositionClear(pos.x, pos.y); attempt++) {
            pos = reachablePlatformPos(anchorX, anchorY);
        }

        Platform* p = makePlatform(pos.x, pos.y);
        platforms.push_back(p);

        if (!p->isBreakable()) {
            anchorX = pos.x;
            anchorY = pos.y;
        }
    }
}

sf::Vector2f Game::reachablePlatformPos(float fromX, float fromY) {
    std::uniform_real_distribution<float> gapDist(
        Constants::MIN_PLATFORM_GAP, Constants::SAFE_MAX_PLATFORM_GAP);

    float gap = gapDist(gen);
    float newY = fromY - gap;

    // Horizontal spread is bounded by what the player can physically cover
    // while reaching a platform this far up, so the result is always reachable.
    float reach = Utils::maxHorizontalReach(gap) * Constants::HORIZONTAL_REACH_SAFETY;
    std::uniform_real_distribution<float> offsetDist(-reach, reach);
    float newX = fromX + offsetDist(gen);

    float maxX = Constants::WINDOW_WIDTH - Constants::PLATFORM_WIDTH;
    newX = std::clamp(newX, 0.f, maxX);   // clamping only shortens the jump

    return {newX, newY};
}

Platform* Game::makePlatform(float x, float y) {
    std::uniform_real_distribution<float> chance(0.f, 1.f);
    float prob = chance(gen);

    Platform* p = nullptr;
    if (prob < 0.7f) {
        p = new NormalPlatform(x, y);
        if (chance(gen) < 0.3f)
            p->setOnSpring();              // springs sit only on some static platforms
    }
    else if (prob < 0.85f) {
        p = new MovingPlatform(x, y);
    }
    else {
        p = new BreakablePlatform(x, y);
    }
    return p;
}

Platform* Game::highestSolidPlatform() {
    Platform* best = nullptr;
    float bestY = Constants::WINDOW_HEIGHT * 2.f;
    for (Platform* p : platforms) {
        if (p->isBreakable())
            continue;
        float y = p->getPosition().y;
        if (y < bestY) {
            bestY = y;
            best = p;
        }
    }
    return best;
}

void Game::startGame() {
    player.initPlayer();
    clearPlatforms();
    genPlatforms();
    distanceTraveled = 0;
    state = GameState::Playing;
}

void Game::run() {
    while (window.isOpen()) {

        processEvents();
        float dt = clock.restart().asSeconds();

        if (state == GameState::Playing) {
            update(dt);
            handle_scroll();
            CollisionSystem::resolvePlayerSpring(player, platforms);
            CollisionSystem::resolvePlayerPlatforms(player, platforms);
        }

        render();
    }
}

void Game::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (state == GameState::Menu) {
                if (key->code == sf::Keyboard::Key::Space ||
                    key->code == sf::Keyboard::Key::Enter)
                    startGame();
            }
            else if (state == GameState::GameOver) {
                if (key->code == sf::Keyboard::Key::Enter ||
                    key->code == sf::Keyboard::Key::Space ||
                    key->code == sf::Keyboard::Key::R)
                    startGame();                          // play again, no restart of program
                else if (key->code == sf::Keyboard::Key::M)
                    state = GameState::Menu;
            }
        }

        if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (click->button == sf::Mouse::Button::Left) {
                sf::Vector2f point(static_cast<float>(click->position.x),
                                   static_cast<float>(click->position.y));
                if (state == GameState::Menu)
                    handleMenuClick(point);
                else if (state == GameState::GameOver)
                    handleGameOverClick(point);
            }
        }
    }

    if (state == GameState::Playing)
        player.handle_input();
}

void Game::handleMenuClick(sf::Vector2f point) {
    if (isButtonClicked(startButton, point))
        startGame();
}

void Game::handleGameOverClick(sf::Vector2f point) {
    if (isButtonClicked(restartButton, point))
        startGame();                  // play again
    else if (isButtonClicked(menuButton, point))
        state = GameState::Menu;       // back to main menu
}

void Game::update(float dt) {
    player.update(dt);
    for (Platform* plat : platforms) {
        plat->update(dt);
    }
    checkGameOver();
}

void Game::render() {
    window.clear();
    window.draw(background);

    if (state == GameState::Menu) {
        renderMenu();
        window.display();
        return;
    }

    for (auto* platform : platforms) {
        platform->render(window);
    }
    player.render(window);
    UIRender();

    if (state == GameState::GameOver)
        renderGameOver();

    window.display();
}

void Game::UIRender() {
    scoreText.setString(to_string(player.getScore()));
    window.draw(scoreText);
}

void Game::drawCenteredText(const std::string& msg, unsigned size,
                            sf::Color color, float y) {
    uiText.setString(msg);
    uiText.setCharacterSize(size);
    uiText.setFillColor(color);
    Utils::centerTextOrigin(uiText);
    uiText.setPosition({Constants::WINDOW_WIDTH / 2.f, y});
    window.draw(uiText);
}

void Game::renderMenu() {
    drawCenteredText("DOODLE JUMP", 48, COLOR_PRIMARY, 180.f);
    drawCenteredText("HIGH SCORE: " + to_string(highScore), 28, COLOR_TEXT, 300.f);

    placeButtonCentered(startButton, MENU_START_BUTTON_Y);
    window.draw(startButton);

    drawCenteredText("Use Left / Right arrows to move", 20, COLOR_TEXT, 560.f);
}

void Game::renderGameOver() {
    // Dim the frozen play field so the message reads clearly.
    sf::RectangleShape overlay({Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT});
    overlay.setFillColor(sf::Color(255, 255, 255, 190));
    window.draw(overlay);

    drawCenteredText("YOU LOST", 54, sf::Color::Red, 210.f);
    drawCenteredText("SCORE: " + to_string(player.getScore()), 30, COLOR_TEXT, 310.f);
    drawCenteredText("HIGH SCORE: " + to_string(highScore), 28, COLOR_TEXT, 360.f);

    placeButtonCentered(restartButton, GAMEOVER_RESTART_BUTTON_Y);
    placeButtonCentered(menuButton, GAMEOVER_MENU_BUTTON_Y);
    window.draw(restartButton);
    window.draw(menuButton);
}

void Game::handle_scroll() {
    if (player.getPosition().y < Constants::SCREEN_MIDDLE) {
        float heightDiff = Constants::SCREEN_MIDDLE - player.getPosition().y;
        distanceTraveled += heightDiff;
        player.addScore(heightDiff);
        scrollPlatforms(heightDiff);
        player.setPosition(player.getPosition().x, Constants::SCREEN_MIDDLE);
    }
}

void Game::scrollPlatforms(float heightDiff) {
    for (auto* plat : platforms) {

        if (heightDiff > 0)
            plat->move(0, heightDiff);

        if (plat->getPosition().y > Constants::WINDOW_HEIGHT)
            regenPlatform(plat, platforms);
    }
}

void Game::regenPlatform(Platform* plat, std::vector<Platform*>& platforms) {

    // Anchor the new platform to the highest SOLID platform so the chain the
    // player actually climbs stays continuous and reachable. A breakable
    // platform never becomes the anchor, so it can never block the path.
    Platform* anchor = highestSolidPlatform();
    sf::Vector2f anchorPos = anchor ? anchor->getPosition()
                                    : findTheHighestPlatPos(platforms);

    sf::Vector2f pos = reachablePlatformPos(anchorPos.x, anchorPos.y);
    for (int attempt = 0; attempt < Constants::PLATFORM_PLACEMENT_ATTEMPTS
                          && !isPositionClear(pos.x, pos.y, plat); attempt++) {
        pos = reachablePlatformPos(anchorPos.x, anchorPos.y);
    }

    plat->reset();
    plat->setPosition(pos.x, pos.y);
}

bool Game::isPositionClear(float x, float y, Platform* exclude) {
    // Expand the candidate platform's box by the required minimum distance on
    // every side. If that padded box touches any other platform, they are too
    // close, so the position is rejected.
    sf::FloatRect candidate(
        {x - Constants::MIN_PLATFORM_DISTANCE, y - Constants::MIN_PLATFORM_DISTANCE},
        {Constants::PLATFORM_WIDTH  + 2.f * Constants::MIN_PLATFORM_DISTANCE,
         Constants::PLATFORM_HEIGHT + 2.f * Constants::MIN_PLATFORM_DISTANCE});

    for (Platform* p : platforms) {
        if (p == exclude)
            continue;
        if (candidate.findIntersection(p->getBounds()).has_value())
            return false;
    }
    return true;
}

sf::Vector2f Game::findTheHighestPlatPos(vector<Platform*>& platforms) {
    sf::Vector2f highestPos(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
    for (auto* plat : platforms) {
        float platY = plat->getPosition().y;
        if (platY < highestPos.y) {
            highestPos = plat->getPosition();
        }
    }
    return highestPos;
}

void Game::checkGameOver() {
    if (player.getPosition().y > Constants::WINDOW_HEIGHT) {
        state = GameState::GameOver;
        if (player.getScore() > highScore) {
            highScore = player.getScore();
            saveHighScore();
        }
    }
}

void Game::loadHighScore() {
    highScore = 0;
    std::ifstream in(HIGH_SCORE_FILE);
    if (in.is_open()) {
        int value;
        if (in >> value && value >= 0)
            highScore = value;
    }
}

void Game::saveHighScore() {
    std::ofstream out(HIGH_SCORE_FILE);
    if (out.is_open())
        out << highScore;
}
