/**
 * @file spot_light.cpp
 * @brief 3D spot light class derived from Light
 */

#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

#include "1805086_color.cpp"
#include "1805086_light.cpp"
#include "1805086_line.cpp"
#include "1805086_vector3d.cpp"

class SpotLight : public Light {
 private:
  /* data */
  Vector3D direction;
  double angle;

 public:
  /**
   * @brief Construct a new Spot Light object
   *
   * @param position
   * @param color
   * @param falloff
   * @param direction
   * @param angle
   */
  SpotLight(Vector3D position,
            Color color,
            double falloff,
            Vector3D direction,
            double angle)
      : Light(position, color, falloff), direction(direction), angle(angle) {}

  /**
   * @brief Construct a new Spot Light object
   *
   */
  SpotLight() : Light(), direction(Vector3D()), angle(0) {}

  /**
   * @brief Get the Direction object
   *
   * @return Vector3D
   */
  Vector3D getDirection() { return direction; }

  /**
   * @brief Get the Angle object
   *
   * @return double
   */
  double getAngle() { return angle; }

  /**
   * @brief Set the Direction object
   *
   * @param direction
   */
  void setDirection(Vector3D direction) { this->direction = direction; }

  /**
   * @brief Set the Angle object
   *
   * @param angle
   */
  void setAngle(double angle) { this->angle = angle; }

  ~SpotLight() {}
};

#endif  // SPOT_LIGHT_H