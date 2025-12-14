//
// Created by Marie Van Nuffel on 14/12/2025.
//

#ifndef PACMANGAME_OBSERVER_H
#define PACMANGAME_OBSERVER_H


#pragma once
#include <vector>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onNotify(int event) = 0;
};



#endif //PACMANGAME_OBSERVER_H