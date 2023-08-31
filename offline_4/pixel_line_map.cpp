/**
 * @file pixel_line_map.cpp
 * @brief This file contains the implementation of the pixel_line_map class
 * has 2 int variables: x and y
 * has 1 Line variable: line
 */

#ifndef PIXEL_LINE_MAP_H
#define PIXEL_LINE_MAP_H

#include "line.cpp"
#include "vector3d.cpp"

/**
 * @brief The pixel_line_map class
 */
class PixelLineMap {
 private:
  int x;
  int y;
  Line line;

 public:
  /**
   * @brief PixelLineMap
   */
  PixelLineMap(int x, int y, Line line) : x(x), y(y), line(line) {}

  /**
   * @brief getX
   * @return x
   */
  int getX() { return x; }

  /**
   * @brief getY
   * @return y
   */
  int getY() { return y; }

  /**
   * @brief getLine
   * @return line
   */

  Line getLine() { return line; }
};

#endif  // PIXEL_LINE_MAP_H