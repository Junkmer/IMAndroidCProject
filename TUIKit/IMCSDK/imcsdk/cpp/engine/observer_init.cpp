//
// Created by Junker Mack on 2023/11/25.
//

#include "observer_init.h"

namespace tim {
    static ObserverManager instance;

    ObserverManager &ObserverManager::getInstance() {
        return instance;
    }

    void ObserverManager::addListener(Observer *observer) {
        auto it = std::find(_listeners.begin(), _listeners.end(), observer);
        if (it == _listeners.end()) {
            _listeners.push_back(observer);
        }
    };

    void ObserverManager::removeListener(Observer *observer) {
        auto it = std::find(_listeners.begin(), _listeners.end(), observer);
        if (it != _listeners.end()) {
            _listeners.erase(it);
        }
    }

    void ObserverManager::notifyInit() {
        for (const auto &item: _listeners) {
            item->initListener();
        }
    }

    void ObserverManager::notifyUnInit() {
        for (const auto &item: _listeners) {
            item->unInitListener();
        }
    }


}