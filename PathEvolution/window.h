#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>
#include <vector>

class Window : public sf::RenderWindow
{
private:
    std::vector<sf::VertexArray> arrays;

public:
    Window(int width, int height);

    void loop();
};

#endif // WINDOW_H
