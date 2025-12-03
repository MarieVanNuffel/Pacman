//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_RECT_H
#define PACMANGAME_RECT_H

#pragma once

struct Rect {
    double x;
    double y;
    double w;
    double h;


    bool intersects(const Rect& other) const {
        return !(x + w/2 < other.x - other.w/2 || other.x + other.w/2 < x - w/2 ||
        y + h/2 < other.y - other.h/2 || other.y + other.h/2 < y - h/2);
    }
};


#endif //PACMANGAME_RECT_H