//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_CAMERA_H
#define PACMANGAME_CAMERA_H


#pragma once

#include <SFML/Graphics.hpp>

class Camera {
public:
    Camera(int pixelWidth, int pixelHeight);
    sf::FloatRect worldToPixels(double x, double y, double w, double h)
    const;
private:
    int wPixels;
    int hPixels;
};


#endif //PACMANGAME_CAMERA_H