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

  // calculate the determinant of a 3x3 matrix
  double determinant(double matrix[3][3]) {
    return matrix[0][0] *
               (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
           matrix[0][1] *
               (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
           matrix[0][2] *
               (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
  }

 public:
  /**
   * @brief Construct a new Triangle object matching the parent class
   */
  Triangle(Vector3D v1,
           Vector3D v2,
           Vector3D v3,
           Color color,
           double ambient_coefficient,
           double diffuse_coefficient,
           double specular_coefficient,
           double reflection_coefficient,
           int specular_exponent)
      : Shape(v1,
              color,
              ambient_coefficient,
              diffuse_coefficient,
              specular_coefficient,
              reflection_coefficient,
              specular_exponent),
        v1(v1),
        v2(v2),
        v3(v3) {}

  /**
   * @brief empty constructor
   */
  Triangle()
      : Shape(),
        v1(Vector3D(1, 0, 0)),
        v2(Vector3D(0, 1, 0)),
        v3(Vector3D(0, 0, 1)) {}

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
  Line getNormal(Vector3D& intersection_point, Line line) {
    Vector3D v12 = v2 - v1;
    Vector3D v13 = v3 - v1;
    Vector3D normal = v12 * v13;
    normal.normalize();

    // check on which side of the triangle the line is
    if (normal.dot_product(line.getDirection()) > 0) {
      normal = normal * (-1);
    }

    return Line(intersection_point, normal);
  }
  /**
   * @overridden
   * @brief returns the point of intersection of the triangle and the line
   * @param ray the incident line
   */
  double getT(Line& ray) {
    // generate teh beta and gamma values
    // martix B
    double bMatrix[3][3] = {
        {v1[0] - ray.getStart()[0], v1[0] - v3[0], ray.getDirection()[0]},
        {v1[1] - ray.getStart()[1], v1[1] - v3[1], ray.getDirection()[1]},
        {v1[2] - ray.getStart()[2], v1[2] - v3[2], ray.getDirection()[2]}};
    // matrix gamma
    double gammaMatrix[3][3] = {
        {v1[0] - v2[0], v1[0] - ray.getStart()[0], ray.getDirection()[0]},
        {v1[1] - v2[1], v1[1] - ray.getStart()[1], ray.getDirection()[1]},
        {v1[2] - v2[2], v1[2] - ray.getStart()[2], ray.getDirection()[2]}};

    // matrix t
    double tMatrix[3][3] = {
        {v1[0] - v2[0], v1[0] - v3[0], v1[0] - ray.getStart()[0]},
        {v1[1] - v2[1], v1[1] - v3[1], v1[1] - ray.getStart()[1]},
        {v1[2] - v2[2], v1[2] - v3[2], v1[2] - ray.getStart()[2]}};

    // matrix A
    double aMatrix[3][3] = {
        {v1[0] - v2[0], v1[0] - v3[0], ray.getDirection()[0]},
        {v1[1] - v2[1], v1[1] - v3[1], ray.getDirection()[1]},
        {v1[2] - v2[2], v1[2] - v3[2], ray.getDirection()[2]}};

    // calculate the determinants
    double a = determinant(aMatrix);
    double b = determinant(bMatrix) / a;
    double gamma = determinant(gammaMatrix) / a;
    double t = determinant(tMatrix) / a;
    // check the following condition for intersection
    // * b>0 and gamma>0 and b+gamma<1 and t>0
    if (b > 0 && gamma > 0 && b + gamma < 1 && t > 0) {
      return t;
    } else {
      return -1;
    }
  }
  /**
   * @overridden
   * @brief returns the color of the triangle at the point of intersection
   * @param intersection_point the point of intersection
   */
  Color getColorAt(Vector3D& intersection_point) { return color; }

  /**
   * @brief returns if a point is inside the triangle
   */
  bool inside(Vector3D& point) {
    // calculate the vector representing the edges
    Vector3D v12 = v2 - v1;
    Vector3D v13 = v3 - v1;

    // calculate the normal vector
    Vector3D normal = v12 * v13;

    // calculate the barycentric coordinates
    double denominator = normal.length() * normal.length();
    Vector3D d = point - v1;
    double u = (d * v13).dot_product(normal) / denominator;
    double v = (v12 * d).dot_product(normal) / denominator;

    // check if the point is inside the triangle
    if (u >= 0 && v >= 0 && u + v <= 1) {
      return true;
    } else {
      return false;
    }
  }

  /**
   * destructor
   */
  ~Triangle() {}
};

#endif  // TRIANGLE_H
