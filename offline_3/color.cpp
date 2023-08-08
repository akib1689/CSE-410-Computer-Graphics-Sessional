/**
 * @file color.cpp
 * @brief This file contains the implementation of the color class
 */

#ifndef COLOR_H
#define COLOR_H

#include <vector>

using namespace std;

/**
 * @brief The Color class
 */
class Color {
  vector<double> v;

public:
  /**
   * @brief Color
   */
  Color() : v(3) {}

  /**
   * @brief Color
   * @param r
   * @param g
   * @param b
   */
  Color(double r, double g, double b) : v(3) {
    v[0] = r;
    v[1] = g;
    v[2] = b;
  }

  /**
   * @brief operator []
   * @param index
   * @return
   */
  double &operator[](int index) { return v[index]; }
};

#endif // COLOR_H
