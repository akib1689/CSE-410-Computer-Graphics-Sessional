#define _USE_MATH_DEFINES
#define SQRT_2 1.41421356237f
#define SQRT_3 1.73205080757f

#include <GL/glut.h> // GLUT, includes glu.h and gl.h

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;
#include "octahedron.h"

octahedron::octahedron(/* args */) {}

octahedron::~octahedron() {}

// generate vertices for +X face only by intersecting 2 circular planes
// (longitudinal and latitudinal) at the given longitude/latitude angles
vector<vector<float>> octahedron::buildUnitPositiveX(int subdivision) {
  const float DEG2RAD = acos(-1) / 180.0f;

  vector<vector<float>> vertices;
  float n1[3]; // normal of longitudinal plane rotating along Y-axis
  float n2[3]; // normal of latitudinal plane rotating along Z-axis
  float v[3];  // direction vector intersecting 2 planes, n1 x n2
  float a1;    // longitudinal angle along Y-axis
  float a2;    // latitudinal angle along Z-axis

  // compute the number of vertices per row, 2^n + 1
  int pointsPerRow = (int)pow(2, subdivision) + 1;

  // rotate latitudinal plane from 45 to -45 degrees along Z-axis
  // (top-to-bottom)
  for (unsigned int i = 0; i < pointsPerRow; ++i) {
    // normal for latitudinal plane
    // if latitude angle is 0, then normal vector of latitude plane is
    // n2=(0,1,0) therefore, it is rotating (0,1,0) vector by latitude angle a2
    a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
    n2[0] = -sin(a2);
    n2[1] = cos(a2);
    n2[2] = 0;
    vector<float> row;
    // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
    for (unsigned int j = 0; j < pointsPerRow; ++j) {
      // normal for longitudinal plane
      // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
      // therefore, it is rotating (0,0,-1) vector by longitude angle a1
      a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
      n1[0] = -sin(a1);
      n1[1] = 0;
      n1[2] = -cos(a1);

      // find direction vector of intersected line, n1 x n2
      v[0] = n1[1] * n2[2] - n1[2] * n2[1];
      v[1] = n1[2] * n2[0] - n1[0] * n2[2];
      v[2] = n1[0] * n2[1] - n1[1] * n2[0];

      // normalize direction vector
      float scale =
          this->sphere_radius / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
      v[0] *= scale;
      v[1] *= scale;
      v[2] *= scale;

      // add a vertex into array
      row.push_back(v[0]);
      row.push_back(v[1]);
      row.push_back(v[2]);
    }
    vertices.push_back(row);
  }

  return vertices;
}

/**
 * generate vertices for x face only by scaling a square
 */
vector<vector<float>> octahedron::generate_vertecies(int subdivision) {
  vector<vector<float>> vertices;

  // take a square it fron -radius / 2 to +radius / 2
  // divide it into 2^n + 1 points

  // compute the number of vertices per row, 2^n + 1
  int points_per_row = (int)pow(2, subdivision) + 1;
  float step = this->sphere_radius / (points_per_row - 1);

  // min y,z
  float min_y = -this->sphere_radius / 2;
  float min_z = -this->sphere_radius / 2;
  float x = this->sphere_radius / 2;
  for (int i = 0; i < points_per_row; i++) {
    vector<float> row;
    // y
    float y = min_y + i * step;
    for (int j = 0; j < points_per_row; j++) {
      // z
      float z = min_z + j * step;

      // scale the square
      float scale = this->sphere_radius / sqrt(x * x + y * y + z * z);
      float x_scaled = x * scale;
      float y_scaled = y * scale;
      float z_scaled = z * scale;

      // push the vertex
      row.push_back(x_scaled);
      row.push_back(y_scaled);
      row.push_back(z_scaled);
    }
    vertices.push_back(row);
  }

  return vertices;
}
void octahedron::draw_partial_sphere(double radius, float color[3]) {

  // this->draw_axis();
  // set the color
  glColor3fv(color);
  // need to rotate the sphere by 90 degrees about the x axis
  // glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

  // generate the vertices
  int division = 4;

  // compute the number of vertices per row, 2^n + 1
  int pointsPerRow = (int)pow(2, division) + 1;
  vector<vector<float>> vertices = this->generate_vertecies(division);
  // vector<vector<float>> vertices_gen;

  // save state
  glPushMatrix();
  // rorate the sphere by 90 degrees about the y axis
  glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);

  // now the vertices are generated
  // draw the quads using the vertices
  for (int i = 0; i < pointsPerRow - 1; i++) {
    // the points are of quads are like this
    // i'th vertex's j'th vertex, (i+1)'th vertex's j'th vertex,
    // (i+1)'th vertex's (j+1)'th vertex, i'th vertex's (j+1)'th vertex
    for (int j = 0; j < pointsPerRow - 1; j++) {
      // draw the quad
      // calculate the vertex indices
      // v1 = vertex[i][j], vertex[i][j+1], vertex[i+1][j+2]
      // v2 = vertex[i+1][j], vertex[i+1][j+1], vertex[i+1][j+2]
      // .....
      float v1[3], v2[3], v3[3], v4[3];
      // bool visible[4];
      // v1
      v1[0] = vertices[i][j * 3];
      v1[1] = vertices[i][j * 3 + 1];
      v1[2] = vertices[i][j * 3 + 2];
      // visible[0] = vertices[i][j * 4 + 3];

      // v2
      v2[0] = vertices[i + 1][j * 3];
      v2[1] = vertices[i + 1][j * 3 + 1];
      v2[2] = vertices[i + 1][j * 3 + 2];
      // visible[1] = vertices[i + 1][j * 4 + 3];

      // v3
      v3[0] = vertices[i + 1][(j + 1) * 3];
      v3[1] = vertices[i + 1][(j + 1) * 3 + 1];
      v3[2] = vertices[i + 1][(j + 1) * 3 + 2];
      // visible[2] = vertices[i + 1][(j + 1) * 3 + 3];

      // v4
      v4[0] = vertices[i][(j + 1) * 3];
      v4[1] = vertices[i][(j + 1) * 3 + 1];
      v4[2] = vertices[i][(j + 1) * 3 + 2];
      // visible[3] = vertices[i][(j + 1) * 3 + 3];

      // draw the quad
      this->draw_triangle(v1, v2, v3, color);
      this->draw_triangle(v1, v3, v4, color);
    }
  }

  // revert the rotation
  glPopMatrix();
}

void octahedron::draw_partial_cylinder(double cylinder_radius, double height,
                                       float color[3], int multiplier) {

  double step = (M_PI - this->phi) / this->cylinder_slices;
  if (multiplier == -1) {
    // rotate 90 degree about z axis this
    // why this rotation is needed?
    // got through trial and error (by drawing axis of the cylinder)
    // one possible explanation might be:
    // this is needed because the cylinder is drawn in the x-y plane
    // so when the world space is shifted by x axis there is a extra rotation
    // needed.
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  }
  // rotate for model space
  glRotatef((-M_PI + this->phi + step) * 90.0f / M_PI, 0.0f, 0.0f, 1.0f);

  // set the color
  glColor3fv(color);
  // generate the vertices
  vector<double> vertices;
  for (int i = 0; i <= this->cylinder_slices + 1; i++) {
    double theta = i * step;
    double x = cylinder_radius * cos(theta);
    double y = cylinder_radius * sin(theta);
    double z = height;
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
  }

  // now the vertices are generated
  // draw the quads
  // at first rotate the model space by step degree about z axis
  glRotatef(-step * 90.0f / M_PI, 0.0f, 0.0f, 1.0f);
  for (int i = 0; i <= this->cylinder_slices; i++) {
    // the points are of quads are like this
    // i'th vertex, i+1'th vertex, i+1'th vertex with -z, i'th vertex with -z
    glBegin(GL_QUADS);
    // first vertex
    glVertex3f(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
    // second vertex
    glVertex3f(vertices[(i + 1) * 3], vertices[(i + 1) * 3 + 1],
               vertices[(i + 1) * 3 + 2]);
    // third vertex
    glVertex3f(vertices[(i + 1) * 3], vertices[(i + 1) * 3 + 1],
               -vertices[(i + 1) * 3 + 2]);
    // fourth vertex
    glVertex3f(vertices[i * 3], vertices[i * 3 + 1], -vertices[i * 3 + 2]);
    glEnd();
  }

  // revert the rotation of model space
  glRotatef(step * 90.0f / M_PI, 0.0f, 0.0f, 1.0f);

  // reverse the rotation of model space
  glRotatef((M_PI - this->phi - step) * 90.0f / M_PI, 0.0f, 0.0f, 1.0f);

  // revert the rotation of model space for multipliers
  if (multiplier == -1) {
    // revert the extra rotation of model space
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
  }
}

void octahedron::draw_face(float a[3], float b[3], float c[3], float color[3],
                           bool draw_bottom_cylinder) {
  if (this->triangle_visibility) {
    this->draw_triangle(a, b, c, color);
  }

  // draw the cylinder with the specified radius
  // and height will be the distance between the points a and b
  // and the center of the cylinder will be the center of the line ab
  // find the distance between the points a and b
  float magnitude = (a[0] - b[0]) * (a[0] - b[0]) +
                    (a[1] - b[1]) * (a[1] - b[1]) +
                    (a[2] - b[2]) * (a[2] - b[2]);
  magnitude = sqrt(magnitude);

  if (this->cylinder_visibility) {
    // rotate the axis by 45 degrees about the x axis
    glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
    // translate the axis at (0,cylinder distance,0)
    glTranslatef(0.0f, this->cylinder_dist_x, 0.0f);
    this->draw_partial_cylinder(this->cylinder_radius, magnitude / 2.0f,
                                this->cylinder_color, -1);
    // revert the translation
    glTranslatef(0.0f, -this->cylinder_dist_x, 0.0f);
    // revert the rotation
    glRotatef(-45.0f, 1.0f, 0.0f, 0.0f);
  }

  // similarly for the other y sides

  if (!draw_bottom_cylinder) {
    return;
  }

  if (this->cylinder_visibility) {
    // rotate the axis by 45 degrees about the y axis
    glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
    // translate the axis at (cylinder diatance,0,0)
    glTranslatef(this->cylinder_dist_x, 0.0f, 0.0f);
    this->draw_partial_cylinder(this->cylinder_radius, magnitude / 2.0f,
                                this->cylinder_color, 1);
    // revert the translation
    glTranslatef(-this->cylinder_dist_x, 0.0f, 0.0f);
    // revert the rotation
    glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
  }
}

void octahedron::draw_triangle(float a[3], float b[3], float c[3],
                               float color[3]) {
  glBegin(GL_TRIANGLES);
  glColor3fv(color);
  glVertex3fv(a);
  glVertex3fv(b);
  glVertex3fv(c);
  glEnd();
}

void octahedron::draw_octahedron() {

  // save state
  glPushMatrix();
  // apply the transformation
  glRotatef(this->angleX, 1.0f, 0.0f, 0.0f);
  glRotatef(this->angleY, 0.0f, 1.0f, 0.0f);
  glRotatef(this->angleZ, 0.0f, 0.0f, 1.0f);

  // draw the first half
  for (int i = 0; i < 4; i++) {
    if (i % 2 == 0) {
      this->draw_face(this->top, this->left, this->right, this->sides_color_1,
                      true);
    } else {
      this->draw_face(this->top, this->left, this->right, this->sides_color_2,
                      true);
    }
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  }

  // draw the other half
  // rotate 180 degrees about the x axis
  glRotatef(180.0f, 1.0f, 0.0f, 0.0f);

  for (int i = 0; i < 4; i++) {
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    if (i % 2 == 0) {
      this->draw_face(this->top, this->left, this->right, this->sides_color_2,
                      false);
    } else {
      this->draw_face(this->top, this->left, this->right, this->sides_color_1,
                      false);
    }
  }
  // now the original state is re stored
  // rotate -180 degrees about the x axis
  glRotatef(-180.0f, 1.0f, 0.0f, 0.0f);
  if (this->sphere_visibility) {

    // save state
    glPushMatrix();
    glTranslatef(0.0f, this->sphere_center_x, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    this->draw_partial_sphere(this->sphere_radius, this->vertex_color_1);
    // revert state
    glPopMatrix();
    // save state
    glPushMatrix();
    glTranslatef(0.0f, -this->sphere_center_x, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    this->draw_partial_sphere(this->sphere_radius, this->vertex_color_1);
    // revert state
    glPopMatrix();
    // save state
    glPushMatrix();
    glTranslatef(this->sphere_center_x, 0.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    this->draw_partial_sphere(this->sphere_radius, this->vertex_color_2);
    // revert state
    glPopMatrix();
    // save state
    glPushMatrix();
    glTranslatef(-this->sphere_center_x, 0.0f, 0.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    this->draw_partial_sphere(this->sphere_radius, this->vertex_color_2);
    // revert statead
    glPopMatrix();
    // save state
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, this->sphere_center_x);
    this->draw_partial_sphere(this->sphere_radius, this->vertex_color_3);
    // revert state
    glPopMatrix();
    // save state
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -this->sphere_center_x);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    this->draw_partial_sphere(this->sphere_radius, this->vertex_color_3);
    // revert state
    glPopMatrix();
  }
  // reverse the global rotation
  glPopMatrix();
}

void octahedron::rotateX(float angle) { this->angleX += angle; }

void octahedron::rotateY(float angle) { this->angleY += angle; }

void octahedron::rotateZ(float angle) { this->angleZ += angle; }

void octahedron::toggle_triangle_visibility() {
  this->triangle_visibility = !this->triangle_visibility;
}
void octahedron::toggle_cylinder_visibility() {
  this->cylinder_visibility = !this->cylinder_visibility;
}

void octahedron::transform_to_sphere() {
  // translate the triangles top, left , right to the center of the triangle
  // the center is the average of the three points for all the triangles
  // (1/3, 1/3, 1/3)
  // divide the line segment from the center to the point (top, left, right) by
  // 16 then set the new point to the center + the new line segment equation is
  // point * 15 + center * 1 / 16 for all coordinates

  if (is_sphere) {
    return;
  }

  // save all the three points of any point (top, left, right) in a variable
  // then transform the point to the center
  float temp_top[3] = {this->top[0], this->top[1], this->top[2]};

  for (int i = 0; i < 3; i++) {
    this->top[i] = (this->top[i] * 15.0f + 1.0 / 3.0f) / 16.0f;
    this->left[i] = (this->left[i] * 15.0f + 1.0 / 3.0f) / 16.0f;
    this->right[i] = (this->right[i] * 15.0f + 1.0 / 3.0f) / 16.0f;
  }

  // find the radius of the cylinder
  // find the diff of prev and new points
  float diff_top[3] = {this->top[0] - temp_top[0], this->top[1] - temp_top[1],
                       this->top[2] - temp_top[2]};

  // find the magnitude of the diff
  float magnitude_diff = diff_top[0] * diff_top[0] + diff_top[1] * diff_top[1] +
                         diff_top[2] * diff_top[2];

  magnitude_diff = sqrt(magnitude_diff);

  if (magnitude_diff < 0.001f) {
    this->make_sphere();
    this->update_sphere_param();
    this->increase_cylinder_param(magnitude_diff);
    return;
  }

  this->increase_cylinder_param(magnitude_diff);

  // update radius
  this->update_sphere_param();

  // debug
  // this->print_all_parameters();
}

void octahedron::transform_to_octahedron() {
  // if any of the coordinates is greater than 1 then the transformation is not
  // needed
  // find the magnitude of any of the vectors
  if (is_sphere) {
    this->make_octahedron();
    this->update_sphere_param();
    return;
  }

  float magnitude = this->top[0] * this->top[0] + this->top[1] * this->top[1] +
                    this->top[2] * this->top[2];

  if (magnitude >= 0.99f) { // vudu magic constant why 1 don't work I don't know
    return;
  }
  float temp_top[3] = {this->top[0], this->top[1], this->top[2]};
  // reverse the above transformation
  for (int i = 0; i < 3; i++) {
    this->top[i] = (this->top[i] * 16.0f - 1.0 / 3.0f) / 15.0f;
    this->left[i] = (this->left[i] * 16.0f - 1.0 / 3.0f) / 15.0f;
    this->right[i] = (this->right[i] * 16.0f - 1.0 / 3.0f) / 15.0f;
  }

  // radius update
  float diff_top[3] = {this->top[0] - temp_top[0], this->top[1] - temp_top[1],
                       this->top[2] - temp_top[2]};

  // find the magnitude of the diff
  magnitude = diff_top[0] * diff_top[0] + diff_top[1] * diff_top[1] +
              diff_top[2] * diff_top[2];

  magnitude = sqrt(magnitude);

  this->decrease_cylinder_param(magnitude);
  // update radius
  this->update_sphere_param();

  // debug print
  // this->print_all_parameters();
}
/**
 * we take the top point
 * then we find the 2 points
 * 1 on same coordinates as top
 * 1 with the flipped sign of the top (only for x axis)
 * then we find the magnitude of the difference of the 2 points
 *
 * sphere center x will be top's z value - the magnitude of the difference / 2
 * radius will be sqrt(3) / 2 * magnitude of the difference
 * z limit will be acos((top's z value - sphere center x) / radius)
 */
void octahedron::update_sphere_param() {

  float magnitude_diff = 2 * abs(this->top[0]);

  // update the radius
  this->sphere_radius = sqrt(3.0f) / 2.0f * magnitude_diff;
  this->sphere_center_x = this->top[2] - magnitude_diff / 2.0f;

  // this->clipping_distance = magnitude_diff / 2.0f;
}

void octahedron::increase_cylinder_param(float magnitude) {
  // the slanted distance will be magnitude cos(60)
  float slanted_distance = magnitude * 0.5f;

  // the radius will be slanted distance tan(phi/2)
  float factor = tan(this->phi / 2.0f);

  float radius_increment = slanted_distance * factor;

  // update the radius
  this->cylinder_radius += radius_increment;

  // update the cylinder dist x

  // slanted distance / cos(phi/2)
  float slanted_dist_x = slanted_distance / cos(this->phi / 2.0f);
  this->cylinder_dist_x -= slanted_dist_x;
}

void octahedron::decrease_cylinder_param(float magnitude) {
  // the slanted distance will be magnitude cos(60)
  float slanted_distance = magnitude * 0.5f;

  // the radius will be slanted distance tan(phi/2)
  float factor = tan(this->phi / 2.0f);

  float radius_increment = slanted_distance * factor;

  // update the radius
  this->cylinder_radius -= radius_increment;

  // update the cylinder dist x
  float slanted_dist_x = slanted_distance / cos(this->phi / 2.0f);
  this->cylinder_dist_x += slanted_dist_x;
}
void octahedron::draw_axis() {
  // draw x, y, z axis
  glBegin(GL_LINES);
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(1.0f, 0.0f, 0.0f);
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 1.0f, 0.0f);
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 1.0f);
  glEnd();
}

void octahedron::draw_quads(float a[3], float b[3], float c[3], float d[3]) {
  // for debug purpose
  //  take 4 combination of 1,-1 for x, z
  int x_sign[] = {1, -1};
  int z_sign[] = {1, -1};
  // produce 4 points from the right
  float points[4][3];
  for (int i = 0; i < 4; i++) {
    points[i][0] = this->right[0] * x_sign[i / 2];
    points[i][1] = this->right[1];
    if (i < 2) {
      points[i][2] = this->right[2] * z_sign[i % 2];
    } else {
      points[i][2] = this->right[2] * z_sign[1 - i % 2];
    }
  }
  // find the distance
  // draw a quad using the above points
  glColor3f(0.95f, 0.5f, 0.0f);
  glBegin(GL_QUADS);
  for (int i = 0; i < 4; i++) {
    glVertex3f(points[i][0], points[i][1], points[i][2]);
  }
  glEnd();
}

void octahedron::make_sphere() {
  this->save_restoring_values();
  // set the top, left , right at same point (1/3, 1/3, 1/3)
  for (int i = 0; i < 3; i++) {
    this->top[i] = 1.0f / 3.0f;
    this->left[i] = 1.0f / 3.0f;
    this->right[i] = 1.0f / 3.0f;
  }
  this->is_sphere = true;
}

void octahedron::make_octahedron() {
  this->restore_values();
  this->is_sphere = false;
}

void octahedron::save_restoring_values() {
  for (int i = 0; i < 3; i++) {
    this->restoring_top[i] = this->top[i];
    this->restoring_left[i] = this->left[i];
    this->restoring_right[i] = this->right[i];
  }
}

void octahedron::restore_values() {
  for (int i = 0; i < 3; i++) {
    this->top[i] = this->restoring_top[i];
    this->left[i] = this->restoring_left[i];
    this->right[i] = this->restoring_right[i];
  }
}

void octahedron::toggle_sphere_visibility() {
  this->sphere_visibility = !this->sphere_visibility;
}

void octahedron::print_all_parameters() {
  // the top, left, right
  std::cout << "top: ";
  for (int i = 0; i < 3; i++) {
    std::cout << this->top[i] << " ";
  }
  std::cout << std::endl;
  std::cout << "left: ";
  for (int i = 0; i < 3; i++) {
    std::cout << this->left[i] << " ";
  }
  std::cout << std::endl;
  std::cout << "right: ";
  for (int i = 0; i < 3; i++) {
    std::cout << this->right[i] << " ";
  }
  std::cout << std::endl;

  // the sphere parameters
  std::cout << "sphere center x: " << this->sphere_center_x << std::endl;
  std::cout << "sphere radius: " << this->sphere_radius << std::endl;

  // cylinder parameters
  std::cout << "cylinder center x: " << this->cylinder_dist_x << std::endl;
  std::cout << "cylinder radius: " << this->cylinder_radius << std::endl;

  cout << "----------------------------------------------------------------"
       << endl;
}
