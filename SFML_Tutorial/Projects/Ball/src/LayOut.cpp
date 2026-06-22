#include "LayOut.h"
#include "GObj.h"

template <typename SizOPosable>
void LayOut::centerVertical(const std::vector<std::reference_wrapper<SizOPosable>> objects, sf::Vector2f winSize, float spacing)
{
    float totalHeight = 0.f;
    for (auto &object : objects)
    {
        totalHeight += object.get().getGlobalBounds().size.y;
    }
    totalHeight += spacing * (objects.size() - 1);

    float currentY =
        (winSize.y - totalHeight) / 2.f;

    for (auto &obj : objects)
    {
        auto size = obj.get().getGlobalBounds().size;
        obj.get().setPosition({(winSize.x - size.x) / 2.f,
                               currentY});

        currentY += size.y + spacing;
    }
}

template <typename SizOPosable>
void LayOut::centerVertical(std::vector<std::reference_wrapper<SizOPosable>> objs, sf::Vector2u b, float spacing)
{
    centerVertical<SizOPosable>(objs, Vector2f(win_size), spacing);
}

template <typename Scalable>
void LayOut::fill(Scalable &object, sf::Vector2f winSize, float percentage)
{
    object.setScale({winSize.x * percentage / object.getLocalBounds().size.x,
                     winSize.y * percentage / object.getLocalBounds().size.y});
}

template <typename Scalable>
void LayOut::fill(Scalable &object, sf::Vector2u winSize, float percentage)
{
    LayOut::fill<Scalable>(object, Vector2f(winSzie), percentage)
}