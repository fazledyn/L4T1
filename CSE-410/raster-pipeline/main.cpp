#include <bits/stdc++.h>

#include "Point.hpp"
#include "Transformation.hpp"
#include "bitmap_image.hpp"

using namespace std;


class Color {
    public:
    int r, g, b;
    
    Color()
    {
        srand(time(0));
        this->r = rand() % 255;
        this->g = rand() % 255;
        this->b = rand() % 255;
    }
};


class Triangle {
    public:
    Point point[3];
    Color color;

    Triangle() {}

    Triangle(Point a, Point b, Point c)
    {
        point[0] = a;
        point[1] = b;
        point[2] = c;
    }
};


int main(int argc, char* argv[])
{
    int n_triangle = 0;

    ifstream input, config;
    ofstream output;

    double eye_x,   eye_y,  eye_z;
    double look_x,  look_y, look_z;
    double up_x,    up_y,   up_z;
    double fov_y, aspect_ratio, near, far;

    input.open("./test_case/1/scene.txt");
    output.open("./test_case/1/my_stage1.txt");

    input >> eye_x  >> eye_y        >> eye_z;
    input >> look_x >> look_y       >> look_z;
    input >> up_x   >> up_y         >> up_z;
    input >> fov_y  >> aspect_ratio >> near     >> far;

    //  Creating and pushing baseline matrix in the stack
    stack<Transformation> t_stack;
    Transformation base = Transformation::identity_matrix();
    t_stack.push(base);

    string command;

    cout << "starting .. stack size = " << t_stack.size() << endl;

    while (true)
    {
        input >> command;

        if (command == "triangle")
        {
            Point p1, p2, p3;
            input >> p1;
            input >> p2;
            input >> p3;

            p1 = t_stack.top() * p1;
            p2 = t_stack.top() * p2;
            p3 = t_stack.top() * p3;

            p1.scale();
            p2.scale();
            p3.scale();

            output << p1 << endl;
            output << p2 << endl;
            output << p3 << endl;
            output << endl;

            n_triangle++;
        }
        else if (command == "translate")
        {
            double tx, ty, tz;
            input >> tx >> ty >> tz;

            Transformation t = Transformation::translation_matrix(tx, ty, tz);
            t = t_stack.top() * t;
            t_stack.pop();
            t_stack.push(t);
        }
        else if (command == "rotate")
        {
            double angle, ax, ay, az;
            input >> angle >> ax >> ay >> az;

            Transformation t = Transformation::rotation_matrix(ax, ay, az, angle);
            t = t_stack.top() * t;
            t_stack.pop();
            t_stack.push(t);
        }
        else if (command == "scale")
        {
            double sx, sy, sz;
            input >> sx >> sy >> sz;

            Transformation t = Transformation::scale_matrix(sx, sy, sz);
            t = t_stack.top() * t;
            t_stack.pop();
            t_stack.push(t);
        }
        else if (command == "push")
        {
            Transformation t = Transformation::identity_matrix();
            t = t_stack.top() * t;      //  correct ?
            t_stack.push(t);
        }
        else if (command == "pop")
        {
            if (t_stack.size() == 1) {
                cout << "[ERROR] Stack size already 0. Can't pop" << endl;
                break;
            }
            t_stack.pop();
        }
        else if (command == "end")
        {
            break;
        }
        else
        {
            cout << "[ERROR] Wrong command -> " << command << endl;
            break;
        }
    }
    
    input.close();
    output.close();

    //  ---------------------------------------------
    //  Stage 1 End
    //  Stage 2 Starts
    //  ---------------------------------------------

    Point eye  (eye_x, eye_y, eye_z);
    Point look (look_x, look_y, look_z);
    Point up   (up_x, up_y, up_z);

    Transformation view_trans = Transformation::identity_matrix();
    view_trans = view_trans.view_matrix(eye, look, up);

    input.open("./test_case/1/my_stage1.txt");
    output.open("./test_case/1/my_stage2.txt");

    for (int i=0; i < n_triangle; i++)
    {
        Point p1, p2, p3;
        input >> p1 >> p2 >> p3;

        p1 = view_trans * p1;
        p2 = view_trans * p2;
        p3 = view_trans * p3;

        p1.scale();
        p2.scale();
        p3.scale();
        
        output << p1 << endl;
        output << p2 << endl;
        output << p3 << endl;
        output << endl;
    }

    input.close();
    output.close();

    //  ---------------------------------------------
    //  Stage 2 End
    //  Stage 3 Starts
    //  ---------------------------------------------
    
    Transformation proj_trans = Transformation::projection_matrix(fov_y, aspect_ratio, near, far);

    input.open("./test_case/1/my_stage2.txt");
    output.open("./test_case/1/my_stage3.txt");

    for (int i=0; i < n_triangle; i++)
    {
        Point p1, p2, p3;
        input >> p1 >> p2 >> p3;

        p1 = proj_trans * p1;
        p2 = proj_trans * p2;
        p3 = proj_trans * p3;

        p1.scale();
        p2.scale();
        p3.scale();

        output << p1 << endl;
        output << p2 << endl;
        output << p3 << endl;
        output << endl;
    }

    input.close();
    output.close();

    //  ---------------------------------------------
    //  Stage 3 End
    //  Stage 4 Starts
    //  ---------------------------------------------

    config.open("./test_case/1/config.txt");
    
    int screen_width, screen_height;
    double x_left, x_right;
    double y_top, y_bottom;
    double z_front, z_rear;
    
    config >> screen_width >> screen_height;
    config >> x_left;
    config >> y_bottom;
    config >> z_front >> z_rear;
    
    x_right = -x_left;
    y_top = -y_bottom;
    
    config.close();
    
    cout << screen_width << " " << screen_height << endl;
    cout << x_left << endl;
    cout << y_bottom << endl;
    cout << z_front << " " << z_rear << endl;
    
    //  LOL KORUM NA

    return 0;
}
