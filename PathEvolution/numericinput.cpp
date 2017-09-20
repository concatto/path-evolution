#include "numericinput.h"
#include "util.h"

NumericInput::NumericInput(float width)
{
    rectangle.setSize(sf::Vector2f(width, 20));
    rectangle.setFillColor(sf::Color(0xFAFAFAFF));
    rectangle.setOutlineColor(sf::Color(0xCCCCCCFF));
    rectangle.setOutlineThickness(1);

    text.setString(textualValue);
    text.setFont(*Util::getFont());
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(40);
    text.setOrigin(0, 0);

    bounds.setFillColor(sf::Color::Transparent);
    bounds.setOutlineColor(sf::Color::Magenta);
    bounds.setOutlineThickness(2);
}

void NumericInput::processEvent(const sf::Event& event)
{
    if (event.type == sf::Event::TextEntered && focused) {
        if (event.text.unicode == '\b') {
            if (!textualValue.empty()) {
                if (textualValue.back() == '.') {
                    seenDot = false;
                }
                textualValue.pop_back();

                if (textualValue.empty()) {
                    textualValue = "0";
                }
            }
        } else {
            if ((event.text.unicode >= '0' && event.text.unicode <= '9')
            ||  (event.text.unicode == '.' && !seenDot)) {
                if (textualValue == "0" && event.text.unicode != '.') {
                    textualValue.pop_back();
                }

                textualValue += event.text.unicode;
            }
            if (event.text.unicode == '.') {
                seenDot = true;
            }
        }

        text.setString(textualValue);
    } else if (event.type == sf::Event::MouseButtonPressed) {
        focused = rectangle.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y);
        if (focused) {
            rectangle.setOutlineColor(sf::Color(0x1C62B9FF));
        } else {
            rectangle.setOutlineColor(sf::Color(0xCCCCCCFF));
        }
    }

    bounds.setSize(sf::Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height));
    bounds.setPosition(text.getGlobalBounds().left, text.getGlobalBounds().top);
}

void NumericInput::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(rectangle);
    target.draw(text);
    target.draw(bounds);
}

void NumericInput::setPosition(const sf::Vector2f& pos)
{
    rectangle.setPosition(pos);
    text.setPosition(pos.x + 2, pos.y);
    bounds.setPosition(text.getGlobalBounds().left, text.getGlobalBounds().top);
}
