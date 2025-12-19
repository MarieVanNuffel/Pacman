//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_CAMERA_H
#define PACMANGAME_CAMERA_H


#pragma once

#include <SFML/Graphics.hpp>

class Camera {
public:
    Camera(int pixelWidth, int pixelHeight, int mazeW, int mazeH);

    sf::FloatRect worldToPixels(double x, double y, double w, double h) const;

    int mazeWidth;
    int mazeHeight;

private:
    int wPixels;
    int hPixels;

    // computed values
    float cellSize;   // square size of one maze cell in pixels
    float offsetX;    // left margin to center the maze
    float offsetY;    // top margin to center the maze

    void recompute();
};



#endif //PACMANGAME_CAMERA_H