//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_FRUITVIEW_H
#define PACMANGAME_FRUITVIEW_H

#include "EntityView.h"
#include "logic/FruitModel.h"

class FruitView : public EntityView, public Observer {
public:
    FruitView(FruitModel* m);
    void draw(sf::RenderWindow& win, const Camera& cam) override;
    void updateSprite(double dt) override;

    void onNotify(int event) override;
private:
    FruitModel* model;
    sf::Sprite sprite;
    sf::Texture texture;
    double animTime = 0.0;
    bool visible;
};


#endif //PACMANGAME_FRUITVIEW_H