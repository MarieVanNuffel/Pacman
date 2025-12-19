#include "logic/Score.h"
#include <algorithm>

Score::Score() {
    loadHighScores();
}

void Score::coinCollected(double dt) {
    int points = 10;

    if (dt < 1.0)      points = 30;
    else if (dt < 2.5) points = 20;
    else if (dt < 5.0) points = 15;

    currentScore += points;
}

void Score::fruitCollected() {
    currentScore += 100;
}

void Score::ghostEaten() {
    currentScore += 200;
}

void Score::pacmanDied() {
    currentScore -= 50;
}

void Score::onNotify(int event) {
    // events zonder timing
    if (event == 2) fruitCollected();
    else if (event == 3) ghostEaten();
}

void Score::loadHighScores() {
    highScores = {0,0,0,0,0};
}

void Score::saveHighScores() {
    highScores.push_back(currentScore);
    std::sort(highScores.rbegin(), highScores.rend());
    if (highScores.size() > 5)
        highScores.resize(5);
}
