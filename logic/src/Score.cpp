//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "../include/logic/Score.h"
#include <fstream>


Score::Score() {
    loadHighScores();
}


void Score::coinCollected() {
    score += 10;
}
void Score::fruitCollected() { score += 100; }
void Score::pacmanDied() { score -= 50; }
void Score::ghostEaten() { score += 200; }


void Score::loadHighScores() {
    // minimal implementation - try to open file
    highScores = {0,0,0,0,0};
}
void Score::saveHighScores() {
    // write to file
}