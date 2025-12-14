//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "../include/view/Camera.h"


Camera::Camera(int pixelW, int pixelH, int mazeW, int mazeH)
    : wPixels(pixelW), hPixels(pixelH), mazeWidth(mazeW), mazeHeight(mazeH) {}


sf::FloatRect Camera::worldToPixels(double x, double y, double w, double h) const {
    float cellW = float(wPixels) / mazeWidth;
    float cellH = float(hPixels) / mazeHeight;

    float px = float(x * cellW);
    float py = float(y * cellH);
    float pw = float(w * cellW);
    float ph = float(h * cellH);

    return {px, py, pw, ph};
}





