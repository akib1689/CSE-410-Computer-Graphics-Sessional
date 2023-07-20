// @author: akibur rahman
#include <iostream>

#include <fstream>

#include <cmath>
#include <sstream>
#include <string>
#include <vector>

#include <stack>

#include "matrix.cpp"
#include "vector3d.cpp"

using namespace std;

// enum for the different types of commands
enum Command { TRIANGLE, TRANSLATE, SCALE, ROTATE, PUSH, POP, END, NONE };

/**
 * Rodrigues' rotation formula
 * @param point the point to be rotated
 * @param axis the axis to rotate the point around
 * @param angle the angle to rotate the point by
 */
vector<double> rodriguesRotation(vector<double> &point, vector<double> &axis,
                                 double angle) {

  // convert the angle to radians
  angle = angle * M_PI / 180;
  // cos and sin of the angle
  double cos_angle = cos(angle);
  double sin_angle = sin(angle);

  vector<double> rotated_point(3);

  // cross product of axis and point
  vector<double> cross_product(3);

  cross_product[0] = axis[1] * point[2] - axis[2] * point[1];
  cross_product[1] = axis[2] * point[0] - axis[0] * point[2];
  cross_product[2] = axis[0] * point[1] - axis[1] * point[0];

  // dot product of axis and point
  double dot_product =
      axis[0] * point[0] + axis[1] * point[1] + axis[2] * point[2];

  // calculate the rotated point
  for (int i = 0; i < 3; i++) {
    rotated_point[i] = point[i] * cos_angle + cross_product[i] * sin_angle +
                       axis[i] * dot_product * (1 - cos_angle);
  }

  return rotated_point;
}

/**
 * from a line returns the parameters of that line
 * @param line the line to be parsed
 * @return vector of doubles
 */
vector<double> splitLine(const string &line) {
  vector<double> values;
  istringstream iss(line);
  string token;

  while (iss >> token) {
    double value = stod(token);
    values.push_back(value);
  }

  return values;
}

/**
 * Transform a point by a matrix
 * @param matrix the matrix to transform the point by
 * @param point the point to be transformed
 * @return the transformed point
 */

vector<double> transformPoint(Matrix &matrix, vector<double> &point) {
  // first check if the coordinates are in homogenous form
  // if not then transform it to homogenous form
  vector<double> homogenous_point;
  if (point.size() == 3) {
    homogenous_point.push_back(point[0]);
    homogenous_point.push_back(point[1]);
    homogenous_point.push_back(point[2]);
    homogenous_point.push_back(1);
  } else {
    homogenous_point = point;
  }

  // get the matrix
  vector<vector<double>> matrix_values = matrix.getMatrix();

  // transform the point
  vector<double> transformed_point(4);
  for (int i = 0; i < 4; i++) {
    double value = 0;
    for (int j = 0; j < 4; j++) {
      value += matrix_values[i][j] * homogenous_point[j];
    }
    transformed_point[i] = value;
  }

  // convert the point back to cartesian form
  vector<double> cartesian_point(3);
  for (int i = 0; i < 3; i++) {
    cartesian_point[i] = transformed_point[i] / transformed_point[3];
  }

  return cartesian_point;
}

/**
 * from a string returns the enum of that string
 */
Command getCommand(const string &command) {
  // cout << command << endl;
  if (command.compare("triangle") == 0) {
    return TRIANGLE;
  } else if (command.compare("translate") == 0) {
    return TRANSLATE;
  } else if (command.compare("scale") == 0) {
    return SCALE;
  } else if (command.compare("rotate") == 0) {
    return ROTATE;
  } else if (command.compare("push") == 0) {
    return PUSH;
  } else if (command.compare("pop") == 0) {
    return POP;
  } else if (command.compare("end") == 0) {
    return END;
  } else {
    return NONE;
  }
}

int main() {
  // open the scene.txt file
  ifstream scene_file("scene.txt");

  // read the first line
  string line;
  getline(scene_file, line);

  // parse the line for eye.x, eye.y, eye.z
  vector<double> eye = splitLine(line);

  // read the second line
  getline(scene_file, line);

  // parse the line for look.x, look.y, look.z
  vector<double> look = splitLine(line);

  // read the third line
  getline(scene_file, line);

  // parse the line for up.x, up.y, up.z
  vector<double> up = splitLine(line);

  // read the fourth line
  getline(scene_file, line);

  // parse the line for fovY, aspectRatio, near, far
  vector<double> perspective = splitLine(line);

  // print the values
  cout << "eye: " << eye[0] << " " << eye[1] << " " << eye[2] << endl;
  cout << "look: " << look[0] << " " << look[1] << " " << look[2] << endl;
  cout << "up: " << up[0] << " " << up[1] << " " << up[2] << endl;
  cout << "fovY: " << perspective[0] << endl;
  cout << "aspectRatio: " << perspective[1] << endl;
  cout << "near: " << perspective[2] << endl;
  cout << "far: " << perspective[3] << endl;

  // initialize the stack
  stack<Matrix> matrix_stack;

  // initialize the matrix to be an identity matrix
  Matrix matrix;

  // stage 1 file to write the output to
  ofstream stage1_file("stage1.txt");
  stage1_file.setf(ios::fixed);
  // stage 2 file to write the output to
  ofstream stage2_file("stage2.txt");
  stage2_file.setf(ios::fixed);
  // stage 3 file to write the output to
  ofstream stage3_file("stage3.txt");
  stage3_file.setf(ios::fixed);

  // generate the view transformation

  // determine the mutually perpendicular vectors
  // l = look - eye
  Vector3D l(look[0] - eye[0], look[1] - eye[1], look[2] - eye[2]);
  l.normalize();
  Vector3D up_vector(up[0], up[1], up[2]);
  up_vector.normalize();
  Vector3D r = l * up_vector;
  r.normalize();
  Vector3D u = r * l;

  // transform the view matrix to eye space
  Matrix view_matrix;
  view_matrix.setMatrixValue(0, 0, r[0]);
  view_matrix.setMatrixValue(0, 1, r[1]);
  view_matrix.setMatrixValue(0, 2, r[2]);

  view_matrix.setMatrixValue(1, 0, u[0]);
  view_matrix.setMatrixValue(1, 1, u[1]);
  view_matrix.setMatrixValue(1, 2, u[2]);

  view_matrix.setMatrixValue(2, 0, -l[0]);
  view_matrix.setMatrixValue(2, 1, -l[1]);
  view_matrix.setMatrixValue(2, 2, -l[2]);

  // translate the view matrix
  Matrix translate_matrix;
  translate_matrix.setMatrixValue(0, 3, -eye[0]);
  translate_matrix.setMatrixValue(1, 3, -eye[1]);
  translate_matrix.setMatrixValue(2, 3, -eye[2]);

  // multiply the matrices
  view_matrix = view_matrix.multiply(translate_matrix);

  // prespective projection

  // fovx = fovY * aspectRatio
  double fovY = perspective[0];
  double aspectRatio = perspective[1];
  double fovX = fovY * aspectRatio;
  double near = perspective[2];
  double far = perspective[3];
  // t = near * tan(fovY / 2)
  double t = near * tan(fovY * M_PI / 360);
  // r = near * tan(fovX / 2)
  double r_p = near * tan(fovX * M_PI / 360);

  // initialize the projection matrix
  Matrix projection_matrix;
  projection_matrix.setMatrixValue(0, 0, near / r_p);
  projection_matrix.setMatrixValue(1, 1, near / t);
  projection_matrix.setMatrixValue(2, 2, -(far + near) / (far - near));
  projection_matrix.setMatrixValue(2, 3, -(2 * far * near) / (far - near));
  projection_matrix.setMatrixValue(3, 2, -1);

  // init i_cap, j_cap, k_cap
  vector<double> i_cap(3);
  vector<double> j_cap(3);
  vector<double> k_cap(3);

  i_cap[0] = 1;
  i_cap[1] = 0;
  i_cap[2] = 0;

  j_cap[0] = 0;
  j_cap[1] = 1;
  j_cap[2] = 0;

  k_cap[0] = 0;
  k_cap[1] = 0;
  k_cap[2] = 1;

  // while loop to read the rest of the lines
  while (getline(scene_file, line)) {
    // check the line for the following cases
    // 1. triangle
    // 2. translate
    // 3. scale
    // 4. rotate
    // 5. push
    // 6. pop
    // 7. end
    // cout << line << endl;

    // remove the ending newline character
    line.erase(line.length() - 1);

    vector<vector<double>> vertex(3);
    vector<double> factors(3);
    Matrix factor_matrix;
    vector<double> axis(3);
    vector<vector<double>> rotated_point(3);
    double magnitude;

    switch (getCommand(line)) {
    case TRIANGLE:
      cout << "triangle" << endl;
      // take another line
      getline(scene_file, line);
      // parse the line for the three vertices
      vertex[0] = splitLine(line);
      getline(scene_file, line);
      vertex[1] = splitLine(line);
      getline(scene_file, line);
      vertex[2] = splitLine(line);

      // transform the vertices
      for (int i = 0; i < 3; i++) {
        vertex[i] = transformPoint(matrix, vertex[i]);
      }

      // print the vertices to the stage1.txt file
      for (int i = 0; i < 3; i++) {
        stage1_file << vertex[i][0] << " " << vertex[i][1] << " "
                    << vertex[i][2] << " " << endl;
      }
      stage1_file << endl;

      // transform the vertices using the view matrix
      for (int i = 0; i < 3; i++) {
        vertex[i] = transformPoint(view_matrix, vertex[i]);
      }

      // print the vertices to the stage2.txt file
      for (int i = 0; i < 3; i++) {
        stage2_file << vertex[i][0] << " " << vertex[i][1] << " "
                    << vertex[i][2] << " " << endl;
      }
      stage2_file << endl;

      // transform the vertices using the projection matrix
      for (int i = 0; i < 3; i++) {
        vertex[i] = transformPoint(projection_matrix, vertex[i]);
      }

      // print the vertices to the stage3.txt file
      for (int i = 0; i < 3; i++) {
        stage3_file << vertex[i][0] << " " << vertex[i][1] << " "
                    << vertex[i][2] << " " << endl;
      }
      stage3_file << endl;

      // print current matrix
      matrix.printMatrix();

      break;
    case TRANSLATE:
      cout << "translate" << endl;
      // read the next line
      getline(scene_file, line);
      // parse the line for tx, ty, tz
      factors = splitLine(line);

      // print the matrix before translation
      // matrix.printMatrix();

      // initialize the translate matrix
      factor_matrix.setMatrixValue(0, 3, factors[0]);
      factor_matrix.setMatrixValue(1, 3, factors[1]);
      factor_matrix.setMatrixValue(2, 3, factors[2]);

      // factor_matrix.printMatrix();

      // multiply the matrices
      matrix = matrix.multiply(factor_matrix);

      // print the matrix after translation
      // matrix.printMatrix();

      break;
    case SCALE:
      cout << "scale" << endl;
      // read the next line
      getline(scene_file, line);
      // parse the line for sx, sy, sz
      factors = splitLine(line);

      // initialize the scale matrix
      factor_matrix.setMatrixValue(0, 0, factors[0]);
      factor_matrix.setMatrixValue(1, 1, factors[1]);
      factor_matrix.setMatrixValue(2, 2, factors[2]);

      // multiply the matrices
      matrix = matrix.multiply(factor_matrix);

      break;
    case ROTATE:
      cout << "rotate" << endl;

      // read the next line'
      getline(scene_file, line);
      // parse the line for angle, axis.x, axis.y, axis.z
      factors = splitLine(line);

      // seperate the axis
      for (int i = 0; i < 3; i++) {
        axis[i] = factors[i + 1];
      }

      // normalize the axis
      magnitude =
          sqrt(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]);

      for (int i = 0; i < 3; i++) {
        axis[i] /= magnitude;
      }

      // get the rotated points
      rotated_point[0] = rodriguesRotation(i_cap, axis, factors[0]);
      rotated_point[1] = rodriguesRotation(j_cap, axis, factors[0]);
      rotated_point[2] = rodriguesRotation(k_cap, axis, factors[0]);

      // initialize the rotation matrix
      factor_matrix.setMatrixValue(0, 0, rotated_point[0][0]);
      factor_matrix.setMatrixValue(0, 1, rotated_point[1][0]);
      factor_matrix.setMatrixValue(0, 2, rotated_point[2][0]);

      factor_matrix.setMatrixValue(1, 0, rotated_point[0][1]);
      factor_matrix.setMatrixValue(1, 1, rotated_point[1][1]);
      factor_matrix.setMatrixValue(1, 2, rotated_point[2][1]);

      factor_matrix.setMatrixValue(2, 0, rotated_point[0][2]);
      factor_matrix.setMatrixValue(2, 1, rotated_point[1][2]);
      factor_matrix.setMatrixValue(2, 2, rotated_point[2][2]);

      factor_matrix.printMatrix();

      // multiply the matrices
      matrix = matrix.multiply(factor_matrix);

      break;
    case PUSH:
      cout << "push" << endl;
      // push the matrix to the stack
      matrix_stack.push(matrix);
      break;
    case POP:
      cout << "pop" << endl;
      // pop the matrix from the stack
      matrix = matrix_stack.top();
      matrix_stack.pop();
      break;
    case END:
      cout << "end" << endl;
      break;
    }
  }

  return 0;
}