/**
 * This file contains the basic definition and implementation of the shape class
 */

#ifndef SHAPE_H
#define SHAPE_H

#include "color.cpp"
#include "line.cpp"
#include "vector3d.cpp"

class Shape {
 protected:
  Vector3D position;              // center of the shape
  Color color;                    // color of the shape
  double ambient_coefficient;     // ka
  double diffuse_coefficient;     // kd
  double specular_coefficient;    // ks
  double reflection_coefficient;  // for metallic reflection
  int specular_exponent;          // for specular reflection(exponent)

 public:
  Shape()
      : ambient_coefficient(0.0),
        diffuse_coefficient(0.0),
        specular_coefficient(0.0),
        reflection_coefficient(0.0),
        specular_exponent(1) {}

  Shape(Vector3D position,
        Color color,
        double ambient_coefficient,
        double diffuse_coefficient,
        double specular_coefficient,
        double reflection_coefficient,
        int specular_exponent)
      : position(position),
        color(color),
        ambient_coefficient(ambient_coefficient),
        diffuse_coefficient(diffuse_coefficient),
        specular_coefficient(specular_coefficient),
        reflection_coefficient(reflection_coefficient),
        specular_exponent(specular_exponent) {}

  Vector3D getPosition() { return position; }
  Color getColor() { return color; }
  double getAmbientCoefficient() { return ambient_coefficient; }
  double getDiffuseCoefficient() { return diffuse_coefficient; }
  double getSpecularCoefficient() { return specular_coefficient; }
  double getReflectionCoefficient() { return reflection_coefficient; }

  void setPosition(Vector3D position) { this->position = position; }
  void setColor(Color color) { this->color = color; }
  void setAmbientCoefficient(double ambient_coefficient) {
    this->ambient_coefficient = ambient_coefficient;
  }
  void setDiffuseCoefficient(double diffuse_coefficient) {
    this->diffuse_coefficient = diffuse_coefficient;
  }
  void setSpecularCoefficient(double specular_coefficient) {
    this->specular_coefficient = specular_coefficient;
  }
  void setReflectionCoefficient(double reflection_coefficient) {
    this->reflection_coefficient = reflection_coefficient;
  }

  virtual Line getNormal(Vector3D& intersection_point, Line line) = 0;
  virtual double getIntersection(Line& line) = 0;
  virtual Color getColorAt(Vector3D& intersection_point) = 0;
  virtual void draw() = 0;
};

#endif  // SHAPE_H
