//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "logic/World.h"
#include "logic/Stopwatch.h"

namespace logic {

World::World() {}

void World::update() {
    Stopwatch::instance().tick();
    // later: entities update
}

}
