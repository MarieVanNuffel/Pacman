//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "../include/view/FruitView.h"
#include "../include/view/Camera.h"
#include "logic/Events.h"
#include "logic/FruitModel.h"
#include "view/Resource.h"
#include <filesystem>
#include <iostream>

namespace view {
FruitView::FruitView(std::shared_ptr<logic::FruitModel> m)
    : EntityView(m), model(m) {
  texturePtr =
      view::Resource::loadTexture("view/assets/pacman.png"); // spritesheet
  if (texturePtr)
    sprite.setTexture(*texturePtr);

  int spriteX = 0;
  int spriteY = 128;

  sprite.setTextureRect(sf::IntRect(spriteX, spriteY, FRAME_SIZE, FRAME_SIZE));

  // Sprite centreren
  sf::FloatRect bounds = sprite.getLocalBounds();
  sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void FruitView::updateSprite(double dt) {
  // statisch, dus doet niets
}

void FruitView::draw(sf::RenderWindow &win, const Camera &cam) {
  auto m = model.lock();
  if (!m)
    return;
  if (!visible)
    return; // als het fruit verzameld is, is die niet meer zichtbaar dus return

  double sizeW = 0.5; // halve cel
  double sizeH = 0.5;

  // tile center
  double tileCenterX = m->getX();
  double tileCenterY = m->getY();

  // genormalizeerd
  auto [nx, ny] = cam.tileToNormalized(tileCenterX, tileCenterY);

  double wFrac = sizeW / static_cast<double>(cam.mazeWidth);
  double hFrac = sizeH / static_cast<double>(cam.mazeHeight);

  // gebruik normalizedToPixels
  sf::FloatRect rect = cam.normalizedToPixels(nx, ny, wFrac, hFrac);

  // centreren in tile
  sprite.setPosition(rect.left + rect.width / 2.f,
                     rect.top + rect.height / 2.f);

  // schalen
  float sx = rect.width / sprite.getLocalBounds().width;
  float sy = rect.height / sprite.getLocalBounds().height;
  float s = std::min(sx, sy);
  sprite.setScale(s * 1.4, s * 1.4); // iets groter tekenen

  win.draw(sprite); // teken het fruit
}

void FruitView::onNotify(int event) {
  if (event == static_cast<int>(logic::Event::FRUIT_EATEN)) {
    visible = false; // niet meer zichtbaar na gegeten
  } else if (event == static_cast<int>(logic::Event::FRUIT_RESPAWN)) {
    visible = true; // terug zichtbaar bij reset
  }
}
} // namespace view
