#ifndef __LAYOUT__
#define __LAYOUT__
#include <vector>
#include "GObj.h"

namespace LayOut
{
    template <typename SizOPosable>
    void centerVertical(
        const std::vector<std::reference_wrapper<SizOPosable>> objects,
        sf::FloatRect area,
        float spacing = 0.f)
    {
        float totalHeight = 0.f;

        for (auto &object : objects)
        {
            auto bounds = object.get().getGlobalBounds();
            totalHeight += bounds.size.y;
        }

        totalHeight += spacing * (objects.size() - 1);

        float currentY =
            area.position.y + (area.size.y - totalHeight) / 2.f;

        for (auto &obj : objects)
        {
            auto bounds = obj.get().getGlobalBounds();

            obj.get().setPosition({area.position.x + (area.size.x - bounds.size.x) / 2.f,
                                   currentY});

            currentY += bounds.size.y + spacing;
        }
    }
    template <typename SizOPosable>
    void centerHorizontal(
        const std::vector<std::reference_wrapper<SizOPosable>> objects,
        sf::FloatRect area,
        float spacing = 0.f)
    {
        float totalWidth = 0.f;

        for (auto &object : objects)
        {
            auto bounds = object.get().getGlobalBounds();
            totalWidth += bounds.size.x;
        }

        totalWidth += spacing * (objects.size() - 1);

        float currentX =
            area.position.x + (area.size.x - totalWidth) / 2.f;

        for (auto &obj : objects)
        {
            auto bounds = obj.get().getGlobalBounds();

            obj.get().setPosition({currentX,
                                   area.position.y + (area.size.y - bounds.size.y) / 2.f});

            currentX += bounds.size.x + spacing;
        }
    }
    template <typename SizOPosable>
    void centerVertical(
        const std::vector<std::reference_wrapper<SizOPosable>> objs,
        sf::Vector2f win_size,
        float spacing = 0.f)
    {
        sf::FloatRect area({0.f, 0.f}, win_size);
        centerVertical<SizOPosable>(objs, area, spacing);
    }

    template <typename SizOPosable>
    void centerVertical(
        const std::vector<std::reference_wrapper<SizOPosable>> objs,
        sf::Vector2u win_size,
        float spacing = 0.f)
    {
        centerVertical<SizOPosable>(objs, sf::Vector2f(win_size), spacing);
    }

    template <typename Scalable>
    void fill(
        Scalable &object,
        sf::Vector2f win_size,
        float x_percentage = 1.f,
        float y_percentage = 1.f)
    {
        auto bounds = object.getLocalBounds();

        if (bounds.size.x == 0.f || bounds.size.y == 0.f)
            return;

        object.setScale({(win_size.x * x_percentage) / bounds.size.x,
                         (win_size.y * y_percentage) / bounds.size.y});
    }
    template <typename Scalable>
    void fill(
        Scalable &object,
        sf::Vector2u win_size,
        float x_percentage = 1.f,
        float y_percentage = 1.f)
    {
        fill(object, sf::Vector2f(win_size), x_percentage, y_percentage);
    }

    template <typename Vec2>
    void fitText(
        sf::Text &text,
        Vec2 win_size,
        float percentage = 1.f)
    {
        int approx_size = static_cast<int>(win_size.y * percentage);
        text.setCharacterSize(approx_size);
    }
};

#endif