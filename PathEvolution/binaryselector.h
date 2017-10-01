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

    float verticalSpacing;
    sf::Text title;
    std::vector<sf::Text> leftTexts;
    std::vector<sf::Text> rightTexts;
    std::vector<sf::Sprite> leftSprites;
    std::vector<sf::Sprite> rightSprites;
    sf::RectangleShape background;
    std::vector<bool> options;

public:
    BinarySelector(int nOptions = 1);

    void setLeftString(const std::wstring& str, int index = 0);
    void setRightString(const std::wstring& str, int index = 0);
    void setTitle(const std::wstring& title);
    bool isLeftActive(int index = 0) const;
    void setLeftActive(bool value, int index = 0);
    bool isRightActive(int index = 0) const;
    void setRightActive(bool value, int index = 0);
    void setBackgroundColor(const sf::Color& color);
    virtual void setWidth(float width);
    virtual void setPosition(const sf::Vector2f& pos);
    virtual void processEvent(const sf::Event& event);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    const sf::RectangleShape& getBackground() const;
};

#endif // BINARYSELECTOR_H
