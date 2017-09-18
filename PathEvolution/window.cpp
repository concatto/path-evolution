#include "window.h"
#include <cmath>
#include <iostream>
#include <vector>

Window::Window(int width, int height)
    : sf::RenderWindow(sf::VideoMode(width, height), "PathEvolution")
{
    setFramerateLimit(60);
    arrays.push_back(sf::VertexArray(sf::LinesStrip));

    destinationTex.loadFromFile("flag.png");
    startTex.loadFromFile("start.png");

    Util::centralizeObject(destination, destinationTex.getSize());
    Util::centralizeObject(start, startTex.getSize());

    destination.setTexture(destinationTex);
    destination.setPosition(sf::Vector2f(width / 2.0, height / 2.0));
    destination.setScale(0.3, 0.3);

    start.setTexture(startTex);
    start.setPosition(sf::Vector2f(width / 2.0, height / 2.0));
    start.setScale(0.3, 0.3);
}

sf::Texture Window::constructScenario()
{
    sf::RenderTexture scenarioTexture;
    scenarioTexture.create(getSize().x, getSize().y);
    scenarioTexture.clear(sf::Color::Transparent);

    sf::RectangleShape border(sf::Vector2f(getSize()) - sf::Vector2f(10, 10));
    Util::centralizeObject(border);
    border.setPosition(getSize().x / 2.0, getSize().y / 2.0);
    border.setOutlineColor(sf::Color::White);
    border.setOutlineThickness(10);
    border.setFillColor(sf::Color::Transparent);

    scenarioTexture.draw(border);

    sf::Event event;
    sf::Vector2f oldPosition;
    bool isLeftPressed = false;
    bool ended = false;

    while (isOpen() && !ended) {
        while (pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Return) {
                    ended = true;
                } else if (event.key.code == sf::Keyboard::Space) {
                    destination.setPosition(sf::Vector2f(sf::Mouse::getPosition(*this)));
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Right) {
                    start.setPosition(event.mouseButton.x, event.mouseButton.y);
                }
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(*this));

            sf::CircleShape circle(6, 100);
            circle.setOrigin(circle.getRadius(), circle.getRadius());
            circle.setPosition(pos);
            circle.setFillColor(sf::Color::White);

            if (isLeftPressed) {
                sf::Vector2f delta = pos - oldPosition;
                sf::RectangleShape internal(sf::Vector2f(std::hypot(delta.x, delta.y), circle.getRadius() * 2));

                internal.setFillColor(sf::Color::White);
                internal.setOrigin(sf::Vector2f(0, circle.getRadius()));
                internal.setPosition(oldPosition);
                internal.setRotation(Util::toDegrees(std::atan2(delta.y, delta.x)));

                scenarioTexture.draw(internal);
            }

            scenarioTexture.draw(circle);
            scenarioTexture.display();
            isLeftPressed = true;
            oldPosition = pos;
        } else {
            isLeftPressed = false;
        }

        scenarioTexture.display();

        clear();
        draw(destination);
        draw(start);
        draw(sf::Sprite(scenarioTexture.getTexture()));
        display();
    }

    sf::Texture tex(scenarioTexture.getTexture());
    return tex;
}

bool Window::isEmpty(const sf::Image& image, sf::FloatRect rect) {
    for (unsigned int x = rect.left; x <= std::ceil(rect.left + rect.width); x++) {
        for (unsigned int y = rect.top; y <= std::ceil(rect.top + rect.height); y++) {
            sf::Color color = image.getPixel(x, y);
            //If not transparent and not black
            if (color.a > 0 && (color.r + color.g + color.b > 0)) {
                return false;
            }
        }
    }

    return true;
}

std::vector<sf::FloatRect> Window::coverScenario(const sf::Image& image, int length)
{
    unsigned int xParts = std::ceil(image.getSize().x / length);
    unsigned int yParts = std::ceil(image.getSize().y / length);

    std::vector<sf::FloatRect> rectangles;

    for (unsigned int x = 0; x < xParts; x++) {
        bool existed = false;
        for (unsigned int y = 0; y < yParts; y++) {
            sf::FloatRect rect(x * length, y * length, length, length);

            if (x == xParts - 1) {
                rect.width = image.getSize().x - (x * length);
            }
            if (y == yParts - 1) {
                rect.height = image.getSize().y - (y * length);
            }

            if (!isEmpty(image, rect)) {
                if (existed) {
                    //Merge
                    sf::FloatRect& previous = rectangles.back();
                    previous.height += rect.height;
                } else {
                    //Insert
                    rectangles.push_back(rect);
                }
                existed = true;
            } else {
                existed = false;
            }
        }
    }

    return rectangles;
}

sf::VertexArray Window::constructBezierCurve(const std::vector<Point2D>& points, double step, sf::Color color)
{
    sf::VertexArray va(sf::PrimitiveType::LinesStrip);

    for (double t = 0; t < (1 + step); t += step) {
        Point2D pos = Util::bezierCurve(t, points);
        sf::Vertex vertex(sf::Vector2f(pos.first * getSize().x, pos.second * getSize().y), color);

        va.append(vertex);
    }

    return va;
}

int Window::calculateNextPosition(int k, float speed, const sf::VertexArray& va)
{
    double deltaPosition = 0;
    int nextK = k;
    while (deltaPosition < speed) {
        nextK++;
        sf::Vector2f change = va[nextK].position - va[nextK - 1].position;
        deltaPosition += std::hypot(change.x, change.y);
    }

    if (nextK >= va.getVertexCount()) {
        return va.getVertexCount() - 1;
    }
    return nextK;
}

void Window::displayTrajectories(const DifferentialEvolver& evolver, const sf::Sprite& scenario)
{
    using EvaluatedIndividual = std::pair<DifferentialEvolver::Individual, double>;
    std::vector<EvaluatedIndividual> pop;
    const std::vector<DifferentialEvolver::Individual>& originalPopulation = evolver.getPopulation();

    for (unsigned int i = 0; i < originalPopulation.size(); i++) {
        pop.push_back({originalPopulation[i], evolver.getFitness(i)});
    }

    std::sort(pop.begin(), pop.end(), [&](const EvaluatedIndividual& a, const EvaluatedIndividual& b) {
        return a.second < b.second;
    });

    sf::Font font;
//    font.loadFromFile("arial.ttf");

    for (const EvaluatedIndividual& ind : pop) {
        clear();
        draw(scenario);

        sf::VertexArray va = constructBezierCurve(Util::toPoints2D(ind.first), 0.005, sf::Color::Red);
        draw(va);

//        sf::Text label(std::to_string(ind.second), font);
//        label.setColor(sf::Color::White);

//        draw(label);
        display();

        sf::sleep(sf::microseconds(1));
    }
}

void Window::loop()
{
    float speed = 1;

    sf::Texture scenarioTexture(constructScenario());
    sf::Image scenarioImage = scenarioTexture.copyToImage();
    sf::Sprite scenario(scenarioTexture);

    std::vector<sf::FloatRect> rects = coverScenario(scenarioImage, 10);
    std::vector<sf::RectangleShape> rectShapes;
    rectShapes.reserve(rects.size());

    std::transform(rects.begin(), rects.end(), std::back_inserter(rectShapes), [&](const sf::FloatRect& r) {
        sf::RectangleShape shape(sf::Vector2f(r.width, r.height));
        shape.setPosition(r.left, r.top);
        shape.setOutlineThickness(1);
        shape.setFillColor(sf::Color(255, 255, 255, 30));
        shape.setOutlineColor(sf::Color::Green);
        return shape;
    });

    DifferentialEvolver evolver(0.8, 0.1);
    evolver.initialize(100, 30, -0.5, 1.5,
        {start.getPosition().x / getSize().x, start.getPosition().y / getSize().y},
        {destination.getPosition().x / getSize().x, destination.getPosition().y / getSize().y}
    );

    sf::Texture carTex;
    carTex.loadFromFile("car.png");

    sf::Texture whiteTex = colorizeTexture(carTex, sf::Color::White);

    sf::Sprite sprite(whiteTex);
    Util::centralizeObject(sprite, carTex.getSize());
    sprite.setScale(0.5, 0.5);

    sf::Clock clock;

    int limit = -1;
    int step = 0;


    evolver.setObjectiveFunction([&](const DifferentialEvolver::Individual& ind) {
        sprite.setTexture(carTex);
        std::vector<Point2D> points = Util::toPoints2D(ind);

        bool collided = false;
        int collisions = 0;

        sf::Vector2f oldPos;
        sprite.setPosition(points[0].first, points[0].second);

        double arcLength = 0;

        for (double t = 0; t <= 1; t += 0.005) {
            Point2D point = Util::bezierCurve(t, points);
            sf::Vector2f pos(point.first * getSize().x, point.second * getSize().y);

            if (t != 0 && !collided) {
                sf::Vector2f delta = pos - oldPos;
                arcLength += std::hypot(delta.x / getSize().x, delta.y / getSize().y);
                float angle = std::atan2(delta.y, delta.x);

                sprite.setPosition(pos);
                sprite.setRotation(Util::toDegrees(angle) - 90);

                for (const sf::FloatRect& rect : rects) {
                    sf::FloatRect intersection;
                    if (limit < 0 && rect.intersects(sprite.getGlobalBounds(), intersection)) {
                        if (!isEmpty(scenarioImage, intersection)) {
                            collisions++;
                        }
                    }
                }
            }

            oldPos = pos;
        }

        sf::Vector2f delta = destination.getPosition() - sprite.getPosition();
//        double euclideanDistance = std::hypot(delta.x / getSize().x, delta.y / getSize().y);
        sprite.setTexture(carTex);
        return 1 / ((collisions * 1) + (arcLength * 0));
    });

    for (unsigned int i = 0; i < 400; i++) {
        evolver.improve();

        displayTrajectories(evolver, scenario);
    }

    std::cout << clock.getElapsedTime().asMilliseconds() << "\n";

    sf::VertexArray va = constructBezierCurve(Util::toPoints2D(evolver.getBestIndividual()), 0.005, sf::Color::Red);
    for (unsigned int i = 0; i < va.getVertexCount(); i++) {
        const sf::Vertex& vertex = va[i];
        std::cout << vertex.position.x << ", " << vertex.position.y << "\n";
    }


    if (limit < 0) {
        limit = va.getVertexCount() - 1;
    }

    unsigned int k = 1;



    sf::Event event;

    sprite.setPosition(va[k].position);

    while (isOpen()) {
        while (pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                close();
            }
        }

        clear();

        int nextK = calculateNextPosition(k, speed, va);

        sf::Vector2f vec = va[nextK].position - va[k].position;

        if (k < limit) {
            k = std::min(limit - 1, nextK);
        }

        float angle = std::atan2(vec.y, vec.x);
        sprite.setPosition(va[k].position.x, va[k].position.y);
        sprite.setRotation(Util::toDegrees(angle) - 90);

        draw(sprite);

        sf::FloatRect rect = sprite.getGlobalBounds();
        sf::RectangleShape bounds(sf::Vector2f(rect.width, rect.height));
        bounds.setPosition(rect.left, rect.top);
        bounds.setOutlineColor(sf::Color::Red);
        bounds.setFillColor(sf::Color::Transparent);
        bounds.setOutlineThickness(2);

        draw(bounds);
        draw(scenario);
        draw(va);
        draw(destination);
        draw(start);

        for (const sf::RectangleShape& shape : rectShapes) {
            draw(shape);
        }

        display();
    }
}

bool Window::checkPixelCollisions(const sf::Image& a, const sf::Image& b, sf::FloatRect bounds)
{
    for (unsigned int x = bounds.left; x <= std::ceil(bounds.left + bounds.width); x++) {
        for (unsigned int y = bounds.top; y <= std::ceil(bounds.top + bounds.height); y++) {
            sf::Color aColor = a.getPixel(x, y);
            sf::Color bColor = b.getPixel(x, y);

            if (aColor == sf::Color::White && bColor == sf::Color::White) {
                return true;
            }
        }
    }

    return false;
}

sf::Texture Window::colorizeTexture(const sf::Texture& tex, sf::Color color)
{
    sf::Image image = tex.copyToImage();

    for (unsigned int x = 0; x < image.getSize().x; x++) {
        for (unsigned int y = 0; y < image.getSize().y; y++) {
            color.a = image.getPixel(x, y).a;
            image.setPixel(x, y, color);
        }
    }

    sf::Texture newTex;
    newTex.loadFromImage(image);

    return newTex;
}
