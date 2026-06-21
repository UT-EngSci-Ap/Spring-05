#include "MainMenu.h"
#include "ResourceManaegr.h"

MainMenu::MainMenu(ResourceManager &resource) : back_ground(resource.getTexture("background.png")),
                                                start(resource.getTexture("start_button.png")),
                                                load(resource.getTexture("load_button.png")),
                                                exit(resource.getTexture("exit_button.png")) {}

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

void MainMenu::onMouseButtonReleased(const sf::Event::MouseButtonReleased &mouse)
{
    if (mouse.button == sf::Mouse::Button::Right)
    {
        if (start.isHovered(mouse.position))
        {
            // do nothing
        }
        else if (load.isHovered(mouse.position))
        {
            // do nothing
        }
        else if (load.isHovered(mouse.position))
        {
            transition = EXIT;
        }
    }
}
