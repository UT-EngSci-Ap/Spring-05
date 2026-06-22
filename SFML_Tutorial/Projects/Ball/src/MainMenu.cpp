#include "MainMenu.h"
#include "ResourceManager.h"
#include "LayOut.h"
#include "State.h"
#include <iostream>
MainMenu::MainMenu(ResourceManager &resource, sf::Vector2u w) : State(w),
                                                                back_ground(
                                                                    resource.getTexture("background.png")),
                                                                start(resource.getTexture("start_button.png"),
                                                                      resource.getFont(""),
                                                                      ,
                                                                      "start"),
                                                                load(resource.getTexture("load_button.png")),
                                                                exit(resource.getTexture("exit_button.png"))
{
    LayOut::fill(back_ground, win_size);

    start.setScale({0.5, 0.5});
    load.setScale({0.5, 0.5});
    exit.setScale({0.5, 0.5});
    LayOut::centerVertical<GObj>(
        {start, load, exit},
        win_size,
        0.1);
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
    return transition;
}

void MainMenu::onMouseButtonReleased(const sf::Event::MouseButtonReleased &mouse)
{
    if (mouse.button == sf::Mouse::Button::Left)
    {
        if (start.isHovered(mouse.position))
        {
            std::cout << "on start button\n";
        }
        else if (load.isHovered(mouse.position))
        {
            std::cout << "on load button\n";
        }
        else if (exit.isHovered(mouse.position))
        {
            transition = EXIT;
        }
    }
}
