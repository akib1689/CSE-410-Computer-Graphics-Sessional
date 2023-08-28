/**
 * This class is used to represent a triangle in 3D space.
 * this triangle extends the base Shape class.
 */

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

#include "shape.cpp"
#include "vector3d.cpp"

class triangles : public Shape {
 private:
  // the three vertices of the triangle
  Vector3D v1, v2, v3;

 public:
  /**
   * @brief Construct a new triangles object
   *
   * @param v1 the first vertex
   * @param v2 the second vertex
   * @param v3 the third vertex
   */
  triangles(Vector3D v1, Vector3D v2, Vector3D v3) : v1(v1), v2(v2), v3(v3) {}
  /**
   * @brief empty constructor
   */
  triangles() : v1(), v2(), v3() {}
  /**
   * @brief Construct a new triangles object
   */
  triangles(const triangles &t) : v1(t.v1), v2(t.v2), v3(t.v3) {}

  /**
   * @brief returns the area of the triangle
   *
   * @return double
   */
  double area() {
    Vector3D v12 = v2 - v1;
    Vector3D v13 = v3 - v1;
    return 0.5 * (v12 * v13).length();
  }

  /**
   * @overriden
   * @brief draw the triangle
   */
  void draw() {
    glBegin(GL_TRIANGLES);
    {
      glVertex3f(v1[0], v1[1], v1[2]);
      glVertex3f(v2[0], v2[1], v2[2]);
      glVertex3f(v3[0], v3[1], v3[2]);
    }
    glEnd();
  }
  /**
   * @overriden
   * @brief returns the normal vector of the triangle
   * @param intersection_point the point of intersection
   * @param line the incident line
   */
  Line getNormal(Vector3D &intersection_point, Line line) {
    Vector3D v12 = v2 - v1;
    Vector3D v13 = v3 - v1;
    Vector3D normal = v12 * v13;
    normal.normalize();

    // check on which side of the triangle the line is
    if (normal.dot_product(line.getDirection()) > 0) {
      normal = normal * -1;
    }

    return Line(intersection_point, normal);
  }

  /**
   * destructor
   */
  ~triangles() {}
};

#endif  // TRIANGLE_H