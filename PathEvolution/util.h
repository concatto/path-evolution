#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <utility>
#include <set>
#include <unordered_set>
#include <SFML/Graphics.hpp>

using Point2D = std::pair<double, double>;

class Util
{
public:
    template <class T, class U>
    static void centralizeObject(T& object, U size) {
        object.setOrigin(size.x / 2, size.y / 2);
    }

    template <class T>
    static void centralizeObject(T& object) {
        centralizeObject(object, object.getSize());
    }

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
};

#endif // UTIL_H
