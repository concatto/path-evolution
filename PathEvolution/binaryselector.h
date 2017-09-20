#ifndef BINARYSELECTOR_H
#define BINARYSELECTOR_H

#include <SFML/Graphics.hpp>
#include "numericinput.h"

class BinarySelector : public sf::Drawable
{
private:
    static sf::Texture leftTexture;
    static sf::Texture rightTexture;
    static const sf::Color TextColor;

    NumericInput input;
    sf::Text title;
    sf::Text left;
    sf::Text right;
    sf::Sprite leftSprite;
    sf::Sprite rightSprite;
    sf::RectangleShape background;
    bool leftActive = true;

public:
    BinarySelector(float width, const std::wstring& leftStr, const std::wstring& rightStr);

    void setTitle(const std::wstring& title);
    bool isLeftActive() const;
    void setLeftActive(bool value);
    void setPosition(const sf::Vector2f& pos);
    void processEvent(const sf::Event& event);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif // BINARYSELECTOR_H
