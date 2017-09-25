#include "binaryselector.h"
#include "util.h"
#include <iostream>

sf::Texture BinarySelector::leftTexture = Util::loadTexture("left3.png");
sf::Texture BinarySelector::rightTexture = Util::loadTexture("right3.png");

BinarySelector::BinarySelector()
{
    leftSprite.setTexture(leftTexture);
    rightSprite.setTexture(rightTexture);

    left.setString("");
    right.setString("");

    for (sf::Text* text : {&left, &right, &title}) {
        sf::Font* font = Util::getFont();
        text->setFont(*font);
        text->setFillColor(sf::Color::Black);
        text->setCharacterSize(14);
    }

    title.setCharacterSize(20);

    Util::centralizeOrigin(leftSprite, leftTexture.getSize());
    Util::centralizeOrigin(rightSprite, rightTexture.getSize());

    setPosition(sf::Vector2f(0, 0));
    setBackgroundColor(sf::Color::White);
}

void BinarySelector::setLeftString(const std::wstring& str)
{
    left.setString(str);
}

void BinarySelector::setRightString(const std::wstring& str)
{
    right.setString(str);
}

void BinarySelector::setBackgroundColor(const sf::Color& color)
{
    background.setFillColor(color);
}

void BinarySelector::setWidth(float width)
{
    background.setSize(sf::Vector2f(width, 110));
}

bool BinarySelector::isLeftActive() const
{
    return leftActive;
}

void BinarySelector::setLeftActive(bool value)
{
    leftActive = value;
}

bool BinarySelector::isRightActive() const
{
    return !isLeftActive();
}

void BinarySelector::setRightActive(bool value)
{
    setLeftActive(!value);
}

void BinarySelector::setPosition(const sf::Vector2f& pos)
{
    sf::FloatRect leftBounds = left.getLocalBounds();
    left.setOrigin(leftBounds.left + leftBounds.width, 0);
    right.setOrigin(0, 0);

    background.setPosition(pos);

    sf::Vector2f size = background.getSize();
    sf::Vector2f spritePos(pos.x + (size.x * 0.5), pos.y + (size.y * 0.7));
    leftSprite.setPosition(spritePos);
    rightSprite.setPosition(spritePos);

    float horizontalShift = leftTexture.getSize().x * 0.666;
    float verticalShift = Util::calculateFontMiddle(left.getFont(), left.getCharacterSize());

    left.setPosition(spritePos - sf::Vector2f(horizontalShift, 0));
    right.setPosition(spritePos + sf::Vector2f(horizontalShift, 0));

    float delta = left.getGlobalBounds().top - (leftSprite.getPosition().y);

    left.move(0, -delta - verticalShift);
    right.move(0, -delta - verticalShift);

    float margin = 12;
    title.setPosition(pos.x + margin, pos.y + margin);
}

void BinarySelector::processEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (leftSprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                setLeftActive(!isLeftActive());
            }
        }
    }
}

void BinarySelector::setTitle(const std::wstring& title)
{
    this->title.setString(title);
}

void BinarySelector::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(background);
    target.draw(isLeftActive() ? leftSprite : rightSprite);
    target.draw(title);
    target.draw(left);
    target.draw(right);
    sf::CircleShape shape(1);

    shape.setFillColor(sf::Color::Red);
    shape.setPosition(left.getGlobalBounds().left, left.getGlobalBounds().top);
    target.draw(shape);
}

const sf::RectangleShape& BinarySelector::getBackground() const
{
    return background;
}

