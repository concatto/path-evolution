#include "binaryselector.h"
#include "util.h"
#include <iostream>

sf::Texture BinarySelector::leftTexture = Util::loadTexture("left3.png");
sf::Texture BinarySelector::rightTexture = Util::loadTexture("right3.png");

BinarySelector::BinarySelector(float width, const std::wstring& leftStr, const std::wstring& rightStr)
    : input(50)
{
    background.setSize(sf::Vector2f(width, 100));
    leftSprite.setTexture(leftTexture);
    rightSprite.setTexture(rightTexture);

    left.setString(leftStr);
    right.setString(rightStr);

    for (sf::Text* text : {&left, &right, &title}) {
        sf::Font* font = Util::getFont();
        text->setFont(*font);
        text->setFillColor(sf::Color::Black);
        text->setCharacterSize(17);
    }

    title.setCharacterSize(26);

    sf::FloatRect leftBounds = left.getLocalBounds();
    left.setOrigin(leftBounds.left + leftBounds.width, 0);
    right.setOrigin(0, 0);

    Util::centralizeOrigin(leftSprite, leftTexture.getSize());
    Util::centralizeOrigin(rightSprite, rightTexture.getSize());

    setPosition(sf::Vector2f(0, 0));
    setBackgroundColor(sf::Color::White);
}

void BinarySelector::setBackgroundColor(const sf::Color& color)
{
    background.setFillColor(color);
}

bool BinarySelector::isLeftActive() const
{
    return leftActive;
}

void BinarySelector::setLeftActive(bool value)
{
    leftActive = value;
}

void BinarySelector::setPosition(const sf::Vector2f& pos)
{
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

    float margin = 10;
    input.setPosition(pos + sf::Vector2f(100, margin));
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

    input.processEvent(event);
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
    target.draw(input);
    sf::CircleShape shape(1);

    shape.setFillColor(sf::Color::Red);
    shape.setPosition(left.getGlobalBounds().left, left.getGlobalBounds().top);
    target.draw(shape);
}
