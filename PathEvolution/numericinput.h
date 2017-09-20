#ifndef NUMERICINPUT_H
#define NUMERICINPUT_H

#include <SFML/Graphics.hpp>

class NumericInput : public sf::Drawable
{
private:
    bool focused = false;
    bool seenDot = false;
    std::string textualValue = "0";
    sf::RectangleShape rectangle;
    sf::Text text;
    sf::RectangleShape bounds;

public:
    NumericInput(float width);

    void processEvent(const sf::Event& event);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void setPosition(const sf::Vector2f& pos);
};

#endif // NUMERICINPUT_H
