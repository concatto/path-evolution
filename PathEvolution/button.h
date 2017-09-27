#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include <utility>

class Button : public sf::Drawable
{
private:
    bool hovered = false;
    bool disabled = false;
    const int margin = 5;
    sf::RectangleShape background;
    sf::Texture* texture = nullptr;
    sf::Sprite sprite;

    void rescaleSprite();
    void updateAppearance();
public:
    Button();

    void setTexture(sf::Texture *tex);
    bool processEvent(const sf::Event& event);
    void setPosition(const sf::Vector2f& pos);
    void setSize(const sf::Vector2f& size);
    void setDisabled(bool disabled);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    const sf::Vector2f& getSize() const;
};

#endif // BUTTON_H
