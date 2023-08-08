/**
 * The triangle class
 * - holds three Vector3D objects, which are the three vertices
 */

#ifndef TRIANGLE_H
#define TRIANGLE_H

class Triangle {
  Vector3D vertex[3]; // the three vertices
  double color[3];    // the color of the triangle (r, g, b) (0-255)

public:
  Triangle() {}
  Triangle(Vector3D v1, Vector3D v2, Vector3D v3) {
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
};

#endif // TRIANGLE_H