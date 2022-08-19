#ifndef _1705066_CLASSES_H
#define _1705066_CLASSES_H

#include <bits/stdc++.h>
#include <GL/glut.h>
using namespace std;

#define pi (2 * acos(0.0))
#define toRad(deg) (double)(deg * pi) / 180

#define N_STACKS 100
#define N_SLICES 100

#define COLOR_RED   0
#define COLOR_GREEN 1
#define COLOR_BLUE  2

#define COEFF_AMBIENT    0
#define COEFF_DIFFUSE    1
#define COEFF_SPECULAR   2
#define COEFF_REFLECTION 3


class point
{
    public:
    	double x, y, z;
};

class unit_vector
{
    public:
    	double x, y, z;
};

class Color
{
    public:
    double r, g, b;
    
    Color()
    {
        this->r = rand() % 255;
        this->g = rand() % 255;
        this->b = rand() % 255;
    }

    Color(double _r, double _g, double _b)
    {
        this->r = _r;
        this->g = _g;
        this->b = _b;
    }
    
    friend ostream& operator<< (ostream& out, Color &color)
    {
        out << fixed << setprecision(7) << color.r << " " << color.g << " " << color.b;
        return out;
    }

    friend ifstream& operator>> (ifstream& in, Color &color)
    {
        in >> color.r >> color.g >> color.b;
        return in;
    }

    static Color set_black()
    {
        Color ret;
        ret.r = 0;
        ret.g = 0;
        ret.b = 0;
        return ret;
    }
};

class Vector
{
    public:
    double x, y, z, w;

    Vector()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->w = 1.0;
    }

    Vector(double _x, double _y, double _z, double _w)
    {
        this->x = _x;
        this->y = _y;
        this->z = _z;
        this->w = _w;
    }

    Vector(double _x, double _y, double _z)
    {
        this->x = _x;
        this->y = _y;
        this->z = _z;
        this->w = 1.0;
    }
    

    Vector(const Vector &p)
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

    Vector operator+ (const Vector p)
    {
        return Vector(this->x + p.x, this->y + p.y, this->z + p.z);
    }

    Vector operator- (const Vector p)
    {
        return Vector(this->x - p.x, this->y - p.y, this->z - p.z);
    }

    Vector operator* (const double value)
    {
        return Vector(this->x*value, this->y*value, this->z*value);
    }

    double operator* (const Vector p)
    {
        return this->x*p.x + this->y*p.y + this->z*p.z;
    }

    Vector operator^ (const Vector p)
    {
        return Vector(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x);
    }

    friend ostream& operator<< (ostream& out, Vector &p)
    {
        out << fixed << setprecision(7) << p.x << " " << p.y << " " << p.z;
        return out;
    }

    friend ifstream& operator>> (ifstream& in, Vector &p)
    {
        in >> p.x >> p.y >> p.z;
        return in;
    }

};


class Object
{
    public:
        Vector ref_point;
        Color color;

        double height, width, length;
        double coEff[4];
        int shine;

        Object() {}
        virtual void draw() {}

        void setColor(Color color) {
            this->color = color;
        }

        void setShine(int shine) {
            this->shine = shine;
        }

        void setCoEff(double ambient, double diffuse, double specular, double reflection)
        {
            coEff[COEFF_AMBIENT] = ambient;
            coEff[COEFF_DIFFUSE] = diffuse;
            coEff[COEFF_SPECULAR] = specular;
            coEff[COEFF_REFLECTION] = reflection;
        }
};


class Floor: public Object
{
    public:
        double floor_width;
        double tile_width;

        Floor(){}

        Floor(double floor_width, double tile_width)
        {
            this->floor_width = floor_width;
            this->tile_width = tile_width;

            this->ref_point = Vector(-floor_width/2, -floor_width/2, 0);
            this->length = tile_width;          //  are you sure ???
        }

        void draw()
        {
            int n_grid = int(floor_width/tile_width);

            for (int row = 0; row < n_grid; row++)
            {
                for (int col = 0; col < n_grid; col++)
                {
                    if ((row + col) % 2 == 0)   { glColor3d(0, 0, 0); }
                    else                        { glColor3d(255, 255, 255); }

                    glBegin(GL_QUADS);
                    {
                        glVertex3f(ref_point.x + col * tile_width,      ref_point.y + tile_width * row,     ref_point.z);
                        glVertex3f(ref_point.x + (col+1) * tile_width,  ref_point.y + tile_width * row,     ref_point.z);
                        glVertex3f(ref_point.x + (col+1) * tile_width,  ref_point.y + tile_width * (row+1), ref_point.z);
                        glVertex3f(ref_point.x + col * tile_width,      ref_point.y + tile_width * (row+1), ref_point.z);
                    }
                    glEnd();
                }
            }
        }
};


class Triangle: public Object
{
    public:
        Vector a, b, c;

        Triangle() {}
        Triangle(Vector _a, Vector _b, Vector _c)
        {
            this->a = _a;
            this->b = _b;
            this->c = _c;
        }

        void draw()
        {
            glColor3d(color.r, color.g, color.b);
            glBegin(GL_TRIANGLES);
            {
                glVertex3d(a.x, a.y, a.z);
                glVertex3d(b.x, b.y, b.z);
                glVertex3d(c.x, c.y, c.z);
            }
            glEnd();
        }

        void setColor(Color color)
        {
            Object::setColor(color);
        }

        void setCoEff(double ambient, double diffuse, double specular, double reflection)
        {
            Object::setCoEff(ambient, diffuse, specular, reflection);
        }

        void setShine(int shine)
        {
            Object::setShine(shine);
        }

};

class Sphere : public Object
{
    void drawOneEighthSphere(double radius)
    {
        struct point points[N_STACKS + 1][N_SLICES + 1];
        double h, r;

        //	Generate Points
        for (int i = 0; i <= N_STACKS; i++)
        {
            h = radius * sin(((double)i / (double)N_STACKS) * (pi / 2));
            r = radius * cos(((double)i / (double)N_STACKS) * (pi / 2));
            for (int j = 0; j <= N_SLICES; j++)
            {
                points[i][j].x = r * cos(((double)j / (double)N_SLICES) * 0.5 * (pi));
                points[i][j].y = r * sin(((double)j / (double)N_SLICES) * 0.5 * (pi));
                points[i][j].z = h;
            }
        }
        //	Draw Quads
        for (int i = 0; i < N_STACKS; i++)
        {
            for (int j = 0; j < N_SLICES; j++)
            {
                glBegin(GL_QUADS);
                {
                    glVertex3d(points[i][j].x, points[i][j].y, points[i][j].z);
                    glVertex3d(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                    glVertex3d(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                    glVertex3d(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                }
                glEnd();
            }
        }
    }

    void drawHalfSphere(double padding, double radius)
    {
        int angle[4] = {0, 270, 90, 180};
        int translation[4][3] = {
            {1, 1, 1},
            {1, -1, 1},
            {-1, 1, 1},
            {-1, -1, 1},
        };

        for (int i = 0; i < 4; i++)
        {
            for (int i = 0; i < 4; i++)
            {
                double x = translation[i][0] * padding;
                double y = translation[i][1] * padding;
                double z = translation[i][2] * padding;
                glPushMatrix();
                {
                    glTranslated(x, y, z);
                    glRotated(angle[i], 0, 0, 1);
                    drawOneEighthSphere(radius);
                }
                glPopMatrix();
            }
        }
    }

    public:
        Sphere(Vector center, double radius)
        {
            this->ref_point = center;
            this->width = radius;
        }

        void draw()
        {
            glColor3d(color.r, color.g, color.b);
            glPushMatrix();
            {
                glTranslated(ref_point.x, ref_point.y, ref_point.z);         
                drawHalfSphere(0, this->width);
                glPushMatrix();
                {
                    glRotated(180, 1, 0, 0);
                    drawHalfSphere(0, this->width);
                }
                glPopMatrix();
            }
            glPopMatrix();
        }

        void setColor(Color color)
        {
            Object::setColor(color);
        }

        void setCoEff(double ambient, double diffuse, double specular, double reflection)
        {
            Object::setCoEff(ambient, diffuse, specular, reflection);
        }

        void setShine(int shine)
        {
            Object::setShine(shine);
        }
};

class General: public Object
{
    public:
        double A, B, C, D, E, F, G, H, I, J;

        General() {}
        General(double A, double B, double C, double D, double E, double F, double G, double H, double I, double J)
        {
            this->A = A;
            this->B = B;
            this->C = C;
            this->D = D;
            this->E = E;
            this->F = F;
            this->G = G;
            this->H = H;
            this->I = I;
            this->J = J;
        }

        void draw()
        {
            //  ...
        }

        void setColor(Color color)
        {
            Object::setColor(color);
        }

        void setCoEff(double ambient, double diffuse, double specular, double reflection)
        {
            Object::setCoEff(ambient, diffuse, specular, reflection);
        }

        void setShine(int shine)
        {
            Object::setShine(shine);
        }

};

class PointLight
{
    public:
        Vector light_pos;
        Color color;

        PointLight() {}
        PointLight(Vector position, Color color)
        {
            this->light_pos = position;
            this->color = color;
        }

        void draw()
        {
            glPushMatrix();
            {
                glTranslated(light_pos.x, light_pos.y, light_pos.z);                
                Sphere point(light_pos, 2);
                point.setColor(color);
                point.draw();
            }
            glPopMatrix();
        }
};

class SpotLight : public PointLight
{
    public:
        Vector light_direction;
        double cutoff_angle;

        SpotLight() {}
        SpotLight(Vector position, Color color, Vector direction, double cutoff_angle)
        {
            this->light_pos = position;
            this->color = color;
            this->light_direction = direction;
            this->cutoff_angle = cutoff_angle;
        }

        void draw()
        {
            glPushMatrix();
            {
                glColor3d(color.r, color.g, color.b);
                glBegin(GL_TRIANGLES);
                {
                    glVertex3d(light_pos.x, light_pos.y, light_pos.z);
                    glVertex3d(light_pos.x + 7, light_pos.y + 7, light_pos.z + 7);
                    glVertex3d(light_pos.x + 7, light_pos.y + 7, light_pos.z + 10);
                }
                glEnd();
            }
            glPopMatrix();

        }
};

#endif