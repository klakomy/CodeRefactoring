#include "Torus.hpp"
#include "IPort.hpp"
#include "EventT.hpp"

#include "SnakeInterface.hpp"
#include "SnakeSegments.hpp"
namespace Snake
{
Torus::Torus(IPort& displayPort, IPort& foodPort, Dimension dimension, Position food)

    : m_displayPort(displayPort),
    m_foodPort(foodPort),
    m_foodPosition(food),
    m_dimension(dimension)
{

}

bool Torus::contains(Position position) const
{
    return true;
}

void Torus::updateFoodPosition(Position position, const Segments &segments)
{
    updateFoodPositionWithCleanPolicy(position, segments, std::bind(&Torus::sendClearOldFood, this));
}

bool Torus::eatFood(Position position) const
{
    bool eaten = (m_foodPosition == position);
    if (eaten) {
        m_foodPort.send(std::make_unique<EventT<FoodReq>>());
    }
    return eaten;
}

void Torus::placeFood(Position position, const Segments &segments)
{
    static auto noCleanPolicy = []{};
    updateFoodPositionWithCleanPolicy(position, segments, noCleanPolicy);
}

void Torus::sendPlaceNewFood(Position position)
{
    m_foodPosition = position;

    DisplayInd placeNewFood;
    placeNewFood.position = position;
    placeNewFood.value = Cell_FOOD;

    m_displayPort.send(std::make_unique<EventT<DisplayInd>>(placeNewFood));
}

void Torus::sendClearOldFood()
{
    DisplayInd clearOldFood;
    clearOldFood.position = m_foodPosition;
    clearOldFood.value = Cell_FREE;

    m_displayPort.send(std::make_unique<EventT<DisplayInd>>(clearOldFood));
}

void Torus::updateFoodPositionWithCleanPolicy(Position position, const Segments &segments, std::function<void ()> clearPolicy)
{
    if (segments.isCollision(position) or not contains(position)) {
        m_foodPort.send(std::make_unique<EventT<FoodReq>>());
        return;
    }

    clearPolicy();
    sendPlaceNewFood(position);
}

Position Torus::correctPosition(Position position) const
{

}
}
