#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

// Define a 3D vector structure
struct Vector3D {
  double x, y, z;
};

// Function to rotate a vector using Rodrigues' formula
Vector3D rotateVector(const Vector3D &v, const Vector3D &axis, double angle) {
  double cosAngle = cos(angle);
  double sinAngle = sin(angle);

  Vector3D rotatedVector;

  // Cross product between the vector and the axis of rotation
  Vector3D crossProduct;
  crossProduct.x = v.y * axis.z - v.z * axis.y;
  crossProduct.y = v.z * axis.x - v.x * axis.z;
  crossProduct.z = v.x * axis.y - v.y * axis.x;

  // Applying Rodrigues' formula
  rotatedVector.x =
      v.x * cosAngle + crossProduct.x * sinAngle +
      axis.x * (1 - cosAngle) * (axis.x * v.x + axis.y * v.y + axis.z * v.z);
  rotatedVector.y =
      v.y * cosAngle + crossProduct.y * sinAngle +
      axis.y * (1 - cosAngle) * (axis.x * v.x + axis.y * v.y + axis.z * v.z);
  rotatedVector.z =
      v.z * cosAngle + crossProduct.z * sinAngle +
      axis.z * (1 - cosAngle) * (axis.x * v.x + axis.y * v.y + axis.z * v.z);

  return rotatedVector;
}

int main() {
  Vector3D vectorToRotate = {1.0, 2.0, 3.0};
  Vector3D i_cap = {1.0, 0.0, 0.0};
  Vector3D j_cap = {0.0, 1.0, 0.0};
  Vector3D k_cap = {0.0, 0.0, 1.0};
  Vector3D rotationAxis = {0.0, 0.0, 1.0};
  double rotationAngle = M_PI / 2; // Rotate 90 degrees (in radians)

  Vector3D rotatedVector =
      rotateVector(vectorToRotate, rotationAxis, rotationAngle);

  Vector3D c1 = rotateVector(i_cap, rotationAxis, rotationAngle);
  Vector3D c2 = rotateVector(j_cap, rotationAxis, rotationAngle);
  Vector3D c3 = rotateVector(k_cap, rotationAxis, rotationAngle);

  cout << "Rotated vector: " << rotatedVector.x << " " << rotatedVector.y << " "
       << rotatedVector.z << endl;

  cout << "Rotated i_cap: " << c1.x << " " << c1.y << " " << c1.z << endl;
  cout << "Rotated j_cap: " << c2.x << " " << c2.y << " " << c2.z << endl;
  cout << "Rotated k_cap: " << c3.x << " " << c3.y << " " << c3.z << endl;

  return 0;
}
