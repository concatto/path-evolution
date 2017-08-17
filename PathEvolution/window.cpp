#include "window.h"
#include "util.h"
#include "differentialevolver.h"
#include <cmath>
#include <iostream>

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

void Window::loop()
{
    sf::Texture scenarioTexture(constructScenario());
    sf::Image scenarioImage = scenarioTexture.copyToImage();
    sf::Sprite scenario(scenarioTexture);

    scenarioImage.saveToFile("test.png");


    DifferentialEvolver evolver(1, 1);
    evolver.initialize(1, 30, -0.2, 1.2);

    sf::RenderTexture pathTexture;
    pathTexture.create(getSize().x, getSize().y);
    pathTexture.clear();

    sf::Texture carTex;
    carTex.loadFromFile("car.png");

    sf::Texture whiteTex = colorizeTexture(carTex, sf::Color::White);

    sf::Sprite sprite(whiteTex);
    Util::centralizeObject(sprite, carTex.getSize());
    sprite.setScale(0.5, 0.5);

    for (const DifferentialEvolver::Individual& ind : evolver.getPopulation()) {
        std::vector<Point2D> points = Util::toPoints2D(ind);

        bool collided = false;
        for (double t = 0; t <= 1; t += 0.01) {
            sf::VertexArray& last = arrays.back();

            Point2D pos = Util::bezierCurve(t, points);
            sf::Vertex vertex(sf::Vector2f(pos.first * getSize().x, pos.second * getSize().y), sf::Color::Red);

            if (t != 0 && !collided) {
                sf::Vertex& oldVertex = last[last.getVertexCount() - 1];
                sf::Vector2f delta = vertex.position - oldVertex.position;

                sprite.setPosition(oldVertex.position);
                sprite.setRotation(Util::toDegrees(std::atan2(delta.y, delta.x)) - 90);

                pathTexture.draw(sprite);
                pathTexture.display();

                clear();
                draw(sf::Sprite(pathTexture.getTexture()));
                draw(scenario);
                draw(last);
                display();

                if (!collided && checkPixelCollisions(pathTexture.getTexture().copyToImage(), scenarioImage, sprite.getGlobalBounds())) {
                    collided = true;
                }
            }

            last.append(vertex);
        }

        arrays.push_back(sf::VertexArray(sf::LinesStrip));
    }


    sprite.setTexture(carTex);


    const sf::VertexArray& va = arrays[arrays.size() - 2];
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
        std::cout << std::atan2(vec.y, vec.x) << "\n";

        sprite.setPosition(va[k].position);
        sprite.setRotation(Util::toDegrees(std::atan2(vec.y, vec.x)) - 90);

        if (k < va.getVertexCount() - 1) {
            k++;
        }

        draw(sf::Sprite(pathTexture.getTexture()));

        for (const sf::VertexArray& array : arrays) {
            draw(array);
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

        display();
    }
}

bool Window::checkPixelCollisions(const sf::Image& a, const sf::Image& b, sf::FloatRect bounds)
{
    for (unsigned int x = bounds.left; x <= bounds.left + bounds.width; x++) {
        for (unsigned int y = bounds.top; y <= bounds.top + bounds.height; y++) {
            sf::Color aColor = a.getPixel(x, y);
            sf::Color bColor = b.getPixel(x, y);

//            if ((aColor.a > 0 || aColor != sf::Color::Black) &&
//                (bColor.a > 0 || bColor != sf::Color::Black)) {
//                return true;
//            }

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
