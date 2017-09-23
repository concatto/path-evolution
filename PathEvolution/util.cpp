#include "util.h"
#include <cmath>
#include <random>
#include <ctime>
#include <thread>
#include <iostream>
#include <fstream>
#include <sstream>

sf::Font* Util::font = nullptr;

sf::Texture Util::loadTexture(const std::string& file)
{
    sf::Texture tex;
    tex.loadFromFile(file);
    return tex;
}

sf::Font* Util::getFont()
{
    if (font == nullptr) {
        font = new sf::Font;
        font->loadFromFile("arial.ttf");
    }

    return font;
}

std::string Util::readEntireFile(const std::string& path) {
    std::ifstream in(path);
    std::ostringstream oss;
    std::string line;
    while (std::getline(in, line)) {
        oss << line;
    }
    return oss.str();
}

float Util::calculateFontMiddle(const sf::Font* font, unsigned int characterSize) {
    const sf::Glyph& upper = font->getGlyph('O', characterSize, false);

    return upper.bounds.height / 2;
}

double Util::factorial(double n)
{
    double result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

double Util::binomialCoefficient(unsigned int n, unsigned int k)
{
    return factorial(n) / (factorial(k) * (factorial(n - k)));
}

double Util::toDegrees(double radians) {
    return (radians / pi()) * 180;
}

Point2D Util::bezierCurve(double t, const std::vector<Point2D> points)
{
    int n = points.size() - 1;
    Point2D point(0, 0);

    for (int i = 0; i <= n; i++) {
        double c = binomialCoefficient(n, i) * std::pow(1 - t, n - i) * std::pow(t, i);

        point.first += points[i].first * c;
        point.second += points[i].second * c;
    }

    return point;
}

std::vector<Point2D> Util::toPoints2D(const std::vector<double> &values)
{
    std::vector<Point2D> points;

    for (unsigned int i = 0; i < values.size(); i += 2) {
        points.push_back({values[i], values[i + 1]});
    }

    return points;
}

double Util::random(double min, double max)
{
    static std::mt19937* generator = nullptr;
    if (!generator) generator = new std::mt19937(/*std::clock() + std::hash<std::thread::id>()(std::this_thread::get_id())*/);

    std::uniform_real_distribution<double> distribution(min, max);

    return distribution(*generator);
}

int Util::irandom(double min, double max)
{
   return std::round(random(min, max));
}

double Util::pi(double multiplier, double denominator)
{
    return (3.14159265359 * multiplier) / denominator;
}

double Util::saferandom(double min, double max)
{
    static thread_local std::mt19937* generator = nullptr;
    if (!generator) generator = new std::mt19937(std::clock() + std::hash<std::thread::id>()(std::this_thread::get_id()));
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(*generator);
}

int Util::safeirandom(double min, double max)
{
    return std::round(saferandom(min, max));
}

std::set<unsigned int> Util::chooseRandomlyOrdered(unsigned int count, unsigned int min, unsigned int max)
{
    std::set<unsigned int> elements;
    while (elements.size() < count) {
        elements.insert(Util::irandom(min, max));
    }

    return elements;
}

std::unordered_set<unsigned int> Util::chooseRandomly(unsigned int count, unsigned int min, unsigned int max)
{
    std::unordered_set<unsigned int> elements;
    while (elements.size() < count) {
        elements.insert(Util::irandom(min, max));
    }

    return elements;
}

std::unordered_set<unsigned int> Util::safeChooseRandomly(unsigned int count, unsigned int min, unsigned int max)
{
    std::unordered_set<unsigned int> elements;
    while (elements.size() < count) {
        elements.insert(Util::safeirandom(min, max));
    }

    return elements;
}

std::unordered_set<unsigned> Util::chooseRandomlyRestricted(unsigned max, unsigned quantity, std::unordered_set<unsigned> restricted) {
    std::unordered_set<unsigned> values;
    std::unordered_set<unsigned>::iterator nav;
    std::unordered_set<unsigned>::iterator restrictedNav;

    while (values.size() != quantity){
        unsigned element = irandom(0, max - 1); //Tamanho máximo (exclusivo)
        nav = values.find(element);
        restrictedNav = restricted.find(element);

        if (nav == values.end() && restrictedNav == restricted.end()) { //Se não está presente em nenhum dos conjuntos
            values.insert(element);
        }
    }

    return values;
}
