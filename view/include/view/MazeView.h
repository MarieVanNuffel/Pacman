//
// Created by Marie Van Nuffel on 30/11/2025.
//

#ifndef PACMANGAME_MAZEVIEW_H
#define PACMANGAME_MAZEVIEW_H


#pragma once

#include <SFML/Graphics.hpp>
#include "logic/World.h"
#include "Camera.h"

namespace view {
    class MazeView {
    public:
        explicit MazeView(const logic::World* w);
        void draw(sf::RenderWindow& win, const Camera& cam);

    private:
        const logic::World* world;

        float cellSize = 48.f; // of dynamisch aangepast

        sf::Texture wallTexture;
        sf::Sprite wallSprite;
    };
}


#endif //PACMANGAME_MAZEVIEW_H