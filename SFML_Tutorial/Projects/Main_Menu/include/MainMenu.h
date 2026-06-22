#ifndef __MAIN_MENU__
#define __MAIN_MENU__
#include "Button.h"
#include "GObj.h"
#include "Transition.h"
#include "ResourceManager.h"

class MainMenu
{
private:
    GObj back_ground;
    Button start;
    Button load;
    Button exit;

    Transition transition;

    void onMouseButtonReleased(const sf::Event::MouseButtonReleased &);

public:
    MainMenu(ResourceManager &, sf::Vector2u);
    void handleEvent(const sf::Event &);
    void draw(sf::RenderWindow &);
    Transition getTransition() const;
};

#endif