/**
 * @file pyramid.cpp
 * @brief 3D pyramid class base class: Shape
 *
 */

#ifndef PYRAMID_H
#define PYRAMID_H

#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>

#include "line.cpp"
#include "shape.cpp"
#include "triangle.cpp"
#include "vector3d.cpp"

class Pyramid : public Shape {
 private:
  double baseSideLength;        // side length of the pyramid's base
  double height;                // height of the pyramid
  vector<Triangle*> triangles;  // vector of triangles that make up the pyramid

 public:
  // Constructor to match the parent class
  Pyramid(Vector3D position,
          Color color,
          double ambient_coefficient,
          double diffuse_coefficient,
          double specular_coefficient,
          double reflection_coefficient,
          int specular_exponent,
          double baseSideLength,
          double height)
      : Shape(position,
              color,
              ambient_coefficient,
              diffuse_coefficient,
              specular_coefficient,
              reflection_coefficient,
              specular_exponent),
        baseSideLength(baseSideLength),
        height(height) {
    // calculate the vertices
    Vector3D vertices[5];
    // base vertices are (-width/2 , -width/2, 0) to (width/2, width/2, 0)
    vertices[0] =
        position + Vector3D(baseSideLength / 2, baseSideLength / 2, 0);
    vertices[1] =
        position + Vector3D(-baseSideLength / 2, baseSideLength / 2, 0);
    vertices[2] =
        position + Vector3D(-baseSideLength / 2, -baseSideLength / 2, 0);
    vertices[3] =
        position + Vector3D(baseSideLength / 2, -baseSideLength / 2, 0);
    // apex is at (0, 0, height)
    vertices[4] = position + Vector3D(0, 0, height);

    // create a vector of faces with 2 triangles each
    int faces[6][3] = {{0, 1, 2}, {0, 2, 3}, {0, 1, 4},
                       {1, 2, 4}, {2, 3, 4}, {3, 0, 4}};
    // create the triangles
    {
      for (int i = 0; i < 6; i++) {
        Vector3D v1 = vertices[faces[i][0]];
        Vector3D v2 = vertices[faces[i][1]];
        Vector3D v3 = vertices[faces[i][2]];
        triangles.push_back(new Triangle(
            v1, v2, v3, color, ambient_coefficient, diffuse_coefficient,
            specular_coefficient, reflection_coefficient, specular_exponent));
      }
    }
  }

  // Empty constructor
  Pyramid() : Shape(), baseSideLength(0), height(0) {}

  // Getter and setter for the base side length
  double getBaseSideLength() { return baseSideLength; }
  void setBaseSideLength(double baseSideLength) {
    this->baseSideLength = baseSideLength;
  }

  // Getter and setter for the height
  double getHeight() { return height; }
  void setHeight(double height) { this->height = height; }

  // Method to get the normal vector at an intersection point
  Line getNormal(Vector3D& intersection_point, Line line) {
    // TODO: Implement normal calculation for the pyramid
    Vector3D normal =
        Vector3D(0, 0, 0);  // Replace with actual normal calculation
    return Line(intersection_point, normal);
  }

  // Method to calculate the intersection point of the line with the pyramid
  double getT(Line& line, Color& color, int current_level) {
    // TODO: Implement intersection calculation for the pyramid
    return 0.0;  // Replace with actual intersection calculation
  }

  // Method to draw the pyramid
  void draw() {
    // Set the color of the pyramid
    glColor3f(1.0 * color[0] / 255, 1.0 * color[1] / 255, 1.0 * color[2] / 255);

    // TODO: Draw the pyramid using your chosen drawing method
    for (int i = 0; i < triangles.size(); i++) {
      triangles[i]->draw();
    }
  }

  // Method to get the color at an intersection point
  Color getColorAt(Vector3D& intersection_point) {
    // The color of the pyramid is the color of the pyramid
    return color;
  }
};

#endif  // PYRAMID_H
