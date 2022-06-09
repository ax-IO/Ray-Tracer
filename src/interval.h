#ifndef INTERVAL_H
#define INTERVAL_H
#include "rtweekend.h"
class interval
{
public:
    interval() : min(+infinity), max(-infinity) {} // Default interval is empty

    interval(double _min, double _max) : min(_min), max(_max) {}

    interval(const interval &a, const interval &b)
        : min(fmin(a.min, b.min)), max(fmax(a.max, b.max)) {}
    double size() const
    {
        return max - min;
    }

    interval expand(double delta) const
    {
        const auto padding = delta / 2;
        return interval(min - padding, max + padding);
    }

    bool contains(double x) const
    {
        return min <= x && x <= max;
    }

    double clamp(double x) const
    {
        if (x < min)
            return min;
        if (x > max)
            return max;
        return x;
    }

    static const interval empty, universe;

public:
    double min, max;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

interval operator+(const interval &ival, double displacement)
{
    return interval(ival.min + displacement, ival.max + displacement);
}

interval operator+(double displacement, const interval &ival)
{
    return ival + displacement;
}

interval operator*(const interval &ival, double scale)
{
    // return interval(ival.min - (scale*ival.size())/2, ival.max + scale/);
    return ival.expand(ival.size() * (scale - 1));
}

interval operator*(double scale, const interval &ival)
{
    return ival * scale;
}
#endif