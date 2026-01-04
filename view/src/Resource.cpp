//
// Created by Marie Van Nuffel on 4/01/2026.
//

#include "../include/view/Resource.h"

namespace view {

    std::shared_ptr<sf::Texture> Resource::loadTexture(const std::string& path) {
        static std::unordered_map<std::string, std::shared_ptr<sf::Texture>> cache;
        static std::mutex mu;
        std::lock_guard lock(mu);

        auto it = cache.find(path);
        if (it != cache.end()) return it->second;

        auto tex = std::make_shared<sf::Texture>();
        if (!tex->loadFromFile(path)) return nullptr;
        cache[path] = tex; // store shared_ptr -> cache owns it
        return tex;
    }

}