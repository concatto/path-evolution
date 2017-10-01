#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <utility>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <SFML/Graphics.hpp>

using Point2D = std::pair<double, double>;

class Util
{
private:
    using BezierMemo = std::unordered_map<int, std::unordered_map<double, double>>;
    static sf::Font* font;
    static BezierMemo bezierMemo;

public:
    template <class T, class U>
    static void centralizeOrigin(T& object, U size) {
        object.setOrigin(size.x / 2, size.y / 2);
    }

    template <class T>
    static void centralizeOrigin(T& object) {
        centralizeOrigin(object, object.getSize());
    }

    static sf::Texture loadTexture(const std::string& file);
    static sf::Font* getFont();
    static double factorial(double n);
    static double binomialCoefficient(unsigned int n, unsigned int k);
    static Point2D bezierCurve(double t, const std::vector<Point2D> points);
    static std::vector<Point2D> toPoints2D(const std::vector<double>& values);

    static double random(double min = 0, double max = 1);
    static int irandom(double min = 0, double max = 1);
    static double pi(double multiplier = 1, double denominator = 1);

    static double saferandom(double min = 0, double max = 1);
    static int safeirandom(double min = 0, double max = 1);
    static std::unordered_set<unsigned int> chooseRandomly(unsigned int count, unsigned int min, unsigned int max);
    static std::unordered_set<unsigned int> safeChooseRandomly(unsigned int count, unsigned int min, unsigned int max);
    static std::set<unsigned int> chooseRandomlyOrdered(unsigned int count, unsigned int min, unsigned int max);
    static std::unordered_set<unsigned> chooseRandomlyRestricted(unsigned max, unsigned quantity, std::unordered_set<unsigned> restricted);
    static double toDegrees(double radians);
    static float calculateFontMiddle(const sf::Font* font, unsigned int characterSize);
    static std::string readEntireFile(const std::string& path);
    static sf::Color fromHSV(float hue, float saturation, float value);

    template <class T>
    static sf::Vector2<T> getCenter(const sf::Rect<T>& rect) {
        T x = rect.left + (rect.width / 2);
        T y = rect.top + (rect.height / 2);
        return sf::Vector2<T>(x, y);
    }
};

#endif // UTIL_H
