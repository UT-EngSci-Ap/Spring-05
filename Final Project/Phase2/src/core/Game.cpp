#include <random>
#include <optional>
#include <algorithm>
#include <fstream>
#include <cmath>
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
    const char* SETTINGS_FILE = "settings.txt";

    // On-screen button image layout (the source PNGs are 400x100, a 4:1 ratio).
    const float BUTTON_WIDTH = 240.f;

    // Menu layout
    const float MENU_TITLE_Y = 130.f;
    const float MENU_HIGHSCORE_Y = 220.f;
    const float MENU_START_BUTTON_Y = 290.f;
    const float MENU_SETTINGS_BUTTON_Y = 370.f;
    const float MENU_MODE_Y = 470.f;
    const float MENU_INSTRUCTIONS_Y = 510.f;
    const float MENU_INSTRUCTIONS2_Y = 542.f;

    // Game-over layout
    const float GAMEOVER_RESTART_BUTTON_Y = 440.f;
    const float GAMEOVER_MENU_BUTTON_Y = 530.f;

    // Settings layout
    const float SETTINGS_TITLE_Y = 90.f;
    const float VOLUME_LABEL_Y = 190.f;
    const float VOLUME_TRACK_Y = 240.f;
    const float VOLUME_TRACK_X_START = 130.f;
    const float VOLUME_TRACK_X_END = 470.f;
    const float VOLUME_TRACK_HEIGHT = 6.f;
    const float VOLUME_HANDLE_RADIUS = 13.f;
    const float VOLUME_VALUE_Y = 280.f;
    const float DIFFICULTY_LABEL_Y = 350.f;
    const float DIFF_BOX_Y = 400.f;
    const float DIFF_BOX_WIDTH = 150.f;
    const float DIFF_BOX_HEIGHT = 60.f;
    const float DIFF_BOX_GAP = 20.f;
    const float SETTINGS_BACK_BUTTON_Y = 660.f;

    std::string difficultyName(Constants::Difficulty d) {
        switch (d) {
            case Constants::Difficulty::Easy:   return "EASY";
            case Constants::Difficulty::Medium: return "MEDIUM";
            case Constants::Difficulty::Hard:   return "HARD";
        }
        return "EASY";
    }
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
    settingsButton(settingsBtnTexture),
    backButton(backBtnTexture),
    jumpSound(jumpBuffer),
    loseSound(loseBuffer),
    shootSound(shootBuffer),
    background(backgroundTexture)
{
    gen.seed(std::random_device{}());
    Utils::loadTexture(backgroundTexture, "assets/images/background.png");
    background.setTexture(backgroundTexture, true);
    textsInit();
    buttonsInit();
    audioInit();
    loadHighScore();
    loadSettings();
    settingsInit();
    applyVolume();
    state = GameState::Menu;
}

Game::~Game() {
    clearPlatforms();
    clearMonsters();
    clearBullets();
    clearHoles();
}

void Game::clearPlatforms() {
    for (Platform* plat : platforms)
        delete plat;
    platforms.clear();
}

void Game::clearMonsters() {
    for (Monster* m : monsters)
        delete m;
    monsters.clear();
}

void Game::clearBullets() {
    for (Bullet* b : bullets)
        delete b;
    bullets.clear();
}

void Game::clearHoles() {
    for (Hole* h : holes)
        delete h;
    holes.clear();
}

void Game::textsInit() {
    Utils::loadFont(font, "fonts/ariblk.ttf");

    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setPosition({10.f, 10.f});
}

void Game::buttonsInit() {
    Utils::loadTexture(startBtnTexture, "assets/images/start_button.png");
    Utils::loadTexture(restartBtnTexture, "assets/images/restart_button.png");
    Utils::loadTexture(menuBtnTexture, "assets/images/menu_button.png");

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

void Game::audioInit() {
    Utils::openMusic(menuMusic, "assets/sounds/MainMenu_Song.flac");
    menuMusic.setLooping(true);

    Utils::loadSoundBuffer(jumpBuffer, "assets/sounds/Jumping_Sound.wav");
    Utils::loadSoundBuffer(loseBuffer, "assets/sounds/Loosing_Sound.wav");
    Utils::loadSoundBuffer(shootBuffer, "assets/sounds/Shooting_Sound.wav");
}

void Game::updateMenuMusic() {
    bool shouldPlay = (state == GameState::Menu || state == GameState::Settings);
    if (shouldPlay && menuMusic.getStatus() != sf::Music::Status::Playing)
        menuMusic.play();
    else if (!shouldPlay && menuMusic.getStatus() == sf::Music::Status::Playing)
        menuMusic.stop();
}

void Game::settingsInit() {
    Utils::loadTexture(settingsBtnTexture, "assets/images/Settings_button.png");
    Utils::loadTexture(backBtnTexture, "assets/images/back_button.png");
    settingsButton.setTexture(settingsBtnTexture, true);
    backButton.setTexture(backBtnTexture, true);
    Utils::scaleSprite(settingsButton, BUTTON_WIDTH, 0.f);
    Utils::scaleSprite(backButton, BUTTON_WIDTH, 0.f);

    volumeTrack.setSize({VOLUME_TRACK_X_END - VOLUME_TRACK_X_START, VOLUME_TRACK_HEIGHT});
    volumeTrack.setPosition({VOLUME_TRACK_X_START, VOLUME_TRACK_Y});
    volumeTrack.setFillColor(sf::Color(190, 190, 190));

    volumeHandle.setRadius(VOLUME_HANDLE_RADIUS);
    volumeHandle.setFillColor(COLOR_PRIMARY);
    positionVolumeHandle();

    float totalWidth = DIFF_BOX_WIDTH * 3.f + DIFF_BOX_GAP * 2.f;
    float startX = (Constants::WINDOW_WIDTH - totalWidth) / 2.f;

    easyBox.setSize({DIFF_BOX_WIDTH, DIFF_BOX_HEIGHT});
    mediumBox.setSize({DIFF_BOX_WIDTH, DIFF_BOX_HEIGHT});
    hardBox.setSize({DIFF_BOX_WIDTH, DIFF_BOX_HEIGHT});
    easyBox.setPosition({startX, DIFF_BOX_Y});
    mediumBox.setPosition({startX + DIFF_BOX_WIDTH + DIFF_BOX_GAP, DIFF_BOX_Y});
    hardBox.setPosition({startX + 2.f * (DIFF_BOX_WIDTH + DIFF_BOX_GAP), DIFF_BOX_Y});
}

void Game::positionVolumeHandle() {
    float t = volume / 100.f;
    float x = VOLUME_TRACK_X_START + t * (VOLUME_TRACK_X_END - VOLUME_TRACK_X_START);
    volumeHandle.setPosition({x - VOLUME_HANDLE_RADIUS,
                              VOLUME_TRACK_Y + VOLUME_TRACK_HEIGHT / 2.f - VOLUME_HANDLE_RADIUS});
}

void Game::updateVolumeFromMouseX(float mouseX) {
    float x = std::clamp(mouseX, VOLUME_TRACK_X_START, VOLUME_TRACK_X_END);
    float t = (x - VOLUME_TRACK_X_START) / (VOLUME_TRACK_X_END - VOLUME_TRACK_X_START);
    volume = static_cast<int>(std::round(t * 100.f));
    positionVolumeHandle();
    applyVolume();
    saveSettings();
}

void Game::handleSettingsClick(sf::Vector2f point) {
    if (isButtonClicked(backButton, point)) {
        state = GameState::Menu;
        return;
    }

    if (easyBox.getGlobalBounds().contains(point)) {
        difficulty = Constants::Difficulty::Easy;
        saveSettings();
        return;
    }
    if (mediumBox.getGlobalBounds().contains(point)) {
        difficulty = Constants::Difficulty::Medium;
        saveSettings();
        return;
    }
    if (hardBox.getGlobalBounds().contains(point)) {
        difficulty = Constants::Difficulty::Hard;
        saveSettings();
        return;
    }

    // Volume slider: clicking anywhere near the track jumps the handle there
    // and starts a drag.
    sf::FloatRect trackBounds = volumeTrack.getGlobalBounds();
    sf::FloatRect grabArea(
        {trackBounds.position.x - VOLUME_HANDLE_RADIUS, trackBounds.position.y - 14.f},
        {trackBounds.size.x + 2.f * VOLUME_HANDLE_RADIUS, trackBounds.size.y + 28.f});

    if (grabArea.contains(point)) {
        draggingVolumeHandle = true;
        updateVolumeFromMouseX(point.x);
    }
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
        // minimum clear distance from every platform/monster/hole already placed.
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

        // Monsters/holes are optional hazards placed near the new platform,
        // never blocking it - the player always has room to dodge sideways.
        // They are also withheld entirely until the player has climbed past
        // the safe-start zone, so nothing can appear right on top of the
        // player the moment a session begins.
        float heightClimbed = startPlatformY - pos.y;
        if (heightClimbed >= Constants::SAFE_START_HEIGHT) {
            trySpawnMonster(pos.x, pos.y);
            if (holesEnabled())
                trySpawnHole(pos.x, pos.y);
        }
    }
}

sf::Vector2f Game::reachablePlatformPos(float fromX, float fromY) {
    float gapMin = Constants::MIN_PLATFORM_GAP;
    float gapMax = Constants::SAFE_MAX_PLATFORM_GAP;

    // Medium/Hard: bias toward the upper part of the safe range so platforms
    // end up more spread out (fewer of them) while remaining reachable.
    if (difficulty != Constants::Difficulty::Easy)
        gapMin = gapMin + (gapMax - gapMin) * Constants::REDUCED_DENSITY_GAP_BIAS;

    std::uniform_real_distribution<float> gapDist(gapMin, gapMax);

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
        p = new MovingPlatform(x, y, movingPlatformSpeedMultiplier());
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

int Game::currentMonsterHealth() const {
    return (difficulty == Constants::Difficulty::Easy)
        ? Constants::MONSTER_HEALTH_EASY
        : Constants::MONSTER_HEALTH_HARDER;
}

float Game::movingPlatformSpeedMultiplier() const {
    return (difficulty == Constants::Difficulty::Easy)
        ? 1.f
        : Constants::MOVING_PLATFORM_SPEED_MULT_HARDER;
}

float Game::currentShootCooldown() const {
    return (difficulty == Constants::Difficulty::Easy)
        ? Constants::SHOOT_COOLDOWN_EASY
        : Constants::SHOOT_COOLDOWN_HARDER;
}

bool Game::holesEnabled() const {
    return difficulty == Constants::Difficulty::Hard;
}

void Game::trySpawnMonster(float nearX, float nearY) {
    std::uniform_real_distribution<float> chance(0.f, 1.f);
    if (chance(gen) >= Constants::MONSTER_SPAWN_CHANCE)
        return;

    std::uniform_real_distribution<float> offsetX(-90.f, 90.f);
    std::uniform_real_distribution<float> offsetY(-60.f, 60.f);
    float maxX = Constants::WINDOW_WIDTH - Constants::MONSTER_WIDTH;

    for (int attempt = 0; attempt < Constants::PLATFORM_PLACEMENT_ATTEMPTS; attempt++) {
        float x = std::clamp(nearX + offsetX(gen), 0.f, maxX);
        float y = nearY + offsetY(gen);
        if (isAreaClear(x, y, Constants::MONSTER_WIDTH, Constants::MONSTER_HEIGHT)) {
            bool blueVariant = chance(gen) < 0.5f;
            monsters.push_back(new Monster(x, y, currentMonsterHealth(),
                                            Constants::MONSTER_SPEED, blueVariant));
            return;
        }
    }
}

void Game::trySpawnHole(float nearX, float nearY) {
    std::uniform_real_distribution<float> chance(0.f, 1.f);
    if (chance(gen) >= Constants::HOLE_SPAWN_CHANCE)
        return;

    bool big = chance(gen) < 0.4f;
    float size = big ? Constants::HOLE_BIG_SIZE : Constants::HOLE_SMALL_SIZE;

    std::uniform_real_distribution<float> offsetX(-100.f, 100.f);
    std::uniform_real_distribution<float> offsetY(-70.f, 70.f);
    float maxX = Constants::WINDOW_WIDTH - size;

    for (int attempt = 0; attempt < Constants::PLATFORM_PLACEMENT_ATTEMPTS; attempt++) {
        float x = std::clamp(nearX + offsetX(gen), 0.f, maxX);
        float y = nearY + offsetY(gen);
        if (isAreaClear(x, y, size, size)) {
            holes.push_back(new Hole(x, y, big ? Hole::Size::Big : Hole::Size::Small));
            return;
        }
    }
}

void Game::regenMonster(Monster* m) {
    Platform* anchor = highestSolidPlatform();
    sf::Vector2f anchorPos = anchor ? anchor->getPosition() : findTheHighestPlatPos(platforms);

    std::uniform_real_distribution<float> offsetX(-90.f, 90.f);
    std::uniform_real_distribution<float> offsetY(-140.f, -20.f);
    float maxX = Constants::WINDOW_WIDTH - Constants::MONSTER_WIDTH;

    sf::Vector2f pos(std::clamp(anchorPos.x, 0.f, maxX), anchorPos.y - 80.f);
    for (int attempt = 0; attempt < Constants::PLATFORM_PLACEMENT_ATTEMPTS; attempt++) {
        float x = std::clamp(anchorPos.x + offsetX(gen), 0.f, maxX);
        float y = anchorPos.y + offsetY(gen);
        if (isAreaClear(x, y, Constants::MONSTER_WIDTH, Constants::MONSTER_HEIGHT, nullptr, m)) {
            pos = {x, y};
            break;
        }
    }
    m->reset(pos.x, pos.y, currentMonsterHealth(), Constants::MONSTER_SPEED);
}

void Game::respawnDeadMonsters() {
    for (Monster* m : monsters) {
        if (m->isDead())
            regenMonster(m);
    }
}

void Game::regenHole(Hole* h) {
    Platform* anchor = highestSolidPlatform();
    sf::Vector2f anchorPos = anchor ? anchor->getPosition() : findTheHighestPlatPos(platforms);

    std::uniform_real_distribution<float> chance(0.f, 1.f);
    bool big = chance(gen) < 0.4f;
    float size = big ? Constants::HOLE_BIG_SIZE : Constants::HOLE_SMALL_SIZE;
    Hole::Size sizeEnum = big ? Hole::Size::Big : Hole::Size::Small;

    std::uniform_real_distribution<float> offsetX(-100.f, 100.f);
    std::uniform_real_distribution<float> offsetY(-140.f, -20.f);
    float maxX = Constants::WINDOW_WIDTH - size;

    sf::Vector2f pos(std::clamp(anchorPos.x, 0.f, maxX), anchorPos.y - 80.f);
    for (int attempt = 0; attempt < Constants::PLATFORM_PLACEMENT_ATTEMPTS; attempt++) {
        float x = std::clamp(anchorPos.x + offsetX(gen), 0.f, maxX);
        float y = anchorPos.y + offsetY(gen);
        if (isAreaClear(x, y, size, size, nullptr, nullptr, h)) {
            pos = {x, y};
            break;
        }
    }
    h->reset(pos.x, pos.y, sizeEnum);
}

void Game::handleShooting(float dt) {
    if (player.updateShootTimer(dt, currentShootCooldown())) {
        sf::Vector2f muzzle = player.getMuzzlePosition();
        bullets.push_back(new Bullet(muzzle.x, muzzle.y));
        shootSound.play();
    }
}

void Game::cleanupBullets() {
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        if ((*it)->isDead()) {
            delete *it;
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::startGame() {
    player.initPlayer();
    clearPlatforms();
    clearMonsters();
    clearBullets();
    clearHoles();
    genPlatforms();
    distanceTraveled = 0;
    state = GameState::Playing;
}

void Game::run() {
    while (window.isOpen()) {

        processEvents();
        float dt = clock.restart().asSeconds();

        updateMenuMusic();

        if (state == GameState::Playing) {
            update(dt);

            if (player.isShrinking() || player.isConsumed()) {
                if (player.isConsumed())
                    triggerGameOver();
            } else {
                handle_scroll();

                CollisionSystem::resolvePlayerHoles(player, holes);

                CollisionSystem::resolvePlayerSpring(player, platforms);

                bool bounced = CollisionSystem::resolvePlayerPlatforms(player, platforms);
                if (bounced)
                    jumpSound.play();

                Monster::CollisionResult monsterResult =
                    CollisionSystem::resolvePlayerMonsters(player, monsters);
                if (monsterResult == Monster::CollisionResult::Kill)
                    triggerGameOver();

                CollisionSystem::resolveBulletsMonsters(bullets, monsters);
                respawnDeadMonsters();
                cleanupBullets();

                handleShooting(dt);
            }
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
            else if (state == GameState::Settings) {
                if (key->code == sf::Keyboard::Key::Escape)
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
                else if (state == GameState::Settings)
                    handleSettingsClick(point);
            }
        }

        if (const auto* release = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (release->button == sf::Mouse::Button::Left)
                draggingVolumeHandle = false;
        }

        if (const auto* moved = event->getIf<sf::Event::MouseMoved>()) {
            if (state == GameState::Settings && draggingVolumeHandle)
                updateVolumeFromMouseX(static_cast<float>(moved->position.x));
        }
    }

    if (state == GameState::Playing)
        player.handle_input();
}

void Game::handleMenuClick(sf::Vector2f point) {
    if (isButtonClicked(startButton, point))
        startGame();
    else if (isButtonClicked(settingsButton, point))
        state = GameState::Settings;
}

void Game::handleGameOverClick(sf::Vector2f point) {
    if (isButtonClicked(restartButton, point))
        startGame();                  // play again
    else if (isButtonClicked(menuButton, point))
        state = GameState::Menu;       // back to main menu
}

void Game::update(float dt) {
    player.update(dt);

    if (player.isShrinking() || player.isConsumed()) {
        // The player is being swallowed by a hole: freeze the entire world
        // (no scrolling, no moving platforms/monsters/bullets) so the only
        // thing happening is the shrink/suck-in animation itself, pulling
        // the player toward the hole's position exactly as it was when
        // caught.
        return;
    }

    for (Platform* plat : platforms) {
        plat->update(dt);
    }
    for (Monster* m : monsters) {
        m->update(dt);
    }
    for (Bullet* b : bullets) {
        b->update(dt);
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

    if (state == GameState::Settings) {
        renderSettings();
        window.display();
        return;
    }

    for (auto* hole : holes) {
        hole->render(window);
    }
    for (auto* platform : platforms) {
        platform->render(window);
    }
    for (auto* monster : monsters) {
        monster->render(window);
    }
    for (auto* bullet : bullets) {
        bullet->render(window);
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
    drawTextAt(msg, size, color, Constants::WINDOW_WIDTH / 2.f, y);
}

void Game::drawTextAt(const std::string& msg, unsigned size,
                      sf::Color color, float x, float y) {
    uiText.setString(msg);
    uiText.setCharacterSize(size);
    uiText.setFillColor(color);
    Utils::centerTextOrigin(uiText);
    uiText.setPosition({x, y});
    window.draw(uiText);
}

void Game::renderMenu() {
    drawCenteredText("DOODLE JUMP", 48, COLOR_PRIMARY, MENU_TITLE_Y);
    drawCenteredText("HIGH SCORE (" + difficultyName(difficulty) + "): " + to_string(currentHighScore()),
                      28, COLOR_TEXT, MENU_HIGHSCORE_Y);

    placeButtonCentered(startButton, MENU_START_BUTTON_Y);
    window.draw(startButton);

    placeButtonCentered(settingsButton, MENU_SETTINGS_BUTTON_Y);
    window.draw(settingsButton);

    drawCenteredText("Mode: " + difficultyName(difficulty), 20, COLOR_TEXT, MENU_MODE_Y);
    drawCenteredText("Use Left / Right arrows to move", 18, COLOR_TEXT, MENU_INSTRUCTIONS_Y);
    drawCenteredText("Hold Space to shoot the monsters", 18, COLOR_TEXT, MENU_INSTRUCTIONS2_Y);
}

void Game::renderGameOver() {
    // Dim the frozen play field so the message reads clearly.
    sf::RectangleShape overlay({Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT});
    overlay.setFillColor(sf::Color(255, 255, 255, 190));
    window.draw(overlay);

    drawCenteredText("YOU LOST", 54, sf::Color::Red, 210.f);
    drawCenteredText("SCORE: " + to_string(player.getScore()), 30, COLOR_TEXT, 310.f);
    drawCenteredText("HIGH SCORE (" + difficultyName(difficulty) + "): " + to_string(currentHighScore()),
                      28, COLOR_TEXT, 360.f);

    placeButtonCentered(restartButton, GAMEOVER_RESTART_BUTTON_Y);
    placeButtonCentered(menuButton, GAMEOVER_MENU_BUTTON_Y);
    window.draw(restartButton);
    window.draw(menuButton);
}

void Game::renderSettings() {
    drawCenteredText("SETTINGS", 44, COLOR_PRIMARY, SETTINGS_TITLE_Y);

    drawCenteredText("Volume", 24, COLOR_TEXT, VOLUME_LABEL_Y);
    window.draw(volumeTrack);
    window.draw(volumeHandle);
    drawCenteredText(to_string(volume) + "%", 20, COLOR_TEXT, VOLUME_VALUE_Y);

    drawCenteredText("Difficulty", 24, COLOR_TEXT, DIFFICULTY_LABEL_Y);

    auto drawDiffBox = [&](sf::RectangleShape& box, const std::string& label,
                           Constants::Difficulty d) {
        bool selected = (difficulty == d);
        box.setFillColor(selected ? COLOR_PRIMARY : sf::Color(224, 224, 224));
        box.setOutlineThickness(2.f);
        box.setOutlineColor(COLOR_PRIMARY);
        window.draw(box);

        sf::FloatRect b = box.getGlobalBounds();
        drawTextAt(label, 20, selected ? sf::Color::White : COLOR_TEXT,
                   b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f);
    };

    drawDiffBox(easyBox, "EASY", Constants::Difficulty::Easy);
    drawDiffBox(mediumBox, "MEDIUM", Constants::Difficulty::Medium);
    drawDiffBox(hardBox, "HARD", Constants::Difficulty::Hard);

    placeButtonCentered(backButton, SETTINGS_BACK_BUTTON_Y);
    window.draw(backButton);
}

void Game::handle_scroll() {
    if (player.getPosition().y < Constants::SCREEN_MIDDLE) {
        float heightDiff = Constants::SCREEN_MIDDLE - player.getPosition().y;
        distanceTraveled += heightDiff;
        player.addScore(heightDiff);
        scrollWorld(heightDiff);
        player.setPosition(player.getPosition().x, Constants::SCREEN_MIDDLE);
    }
}

void Game::scrollWorld(float heightDiff) {
    for (auto* plat : platforms) {
        if (heightDiff > 0)
            plat->move(0, heightDiff);

        if (plat->getPosition().y > Constants::WINDOW_HEIGHT)
            regenPlatform(plat, platforms);
    }

    for (auto* m : monsters) {
        if (heightDiff > 0)
            m->move(0, heightDiff);

        if (m->getPosition().y > Constants::WINDOW_HEIGHT)
            regenMonster(m);
    }

    for (auto* h : holes) {
        if (heightDiff > 0)
            h->move(0, heightDiff);

        if (h->getPosition().y > Constants::WINDOW_HEIGHT)
            regenHole(h);
    }

    for (auto* b : bullets) {
        if (heightDiff > 0)
            b->move(0, heightDiff);
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

bool Game::isAreaClear(float x, float y, float width, float height,
                        Platform* excludePlat, Monster* excludeMonster, Hole* excludeHole) {
    // Expand the candidate box by the required minimum distance on every
    // side. If that padded box touches any other platform/monster/hole, they
    // are too close, so the position is rejected.
    sf::FloatRect candidate(
        {x - Constants::MIN_PLATFORM_DISTANCE, y - Constants::MIN_PLATFORM_DISTANCE},
        {width  + 2.f * Constants::MIN_PLATFORM_DISTANCE,
         height + 2.f * Constants::MIN_PLATFORM_DISTANCE});

    for (Platform* p : platforms) {
        if (p == excludePlat)
            continue;
        if (candidate.findIntersection(p->getBounds()).has_value())
            return false;
    }
    for (Monster* m : monsters) {
        if (m == excludeMonster)
            continue;
        if (candidate.findIntersection(m->getGlobalBounds()).has_value())
            return false;
    }
    for (Hole* h : holes) {
        if (h == excludeHole)
            continue;
        if (candidate.findIntersection(h->getGlobalBounds()).has_value())
            return false;
    }
    return true;
}

bool Game::isPositionClear(float x, float y, Platform* exclude) {
    return isAreaClear(x, y, Constants::PLATFORM_WIDTH, Constants::PLATFORM_HEIGHT, exclude);
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
    if (player.getPosition().y > Constants::WINDOW_HEIGHT)
        triggerGameOver();
}

void Game::triggerGameOver() {
    if (state == GameState::GameOver)
        return;

    state = GameState::GameOver;
    // Only the high score belonging to the difficulty this session was
    // actually played on gets updated - Easy/Medium/Hard stay independent.
    int idx = static_cast<int>(difficulty);
    if (player.getScore() > highScores[idx]) {
        highScores[idx] = player.getScore();
        saveHighScore();
    }
    loseSound.play();
}

int Game::currentHighScore() const {
    return highScores[static_cast<int>(difficulty)];
}

void Game::loadHighScore() {
    highScores[0] = highScores[1] = highScores[2] = 0;
    std::ifstream in(HIGH_SCORE_FILE);
    if (in.is_open()) {
        int easy, medium, hard;
        if (in >> easy >> medium >> hard) {
            if (easy >= 0)   highScores[0] = easy;
            if (medium >= 0) highScores[1] = medium;
            if (hard >= 0)   highScores[2] = hard;
        }
    }
}

void Game::saveHighScore() {
    std::ofstream out(HIGH_SCORE_FILE);
    if (out.is_open())
        out << highScores[0] << " " << highScores[1] << " " << highScores[2];
}

void Game::loadSettings() {
    volume = Constants::DEFAULT_VOLUME;
    difficulty = Constants::Difficulty::Easy;

    std::ifstream in(SETTINGS_FILE);
    if (in.is_open()) {
        int vol = volume, diff = 0;
        if (in >> vol >> diff) {
            volume = std::clamp(vol, 0, 100);
            if (diff >= 0 && diff <= 2)
                difficulty = static_cast<Constants::Difficulty>(diff);
        }
    }
}

void Game::saveSettings() {
    std::ofstream out(SETTINGS_FILE);
    if (out.is_open())
        out << volume << " " << static_cast<int>(difficulty);
}

void Game::applyVolume() {
    float v = static_cast<float>(volume);
    menuMusic.setVolume(v);
    jumpSound.setVolume(v);
    loseSound.setVolume(v);
    shootSound.setVolume(v);
}
