#include <bits/stdc++.h>
#include "bitmap_image.hpp"
using namespace std;

#define IMAGE_NAME		"1_R.bmp"
#define IMAGE_UA_NAME	"1_RUA.bmp"
#define IMAGE_WA_NAME	"1_RWA.bmp"

int image_width, image_height;

bitmap_image *image;
bitmap_image *image_ua;
bitmap_image *image_wa;



void draw_pixel(int x, int y)
{
	if (image_height < y || image_width < x)
	{
		cout << "(ERROR) x or y is out of range" << endl;
		exit(0);
	}

	image->set_pixel(x, image_height - y - 1, 0, 0, 0);

	//	Need to edit this one
	image_ua->set_pixel(x, image_height - y - 1, 0, 0, 0);
	image_wa->set_pixel(x, image_height - y - 1, 0, 0, 0);
}


//	For Unweighted Area Sampling
void draw_ua_pixel(int x, int y)
{
	if (image_height < y || image_width < x)
	{
		cout << "(ERROR) x or y is out of range" << endl;
		exit(0);
	}
	
	

	image_ua->set_pixel(x, image_height - y - 1, 0, 0, 0);
}


//	For Weighted Area Sampling
void draw_wa_pixel(int x, int y)
{
	if (image_height < y || image_width < x)
	{
		cout << "(ERROR) x or y is out of range" << endl;
		exit(0);
	}

	image_wa->set_pixel(x, image_height - y - 1, 0, 0, 0);
}




void midpoint_line(int x_0, int y_0, int x_1, int y_1)
{
	int dx = x_1 - x_0;
	int dy = y_1 - y_0;

	//	m <= 1 
	if (dy <= dx)
	{
		int d = 2 * dy - dx;
		int inc_E = 2 * dy;
		int incr_NE = 2 * (dy - dx);

		int x = x_0;
		int y = y_0;

		draw_pixel(x, y);
		draw_ua_pixel(x, y);
		draw_wa_pixel(x, y);

		while (x < x_1)
		{
			if (d <= 0)
			{
				d += inc_E;
				x++;
			}
			else
			{
				d += incr_NE;
				x++;
				y++;
			}
			draw_pixel(x, y);
			draw_ua_pixel(x, y);
			draw_wa_pixel(x, y);
		}
	}
	//	m > 1
	else if (dy > dx)
	{
		int d = 2 * dx - dy;
		int inc_E = 2 * dx;
		int incr_NE = 2 * (dx - dy);

		int x = x_0;
		int y = y_0;

		draw_pixel(x, y);
		draw_ua_pixel(x, y);
		draw_wa_pixel(x, y);

		while (y < y_1)
		{
			if (d <= 0)
			{
				d += inc_E;
				y++;
			}
			else
			{
				d += incr_NE;
				x++;
				y++;
			}
			draw_pixel(x, y);
			draw_ua_pixel(x, y);
			draw_wa_pixel(x, y);
		}
	}
}





int main()
{
	ifstream input;
	input.open("input.txt");

	if (!input.is_open())
	{
		cout << "Error opening file" << endl;
		return 1;
	}

	input >> image_width >> image_height;

	image = 	new bitmap_image(image_width, image_height);
	image_ua = 	new bitmap_image(image_width, image_height);
	image_wa = 	new bitmap_image(image_width, image_height);

	for (int col = 0; col < image_width; col++)
	{
		for (int row = 0; row < image_height; row++)
		{
			image->set_pixel(col, row, 255, 255, 255);
			image_ua->set_pixel(col, row, 255, 255, 255);
			image_wa->set_pixel(col, row, 255, 255, 255);
		}
	}

	int n_lines;
	input >> n_lines;

	for (int i = 0; i < n_lines; i++)
	{
		int x_0, y_0, x_1, y_1;
		input >> x_0 >> y_0 >> x_1 >> y_1;

		midpoint_line(x_0, y_0, x_1, y_1);
	}

	// Saving Image
	// ------------
	image->save_image(IMAGE_NAME);
	image_ua->save_image(IMAGE_UA_NAME);
	image_wa->save_image(IMAGE_WA_NAME);

	return 0;
}
