/**
 * @file cube.cpp
 * @brief 3D cube class base class: Shape
 */

#ifndef CUBE_H
#define CUBE_H

#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>

#include "line.cpp"
#include "shape.cpp"
#include "triangle.cpp"
#include "vector3d.cpp"

class Cube : public Shape {
 private:
  double sideLength;  // side length of the cube
  vector<Triangle*> triangles;

 public:
  // Constructor to match the parent class
  Cube(Vector3D position,
       Color color,
       double ambient_coefficient,
       double diffuse_coefficient,
       double specular_coefficient,
       double reflection_coefficient,
       int specular_exponent,
       double sideLength)
      : Shape(position,
              color,
              ambient_coefficient,
              diffuse_coefficient,
              specular_coefficient,
              reflection_coefficient,
              specular_exponent),
        sideLength(sideLength) {
    // calculate the vertices
    Vector3D vertices[8];
    vertices[0] = position;
    vertices[1] = position + Vector3D(0, 0, sideLength);
    vertices[2] = position + Vector3D(sideLength, 0, sideLength);
    vertices[3] = position + Vector3D(sideLength, 0, 0);
    vertices[4] = position + Vector3D(0, sideLength, 0);
    vertices[5] = position + Vector3D(0, sideLength, sideLength);
    vertices[6] = position + Vector3D(sideLength, sideLength, sideLength);
    vertices[7] = position + Vector3D(sideLength, sideLength, 0);

    // create a vector of faces with 2 triangles each
    int faces[12][3] = {{0, 1, 2}, {0, 2, 3}, {0, 4, 5}, {0, 5, 1},
                        {1, 5, 6}, {1, 6, 2}, {2, 6, 7}, {2, 7, 3},
                        {3, 7, 4}, {3, 4, 0}, {4, 7, 6}, {4, 6, 5}};

    // create the triangles
    {
      for (int i = 0; i < 12; i++) {
        Vector3D v1 = vertices[faces[i][0]];
        Vector3D v2 = vertices[faces[i][1]];
        Vector3D v3 = vertices[faces[i][2]];
        // create the triangle
        triangles.push_back(new Triangle(
            v1, v2, v3, color, ambient_coefficient, diffuse_coefficient,
            specular_coefficient, reflection_coefficient, specular_exponent));
      }
    }
  }

  // Empty constructor
  Cube() : Shape(), sideLength(0) {}

  // Getter and setter for the side length
  double getSideLength() { return sideLength; }
  void setSideLength(double sideLength) { this->sideLength = sideLength; }

  // Method to get the normal vector at an intersection point
  Line getNormal(Vector3D& intersection_point, Line line) {
    // Implementation of normal calculation depends on how you define normals
    // for a cube This is just a placeholder
    Vector3D normal =
        Vector3D(0, 0, 0);  // todo Replace with actual normal calculation
    return Line(intersection_point, normal);
  }

  // Method to calculate the intersection point of the line with the cube
  double getIntersection(Line& line) {
    // Implementation of intersection calculation depends on how you define
    // intersections with a cube This is just a placeholder
    return 0.0;  // todo Replace with actual intersection calculation
  }

  // Method to draw the cube
  void draw() {
    // Set the color of the cube
    glColor3f(1.0 * color[0] / 255, 1.0 * color[1] / 255, 1.0 * color[2] / 255);
    // draw the triangles
    for (int i = 0; i < triangles.size(); i++) {
      triangles[i]->draw();
    }
  }

  // Method to get the color at an intersection point
  Color getColorAt(Vector3D& intersection_point) {
    // The color of the cube is the color of the cube
    return color;
  }
};

#endif  // CUBE_H
