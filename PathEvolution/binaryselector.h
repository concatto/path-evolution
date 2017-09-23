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

    sf::Text title;
    sf::Text left;
    sf::Text right;
    sf::Sprite leftSprite;
    sf::Sprite rightSprite;
    sf::RectangleShape background;
    bool leftActive = true;

public:
    BinarySelector();

    void setLeftString(const std::wstring& str);
    void setRightString(const std::wstring& str);
    void setTitle(const std::wstring& title);
    bool isLeftActive() const;
    void setLeftActive(bool value);
    void setBackgroundColor(const sf::Color& color);
    virtual void setWidth(float width);
    virtual void setPosition(const sf::Vector2f& pos);
    virtual void processEvent(const sf::Event& event);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    const sf::RectangleShape& getBackground() const;
};

#endif // BINARYSELECTOR_H
