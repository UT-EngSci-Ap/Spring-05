#include "LayOut.h"
#include "GObj.h"

void LayOut::centerVertical(const std::vector<std::reference_wrapper<GObj>> objects, sf::Vector2u winSize, float spacing)
{
    float totalHeight = 0.f;
    for (auto &object : objects)
    {
        totalHeight += object.get().getSize().y;
    }
    totalHeight += spacing * (objects.size() - 1);

    float currentY =
        (static_cast<float>(winSize.y) - totalHeight) / 2.f;

    for (auto &obj : objects)
    {
        auto size = obj.get().getSize();
        obj.get().setPosition({(static_cast<float>(winSize.x) - size.x) / 2.f,
                               currentY});

        currentY += size.y + spacing;
    }
}
void LayOut::fill(GObj &object, sf::Vector2u winSize)
{
    object.setScale({static_cast<float>(winSize.x) / object.getGlobalBounds().size.x,
                     static_cast<float>(winSize.y) / object.getGlobalBounds().size.y});
}