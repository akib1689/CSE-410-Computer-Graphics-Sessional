/**
 * Light class implementation with declaretion.
 */

#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

#include "color.cpp"
#include "vector3d.cpp"

class Light {
 private:
  Vector3D position;
  Color color;
  double falloff;

 public:
  /**
   * @brief Construct a new Light object
   */
  Light(Vector3D position, Color color, double falloff)
      : position(position), color(color), falloff(falloff) {}

  /**
   * @brief Construct a new Light object
   */
  Light() : position(Vector3D()), color(Color()) {}

  /**
   * @brief Get the Position object
   *
   * @return Vector3D
   */
  Vector3D getPosition() { return position; }

  /**
   * @brief Get the Color object
   *
   * @return Color
   */
  Color getColor() { return color; }

  /**
   * @brief Set the Position object
   *
   * @param position
   */
  void setPosition(Vector3D position) { this->position = position; }

  /**
   * @brief Set the Color object
   *
   * @param color
   */
  void setColor(Color color) { this->color = color; }

  /**
   * @brief draw the light
   */
  void draw() {
    glPushMatrix();
    glTranslatef(position[0], position[1], position[2]);
    glColor3f(1.0 * color[0] / 255, 1.0 * color[1] / 255, 1.0 * color[2] / 255);
    glutSolidSphere(2, 20, 20);
    glPopMatrix();
  }
};

#endif  // !LIGHT_H
