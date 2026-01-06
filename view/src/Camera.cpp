//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "../include/view/Camera.h"

namespace view {
    Camera::Camera(int pixelW, int pixelH, int mazeW, int mazeH)
        : wPixels(pixelW), hPixels(pixelH), mazeWidth(mazeW), mazeHeight(mazeH) {
        recompute();
    }


    sf::FloatRect Camera::worldToPixels(double x, double y, double w, double h) const {
        // converteren naar pixels + naar midden verschuiven
        float px = offsetX + float(x) * cellSize;
        float py = offsetY + float(y) * cellSize;
        float pw = float(w) * cellSize;
        float ph = float(h) * cellSize;

        return {px, py, pw, ph};
    }

    void Camera::recompute() {
        // berekent cellsize per as
        float cellW = float(wPixels) / std::max(1, mazeWidth); // horizontaal
        float cellH = float(hPixels) / std::max(1, mazeHeight); // verticaal

        cellSize = std::min(cellW, cellH); // kleinste nemen

        float totalW = cellSize * mazeWidth;
        float totalH = cellSize * mazeHeight;

        // centreren
        offsetX = (wPixels - totalW) / 2.0f;
        offsetY = (hPixels - totalH) / 2.0f;
    }
}
