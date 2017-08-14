#include "window.h"
#include "util.h"
#include "differentialevolver.h"

Window::Window(int width, int height)
    : sf::RenderWindow(sf::VideoMode(width, height), "PathEvolution")
{
    setFramerateLimit(60);
    arrays.push_back(sf::VertexArray(sf::LinesStrip));
}

void Window::loop()
{
    DifferentialEvolver evolver(1, 1);
    evolver.initialize(10, 10, -0.5, 1.5);

    for (const DifferentialEvolver::Individual& ind : evolver.getPopulation()) {
        std::vector<Point2D> points = Util::toPoints2D(ind);

        for (double t = 0; t <= 1; t += 0.02) {
            Point2D pos = Util::bezierCurve(t, points);

            arrays.back().append(sf::Vertex(sf::Vector2f(pos.first * getSize().x, pos.second * getSize().y), sf::Color::Red));
        }

        arrays.push_back(sf::VertexArray(sf::LinesStrip));
    }

    sf::Event event;
    while (isOpen()) {
        while (pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                close();
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    //Add new strip
                    arrays.push_back(sf::VertexArray(sf::LinesStrip));
                }
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i pos = sf::Mouse::getPosition(*this);

            arrays.back().append(sf::Vertex(sf::Vector2f(pos), sf::Color::White));
        }

        clear();

        for (const sf::VertexArray& array : arrays) {
            draw(array);
        }



        display();
    }
}

