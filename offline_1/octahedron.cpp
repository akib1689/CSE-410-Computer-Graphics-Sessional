#define _USE_MATH_DEFINES
#define SQRT_2 1.41421356237f
#define SQRT_3 1.73205080757f

#include <GL/glut.h> // GLUT, includes glu.h and gl.h

#include <cmath>
#include <iostream>
#include <vector>

#include "octahedron.h"
using namespace std;

octahedron::octahedron(/* args */) {}

octahedron::~octahedron() {}

void octahedron::draw_partial_sphere(double radius, float color[3]) {

  // this->draw_axis();
  // set the color
  glColor3fv(color);
  // need to rotate the sphere by 90 degrees about the x axis
  // glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

  // generate the vertices
  vector<vector<float>> vertices;
  // there are stack_count + 1 vectors each of which contains
  // sector_count + 1 vertices

  // calculate the steps
  double stack_step =
      this->sphere_z_limit /
      this->stack_count; // phi of the sphere (determines the height)
  double sector_step =
      2 * M_PI / this->sector_count; // theta of the sphere xy plane

  for (int i = 0; i <= stack_count + 1; i++) {
    // we are generating two more points than needed
    // calculate the phi
    double phi = this->sphere_z_limit + i * stack_step; // phi = i * phi_step
    double xy = radius * cos(phi);                      // r * cos(phi)
    double z = radius * sin(phi);
    // local vector to store the vertices
    vector<float> local_vector; // holds the vertices for a single phi
    for (int j = 0; j <= this->sector_count; j++) {
      // calculate the theta
      double theta = j * sector_step;
      // calculate the x, y, z
      // x = r * cos(phi) * cos(theta)
      // y = r * cos(phi) * sin(theta)
      // z = r * sin(phi)
      double x = xy * cos(theta);
      double y = xy * sin(theta);
      // if the magnitude (x*x) + (y*y) is greater than the clipping distance
      // then keep the smaller value of x and y
      // and the larger value should be clipping distance
      double magnitude = x * x + y * y;
      magnitude = sqrt(magnitude);
      if (magnitude > this->clipping_distance) {
        // find the absolute value of x and y
        double abs_x = abs(x);
        double abs_y = abs(y);
        // find the smaller value
        double smaller = min(abs_x, abs_y);
        if (smaller == abs_x) {
          // set the value of y to clipping distance
          // but keep the sign of y
          y = this->clipping_distance * y / abs_y;
        } else {
          // set the value of x to clipping distance
          // but keep the sign of x
          x = this->clipping_distance * x / abs_x;
        }
      }
      // add the vertex to the vector
      local_vector.push_back(x);
      local_vector.push_back(y);
      local_vector.push_back(z);
    }
    vertices.push_back(local_vector);
  }
  // cout << "length of vertices: " << vertices.size() << endl;
  // now the vertices are generated
  // draw the quads using the vertices
  // glRotatef(-this->sphere_z_limit, 0.0f, 0.0f, 1.0f);
  for (int i = 0; i < this->stack_count; i++) {
    // the points are of quads are like this
    // i'th vertex's j'th vertex, (i+1)'th vertex's j'th vertex,
    // (i+1)'th vertex's (j+1)'th vertex, i'th vertex's (j+1)'th vertex
    if (i >= this->stack_count) {
      continue;
    }
    for (int j = 0; j < this->sector_count; j++) {
      // draw the quad
      // calculate the vertex indices
      // v1 = vertex[i][j], vertex[i][j+1], vertex[i+1][j+2]
      // v2 = vertex[i+1][j], vertex[i+1][j+1], vertex[i+1][j+2]
      // .....
      float v1[3], v2[3], v3[3], v4[3];
      // v1
      v1[0] = vertices[i][j * 3];
      v1[1] = vertices[i][j * 3 + 1];
      v1[2] = vertices[i][j * 3 + 2];

      // v2
      v2[0] = vertices[i + 1][j * 3];
      v2[1] = vertices[i + 1][j * 3 + 1];
      v2[2] = vertices[i + 1][j * 3 + 2];

      // v3
      v3[0] = vertices[i + 1][(j + 1) * 3];
      v3[1] = vertices[i + 1][(j + 1) * 3 + 1];
      v3[2] = vertices[i + 1][(j + 1) * 3 + 2];

      // v4
      v4[0] = vertices[i][(j + 1) * 3];
      v4[1] = vertices[i][(j + 1) * 3 + 1];
      v4[2] = vertices[i][(j + 1) * 3 + 2];

      // draw the quad with two triangles
      this->draw_triangle(v1, v2, v3, color);
      this->draw_triangle(v1, v3, v4, color);
    }
  }
  // glRotatef(this->sphere_z_limit, 0.0f, 0.0f, 1.0f);

  // revert the rotation
  // glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
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
    // revert state
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
  glRotatef(-this->angleZ, 0.0f, 0.0f, 1.0f);
  glRotatef(-this->angleY, 0.0f, 1.0f, 0.0f);
  glRotatef(-this->angleX, 1.0f, 0.0f, 0.0f);
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
    return;
  }

  // the slanted distance will be magnitude cos(60)
  float slanted_distance = magnitude_diff * 0.5f;

  // the radius will be slanted distance tan(phi/2)
  float factor = tan(this->phi / 2.0f);

  float radius_increment = slanted_distance * factor;

  // update the radius
  this->cylinder_radius += radius_increment;

  // update the cylinder dist x

  // slanted distance / cos(phi/2)
  float slanted_dist_x = slanted_distance / cos(this->phi / 2.0f);
  this->cylinder_dist_x -= slanted_dist_x;

  // update radius
  this->update_sphere_param();

  // debug
  this->print_all_parameters();
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

  // update radius
  this->update_sphere_param();

  // debug print
  this->print_all_parameters();
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

  // flip for x coordinate
  float temp_top_x[3] = {-this->top[0], this->top[1], this->top[2]};

  float magnitude_top = this->top[0] * this->top[0] +
                        this->top[1] * this->top[1] +
                        this->top[2] * this->top[2];

  magnitude_top = sqrt(magnitude_top);

  // find the magnitude of the difference
  float diff_top[3] = {this->top[0] - temp_top_x[0],
                       this->top[1] - temp_top_x[1],
                       this->top[2] - temp_top_x[2]};

  float magnitude_diff = diff_top[0] * diff_top[0] + diff_top[1] * diff_top[1] +
                         diff_top[2] * diff_top[2];
  magnitude_diff = sqrt(magnitude_diff);

  // update the radius
  this->sphere_radius = sqrt(3.0f) / 2.0f * magnitude_diff;
  this->sphere_center_x = this->top[2] - magnitude_diff / 2.0f;

  // this->clipping_distance = magnitude_diff / 2.0f;
}

void octahedron::draw_axis() {
  // draw x, y, z axis
  glBegin(GL_LINES);
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(-1.0f, 0.0f, 0.0f);
  glVertex3f(1.0f, 0.0f, 0.0f);
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(0.0f, -1.0f, 0.0f);
  glVertex3f(0.0f, 1.0f, 0.0f);
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(0.0f, 0.0f, -1.0f);
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

  // the clipping distance
  std::cout << "clipping distance: " << this->clipping_distance << std::endl;

  // cylinder parameters
  std::cout << "cylinder center x: " << this->cylinder_dist_x << std::endl;
  std::cout << "cylinder radius: " << this->cylinder_radius << std::endl;

  cout << "----------------------------------------------------------------"
       << endl;
}
