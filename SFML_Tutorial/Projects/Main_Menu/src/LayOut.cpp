#include "LayOut.h"
#include "GObj.h"

void LayOut::centerVertical(const std::vector<std::reference_wrapper<GObj>> objects, sf::Vector2u win_size, float spacing)
{
    float total_height = 0.f;
    for (auto &object : objects)
    {
        total_height += object.get().getSize().y;
    }
    total_height += spacing * (objects.size() - 1);

    float current_y =
        (static_cast<float>(win_size.y) - total_height) / 2.f;

    for (auto &obj : objects)
    {
        auto size = obj.get().getSize();
        obj.get().setPosition({(static_cast<float>(win_size.x) - size.x) / 2.f,
                               current_y});

        current_y += size.y + spacing;
    }
}
void LayOut::fill(GObj &object, sf::Vector2u win_size)
{
    object.setScale({static_cast<float>(win_size.x) / object.getGlobalBounds().size.x,
                     static_cast<float>(win_size.y) / object.getGlobalBounds().size.y});
}