#ifndef WEIGHTEDBINARYSELECTOR_H
#define WEIGHTEDBINARYSELECTOR_H

#include "numericinput.h"
#include "binaryselector.h"

class WeightedBinarySelector : public BinarySelector
{
private:
    NumericInput input;

public:
    WeightedBinarySelector(int nOptions = 1);
    float getWeight() const;
    virtual void setWidth(float width) override;
    virtual void setPosition(const sf::Vector2f& pos) override;
    virtual void processEvent(const sf::Event &event) override;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif // WEIGHTEDBINARYSELECTOR_H
