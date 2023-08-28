/**
 * This file contains the implementation of the checker board class
 * extends the base Shape class.
 */

#ifndef CHECKER_BOARD_H
#define CHECKER_BOARD_H

#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

#include "shape.cpp"
#include "vector3d.cpp"

class CheckerBoard : public Shape {
 private:
  Vector3D v1, v2, v3, v4;

 public:
  /**
   * @brief Construct a new CheckerBoard object
   *
   * @param v1 the first vertex
   * @param v2 the second vertex
   * @param v3 the third vertex
   * @param v4 the fourth vertex
   */
  CheckerBoard(Vector3D v1, Vector3D v2, Vector3D v3, Vector3D v4)
      : v1(v1), v2(v2), v3(v3), v4(v4) {}
  /**
   * @brief empty constructor
   */
  CheckerBoard() : v1(), v2(), v3(), v4() {}
  /**
   * @brief Construct a new CheckerBoard object
   */
  CheckerBoard(const CheckerBoard& t)
      : v1(t.v1), v2(t.v2), v3(t.v3), v4(t.v4) {}

  /**
   * @overridden
   * @brief draw the checker board
   */
  void draw() {
    glBegin(GL_QUADS);
    {
      glVertex3f(v1[0], v1[1], v1[2]);
      glVertex3f(v2[0], v2[1], v2[2]);
      glVertex3f(v3[0], v3[1], v3[2]);
      glVertex3f(v4[0], v4[1], v4[2]);
    }
    glEnd();
  }
  /**
   * @overridden
   * @brief returns the normal vector of the checker board
   *
   * @param intersection_point the point of intersection
   * @param line the line from the camera
   * @return Line
   */
  Line getNormal(Vector3D& intersection_point, Line line) {
    Vector3D v12 = v2 - v1;
    Vector3D v13 = v3 - v1;
    Vector3D normal = v12 * v13;
    normal.normalize();
    return Line(intersection_point, normal);
  }
};

#endif  // CHECKER_BOARD_H