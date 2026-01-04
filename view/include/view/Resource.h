#ifndef PACMANGAME_RESOURCE_H
#define PACMANGAME_RESOURCE_H

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>
#include <SFML/Graphics.hpp>


namespace view {
    class Resource {
    public:
        static std::shared_ptr<sf::Texture> loadTexture(const std::string& path);
    };

}

#endif //PACMANGAME_RESOURCE_H