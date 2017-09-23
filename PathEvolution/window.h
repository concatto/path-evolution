#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>
#include "util.h"
#include "differentialevolver.h"
#include "binaryselector.h"

class Window : public sf::RenderWindow
{
private:
    static const sf::Color paneColor;
    float paneWidth = 300;
    sf::Vector2f stageSize;
    std::vector<sf::VertexArray> arrays;

    sf::RectangleShape pane;
    BinarySelector collisionSelector;

    sf::Texture destinationTex;
    sf::Texture startTex;

    sf::Sprite destination;
    sf::Sprite start;

    std::deque<sf::VertexArray> trajectories;
    sf::Shader shader;

    int generation;

    bool checkPixelCollisions(const sf::Image &a, const sf::Image &b, sf::FloatRect bounds);
    bool isEmpty(const sf::Image &image, sf::FloatRect rect);

    sf::Texture colorizeTexture(const sf::Texture &tex, sf::Color color);
    sf::Texture constructScenario();
    std::vector<sf::FloatRect> coverScenario(const sf::Image &image, int length);
    sf::VertexArray constructBezierCurve(const std::vector<Point2D> &points, double step, sf::Color color);

    int calculateNextPosition(int k, float speed, const sf::VertexArray &va);
    void displayTrajectories(const DifferentialEvolver &evolver, const sf::Sprite &scenario);
    bool isInStage(const sf::Vector2f& point);
    void drawPane();
public:
    Window(int width, int height);

    void loop();
};

#endif // WINDOW_H
