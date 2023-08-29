/**
 * This file contains the implementation of the checker board class
 * extends the base Shape class.
 */

#ifndef CHECKER_BOARD_H
#define CHECKER_BOARD_H

#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

#include "line.cpp"
#include "shape.cpp"
#include "vector3d.cpp"

class CheckerBoard : public Shape {
 private:
  Vector3D normal;        // the normal vector of the checker board plane
  double width;           // the width of each square of the checker board
  int number_of_squares;  // the number of squares in each row/column

 public:
  CheckerBoard(Vector3D position,
               Color color,
               double ambient_coefficient,
               double diffuse_coefficient,
               double specular_coefficient,
               double reflection_coefficient,
               double width)
      : Shape(position,
              color,
              ambient_coefficient,
              diffuse_coefficient,
              0,
              reflection_coefficient,
              1),
        width(width) {
    // set the normal vector of the checker board
    normal = Vector3D(0, 1, 0);
    number_of_squares = 128;
  }

  /**
   * @brief empty constructor
   */
  CheckerBoard()
      : Shape(), normal(Vector3D(0, 1, 0)), width(0), number_of_squares(0) {}

  /**
   * @overridden
   * @brief returns the normal vector of the checker board
   * @param intersection_point the point of intersection
   * @param line the incident line
   */
  Line getNormal(Vector3D& intersection_point, Line line) {
    // check on the direction of the line
    // if the line is going away from the checker board, then the normal vector
    // should be reversed
    if (line.getDirection().dot_product(normal) > 0) {
      return Line(intersection_point, normal * -1);
    }
    return Line(intersection_point, normal);
  }

  /**
   * @overridden
   * @brief draw the checker board
   */
  void draw() {
    {
      // set the color of the checker board

      // draw the checker board
      glBegin(GL_QUADS);
      for (int i = -number_of_squares + 1; i < number_of_squares; i++) {
        for (int j = -number_of_squares + 1; j < number_of_squares; j++) {
          // find the four corners of the square
          // * position holds the center of the checker board
          Vector3D bottom_left = position + Vector3D(i * width - width / 2, 0,
                                                     j * width - width / 2);
          Vector3D bottom_right = position + Vector3D(i * width + width / 2, 0,
                                                      j * width - width / 2);
          Vector3D top_right = position + Vector3D(i * width + width / 2, 0,
                                                   j * width + width / 2);
          Vector3D top_left = position + Vector3D(i * width - width / 2, 0,
                                                  j * width + width / 2);

          // debug print
          // cout<< "i: " << i << " j: " << j << endl;

          // draw the square
          {
            // * set the color of the square using i, j
            if ((i + j) % 2 == 0) {
              // cout<< "black" << endl;
              glColor3f(0.0, 0.0, 0.0);
            } else {
              // cout<< "white" << endl;
              glColor3f(1.0 * color[1] / 255, 1.0 * color[1] / 255,
                        1.0 * color[2] / 255);
            }
            glVertex3d(bottom_left[0], bottom_left[1], bottom_left[2]);
            glVertex3d(bottom_right[0], bottom_right[1], bottom_right[2]);
            glVertex3d(top_right[0], top_right[1], top_right[2]);
            glVertex3d(top_left[0], top_left[1], top_left[2]);
          }
        }
      }
      glEnd();
      // cout<< "done" << endl;
      // glutPostRedisplay();
    }
  }

  /**
   * @overridden
   * @brief returns the point of intersection of the checker board and the line
   * @param ray the incident line
   */
  double getIntersection(Line& ray) {
    // check if the line is parallel to the checker board
    if (ray.getDirection().dot_product(normal) == 0) {
      return -1;
    }

    // find the point of intersection
    double t = (position - ray.getStart()).dot_product(normal) /
               ray.getDirection().dot_product(normal);

    // check if the point of intersection is behind the camera
    if (t < 0) {
      return -1;
    }

    // check if the point of intersection is within the checker board
    Vector3D intersection_point = ray.getPoint(t);
    if (abs(intersection_point[0] - position[0]) > width * number_of_squares ||
        abs(intersection_point[2] - position[2]) > width * number_of_squares) {
      return -1;
    }

    return t;
  }

  /**
   * @overridden
   * @brief returns the color of the checker board at the point of intersection
   * @param intersection_point the point of intersection
   */
  Color getColorAt(Vector3D& intersection_point) {
    // find the color of the square
    int i = (intersection_point[0] - position[0]) / width;
    int j = (intersection_point[2] - position[2]) / width;
    if ((i + j) % 2 == 0) {
      return Color(0.0, 0.0, 0.0);
    } else {
      return Color(1.0 * color[0] / 255, 1.0 * color[1] / 255,
                   1.0 * color[2] / 255);
    }
  }

  /**
   * @brief prints the information of the checker board
   *
   */
  void print() {
    std::cout << "Checker Board" << std::endl;
    std::cout << "Position: ";
    position.print();
    std::cout << "Normal: ";
    normal.print();
    std::cout << "Width: " << width << std::endl;
    std::cout << "Number of Squares: " << number_of_squares << std::endl;
    std::cout << "Color: " << color[0] << " " << color[1] << " " << color[2]
              << std::endl;
    std::cout << "Ambient Coefficient: " << ambient_coefficient << std::endl;
    std::cout << "Diffuse Coefficient: " << diffuse_coefficient << std::endl;
    std::cout << "Specular Coefficient: " << specular_coefficient << std::endl;
    std::cout << "Reflection Coefficient: " << reflection_coefficient
              << std::endl;
  }
};

#endif  // CHECKER_BOARD_H