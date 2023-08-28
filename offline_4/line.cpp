/**
 * This file contains the implementation of the line class
 */

#ifndef LINE_H
#define LINE_H

#include "vector3d.cpp"

class Line {
 private:
  Vector3D start;
  Vector3D direction;

 public:
  Line(Vector3D start, Vector3D direction) {
    this->start = start;
    direction.normalize();
    this->direction = direction;
  }

  Vector3D getStart() { return start; }
  Vector3D getDirection() { return direction; }

  Vector3D getPoint(double t) { return start + direction * t; }

  double getDistance(Line &another_line) {
    Vector3D v = start - another_line.getStart();
    Vector3D n = direction * another_line.getDirection();
    return v.dot_product(n);
  }

  double getDistance(Vector3D &point) {
    Vector3D v = start - point;
    Vector3D n = direction * point;
    return v.dot_product(n);
  }

  bool isParallel(Line &another_line) {
    Vector3D n = direction * another_line.getDirection();
    return n[0] == 0 && n[1] == 0 && n[2] == 0;
  }

  bool isPerpendicular(Line &another_line) {
    return direction.dot_product(another_line.getDirection()) == 0;
  }

  bool isIntersecting(Line &another_line) {
    return !isParallel(another_line) && !isPerpendicular(another_line);
  }

  Vector3D getIntersection(Line &another_line) {
    Vector3D v = start - another_line.getStart();
    Vector3D n = direction * another_line.getDirection();
    double t = v.dot_product(n) / n.dot_product(n);
    return getPoint(t);
  }
};

#endif  // LINE_H
