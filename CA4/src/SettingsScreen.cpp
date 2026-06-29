#include "SettingsScreen.h"

#include "ConfigManager.h"
#include "InputHandler.h"
#include "Player.h"
#include "UIRenderer.h"

namespace {

std::string marker(PlaybackMode current, PlaybackMode option) {
    return current == option ? "  ◀" : "";
}

}  // namespace

SettingsScreen::SettingsScreen(UIRenderer& ui, InputHandler& input,
                               Player& player, ConfigManager& config)
    : Screen(ui, input), player_(player), config_(config) {}

void SettingsScreen::render() {
    PlaybackMode mode = player_.mode();

    ui_.clearScreen();
    ui_.boxTop();
    ui_.boxCentered("⚙  Settings", Color::White);
    ui_.boxSep();
    ui_.boxLine("  Playback Mode  (current: " + toString(mode) + ")",
                Color::White);
    ui_.boxSep();
    ui_.boxLine("  1.  NO_REPEAT     play in order, stop at end" +
                    marker(mode, PlaybackMode::NO_REPEAT),
                mode == PlaybackMode::NO_REPEAT ? Color::Yellow : Color::Gray);
    ui_.boxLine("  2.  REPEAT_ONE    repeat current song forever" +
                    marker(mode, PlaybackMode::REPEAT_ONE),
                mode == PlaybackMode::REPEAT_ONE ? Color::Yellow : Color::Gray);
    ui_.boxLine("  3.  REPEAT_ALL    loop whole playlist" +
                    marker(mode, PlaybackMode::REPEAT_ALL),
                mode == PlaybackMode::REPEAT_ALL ? Color::Yellow : Color::Gray);
    ui_.boxLine("  4.  SHUFFLE       random order" +
                    marker(mode, PlaybackMode::SHUFFLE),
                mode == PlaybackMode::SHUFFLE ? Color::Yellow : Color::Gray);
    ui_.boxSep();
    ui_.boxLine("  0.  Back (changes saved automatically)", Color::Dim);
    ui_.boxBottom();
}

ScreenId SettingsScreen::handleInput() {
    int choice = input_.readInt("Choice: ", 0, 4);
    if (choice == 0) {
        return ScreenId::MainMenu;
    }

    PlaybackMode mode = PlaybackMode::NO_REPEAT;
    switch (choice) {
        case 1: mode = PlaybackMode::NO_REPEAT;  break;
        case 2: mode = PlaybackMode::REPEAT_ONE; break;
        case 3: mode = PlaybackMode::REPEAT_ALL; break;
        case 4: mode = PlaybackMode::SHUFFLE;    break;
    }
    player_.setMode(mode);
    config_.set("playback_mode", toString(mode));
    config_.save();   // applied immediately and persisted
    return ScreenId::None;
}
