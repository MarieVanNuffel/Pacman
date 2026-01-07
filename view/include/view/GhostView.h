//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_GHOSTVIEW_H
#define PACMANGAME_GHOSTVIEW_H

#include <SFML/Graphics/RenderWindow.hpp>
#include "Camera.h"
#include "EntityView.h"
#include "logic/Direction.h"

namespace logic {
    class GhostModel;
}

namespace view {

    /**
     * @brief View voor de ghosts
     *
     * GhostView tekent en animeert de ghosts op basis van hun richting, type en modus (chase, fear, eaten).
     */
    class GhostView : public EntityView {
    public:

        /**
         * @brief Constructor
         * @param m GhostModel
         */
        explicit GhostView(std::shared_ptr<logic::GhostModel> m);

        /**
         * @brief Tekent de ghosts
         */
        void draw(sf::RenderWindow& win, const Camera& cam) override;

        /**
         * @brief Update van animatie
         */
        void updateSprite(double dt) override;
    private:
        // Animatie
        double animTimer = 0.0;
        double frameTime = 0.15;

        // Sprite sheet frames
        static constexpr int FRAME_SIZE = 16;
        static constexpr int NUM_FRAMES = 2;
        int frameIndex = 0;
        int frames[NUM_FRAMES] = {0, 16}; // kolommen voor bewegingen

        // Ghosts
        std::weak_ptr<logic::GhostModel> model;
        sf::Sprite sprite;
        std::shared_ptr<sf::Texture> texturePtr;

        logic::Direction lastDirection = logic::Direction::RIGHT;
    };
}

#endif //PACMANGAME_GHOSTVIEW_H