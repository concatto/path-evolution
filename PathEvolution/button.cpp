#include "button.h"
#include "util.h"

Button::Button()
{
    background.setOutlineThickness(1);
    updateAppearance();
}

void Button::setTexture(sf::Texture *tex)
{
    this->texture = tex;
    sprite.setTexture(*tex);
    Util::centralizeOrigin(sprite, tex->getSize());

    rescaleSprite();
}

bool Button::processEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed && !disabled) {
        if (background.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
            return true;
        }
    } else if (event.type == sf::Event::MouseMoved) {
        hovered = background.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y);
        updateAppearance();
    }

    return false;
}

void Button::setPosition(const sf::Vector2f& pos)
{
    background.setPosition(pos);
    sprite.setPosition(Util::getCenter(background.getGlobalBounds()));
}

void Button::setSize(const sf::Vector2f& size)
{
    background.setSize(size);
    if (texture != nullptr) {
        rescaleSprite();
    }
}

void Button::setDisabled(bool disabled)
{
    this->disabled = disabled;
    updateAppearance();
}

void Button::updateAppearance()
{
    sf::Color fill(0xD7D7D7FF);
    sf::Color border(0xAAAAAAFF);
    if (disabled) {
        fill = sf::Color(0xBFBFBFFF);
        border = sf::Color(0xAFAFAFFF);
        sprite.setColor(sf::Color(0xFFFFF3A));
    } else {
        sprite.setColor(sf::Color::White);
        if (hovered) {
            fill = sf::Color(0xEAEAEAFF);
            border = sf::Color(0x428FEFFF);
        }
    }

    background.setFillColor(fill);
    background.setOutlineColor(border);
}

void Button::rescaleSprite()
{
    float xScale = (background.getSize().x - margin) / texture->getSize().x;
    float yScale = (background.getSize().y - margin) / texture->getSize().y;

    float scale = std::min(1.0f, std::min(xScale, yScale));
    sprite.setScale(scale, scale);
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(background);
    if (texture != nullptr) {
        target.draw(sprite);
    }
}

const sf::Vector2f& Button::getSize() const
{
    return background.getSize();
}
