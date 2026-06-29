#include "MainMenuScreen.h"

#include "InputHandler.h"
#include "Player.h"
#include "Song.h"
#include "UIRenderer.h"

MainMenuScreen::MainMenuScreen(UIRenderer& ui, InputHandler& input,
                               Player& player)
    : Screen(ui, input), player_(player) {}

void MainMenuScreen::render() {
    ui_.clearScreen();
    ui_.boxTop();
    ui_.boxCentered("♫  Terminal Music Player  ♫", Color::White);
    ui_.boxSep();

    Song* last = player_.currentSong();
    if (last != nullptr) {
        ui_.boxLine("  Last played: " + last->title() + " - " +
                        last->artist(),
                    Color::Gray);
        ui_.boxSep();
    }

    ui_.boxLine("  1.  Now Playing", Color::White);
    ui_.boxLine("  2.  Playlists", Color::White);
    ui_.boxLine("  3.  Browse Playlist", Color::White);
    ui_.boxLine("  4.  Settings", Color::White);
    ui_.boxSep();
    ui_.boxLine("  0.  Quit (saves state)", Color::Dim);
    ui_.boxBottom();
}

ScreenId MainMenuScreen::handleInput() {
    int choice = input_.readInt("Enter choice: ", 0, 4);
    switch (choice) {
        case 1: return ScreenId::NowPlaying;
        case 2: return ScreenId::PlaylistList;
        case 3: return ScreenId::PlaylistView;
        case 4: return ScreenId::Settings;
        case 0: return ScreenId::Quit;
    }
    return ScreenId::None;
}
