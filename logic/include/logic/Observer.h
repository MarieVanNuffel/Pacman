//
// Created by Marie Van Nuffel on 14/12/2025.
//

#ifndef PACMANGAME_OBSERVER_H
#define PACMANGAME_OBSERVER_H


#pragma once

namespace logic {

    /**
     * @brief Observer interface
     *
     * Ontvangt events van Subjects.
     */
    class Observer {
    public:
        virtual ~Observer() = default;

        /**
         * @brief Wordt aangeroepen bij een event
         * @param event Event id
         */
        virtual void onNotify(int event) = 0;
    };
}


#endif //PACMANGAME_OBSERVER_H