/**
 * This is a sphere class
 * parent class of the sphere: shape
 */

#ifndef SPHERE_H
#define SPHERE_H

#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

#include "line.cpp"
#include "shape.cpp"
#include "vector3d.cpp"

class Sphere : public Shape {
 private:
  double radius;  // radius of the sphere
 public:
  // constructor to match the parent class
  Sphere(Vector3D position,
         Color color,
         double ambient_coefficient,
         double diffuse_coefficient,
         double specular_coefficient,
         double reflection_coefficient,
         int specular_exponent,
         double radius)
      : Shape(position,
              color,
              ambient_coefficient,
              diffuse_coefficient,
              specular_coefficient,
              reflection_coefficient,
              specular_exponent),
        radius(radius) {}

  // empty constructor
  Sphere() : Shape(), radius(0) {}

  // getter and setter for the radius
  double getRadius() { return radius; }
  void setRadius(double radius) { this->radius = radius; }

  /**
   * @overridden
   * @brief returns the normal vector of the sphere
   * @param intersection_point the point of intersection
   * @param line the incident line
   */
  virtual Line getNormal(Vector3D& intersection_point, Line line) {
    // the normal vector is the vector from the center of the sphere to the
    // point of intersection
    Vector3D normal = intersection_point - position;
    return Line(intersection_point, normal);
  }

  /**
   * @brief returns the intersection point of the line with the sphere
   * @param line the incident line
   */
  virtual double getT(Line& line, Color& color, int current_level) {
    // first adjust the line so that it is in the coordinate system of the
    // sphere
    Vector3D adjusted_position = line.getStart() - position;
    Line adjusted_line = Line(adjusted_position, line.getDirection());
    double a = 1;
    double b =
        2 * adjusted_line.getDirection().dot_product(adjusted_line.getStart());
    double c = adjusted_line.getStart().dot_product(adjusted_line.getStart()) -
               radius * radius;

    // calculate the discriminant
    double discriminant = b * b - 4 * a * c;
    double t = -1;
    if (discriminant < 0) {
      t = -1;
    } else {
      // check if discriminant is zero
      if (discriminant < 0.00001) {
        t = -b / (2 * a);
      } else {
        // calculate the two roots
        double t1 = (-b + sqrt(discriminant)) / (2 * a);
        double t2 = (-b - sqrt(discriminant)) / (2 * a);
        // take the positive root
        // if both are positive, take the smaller one
        if (t1 > 0 && t2 > 0) {
          t = t1 < t2 ? t1 : t2;
        } else {
          // if one of them is negative, take the positive one
          if (t1 > 0) {
            t = t1;
          } else if (t2 > 0) {
            t = t2;
          } else {
            t = -1;  // if both are negative, there is no intersection
          }
        }
      }
    }

    return t;
  }

  /**
   * @overridden
   * @brief draw the sphere
   */
  virtual void draw() {
    // save the current state of OpenGL
    glPushMatrix();
    // translate to the position of the sphere
    glTranslated(position[0], position[1], position[2]);
    // set the color of the sphere
    glColor3f(1.0 * color[0] / 255, 1.0 * color[1] / 255, 1.0 * color[2] / 255);
    // draw the sphere
    glutSolidSphere(radius, 100, 100);
    // restore the state of OpenGL
    glPopMatrix();
  }

  /**
   * @override
   * @brief returns the color of the sphere at the intersection point
   * @param intersection_point the point of intersection
   */
  virtual Color getColorAt(Vector3D& intersection_point) {
    // the color of the sphere is the color of the sphere
    return color;
  }
};

#endif  // SPHERE_H