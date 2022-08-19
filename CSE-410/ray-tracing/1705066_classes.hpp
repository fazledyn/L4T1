#ifndef _1705066_CLASSES_H
#define _1705066_CLASSES_H

#include <bits/stdc++.h>
#include <GL/glut.h>
using namespace std;

#define pi (2 * acos(0.0))
#define toRad(deg) (double)(deg * pi) / 180

#define DIFF_POSITION   0.0000000001
#define EPSILON         0.0000001

#define N_STACKS 100
#define N_SLICES 100

#define COLOR_RED   0
#define COLOR_GREEN 1
#define COLOR_BLUE  2

#define COEFF_AMBIENT    0
#define COEFF_DIFFUSE    1
#define COEFF_SPECULAR   2
#define COEFF_REFLECTION 3

class point;
class Ray;
class Color;
class Vector;
class Object;
class Triangle;
class General;
class PointLight;
class SpotLight;
class Sphere;


extern int N_RECURSION;

extern vector<Object*> objects;
extern vector<PointLight> pointLights;
extern vector<SpotLight> spotLights;


//  Simple point object for drawing shapes
class point
{
    public:
    	double x, y, z;
};


class Color
{
    public:
    double r, g, b;
    
    Color() {
        this->r = 0;
        this->g = 0;
        this->b = 0;
    }

    Color(double _r, double _g, double _b)
    {
        this->r = _r;
        this->g = _g;
        this->b = _b;
    }

    Color (const Color &color)
    {
        this->r = color.r;
        this->g = color.g;
        this->b = color.b;
    }

    Color operator* (const double value)
    {
        return Color(this->r * value, this->g * value, this->b * value);
    }

    Color operator* (const Color color)
    {
        return Color(this->r * color.r, this->g * color.g, this->b * color.b);
    }

    Color operator+ (const Color color)
    {
        return Color(this->r + color.r, this->g + color.g, this->b + color.b);
    }

    //  Wasted 5h just for this fault
    //  Color needs to be between (0-1) for this offline
    //  -______-
    void normalize()
    {
        if (r < 0)  r = 0;
        if (g < 0)  g = 0;
        if (b < 0)  b = 0;
        if (r > 1)  r = 1;
        if (g > 1)  g = 1;
        if (b > 1)  b = 1;        
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
    
    static Color set_white()
    {
        Color ret;
        ret.r = 1;
        ret.g = 1;
        ret.b = 1;
        return ret;
    }


};

class Vector
{
    public:
    double x, y, z, w;

    Vector() {}

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
            // glPushMatrix();
            // {
            //     glTranslated(light_pos.x, light_pos.y, light_pos.z);                
            //     // Sphere point(light_pos, 2);
            //     // point.setColor(color);
            //     // point.draw();
            // }
            // glPopMatrix();
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

class Ray
{
    public:
        Vector start;
        Vector direction;

        Ray() {}
        
        Ray(const Ray &ray)
        {
            this->start = ray.start;
            this->direction = ray.direction;
        }

        Ray(Vector _start, Vector _direction)
        {
            this->start = _start;
            this->direction = _direction;
            this->direction.normalize();
        }

        friend ostream& operator<< (ostream& out, Ray &ray)
        {
            out << "start: " << ray.start << endl;
            out << "direc: " << ray.direction << endl;
            return out;
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

        double intersectIlluminate(Ray &ray, Color &color, int level)
        {
            double t_min = this->intersect(ray, color, level);

            if (level == 0) return t_min;

            Vector intersectionPoint;
            intersectionPoint = ray.start + ray.direction * t_min;

            color = this->getColorAt(intersectionPoint) * this->coEff[COEFF_AMBIENT];
            color.normalize();
      
            //  Calculate normal at intersectionPoint
            Vector normal = this->getNormalAt(intersectionPoint);
            normal.normalize();

            for (PointLight eachLight: pointLights)
            {
                Vector lightDirection;
                lightDirection = eachLight.light_pos - intersectionPoint;                
                lightDirection.normalize();

                Vector lightPosition;
                lightPosition = intersectionPoint + lightDirection * DIFF_POSITION;
                
                Ray lightRay(eachLight.light_pos, lightDirection);

                Color tempColor;
                bool inShadow = false;
                double t, t_min_original = INFINITY;

                for (Object* eachObject : objects)
                {
                    t = eachObject->intersectIlluminate(ray, tempColor, 0);
                    if (t > 0 && t < t_min_original)
                    {
                        t_min_original = t;
                    }
                }

                if (t_min > t_min_original)
                {
                    inShadow = true;
                }

                //  If we aren't in the shadow region, we compute
                //  DIFFUSE and SPECULAR componenets
                //  R = 2.(L.N)N - L

                /*
                    lambert val is cos theta. theta is the ang betn L and N
                    phong val is cos(ang betn R & V) to the power shine                
                */
                if ( !inShadow )
                {
                    Vector R = normal * 2.0 * (normal * lightDirection) - lightDirection;

                    R.normalize();

                    double lambertValue = max(normal * lightDirection, 0.0);
                    double phongValue = max(pow(R * lightDirection, this->shine), 0.0);

                    color = color + this->getColorAt(intersectionPoint) * eachLight.color * lambertValue * this->coEff[COEFF_DIFFUSE];
                    color.normalize();

                    color = color + eachLight.color * phongValue * this->coEff[COEFF_SPECULAR];
                    color.normalize();
                }
            }
            
            //  Perform recursive reflection
            //  FFS
            if (level >= N_RECURSION)   return t_min;

            Vector refRayDirection;
            refRayDirection = ray.direction - normal * 2.0 * (normal * ray.direction);

            cout << "Before normalize() at: " << __LINE__ << endl;            
            refRayDirection.normalize();

            Vector refRayStart;
            refRayStart = intersectionPoint + refRayDirection * DIFF_POSITION;
            Ray refRay(refRayStart, refRayDirection);

            //  Nearest intersected Object
            Color refColor;
            int nearest = -1;
            double t_ref, t_ref_min = INFINITY;

            for (int i=0; i < objects.size(); i++)
            {
                t_ref = objects[i]->intersectIlluminate(refRay, refColor, 0);
                if (t_ref > 0 && t_ref < t_ref_min)
                {
                    t_ref_min = t_ref;
                    nearest = i;
                }
            }

            if (nearest != -1)
            {
                t_ref_min = objects[nearest]->intersectIlluminate(refRay, refColor, level + 1);
                color = color + refColor * this->coEff[COEFF_REFLECTION];
                color.normalize();
            }

            return t_min;
        }

        Vector getNormalAt(Vector &intersectionPoint)
        {
            return Vector();
        }

        virtual Color getColorAt(Vector &intersectionPoint)
        {
            return this->color;
        }

        virtual double intersect(Ray &ray, Color &color, int level)
        {
            return -1.0;
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
            cout << "ref_point: " << ref_point << endl;

            this->length = tile_width;          //  are you sure ???
        }

        void draw() override
        {
            int n_grid = (int) floor_width/tile_width;

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

        double intersect(Ray &ray, Color &color, int level)
        {
            Vector normal(0, 0, 1);

            double t = (-1.0) * ((normal * ray.start)/(normal * ray.direction));
            Vector intPoint = ray.start + ray.direction * t;

            if (intPoint.x < ref_point.x || intPoint.x > -ref_point.x) {
                return -1;
            }

            if (intPoint.y < ref_point.y || intPoint.y > -ref_point.y) {
                return -1;
            }

            return t;
        }

        Vector getNormalAt(Vector &intersectionVector)
        {
            return Vector(0, 0, 1);
        }

        Color getColorAt(Vector &intersectionPoint)
        {
            if (intersectionPoint.x < ref_point.x || intersectionPoint.x > -ref_point.x) {
                return Color(0, 0, 0);
            }

            if (intersectionPoint.y < ref_point.y || intersectionPoint.y > -ref_point.y) {
                return Color(0, 0, 0);
            }

            int col = (ref_point.x + intersectionPoint.x) / tile_width;
            int row = (ref_point.y + intersectionPoint.y) / tile_width;

            if ((row + col) % 2 == 0) {
                return Color(0, 0, 0);
            }

            return Color(1, 1, 1);
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

        double intersect(Ray &ray, Color &color, int level)
        {
            Vector edge_ba = b - a;
            Vector edge_ca = c - a;

            Vector vert = ray.direction ^ edge_ca;
            double const_a = edge_ba * vert;

            //  Ray parallel to the surface of triangle
            if (const_a > -EPSILON && const_a < EPSILON)
            {
                return -1;
            }

            double f = 1.0/const_a;
            Vector s = ray.start - a;

            double u = f * (s * vert);
            if (u < 0.0 || u > 1.0) {
                return -1;
            }

            Vector q = s ^ edge_ba;
            double v = f * (ray.direction * q);
            if (v < 0.0 || (u + v) > 1.0) {
                return -1;
            }

            double t = f * (edge_ca * q);
            if (t > EPSILON) {
                return t;
            }

            return -1;
        }

        Vector getNormalAt(Vector &intPoint)
        {
            Vector normal = (b - a) ^ (c - a);
            normal.normalize();
            return normal;
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
        double radius;

        Sphere(Vector center, double radius)
        {
            this->ref_point = center;
            this->radius = radius;
        }

        void draw()
        {
            glColor3d(color.r, color.g, color.b);
            glPushMatrix();
            {
                glTranslated(ref_point.x, ref_point.y, ref_point.z);         
                drawHalfSphere(0, this->radius);
                glPushMatrix();
                {
                    glRotated(180, 1, 0, 0);
                    drawHalfSphere(0, this->radius);
                }
                glPopMatrix();
            }
            glPopMatrix();
        }

        double intersect(Ray &ray, Color &color, int level)
        {
            Vector sphereOrigin = ray.start - ref_point;

            double a = 1;
            double b = (ray.direction * sphereOrigin) * 2.0;
            double c = (sphereOrigin * sphereOrigin) - radius * radius;

            double det = b*b - 4*a*c;
            if (det < 0) {
                return -1;
            }

            double d = sqrt(det);
            double t1 = (-b - d)/(2*a);
            double t2 = (-b + d)/(2*a);
            
            if (t1 < 0 && t2 < 0) {
                return -1;
            }
            
            if (t1 > 0) {
                return t1;
            }
            
            if (t2 > 0) {
                return t2;
            }
            
            return -1;
        }

        Vector getNormalAt(Vector &intPoint)
        {
            Vector normal = intPoint - ref_point;
            normal.normalize();
            return normal;
        }

};

/**
 * General Quadric Surface
 * More Info: http://skuld.bmsc.washington.edu/people/merritt/graphics/quadrics.html
*/

class General: public Object
{
    public:
        double A, B, C, D, E, F, G, H, I, J;

        General() {}
        General(double A, double B, double C, double D, double E, double F, double G, double H, double I, double J, double length, double width, double height, Vector point)
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

            this->length = length;
            this->width = width;
            this->height = height;
            this->ref_point = point;

            cout << "CO EFFs: " << A << ", " << B << ", " << C << ", " << D << ", " << E << ", " << F << ", " << G << ", " << H << ", " << I << ", " << J << endl;
            cout << "LWH: " << length << ", " << width << ", " << height << endl;
            cout << "REF: " << ref_point << endl; 
        }

        bool isWithinRange(Vector point)
        {
            if (length != 0)
            {
                if (point.x < ref_point.x || point.x > ref_point.x + length)
                    return false;
            }
            if (width != 0)
            {
                if (point.y < ref_point.y || point.y > ref_point.y + width)
                    return false;
            }
            if (height != 0)
            {
                if (point.z < ref_point.z || point.z > ref_point.z + height)
                    return false;
            }
            return true;
        }

        /**
         * 
            F(x, y, z) = Ax2 + By2 + Cz2 + Dxy+ Exz + Fyz + Gx + Hy + Iz + J = 0
            Then substitute in ray equation R(t) = Ro + Rd and we get quadratic equation:

            Aq.t2 + Bq.t + Cq = 0 with
            
            Aq = A.xd2    + B.yd2   + C.zd2 + 
                + D.xd.yd + E.xd.zd + F.yd.zd
            
            Bq = 2*A.xo.xd          + 2*B.yo.yd         + 2*C.zo.zd         +
                + D(xo.yd + yo.xd)  + E(xo.zd + zo.xd)  + F(yo.zd + yd.zo)  +
                + G.xd              + H.yd              + I.zd

            Cq = A.xo2 + B.yo2 + C.zo2 +
                + D.xo.yo + E.xo.zo + F.yo.zo +
                + G.xo + H.yo + I.zo + J
        */

        double intersect(Ray &ray, Color &color, int level)
        {
            double xo = ray.start.x,    xd = ray.direction.x;
            double yo = ray.start.y,    yd = ray.direction.y;
            double zo = ray.start.z,    zd = ray.direction.z;

            double aq = 0;
            aq += A*pow(xd, 2) + B*pow(yd, 2) + C*pow(zd, 2);
            aq += D*xd*yd + E*xd*zd + F*yd*zd;
            
            double bq = 0;
            bq += 2*A * xo*xd + 2*B * yo*yd + 2*C * zo*zd;
            bq += D * (xo*yd + yo*xd) + E * (xo*zd + zo*xd) + F * (yo*zd + yd*zo);
            bq += G * zd + H * yd + I * zd;
            
            double cq = 0;
            cq += A*pow(xo, 2) + B*pow(yo, 2) + C*pow(zo, 2);
            cq += D * (xo * yo) + E * (xo * zo) + F * (yo * zo);
            cq += G * xo + H * yo + I * zo * J;

            double det = bq*bq - 4*aq*cq;
            if (det < 0) {
                return -1;
            }

            double d = sqrt(det);
            double t1 = (-bq - d)/(2 * aq);
            double t2 = (-bq + d)/(2 * aq);

            Vector point_1 = ray.start + ray.direction * t1;
            Vector point_2 = ray.start + ray.direction * t2;
            
            if (t1 < 0 && t2 < 0) {
                return -1;
            }
            if (t1 > 0 && isWithinRange(point_1)) {
                return t1;
            }
            if (t2 > 0 && isWithinRange(point_2)) {
                return t2;
            }

            return -1;
        }

        /*
        *   Equation : F(x,y,z) = Ax2 + By2 + Cz2 + Dxy + Exz + Fyz + Gx + Hy + Iz + J = 0
        *   Normal (Partial derivative):
        *   -----------------------------
        *       dF/dx = 2Ax + Dy + Ez + G
        *       dF/dy = 2By + Dx + Fz + H
        *       dF/dz = 2Cz + Ex + Fy + I       
        */

        Vector getNormalAt(Vector &intPoint)
        {
            double df_dx = 2*A * intPoint.x + D * intPoint.y + E * intPoint.z + G;
            double df_dy = 2*B * intPoint.y + D * intPoint.x + F * intPoint.z + H;
            double df_dz = 2*C * intPoint.z + E * intPoint.x + F * intPoint.y + I;

            Vector normal(df_dx, df_dy, df_dz);
            normal.normalize();            
            return normal;
        }


};


#endif