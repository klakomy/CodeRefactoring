#pragma once

#include <memory>

class Event;

class IEventHandler
{
public:
    virtual ~IEventHandler() = default;
    virtual void receive(std::shared_ptr<Event>) = 0;
};
