#include <bits/stdc++.h>

#include "Point.hpp"
#include "Transformation.hpp"
#include "bitmap_image.hpp"

using namespace std;

#define INF INFINITY

//  Input Filenames
//  -----------------------
#define SCENE_FILE  "./test_case/4/scene.txt"
#define CONFIG_FILE "./test_case/4/config.txt"

#define STAGE_1_FILE    "./test_case/4/my_stage1.txt"
#define STAGE_2_FILE    "./test_case/4/my_stage2.txt"
#define STAGE_3_FILE    "./test_case/4/my_stage3.txt"
#define Z_BUFFER_FILE   "./test_case/4/my_z_buffer.txt"
#define IMAGE_FILE      "./test_case/4/my_out.bmp"
//  -----------------------


class Color
{
    public:
    int r, g, b;
    
    Color()
    {
        this->r = rand() % 255;
        this->g = rand() % 255;
        this->b = rand() % 255;
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


class Triangle
{
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

    double get_max_y()
    {
        double max_1 = max(point[0].y, point[1].y);
        return max(max_1, point[2].y);
    }

    double get_min_y()
    {
        double min_1 = min(point[0].y, point[1].y);
        return min(min_1, point[2].y);
    }
};


int main(int argc, char* argv[])
{
    //  Seeding rand() function for colors
    srand(time(0));
    int n_triangle = 0;

    ifstream input, config;
    ofstream output;

    double eye_x,   eye_y,  eye_z;
    double look_x,  look_y, look_z;
    double up_x,    up_y,   up_z;
    double fov_y, aspect_ratio, near, far;

    //  ---------------------------------------------
    //  Stage 1 Starts
    //  ---------------------------------------------

    input.open(SCENE_FILE);
    output.open(STAGE_1_FILE);

    input >> eye_x  >> eye_y        >> eye_z;
    input >> look_x >> look_y       >> look_z;
    input >> up_x   >> up_y         >> up_z;
    input >> fov_y  >> aspect_ratio >> near     >> far;

    //  Creating and pushing baseline matrix in the stack
    stack<Transformation> t_stack;
    string command;

    Transformation base = Transformation::identity_matrix();
    t_stack.push(base);

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

    input.open(STAGE_1_FILE);
    output.open(STAGE_2_FILE);

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

    input.open(STAGE_2_FILE);
    output.open(STAGE_3_FILE);

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

    config.open(CONFIG_FILE);
    input.open(STAGE_3_FILE);
    output.open(Z_BUFFER_FILE);
    
    /*
        These variable definitions are as per configuration files.
        Not to be confused with scan line values.
    */

    int screen_width, screen_height;
    double x_min, x_max;
    double y_min, y_max;
    double z_front, z_rear;
    
    config >> screen_width >> screen_height;
    config >> x_min;
    config >> y_min;
    config >> z_front >> z_rear;
    
    x_max = -x_min;
    y_max = -y_min;
    
    vector<Triangle> triangle_list;

    for (int i=0; i < n_triangle; i++)
    {
        Point a, b, c;
        input >> a >> b >> c;

        triangle_list.push_back(Triangle(a, b, c));
        // cout << "Triangle input from stage3 complete" << endl;
    }

    vector <vector <double>> z_buffer;
    vector <vector <Color>> frame_buffer;

    //  Init z_buffer & frame_buffer (dynamically)
    for (int i=0; i < screen_height; i++)
    {
        vector<double> z_temp;
        vector<Color> f_temp;

        for (int j=0; j < screen_width; j++)
        {
            z_temp.push_back(z_rear);
            f_temp.push_back(Color::set_black());
        }
        z_buffer.push_back(z_temp);
        frame_buffer.push_back(f_temp);
    }

    /*
        Following variable definitions are for scanning.
        dx, dy -> grid segments width and height.

        scan_x_max -> Topmost scan limit    = x_max - dx/2
        scan_x_min -> Bottom most           = x_min + dx/2
        scan_y_max -> Rightmost scan limit
        scan_y_min -> Leftmost
    */

    double dx = (x_max - x_min)/screen_width;
    double dy = (y_max - y_min)/screen_height;

    double scan_x_max = (x_max - dx/2);
    double scan_x_min = (x_min + dx/2);
    double scan_y_max = (y_max - dy/2);
    double scan_y_min = (y_min + dy/2);

    //  LOL KORUM NA

    for (Triangle each_triangle: triangle_list)
    {
        double tri_max_y = each_triangle.get_max_y();
        double tri_min_y = each_triangle.get_min_y();

        int top_scan_index, bottom_scan_index;

        if (tri_max_y >= scan_y_max) {
            top_scan_index = 0;
        }
        else {
            top_scan_index = int(round((scan_y_max - tri_max_y)/dy));
        }

        if (tri_min_y <= scan_y_min) {
            bottom_scan_index = screen_height - 1;
        }
        else {
            bottom_scan_index = screen_height - 1 - int(round((tri_min_y - scan_y_min)/dy));
        }
        

        for (int row=top_scan_index; row <= bottom_scan_index; row++)
        {
            double yp = scan_y_max - row * dy;

            //  Setting intersection point with y-value only
            Point intersection[3];
            for (int i=0; i < 3; i++) {
                intersection[i] = Point(NAN, yp, INFINITY);
            }

            //  Find intersection on 3 lines/edges
            for (int i=0; i < 3; i++)
            {
                Point m = each_triangle.point[(3-i) % 3];
                Point n = each_triangle.point[(3-i-1) % 3];
                // cout << "Selecting line between " << (3-i) % 3 << " and " << (3-i-1)%3 << endl;

                if (m.y != n.y)
                {
                    // cout << endl << "m.y is not equal to n.y" << endl;
                    /*
                                              xn - xm
                        x = xm + (y - ym) x __________
                                              yn - ym
                    */
                    double temp_x = m.x + (yp - m.y) * ((n.x - m.x)/(n.y - m.y));
                    double temp_z = m.z + (yp - m.y) * ((n.z - m.z)/(n.y - m.y));    //  Newly Added

                    intersection[i].x = temp_x;
                    intersection[i].z = temp_z;                                         //  Newsly Added
                }
            }

            //  Let's filter out the invalid points
            //  (a line can't intersect a triangle in more than 2 points)

            for (int i=0; i < 3; i++)
            {
                Point m = each_triangle.point[(3-i) % 3];
                Point n = each_triangle.point[(3-i-1) % 3];
                // cout << "Selecting line between " << (3-i) % 3 << " and " << (3-i-1)%3 << endl;

                if (intersection[i].x != NAN)
                {
                    if ((intersection[i].x > max(m.x, n.x)) || (intersection[i].x < min(m.x, n.x))) {
                        intersection[i].x = NAN;
                    }
                }

                //  Newly Added
                if (intersection[i].z != INF)
                {
                    if ((intersection[i].z > max(m.z, n.z)) || (intersection[i].z < min(m.z, n.z))) {
                        intersection[i].z = INF;
                    }
                }
            }

            int x_max_index = -1;
            int x_min_index = -1;

            double tri_max_x = -INF;
            double tri_min_x = INF;

            for (int i=0; i < 3; i++)
            {                
                if (intersection[i].x != NAN)
                {
                    //  Lftmost intersection point
                    if (intersection[i].x < tri_min_x) {
                        x_min_index = i;
                        tri_min_x = intersection[i].x;
                    }

                    //  Rightmost intersection point
                    if (intersection[i].x > tri_max_x) {
                        x_max_index = i;
                        tri_max_x = intersection[i].x;
                    }
                }
            }


            int left_scan_index, right_scan_index;

            if (tri_min_x <= scan_x_min) {
                left_scan_index = 0;
            }
            else {
                left_scan_index = int(round((tri_min_x - scan_x_min)/dx));
            }

            if (tri_max_x >= scan_x_max) {
                right_scan_index = screen_width - 1;
            }
            else {
                right_scan_index = screen_width - 1 - int(round((scan_x_max - tri_max_x)/dx));
            }

            //  Apply the formula from class
            double xa = intersection[x_min_index].x;
            double za = intersection[x_min_index].z;

            double xb = intersection[x_max_index].x;
            double zb = intersection[x_max_index].z;

            double const_term = dx * ((zb-za)/(xb-xa));

            for (int col = left_scan_index; col <= right_scan_index; col++)
            {
                double xp = scan_x_min + col * dx;
                double zp;

                if (col == left_scan_index) {
                    zp = za + ((xp-xa)/(xb-xa))*(zb-za);
                }
                else {
                    zp += const_term;
                }

                if (zp > z_front)
                {
                    if (zp < z_buffer[row][col])
                    {
                        z_buffer[row][col] = zp;
                        frame_buffer[row][col] = each_triangle.color;
                    }
                }
            }
        }
    }

    //  Confusing times. Width comes first
    //  Column is set first, then row
    bitmap_image image(screen_width, screen_height);

    for (int row=0; row < screen_height; row++)
    {
        for (int col=0; col < screen_width; col++)
        {
            //  Exporting z_buffer value to file
            if (z_buffer[row][col] < z_rear) {
                output << fixed << setprecision(6) << z_buffer[row][col] << "\t";
            }

            //  Save pixel color to image
            int red = frame_buffer[row][col].r;
            int green = frame_buffer[row][col].g;
            int blue = frame_buffer[row][col].b;
            
            image.set_pixel(col, row, red, green, blue);
        }
        output << endl;
    }

    image.save_image(IMAGE_FILE);

    input.close();
    output.close();
    config.close();

    //  ---------------------------------------------
    //  Stage 4 End
    //  Cleanup Starts
    //  ---------------------------------------------

    for (int i=0; i < screen_height; i++) {
        z_buffer[i].clear();
        z_buffer[i].shrink_to_fit();
        frame_buffer[i].clear();
        frame_buffer[i].shrink_to_fit();
    }
    
    z_buffer.clear();
    z_buffer.shrink_to_fit();
    frame_buffer.clear();
    frame_buffer.shrink_to_fit();

    return 0;
}
