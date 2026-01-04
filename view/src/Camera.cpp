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
        // x,y,w,h are in maze tile units. Use cellSize and offsets to convert to pixels.
        float px = offsetX + float(x) * cellSize;
        float py = offsetY + float(y) * cellSize;
        float pw = float(w) * cellSize;
        float ph = float(h) * cellSize;

        return {px, py, pw, ph};
    }

    void Camera::recompute() {
        // compute cell sizes per axis, pick the smallest to keep cells square
        float cellW = float(wPixels) / std::max(1, mazeWidth);
        float cellH = float(hPixels) / std::max(1, mazeHeight);

        cellSize = std::min(cellW, cellH);

        // center the maze in window (letterbox)
        float totalW = cellSize * mazeWidth;
        float totalH = cellSize * mazeHeight;

        offsetX = (wPixels - totalW) / 2.0f;
        offsetY = (hPixels - totalH) / 2.0f;
    }
}
