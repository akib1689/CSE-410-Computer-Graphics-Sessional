/**
 * The triangle class
 * - holds three Vector3D objects, which are the three vertices
 */

#ifndef TRIANGLE_H
#define TRIANGLE_H

class Triangle {
  Vector3D vertex[3]; // the three vertices
  Color color;        // the color of the triangle

public:
  Triangle() {}
  /**
   * @brief Construct a new Triangle object
   *        don't assign variables right away
   *        assign after sorting with respect to y values
   */
  Triangle(Vector3D v1, Vector3D v2, Vector3D v3) {
    // sort the vertices with respect to y values
    if (v1[1] > v2[1]) {
      Vector3D temp = v1;
      v1 = v2;
      v2 = temp;
    }
    if (v2[1] > v3[1]) {
      Vector3D temp = v2;
      v2 = v3;
      v3 = temp;
    }
    if (v1[1] > v2[1]) {
      Vector3D temp = v1;
      v1 = v2;
      v2 = temp;
    }

    // assign the vertices
    vertex[0] = v1;
    vertex[1] = v2;
    vertex[2] = v3;
  }

  Vector3D &operator[](int index) { return vertex[index]; }

  /**
   * @brief print
   */
  void print() {
    cout << "Triangle: " << endl;
    cout << vertex[0][0] << " " << vertex[0][1] << " " << vertex[0][2] << endl;
    cout << vertex[1][0] << " " << vertex[1][1] << " " << vertex[1][2] << endl;
    cout << vertex[2][0] << " " << vertex[2][1] << " " << vertex[2][2] << endl;
  }

  /**
   * @brief set_color
   */
  void set_color(Color c) { color = c; }

  /**
   * @brief get_color
   * @return
   */
  Color get_color() { return color; }

  /**
   * @brief get_normal
   * @return
   */
  Vector3D get_normal() {
    Vector3D v1 = vertex[1] - vertex[0];
    Vector3D v2 = vertex[2] - vertex[0];
    Vector3D normal = v1 * v2;
    return normal;
  }
  /**
   * @brief get_z   get the z value of the triangle at (x, y)
   * @param x       x coordinate
   * @param y       y coordinate
   * @return        z value
   */
  double get_z(double x, double y) {
    Vector3D normal = this->get_normal();
    // ax + by + cz + d = 0:    we have a, b, c
    // d = -(ax + by + cz)
    double d = -(normal[0] * vertex[0][0] + normal[1] * vertex[0][1] +
                 normal[2] * vertex[0][2]);
    // now put x, y in the equation
    double z = -(normal[0] * x + normal[1] * y + d) / normal[2];
    return z;
  }
};

#endif // TRIANGLE_H