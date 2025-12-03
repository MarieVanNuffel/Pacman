//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_SCORE_H
#define PACMANGAME_SCORE_H


#pragma once


#include <vector>
#include <string>


class Score {
public:
    Score();
    void coinCollected();
    void fruitCollected();
    void pacmanDied();
    void ghostEaten();


    int getScore() const { return score; }
    void loadHighScores();
    void saveHighScores();
private:
    int score = 0;
    std::vector<int> highScores;
    double lastCoinTime = 0.0;
};



#endif //PACMANGAME_SCORE_H