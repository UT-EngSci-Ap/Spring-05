#include "MainMenu.h"
#include "ResourceManager.h"
#include "GObj.h"
#include "LayOut.h"
#include "State.h"
#include <iostream>
const sf::Color BLUE = sf::Color(0x8DAED6FF);
const sf::Color GREEN = sf::Color(0x59C756FF);
const sf::Color RED = sf::Color(0xD44242FF);
constexpr float MENU_BUTTON_SPACING = 10;
constexpr float BUTTON_FILL_WIDTH_RATIO = 0.2f;
constexpr float BUTTON_FILL_HEIGHT_RATIO = 0.1f;
MainMenu::MainMenu(ResourceManager &resource, sf::Vector2u w)
    : State(w),
      back_ground(resource.getTexture("background.png")),
      start(resource.getTexture("button.png"),
            resource.getFont("Roboto-Black.ttf"),
            BLUE,
            "start"),
      load(resource.getTexture("button.png"),
           resource.getFont("Roboto-Black.ttf"),
           GREEN,
           "load"),
      exit(resource.getTexture("button.png"),
           resource.getFont("Roboto-Black.ttf"),
           RED,
           "exit")
{
    std::cout << "resizing background\n";
    LayOut::fill<BackGround>(back_ground, win_size);

    std::cout << "resizing buttons\n";
    LayOut::fill<Button>(start, win_size, BUTTON_FILL_WIDTH_RATIO, BUTTON_FILL_HEIGHT_RATIO);
    LayOut::fill<Button>(load, win_size, BUTTON_FILL_WIDTH_RATIO, BUTTON_FILL_HEIGHT_RATIO);
    LayOut::fill<Button>(exit, win_size, BUTTON_FILL_WIDTH_RATIO, BUTTON_FILL_HEIGHT_RATIO);

    std::vector<std::reference_wrapper<Button>> objs = {start, load, exit};
    std::cout << "repositioning buttons\n";
    LayOut::centerVertical(objs, win_size, MENU_BUTTON_SPACING);
}
void MainMenu::handleEvent(const sf::Event &event)
{
    if (event.is<sf::Event::Closed>())
    {
        transition = EXIT;
    }
    else if (const auto *e = event.getIf<sf::Event::MouseButtonReleased>())
    {
        onMouseButtonReleased(*e);
    }
}

void MainMenu::draw(sf::RenderWindow &window)
{
    back_ground.draw(window);
    start.draw(window);
    load.draw(window);
    exit.draw(window);
}

Transition MainMenu::getTransition() const
{
    std::cout << "got transition\n";
    return transition;
}

void MainMenu::onMouseButtonReleased(const sf::Event::MouseButtonReleased &mouse)
{
    if (mouse.button == sf::Mouse::Button::Left)
    {
        if (start.isHovered(mouse.position))
        {
            transition = PLAY;
        }
        else if (exit.isHovered(mouse.position))
        {
            transition = EXIT;
        }
    }
}
