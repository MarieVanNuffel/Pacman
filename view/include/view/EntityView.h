//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_ENTITYVIEW_H
#define PACMANGAME_ENTITYVIEW_H


#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Camera.h"

namespace logic {
    class Entity;
}

namespace view {
    class EntityView {
    public:

        /**
        * @brief Basis klasse voor entit views
        *
        * Verbindt een logic Entity met rendering.
        */
        explicit EntityView(std::shared_ptr<logic::Entity> m) : model(m) {}
        virtual ~EntityView() = default;

        /**
         * @brief Tekent entity
         * @param win RenderWindow
         * @param cam Camera voor world to pixel conversie
         */
        virtual void draw(sf::RenderWindow&, const Camera&) = 0;

        /**
         * @brief Update sprite per frame
         * @param dt Delta time
         */
        virtual void updateSprite(double dt) = 0;
    protected:
        std::weak_ptr<logic::Entity> model; // model waarvan de view getekent gaat worden
    };
}


#endif //PACMANGAME_ENTITYVIEW_H