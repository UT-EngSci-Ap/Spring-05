#ifndef __MAIN_MENU__
#define __MAIN_MENU__
#include "Button.h"
#include "GObj.h"
#include "Transition.h"
#include "ResourceManaegr.h"
#include "State.h"

class MainMenu : public State
{
private:
    GObj back_ground;
    Button start;
    Button load;
    Button exit;

    void onMouseButtonReleased(const sf::Event::MouseButtonReleased &);

public:
    MainMenu(resource_manager &, sf::Vector2u);
    void handleEvent(const sf::Event &) override;
    void draw(sf::RenderWindow &) override;
    Transition getTransition() const;
};

#endif