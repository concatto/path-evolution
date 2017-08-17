#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>
#include <vector>

class Window : public sf::RenderWindow
{
private:
    std::vector<sf::VertexArray> arrays;

    bool checkPixelCollisions(const sf::Image &a, const sf::Image &b, sf::FloatRect bounds);
    sf::Texture colorizeTexture(const sf::Texture &tex, sf::Color color);
    sf::Texture constructScenario();
public:
    Window(int width, int height);

    void loop();
};

#endif // WINDOW_H
