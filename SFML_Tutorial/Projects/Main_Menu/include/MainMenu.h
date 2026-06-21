#ifndef __MAIN_MENU__
#define __MAIN_MENU__
#include "Button.h"
#include "GObj.h"
#include "Transition.h"
#include "ResourceManaegr.h"

class MainMenu
{
private:
    GObj back_ground;
    Button start;
    Button load;
    Button exit;

    Transition transition;

public:
    MainMenu(ResourceManager &);
    void handleEvent(const sf::Event &);
    void draw(sf::RenderWindow &);
    void onMouseButtonReleased(const sf::Event::MouseButtonReleased &);
};

#endif