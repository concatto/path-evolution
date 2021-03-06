#include "binaryselector.h"
#include "util.h"
#include <iostream>

sf::Texture BinarySelector::leftTexture = Util::loadTexture("left3.png");
sf::Texture BinarySelector::rightTexture = Util::loadTexture("right3.png");
sf::Texture BinarySelector::leftTextureDisabled = Util::loadTexture("left3bw.png");
sf::Texture BinarySelector::rightTextureDisabled = Util::loadTexture("right3bw.png");

BinarySelector::BinarySelector(int nOptions)
{
    sf::Font* font = Util::getFont();
    
    for (int i = 0; i < nOptions; i++) {
        options.push_back(true);

        sprites.push_back(sf::Sprite(leftTexture));

        leftTexts.push_back(sf::Text("", *font, 14));
        rightTexts.push_back(sf::Text("", *font, 14));

        leftTexts.back().setFillColor(sf::Color::Black);
        rightTexts.back().setFillColor(sf::Color::Black);

        Util::centralizeOrigin(sprites.back(), leftTexture.getSize());
    }

    title.setFont(*font);
    title.setFillColor(sf::Color::Black);
    title.setCharacterSize(20);
    setPosition(sf::Vector2f(0, 0));
    setBackgroundColor(sf::Color::White);

    verticalSpacing = leftTexture.getSize().y + 20;
}

bool BinarySelector::isDisabled() const
{
    return disabled;
}

void BinarySelector::setDisabled(bool disabled)
{
    this->disabled = disabled;
    for (int i = 0; i < sprites.size(); i++) {
        if (disabled) {
            sprites[i].setTexture(isLeftActive(i) ? leftTextureDisabled : rightTextureDisabled);
        } else {
            sprites[i].setTexture(isLeftActive(i) ? leftTexture : rightTexture);
        }
    }
}

void BinarySelector::setLeftString(const std::wstring& str, int index)
{
    leftTexts[index].setString(str);
}

void BinarySelector::setRightString(const std::wstring& str, int index)
{
    rightTexts[index].setString(str);
}

void BinarySelector::setBackgroundColor(const sf::Color& color)
{
    background.setFillColor(color);
}

void BinarySelector::setWidth(float width)
{
    background.setSize(sf::Vector2f(width, 65 + options.size() * verticalSpacing));
}

bool BinarySelector::isLeftActive(int index) const
{
    return options[index];
}

void BinarySelector::setLeftActive(bool value, int index)
{
    options[index] = value;
    setDisabled(isDisabled()); // Update sprites
}

bool BinarySelector::isRightActive(int index) const
{
    return !isLeftActive(index);
}

void BinarySelector::setRightActive(bool value, int index)
{
    setLeftActive(!value, index);
}

void BinarySelector::setPosition(const sf::Vector2f& pos)
{
    background.setPosition(pos);
    sf::Vector2f size = background.getSize();

    for (int i = 0; i < sprites.size(); i++) {
        sf::FloatRect leftBounds = leftTexts[i].getLocalBounds();
        leftTexts[i].setOrigin(leftBounds.left + leftBounds.width, 0);
        rightTexts[i].setOrigin(0, 0);

        float dy = i * verticalSpacing;

        sf::Vector2f spritePos(pos.x + (size.x * 0.5), pos.y + title.getLocalBounds().height + 50 + dy);
        sprites[i].setPosition(spritePos);

        float horizontalShift = leftTexture.getSize().x * 0.666;
        float verticalShift = Util::calculateFontMiddle(leftTexts[i].getFont(), leftTexts[i].getCharacterSize());

        leftTexts[i].setPosition(spritePos - sf::Vector2f(horizontalShift, 0));
        rightTexts[i].setPosition(spritePos + sf::Vector2f(horizontalShift, 0));

        float delta = leftTexts[i].getGlobalBounds().top - (sprites[i].getPosition().y);

        leftTexts[i].move(0, -delta - verticalShift);
        rightTexts[i].move(0, -delta - verticalShift);
    }

    float margin = 12;
    title.setPosition(pos.x + margin, pos.y);
    title.move(0, pos.y - title.getGlobalBounds().top + margin);
}

void BinarySelector::processEvent(const sf::Event& event)
{
    if (!disabled && event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            for (int i = 0; i < sprites.size(); i++) {
                if (sprites[i].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    setLeftActive(!isLeftActive(i), i);
                    break; // Can't click on two selectors at the same time!
                }
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
    target.draw(title);

    for (int i = 0; i < sprites.size(); i++) {
        target.draw(sprites[i]);
        target.draw(leftTexts[i]);
        target.draw(rightTexts[i]);
    }
}

const sf::RectangleShape& BinarySelector::getBackground() const
{
    return background;
}

