//
// Created by Marie Van Nuffel on 4/01/2026.
//

#include "../include/view/Resource.h"

#include <mutex>
#include <unordered_map>

namespace view {

std::shared_ptr<sf::Texture> Resource::loadTexture(const std::string &path) {
  static std::unordered_map<std::string, std::shared_ptr<sf::Texture>> cache;
  static std::mutex mu;
  std::lock_guard lock(
      mu); // dit maakt het thread-safe, voor als er ooit vanuit verschillende
           // delen van de code geladen worden

  auto it = cache.find(path);
  if (it != cache.end())
    return it->second;

  auto tex = std::make_shared<sf::Texture>();
  if (!tex->loadFromFile(path))
    return nullptr;
  cache[path] = tex;
  return tex;
}

} // namespace view