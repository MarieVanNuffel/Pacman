//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "../include/view/Camera.h"

Camera::Camera(int pixelWidth, int pixelHeight)
    : wPixels(pixelWidth), hPixels(pixelHeight) {}

sf::FloatRect Camera::worldToPixels(double x, double y, double w, double h) const {
    float px = static_cast<float>((x + 1.0) / 2.0 * wPixels);
    float py = static_cast<float>((y + 1.0) / 2.0 * hPixels);
    float pw = static_cast<float>(w * wPixels);
    float ph = static_cast<float>(h * hPixels);
    return sf::FloatRect(px, py, pw, ph);
}



