//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_FRUITVIEW_H
#define PACMANGAME_FRUITVIEW_H

#include "EntityView.h"
#include "logic/Observer.h"

namespace logic {
class FruitModel;
}

namespace view {

/**
 * @brief View voor het fruit
 *
 * Tekent het fruit en reageert op events via observer.
 */
class FruitView : public EntityView, public logic::Observer {
public:
  /**
   * @brief Constructor
   * @param m Fruit model
   */
  explicit FruitView(const std::shared_ptr<logic::FruitModel> &m);

  /**
   * @brief Tekent het fruit
   * @param win RenderWindow
   * @param cam Camera voor world to pixel conversie
   */
  void draw(sf::RenderWindow &win, const Camera &cam) override;

  /**
   * @brief Update sprite, maar is beweegt niet dus doet niets.
   * @param dt Delta time
   */
  void updateSprite(double dt) override;

  /**
   * @brief Ontvangt events
   * @param event Event code
   */
  void onNotify(int event) override;

private:
  std::weak_ptr<logic::FruitModel> model;
  sf::Sprite sprite;
  std::shared_ptr<sf::Texture> texturePtr;
  static constexpr int FRAME_SIZE = 15;
  double animTime = 0.0;
  bool visible = true;
};
} // namespace view

#endif // PACMANGAME_FRUITVIEW_H