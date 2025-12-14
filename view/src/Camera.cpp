//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "../include/view/Camera.h"

Camera::Camera(int pixelWidth, int pixelHeight, int mazeW, int mazeH)
    : wPixels(pixelWidth),
      hPixels(pixelHeight),
      mazeWidth(mazeW),
      mazeHeight(mazeH)
{}


sf::FloatRect Camera::worldToPixels(double x, double y, double w, double h) const {
    float cellW = static_cast<float>(wPixels / mazeWidth);
    float cellH = static_cast<float>(hPixels / mazeHeight);

    float px = static_cast<float>(x * cellW);
    float py = static_cast<float>(y * cellH);
    float pw = static_cast<float>(w * cellW);
    float ph = static_cast<float>(h * cellH);

    return sf::FloatRect(px, py, pw, ph);
}



