#ifndef NUMERICINPUT_H
#define NUMERICINPUT_H

#include <SFML/Graphics.hpp>

class NumericInput : public sf::Drawable
{
private:
    bool focused = false;
    bool seenDot = false;
    std::string textualValue = "0";
    sf::RectangleShape background;
    sf::Text text;
    sf::RectangleShape bounds;
    bool firstInput = false;

public:
    NumericInput();

    void setWidth(float width);
    void setValue(float value);
    float getValue() const;
    void processEvent(const sf::Event& event);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void setPosition(const sf::Vector2f& pos);
    const sf::RectangleShape& getBackground() const;
};

#endif // NUMERICINPUT_H
