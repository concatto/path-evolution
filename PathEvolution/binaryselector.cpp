#include "binaryselector.h"
#include "util.h"
#include <iostream>

sf::Texture BinarySelector::leftTexture = Util::loadTexture("left2.png");
sf::Texture BinarySelector::rightTexture = Util::loadTexture("right2.png");
const sf::Color BinarySelector::TextColor = sf::Color(0x007fea);

BinarySelector::BinarySelector(float width, const std::wstring& leftStr, const std::wstring& rightStr)
    : input(50)
{
    background.setSize(sf::Vector2f(width, 100));
    background.setFillColor(sf::Color::White);
    leftSprite.setTexture(leftTexture);
    rightSprite.setTexture(rightTexture);

    left.setString(leftStr);
    right.setString(rightStr);

    for (sf::Text* text : {&left, &right, &title}) {
        sf::Font* font = Util::getFont();
        text->setFont(*font);
        text->setFillColor(TextColor);
        text->setCharacterSize(16);
    }

    title.setCharacterSize(24);

    sf::FloatRect leftBounds = left.getLocalBounds();
    left.setOrigin(leftBounds.left + leftBounds.width, leftBounds.height);
    right.setOrigin(0, right.getLocalBounds().height);

    Util::centralizeOrigin(leftSprite, leftTexture.getSize());
    Util::centralizeOrigin(rightSprite, rightTexture.getSize());

    setPosition(sf::Vector2f(0, 0));
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

    float displacement = leftTexture.getSize().x * 0.6;

    left.setPosition(spritePos - sf::Vector2f(displacement, 0));
    right.setPosition(spritePos + sf::Vector2f(displacement, 0));

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
}
