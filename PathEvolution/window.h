#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>
#include "util.h"
#include "differentialevolver.h"
#include "weightedbinaryselector.h"
#include "binaryselector.h"
#include "button.h"

#include <mutex>

struct Trajectory {
    sf::VertexArray va;
    double fitness;
    int remainingTime;

    Trajectory(const sf::VertexArray& va, double fitness, int remainingTime) :
        va(va), fitness(fitness), remainingTime(remainingTime) {}
};

struct SelectorLabelling {
    sf::String title;
    std::vector<sf::String> left;
    std::vector<sf::String> right;

    SelectorLabelling(const sf::String& title, const std::vector<sf::String>& left, const std::vector<sf::String>& right) :
        title(title), left(left), right(right) {}

    SelectorLabelling(const sf::String& title, const sf::String& left, const sf::String& right) :
        title(title), left({left}), right({right}) {}
};

class Window : public sf::RenderWindow
{
private:
    using SelectorConfig = std::pair<BinarySelector*, SelectorLabelling>;

    std::mutex mutex;
    bool running;

    static const sf::Color paneColor;
    float paneWidth = 350;
    sf::Vector2f stageSize;
    std::vector<sf::VertexArray> arrays;

    sf::RenderTexture scenarioTexture;

    std::vector<SelectorConfig> objectiveData;
    sf::RectangleShape pane;
    WeightedBinarySelector collisionSelector;
    WeightedBinarySelector distanceSelector;
    WeightedBinarySelector arcLengthSelector;
    BinarySelector automaticDestinationSelector;
    BinarySelector stopSelector;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture obstacleTexture;
    sf::Sprite obstacleSprite;

    sf::Texture startTexture;
    sf::Texture stopTexture;
    sf::Texture clearTexture;
    Button startButton;
    Button stopButton;
    Button clearButton;

    sf::Sprite carSprite;
    sf::Image scenarioImage;
    std::vector<sf::FloatRect> obstacles;

    sf::Texture destinationTex;
    sf::Texture startTex;

    sf::Sprite destination;
    sf::Sprite start;

    std::deque<Trajectory> trajectories;
    sf::RenderTexture offscreenStage;
    sf::RenderTexture stageBuffer;
    bool dataAvailable = false;

    sf::Shader shader;

    int generation;

    bool checkPixelCollisions(const sf::Image &a, const sf::Image &b, sf::FloatRect bounds);
    bool isEmpty(const sf::Image &image, sf::FloatRect rect) const;

    sf::Texture colorizeTexture(const sf::Texture &tex, sf::Color color);
    sf::Texture constructScenario();
    std::vector<sf::FloatRect> coverScenario(const sf::Image &image, int length);
    sf::VertexArray constructBezierCurve(const std::vector<Point2D> &points, double step, sf::Color color);

    int calculateNextPosition(int k, float speed, const sf::VertexArray &va);
    void updateTrajectories(const DifferentialEvolver &evolver, const sf::Sprite &scenario, std::deque<Trajectory>& trajectories, sf::RenderTexture& offscreenStage);
    bool isInStage(const sf::Vector2f& point);
    void drawPane();
    bool carCollides() const;
    void drawBorder(sf::RenderTexture &texture);
public:
    Window(int width, int height);

    bool loop();
};

#endif // WINDOW_H
