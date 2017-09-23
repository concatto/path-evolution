#include "weightedbinaryselector.h"

WeightedBinarySelector::WeightedBinarySelector()
{
    input.setValue(1);
}

float WeightedBinarySelector::getWeight() const
{
    return input.getValue();
}

void WeightedBinarySelector::setWidth(float width)
{
    BinarySelector::setWidth(width);
    input.setWidth(width * 0.333);
}

void WeightedBinarySelector::setPosition(const sf::Vector2f& pos)
{
    BinarySelector::setPosition(pos);

    float margin = 10;
    sf::Vector2f backgroundSize = getBackground().getSize();
    sf::Vector2f inputSize = input.getBackground().getSize();

    input.setPosition(pos + sf::Vector2f(backgroundSize.x - inputSize.x - margin, margin));
}

void WeightedBinarySelector::processEvent(const sf::Event& event)
{
    BinarySelector::processEvent(event);
    input.processEvent(event);
}

void WeightedBinarySelector::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    BinarySelector::draw(target, states);
    target.draw(input);
}


