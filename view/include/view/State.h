//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_STATE_H
#define PACMANGAME_STATE_H

#pragma once

class State {
public:
    virtual ~State() = default;
    virtual void update(float dt) = 0;
    virtual void render() = 0;
};

#endif //PACMANGAME_STATE_H