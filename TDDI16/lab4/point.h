#ifndef POINT_H
#define POINT_H

#include <iostream>
#include "SDL/SDL.h"

#define COORD_MAX 32767 // max value of x and y coordinates

using namespace std;

/**
 * An immutable data type for points in the plane.
 */
class Point {
public:
    Point() = delete;
    Point(unsigned int x, unsigned int y) : x{x}, y{y} {}

    double slopeTo(const Point&) const;
    void draw(SDL_Surface*) const;
    void lineTo(SDL_Surface*, const Point&) const;

    bool operator <(const Point&) const;
    bool operator >(const Point&) const;

    friend ostream& operator<<(ostream&, const Point&);

private:
    unsigned int x, y; // position
};


/**
 * Sort points according to their slope relative to 'origin'.
 */
class PolarSorter {
public:
    PolarSorter(const Point &origin) : origin{origin} {}

    bool operator() (const Point& p1, const Point& p2) const {
        return origin.slopeTo(p1) < origin.slopeTo(p2);
    }

private:
    Point origin;
};

#endif
