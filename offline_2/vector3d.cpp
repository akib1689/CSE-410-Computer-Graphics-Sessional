/**
 * @file vector3d.h
 * @brief This file contains the Vector3D class.
 */

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <vector>

using namespace std;

/**
 * @brief The Vector3D class
 */
class Vector3D {
  vector<double> v;

public:
  Vector3D() : v(3) {}
  Vector3D(double x, double y, double z) : v(3) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
  }

  double &operator[](int index) { return v[index]; }

  /**
   * @brief operator *
   * @param another_vector    the vector to be multiplied with (cross product)
   */
  Vector3D operator*(Vector3D &another_vector) {
    Vector3D result;
    result[0] = v[1] * another_vector[2] - v[2] * another_vector[1];
    result[1] = v[2] * another_vector[0] - v[0] * another_vector[2];
    result[2] = v[0] * another_vector[1] - v[1] * another_vector[0];
    return result;
  }

  /**
   * @brief dot_product
   * @param another_vector    the vector to be multiplied with (dot product)
   */
  double dot_product(Vector3D &another_vector) {
    return v[0] * another_vector[0] + v[1] * another_vector[1] +
           v[2] * another_vector[2];
  }

  /**
   * @brief operator +
   * @param another_vector    the vector to be added with
   */

  Vector3D operator+(Vector3D &another_vector) {
    Vector3D result;
    result[0] = v[0] + another_vector[0];
    result[1] = v[1] + another_vector[1];
    result[2] = v[2] + another_vector[2];
    return result;
  }

  /**
   * @brief operator -
   * @param another_vector    the vector to be subtracted with
   */
  Vector3D operator-(Vector3D &another_vector) {
    Vector3D result;
    result[0] = v[0] - another_vector[0];
    result[1] = v[1] - another_vector[1];
    result[2] = v[2] - another_vector[2];
    return result;
  }

  /**
   * @brief operator /
   * @param scalar    the scalar to be divided with
   */
  Vector3D operator/(double scalar) {
    Vector3D result;
    result[0] = v[0] / scalar;
    result[1] = v[1] / scalar;
    result[2] = v[2] / scalar;
    return result;
  }

  /**
   * @brief operator *
   * @param scalar    the scalar to be multiplied with
   */
  Vector3D operator*(double scalar) {
    Vector3D result;
    result[0] = v[0] * scalar;
    result[1] = v[1] * scalar;
    result[2] = v[2] * scalar;
    return result;
  }

  /**
   * @brief operator =
   * @param another_vector    the vector to be assigned
   */
  Vector3D operator=(Vector3D &another_vector) {
    v[0] = another_vector[0];
    v[1] = another_vector[1];
    v[2] = another_vector[2];
    return *this;
  }

  /**
   * @brief operator ==
   * @param another_vector    the vector to be compared with
   */
  bool operator==(Vector3D &another_vector) {
    return v[0] == another_vector[0] && v[1] == another_vector[1] &&
           v[2] == another_vector[2];
  }

  /**
   * @brief normalize
   */
  void normalize() {
    double magnitude = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] /= magnitude;
    v[1] /= magnitude;
    v[2] /= magnitude;
  }
};

#endif // VECTOR3D_H