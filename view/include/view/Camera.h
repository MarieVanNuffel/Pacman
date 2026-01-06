//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_CAMERA_H
#define PACMANGAME_CAMERA_H


#pragma once

#include <SFML/Graphics.hpp>

namespace view {

    /**
     * @brief Camera voor world naar pixels om te zetten
     *
     * Zet maze-coördinaten om naar pixelcoördinaten en centreert de maze in het venster.
     */
    class Camera {
    public:

        /**
         * @brief Constructor
         * @param pixelWidth Breedte van window in pixels
         * @param pixelHeight Hoogte van window in pixels
         * @param mazeW Breedte van de maze (tiles)
         * @param mazeH Hoogte van de maze (tiles)
         */
        Camera(int pixelWidth, int pixelHeight, int mazeW, int mazeH);

        /**
         * @brief Converteert world-coördinaten naar pixel rectangle.
         * Dit heeft SFML nodig om te plaatsen en tekenen
         * @param x X-positie in world units
         * @param y Y-positie in world units
         * @param w Breedte in world units
         * @param h Hoogte in world units
         * @return SFML Rect in pixelcoördinaten
         */
        sf::FloatRect worldToPixels(double x, double y, double w, double h) const;

        int mazeWidth;
        int mazeHeight;

    private:
        int wPixels;
        int hPixels;

        float cellSize;
        float offsetX;
        float offsetY;

        /**
         * @brief Herberekent cell size en offsets
         */
        void recompute();
    };
}


#endif //PACMANGAME_CAMERA_H