#ifndef PACMANGAME_RESOURCE_H
#define PACMANGAME_RESOURCE_H

#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

namespace view {

/**
 * @brief Beheert het laden en hergebruiken van de spritesheet.
 *
 * Deze klasse wordt gebruikt zodat de spritesheet maar 1 keer geladen wordt ipv
 * voor elke sprite opnieuw. Dit gebeurt met een shared pointer zodat meerdere
 * views dezelfde kunnen gebruiken.
 */
class Resource {
public:
  /**
   * @brief Laadt een texture uit een bestand
   *
   * Laadt de spritesheet vanuit een pad en desnoods uit de cache. Als de
   * texture al eens eerder werd geladen, wordt een shared pointer naar de
   * bestaande texture teruggegeven. De shared_ptr zorgt er ook voor dat de
   * texture in het geheugen blijft zolang minstens 1 sprite ernaar verwijst.
   *
   * De functie is thread-safe door de lock.
   *
   * @param path Pad naar het texturebestand.
   * @return std::shared_ptr<sf::Texture> pointer indien het laden succesvol
   * was, anders nullptr
   */
  static std::shared_ptr<sf::Texture> loadTexture(const std::string &path);
};

} // namespace view

#endif // PACMANGAME_RESOURCE_H