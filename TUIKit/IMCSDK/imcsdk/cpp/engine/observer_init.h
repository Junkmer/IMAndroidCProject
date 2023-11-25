//
// Created by Junker Mack on 2023/11/24.
//

#ifndef IMANDROIDCDEMO_OBSERVER_INIT_H
#define IMANDROIDCDEMO_OBSERVER_INIT_H

#include<iostream>
#include<vector>
#include<algorithm>

class Observer {
public:
    virtual void initListener() = 0;
    virtual void unInitListener() = 0;
};

namespace tim {
    class ObserverManager{
    public:
        static ObserverManager& getInstance();

        void addListener(Observer *observer);

        void removeListener(Observer *observer);

        void notifyInit();

        void notifyUnInit();

    private:
        std::vector<Observer*> _listeners;
    };
}

#endif //IMANDROIDCDEMO_OBSERVER_INIT_H
