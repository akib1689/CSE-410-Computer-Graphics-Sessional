/**
 * This file contains the basic definition and implementation of the shape class
 */

#ifndef SHAPE_H
#define SHAPE_H

#include "color.cpp"
#include "line.cpp"
#include "vector3d.cpp"

/**
 * @brief The Shape class
 */

class Shape {
 protected:
  Vecton3D position;                   // center of the shape
  Color color;                         // color of the shape
  vector<double> ambient_coefficient;  // ka(for red, green, blue)
  vector<double> diffuse_coefficient;  // kd(for red, green, blue)
  double specular_coefficient;         // ks
  double reflection_coefficient;       // for metallic reflection

 public:
  Shape() : ambient_coefficient(3), diffuse_coefficient(3) {}
  Shape(Vector3D position, Color color, vector<double> ambient_coefficient,
        vector<double> diffuse_coefficient, double specular_coefficient,
        double reflection_coefficient) {
    this->position = position;
    this->color = color;
    this->ambient_coefficient = ambient_coefficient;
    this->diffuse_coefficient = diffuse_coefficient;
    this->specular_coefficient = specular_coefficient;
    this->reflection_coefficient = reflection_coefficient;
  }

  Vector3D getPosition() { return position; }
  Color getColor() { return color; }
  vector<double> getAmbientCoefficient() { return ambient_coefficient; }
  vector<double> getDiffuseCoefficient() { return diffuse_coefficient; }
  double getSpecularCoefficient() { return specular_coefficient; }
  double getReflectionCoefficient() { return reflection_coefficient; }

  void setPosition(Vector3D position) { this->position = position; }
  void setColor(Color color) { this->color = color; }
  void setAmbientCoefficient(vector<double> ambient_coefficient) {
    this->ambient_coefficient = ambient_coefficient;
  }
  void setDiffuseCoefficient(vector<double> diffuse_coefficient) {
    this->diffuse_coefficient = diffuse_coefficient;
  }
  void setSpecularCoefficient(double specular_coefficient) {
    this->specular_coefficient = specular_coefficient;
  }
  void setReflectionCoefficient(double reflection_coefficient) {
    this->reflection_coefficient = reflection_coefficient;
  }

  virtual Vector3D getNormal(Vector3D &intersection_point) = 0;
  virtual double getIntersection(line &ray) = 0;
  virtual Color getColorAt(Vector3D &intersection_point) = 0;
  virtual draw() = 0;
};

#endif  // SHAPE_H