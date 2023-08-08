// This file is the 2nd part of the graphics pipeline
// @author: akibur rahman
#include <iostream>

#include <fstream>

#include <cmath>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#include "matrix.cpp"
#include "vector3d.cpp"

#include "color.cpp"
#include "triangle.cpp"

using namespace std;

/**
 * file structure: (stage3.txt)
 * stage 3 contains the following information:
 *          - all the triangles 3D coordinates
 *          - seperated by a new line
 *          - each line contains 3 coordinates seperated by a space
 *          - after each 3 line there is a blank line
 *          - file is ended by a blank line
 * file structure: (config.txt)
 * config contains the following information:
 *          - the height and width of the image
 */
/**
 * @brief find the top scanline of the triangle and does the clipping
 * @param t     the triangle
 * @param top   the top scanline of the canvas
 * @return      the top scanline
 */
double find_top(Triangle &t, double top_scanline) {
  double top = t[0][1];
  for (int i = 1; i < 3; i++) {
    if (t[i][1] > top) {
      top = t[i][1];
    }
  }
  if (top > top_scanline) {
    top = top_scanline;
  }
  return top;
}

/**
 * @brief find the bottom scanline of the triangle and does the clipping
 * @param t         the triangle
 * @param bottom    the bottom scanline of the canvas
 * @return          the bottom scanline
 */
double find_bottom(Triangle &t, double bottom_scanline) {
  double bottom = t[0][1];
  for (int i = 1; i < 3; i++) {
    if (t[i][1] < bottom) {
      bottom = t[i][1];
    }
  }
  if (bottom < bottom_scanline) {
    bottom = bottom_scanline;
  }
  return bottom;
}

/**
 * @brief find the intersection point of a line with a plane
 * @param p1    the first point of the line
 * @param p2    the second point of the line
 * @param y     the y value of the plane
 * @return      the intersection point
 */
Vector3D find_intersection(Vector3D &p1, Vector3D &p2, double y) {
  double x1 = p1[0];
  double y1 = p1[1];
  double z1 = p1[2];

  double x2 = p2[0];
  double y2 = p2[1];
  double z2 = p2[2];

  double x = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
  double z = z1 + (z2 - z1) * (y - y1) / (y2 - y1);

  return Vector3D(x, y, z);
}

int main() {
  // read the input files (stage3.txt, config.txt)
  ifstream stage3("stage3.txt");
  ifstream config("config.txt");

  // read the height and width of the image
  int height, width;
  config >> width >> height;

  // read the triangles
  vector<Triangle> triangles;
  double x, y, z;

  while (stage3 >> x >> y >> z) {
    Vector3D v1(x, y, z);
    stage3 >> x >> y >> z;
    Vector3D v2(x, y, z);
    stage3 >> x >> y >> z;
    Vector3D v3(x, y, z);
    Triangle t(v1, v2, v3);
    triangles.push_back(t);
  }

  // print the values
  cout << "Height: " << height << endl;
  cout << "Width: " << width << endl;
  cout << "Triangles: " << endl;
  for (int i = 0; i < triangles.size(); i++) {
    triangles[i].print();
  }

  // ! step 1 finished.

  // init the z-buffer
  double **z_buffer = new double *[height];
  for (int i = 0; i < height; i++) {
    z_buffer[i] = new double[width];
    for (int j = 0; j < width; j++) {
      // init the z-buffer with z_max
      z_buffer[i][j] = 2;
    }
  }

  // init the frame buffer (holds the color)
  Color **frame_buffer = new Color *[height];
  for (int i = 0; i < height; i++) {
    frame_buffer[i] = new Color[width];
    for (int j = 0; j < width; j++) {
      // init the frame buffer with black color
      frame_buffer[i][j] = Color(0, 0, 0);
    }
  }

  // create a pixel mappint between the x-y range values and the screen width
  double dx = 2.0 / width;
  double dy = 2.0 / height;

  double top_y = 1.0 - dy / 2;
  double left_x = -1.0 + dx / 2;

  // ! step 2 finished.

  // apply the procedure for each triangle
  for (int i = 0; i < triangles.size(); i++) {
    // find the top scanline
    double top_scanline = find_top(triangles[i], top_y);
    // find the bottom scanline
    double bottom_scanline = find_bottom(triangles[i], -1 * top_y);
    // print the top and bottom scanline
    cout << "Top: " << top_scanline << endl;
    cout << "Bottom: " << bottom_scanline << endl;

    for (int row = top_scanline; row >= bottom_scanline; row -= dy) {
      // find the left and right intersection columns
      Vector3D left_intersection =
          find_intersection(triangles[i][0], triangles[i][1], row);
    }

    return 0;
  }