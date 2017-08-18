#include "window.h"
#include "differentialevolver.h"
#include <cmath>
#include <iostream>
#include <vector>

Window::Window(int width, int height)
    : sf::RenderWindow(sf::VideoMode(width, height), "PathEvolution")
{
    setFramerateLimit(60);
    arrays.push_back(sf::VertexArray(sf::LinesStrip));
}

sf::Texture Window::constructScenario()
{
    sf::RenderTexture scenarioTexture;
    scenarioTexture.create(getSize().x, getSize().y);
    scenarioTexture.clear(sf::Color::Transparent);

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
                }
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(*this));

            sf::CircleShape circle(10, 100);
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

    for (double t = 0; t <= 1; t += step) {
        Point2D pos = Util::bezierCurve(t, points);
        sf::Vertex vertex(sf::Vector2f(pos.first * getSize().x, pos.second * getSize().y), color);

        va.append(vertex);
    }

    return va;
}

void Window::loop()
{
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

    DifferentialEvolver evolver(0.5, 1);
    evolver.initialize(20, 30, -0.2, 1.2);

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
        for (double t = 0; t <= 1; t += 0.005) {
            Point2D point = Util::bezierCurve(t, points);
            sf::Vector2f pos(point.first * getSize().x, point.second * getSize().y);

            if (t != 0 && !collided) {
                sf::Vector2f delta = pos - oldPos;

                sprite.setPosition(oldPos);
                sprite.setRotation(Util::toDegrees(std::atan2(delta.y, delta.x)) - 90);

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

        sprite.setTexture(carTex);
        return (collisions * 0.005);
    });

    for (unsigned int i = 0; i < 100; i++) {
        evolver.improve();
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

    while (isOpen()) {
        while (pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                close();
            }
        }

        clear();

        sf::Vector2f vec = va[k].position - va[k - 1].position;

        sprite.setPosition(va[k].position);
        sprite.setRotation(Util::toDegrees(std::atan2(vec.y, vec.x)) - 90);

        if (k < limit) {
            k++;
        }

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
