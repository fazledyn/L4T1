#include <bits/stdc++.h>
#include "Point.hpp"

using namespace std;

#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H


#define PI acos(-1.0)
#define deg_to_rad(x) ((PI * x)/180)
#define rad_to_deg(x) ((180 * x)/PI)


class Transformation
{
    public:
    double matrix[4][4];

    Transformation()
    {
        for (int i=0; i < 4; i++) {
            for (int j=0; j < 4; j++) {
                matrix[i][j] = 0.0;
            }
        }
    }

    Transformation(const Transformation &t)
    {
        for (int i=0; i < 4; i++) {
            for (int j=0; j < 4; j++) {
                this->matrix[i][j] = t.matrix[i][j];
            }
        }
    }

    static Point rodrigues_formula(Point x, Point a, double theta)
    {
        Point ret;
        theta = deg_to_rad(theta);
        ret = x * cos(theta) + a * (a*x) * (1 - cos(theta)) + (a^x) * sin(theta);
        return ret;
    }

    static Transformation identity_matrix()
    {
        Transformation ret;
        for (int i=0; i < 4; i++) {
            ret.matrix[i][i] = 1.0;
        }
        return ret;
    }

    static Transformation translation_matrix(double tx, double ty, double tz)
    {
        Transformation ret = identity_matrix();
        ret.matrix[0][3] = tx;
        ret.matrix[1][3] = ty;
        ret.matrix[2][3] = tz;
        
        // cout << "[TEST] translation_matrix" << endl;
        // cout << ret << endl;

        return ret;
    }

    static Transformation scale_matrix(double sx, double sy, double sz)
    {
        Transformation ret = identity_matrix();
        ret.matrix[0][0] = sx;
        ret.matrix[1][1] = sy;
        ret.matrix[2][2] = sz;

        // cout << "[TEST] scale_matrix" << endl;
        // cout << ret << endl;

        return ret;
    }

    static Transformation rotation_matrix(double ax, double ay, double az, double angle)
    {
        Transformation ret = identity_matrix();

        Point a(ax, ay, az);    //  Basically a vector
        a.normalize();          //  Making it unit vector for ROD formula

        Point p1 = rodrigues_formula(Point(1, 0, 0), a, angle);
        Point p2 = rodrigues_formula(Point(0, 1, 0), a, angle);
        Point p3 = rodrigues_formula(Point(0, 0, 1), a, angle);

        //  p1
        ret.matrix[0][0] = p1.x;
        ret.matrix[1][0] = p1.y;
        ret.matrix[2][0] = p1.z;

        //  p2
        ret.matrix[0][1] = p2.x;
        ret.matrix[1][1] = p2.y;
        ret.matrix[2][1] = p2.z;

        //  p3
        ret.matrix[0][2] = p3.x;
        ret.matrix[1][2] = p3.y;
        ret.matrix[2][2] = p3.z;

        // cout << "[TEST] rotation_matrix" << endl;
        // cout << ret << endl;

        return ret;
    }

    Transformation view_matrix(Point eye, Point look, Point up)
    {
        Point l = look - eye;
        l.normalize();

        Point r = l ^ up;
        r.normalize();

        Point u = r ^ l;

        Transformation trans_matrix = translation_matrix(-eye.x, -eye.y, -eye.z);

        this->matrix[0][0] = r.x;
        this->matrix[0][1] = r.y;
        this->matrix[0][2] = r.z;
        
        this->matrix[1][0] = u.x;
        this->matrix[1][1] = u.y;
        this->matrix[1][2] = u.z;
        
        this->matrix[2][0] = -l.x;
        this->matrix[2][1] = -l.y;
        this->matrix[2][2] = -l.z;

        return *this * trans_matrix;
    }

    static Transformation projection_matrix(double fov_y, double aspect_ratio, double near, double far)
    {
        double fov_x = fov_y * aspect_ratio;
        double t = near * tan(deg_to_rad(fov_y/2));
        double r = near * tan(deg_to_rad(fov_x/2));

        Transformation ret = identity_matrix();

        ret.matrix[0][0] = near/r;
        ret.matrix[1][1] = near/t;
        ret.matrix[2][2] = -(far+near)/(far-near);
        ret.matrix[3][3] = 0;

        ret.matrix[3][2] = -1;
        ret.matrix[2][3] = -(2*far*near)/(far-near);

        return ret;
    }

    Transformation operator* (const Transformation t)
    {  
        Transformation ret;

        for (int i=0; i < 4; i++) {
            for (int j=0; j < 4; j++) {
                ret.matrix[i][j] = 0;
                /*
                    this --> left (i->k)
                    t -----> right
                */
                for (int k=0; k < 4; k++) {
                    ret.matrix[i][j] += this->matrix[i][k] * t.matrix[k][j];
                }
            }
        }
        return ret;
    }

    Point operator* (const Point p)
    {
        double ret[4];
        double value;

        for (int i=0; i < 4; i++) {
            ret[i] = 0;
            
            for (int j=0; j < 4; j++) {
                if      (j == 0)    value = p.x;
                else if (j == 1)    value = p.y;
                else if (j == 2)    value = p.z;
                else if (j == 3)    value = p.w;

                ret[i] += this->matrix[i][j] * value;
            }
        }
        return Point(ret[0], ret[1], ret[2], ret[3]);
    }

    friend ostream& operator<< (ostream &out, Transformation &t)
    {
        for (int i=0; i < 4; i++) {
            for (int j=0; j < 4; j++) {
                out << t.matrix[i][j] << "\t";
            }
            out << "\n";
        }
        return out;
    }


};


#endif // TRANSFORMATION_H