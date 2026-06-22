#ifndef __MAIN_MENU__
#define __MAIN_MENU__
#include "Button.h"
#include "Transition.h"
#include "ResourceManager.h"
#include "State.h"
#include "BackGround.h"

class MainMenu : public State
{
private:
    BackGround back_ground;
    Button start;
    Button load;
    Button exit;

    void onMouseButtonReleased(const sf::Event::MouseButtonReleased &);

public:
    MainMenu(ResourceManager &, sf::Vector2u);
    void handleEvent(const sf::Event &) override;
    void draw(sf::RenderWindow &) override;
    Transition getTransition() const;
};

#endif