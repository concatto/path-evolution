#ifndef BINARYSELECTOR_H
#define BINARYSELECTOR_H

#include <SFML/Graphics.hpp>

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
    BinarySelector(float width, const std::string& leftStr, const std::string& rightStr);

    void setTitle(const std::string& title);
    bool isLeftActive() const;
    void setLeftActive(bool value);
    void setPosition(const sf::Vector2f& pos);
    void processEvent(const sf::Event& event);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif // BINARYSELECTOR_H
