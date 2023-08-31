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
  double& operator[](int index) {
    if (index < 0 || index > 2) {
      cout << "Color: index out of bounds" << endl;
      exit(1);
    }
    return v[index];
  }

  /**
   * @brief operator *
   * @param c
   * @return upodated color
   */
  Color operator*(double c) {
    Color color;
    for (int i = 0; i < 3; i++) {
      color[i] = v[i] * c;
    }
    return color;
  }

  /**
   * @brief operator *
   * @param another_color
   * @return updated color
   */
  Color operator*(Color another_color) {
    Color color;
    for (int i = 0; i < 3; i++) {
      color[i] = v[i] * another_color[i];
    }
    return color;
  }

  /**
   * @brief operator +
   * @param another_color
   * @return updated color
   */
  Color operator+(Color another_color) {
    Color color;
    for (int i = 0; i < 3; i++) {
      color[i] = v[i] + another_color[i];
    }
    return color;
  }
};

#endif  // COLOR_H
