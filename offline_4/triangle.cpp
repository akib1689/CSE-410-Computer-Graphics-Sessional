/**
 * This class is used to represent a triangle in 3D space.
 * This triangle extends the base Shape class.
 */

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

#include "shape.cpp"
#include "vector3d.cpp"

class Triangle : public Shape {
 private:
  // the three vertices of the triangle
  Vector3D v1, v2, v3;

 public:
  /**
   * @brief Construct a new Triangle object
   *
   * @param v1 the first vertex
   * @param v2 the second vertex
   * @param v3 the third vertex
   */
  Triangle(Vector3D v1, Vector3D v2, Vector3D v3) : v1(v1), v2(v2), v3(v3) {}
  /**
   * @brief empty constructor
   */
  Triangle() : v1(), v2(), v3() {}
  /**
   * @brief Construct a new Triangle object
   */
  Triangle(const Triangle &t) : v1(t.v1), v2(t.v2), v3(t.v3) {}

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
   * @overridden
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
   * @overridden
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
   * @overridden
   * @brief returns the point of intersection of the triangle and the line
   * @param ray the incident line
   */
  double getIntersection(Line &ray) {
    Vector3D v12 = v2 - v1;
    Vector3D v13 = v3 - v1;
    Vector3D normal = v12 * v13;
    normal.normalize();

    // check if the line is parallel to the triangle
    if (normal.dot_product(ray.getDirection()) == 0) {
      return -1;
    }

    // check if the line is intersecting the triangle
    Vector3D v1p = ray.getStart() - v1;
    double t = v1p.dot_product(normal) / ray.getDirection().dot_product(normal);
    if (t < 0) {
      return -1;
    }

    // check if the intersection point is inside the triangle
    Vector3D intersection_point = ray.getPoint(t);
    Vector3D v12p = intersection_point - v1;
    Vector3D v23 = v3 - v2;
    Vector3D v23p = intersection_point - v2;
    Vector3D v31 = v1 - v3;
    Vector3D v31p = intersection_point - v3;
    if (normal.dot_product(v12 * v12p) < 0 ||
        normal.dot_product(v23 * v23p) < 0 ||
        normal.dot_product(v31 * v31p) < 0) {
      return -1;
    }

    return t;
  }
  /**
   * @overridden
   * @brief returns the color of the triangle at the point of intersection
   * @param intersection_point the point of intersection
   */
  Color getColorAt(Vector3D &intersection_point) { return color; }

  /**
   * destructor
   */
  ~Triangle() {}
};

#endif  // TRIANGLE_H
