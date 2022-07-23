#include <bits/stdc++.h>
using namespace std;

#ifndef POINT_H
#define POINT_H

class Point
{
    public:
    double x, y, z, w;

    Point()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->w = 1.0;
    }

    Point(double _x, double _y, double _z, double _w)
    {
        this->x = _x;
        this->y = _y;
        this->z = _z;
        this->w = _w;
    }

    Point(double _x, double _y, double _z)
    {
        this->x = _x;
        this->y = _y;
        this->z = _z;
        this->w = 1.0;
    }
    

    Point(const Point &p)
    {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
        this->w = p.w;        
    }

    void normalize()
    {
        double value = sqrt(x*x + y*y + z*z);
        if (value == 0) {
            cout << "ERROR Can't divide by zero" << endl;
            return;
        }
        this->x /= value;
        this->y /= value;
        this->z /= value;
    }

    void scale()
    {
        this->x /= this->w;
        this->y /= this->w;
        this->z /= this->w;
        this->w /= this->w;
    }

    Point operator+ (const Point p)
    {
        return Point(this->x + p.x, this->y + p.y, this->z + p.z);
    }

    Point operator- (const Point p)
    {
        return Point(this->x - p.x, this->y - p.y, this->z - p.z);
    }

    Point operator* (const double value)
    {
        return Point(this->x*value, this->y*value, this->z*value);
    }

    double operator* (const Point p)
    {
        return this->x*p.x + this->y*p.y + this->z*p.z;
    }

    Point operator^ (const Point p)
    {
        return Point(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x);
    }

    friend ostream& operator<< (ostream& out, Point &p)
    {
        out << fixed << setprecision(7) << p.x << " " << p.y << " " << p.z;
        return out;
    }

    friend ifstream& operator>> (ifstream& in, Point &p)
    {
        in >> p.x >> p.y >> p.z;
        return in;
    }

};

#endif // POINT_H