#define _USE_MATH_DEFINES
#define SQRT_2 1.41421356237f

#include <GL/glut.h> // GLUT, includes glu.h and gl.h

#include <cmath>
#include <iostream>
#include <vector>

#include "octahedron.h"
using namespace std;

octahedron::octahedron(/* args */) {}

octahedron::~octahedron() {}

void octahedron::draw_partial_sphere(double radius, float color[3]) {
  // set the color
  glColor3fv(color);

  // generate the vertices
  vector<vector<float>> vertices;
  // there are stack_count + 1 vectors each of which contains
  // sector_count + 1 vertices

  // calculate the steps
  double stack_step =
      M_PI / this->stack_count; // phi of the sphere (determines the height)
  double sector_step =
      2 * M_PI / this->sector_count; // theta of the sphere xy plane
  for (int i = 0; i <= stack_count + 1; i++) {
    // we are generating two more points than needed
    // calculate the phi
    double phi = M_PI / 2 - i * stack_step; // phi = 90 - i * phi_step
    double xy = radius * cos(phi);          // r * cos(phi)
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
  for (int i = 0; i <= this->stack_count; i++) {
    // the points are of quads are like this
    // i'th vertex's j'th vertex, (i+1)'th vertex's j'th vertex,
    // (i+1)'th vertex's (j+1)'th vertex, i'th vertex's (j+1)'th vertex
    if (i == 0 || i >= this->stack_count - 1) {
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
      this->draw_triangle(v1, v2, v3, this->sides_color_1);
      this->draw_triangle(v1, v3, v4, this->sides_color_2);
    }
  }
}

void octahedron::draw_partial_cylinder(double radius, double height,
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
    double x = radius * cos(theta);
    double y = radius * sin(theta);
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

void octahedron::draw_triangle(float a[3], float b[3], float c[3],
                               float color[3], bool draw_bottom_cylinder) {
  this->draw_triangle(a, b, c, color);

  // draw the cylinder with the specified radius
  // and height will be the distance between the points a and b
  // and the center of the cylinder will be the center of the line ab
  // find the distance between the points a and b
  float magnitude = (a[0] - b[0]) * (a[0] - b[0]) +
                    (a[1] - b[1]) * (a[1] - b[1]) +
                    (a[2] - b[2]) * (a[2] - b[2]);
  magnitude = sqrt(magnitude);

  // rotate the axis by 45 degrees about the x axis
  glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
  // translate the axis at (0,cylinder distance,0)
  glTranslatef(0.0f, this->cylinder_dist_x, 0.0f);
  this->draw_partial_cylinder(this->radius, magnitude / 2.0f,
                              this->cylinder_color, -1);
  // revert the translation
  glTranslatef(0.0f, -this->cylinder_dist_x, 0.0f);
  // revert the rotation
  glRotatef(-45.0f, 1.0f, 0.0f, 0.0f);

  // similarly for the other y sides

  if (!draw_bottom_cylinder) {
    return;
  }

  // rotate the axis by 45 degrees about the y axis
  glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
  // translate the axis at (cylinder diatance,0,0)
  glTranslatef(this->cylinder_dist_x, 0.0f, 0.0f);
  this->draw_partial_cylinder(this->radius, magnitude / 2.0f,
                              this->cylinder_color, 1);
  // revert the translation
  glTranslatef(-this->cylinder_dist_x, 0.0f, 0.0f);
  // revert the rotation
  glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
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
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_1,
                      true);
  // rotate 90 degrees about the y axis
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_2,
                      true);
  // rotate 90 degrees about the y axis
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_1,
                      true);
  // rotate 90 degrees about the y axis
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_2,
                      true);
  // rotate 90 degrees about the y axis
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

  // now the original state is re stored

  // draw the other half
  // rotate 180 degrees about the x axis
  glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_1,
                      false);
  // rotate 90 degrees about the y axis
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_2,
                      false);
  // rotate 90 degrees about the y axis
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_1,
                      false);
  // rotate 90 degrees about the y axis
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_2,
                      false);
  // rotate 90 degrees about the y axis
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

  // now the original state is re stored
  // rotate -180 degrees about the x axis
  glRotatef(-180.0f, 1.0f, 0.0f, 0.0f);
  // test draw of sphere
  glTranslatef(0.0f, 2.0f, 0.0f);
  this->draw_partial_sphere(this->radius, this->cylinder_color);
  glTranslatef(0.0f, -2.0f, 0.0f);
  // reverse the global rotation
  glRotatef(-this->angleZ, 0.0f, 0.0f, 1.0f);
  glRotatef(-this->angleY, 0.0f, 1.0f, 0.0f);
  glRotatef(-this->angleX, 1.0f, 0.0f, 0.0f);
}

void octahedron::rotateX(float angle) { this->angleX += angle; }

void octahedron::rotateY(float angle) { this->angleY += angle; }

void octahedron::rotateZ(float angle) { this->angleZ += angle; }

void octahedron::transform_to_sphere() {
  // translate the triangles top, left , right to the center of the triangle
  // the center is the average of the three points for all the triangles
  // (1/3, 1/3, 1/3)
  // divide the line segment from the center to the point (top, left, right) by
  // 16 then set the new point to the center + the new line segment equation is
  // point * 15 + center * 1 / 16 for all coordinates
  // top
  //   this->top[0] = (this->top[0] * 15.0f + 1.0 / 3.0f) / 16.0f;
  //   this->top[1] = (this->top[1] * 15.0f + 1.0 / 3.0f) / 16.0f;
  //   this->top[2] = (this->top[2] * 15.0f + 1.0 / 3.0f) / 16.0f;

  // left
  //   this->left[0] = (this->left[0] * 15.0f + 1.0 / 3.0f) / 16.0f;
  //   this->left[1] = (this->left[1] * 15.0f + 1.0 / 3.0f) / 16.0f;
  //   this->left[2] = (this->left[2] * 15.0f + 1.0 / 3.0f) / 16.0f;

  // right
  //   this->right[0] = (this->right[0] * 15.0f + 1.0 / 3.0f) / 16.0f;
  //   this->right[1] = (this->right[1] * 15.0f + 1.0 / 3.0f) / 16.0f;
  //   this->right[2] = (this->right[2] * 15.0f + 1.0 / 3.0f) / 16.0f;

  // save all the three points of any point (top, left, right) in a variable
  // then transform the point to the center
  float temp_top[3] = {this->top[0], this->top[1], this->top[2]};
  // transform the above code to loop
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
  float magnitude = diff_top[0] * diff_top[0] + diff_top[1] * diff_top[1] +
                    diff_top[2] * diff_top[2];

  magnitude = sqrt(magnitude);

  // the slanted distance will be magnitude cos(60)
  float slanted_distance = magnitude * 0.5f;

  // the radius will be slanted distance tan(phi/2)
  float factor = tan(this->phi / 2.0f);

  float radius_increment = slanted_distance * factor;

  // update the radius
  this->radius += radius_increment;

  // update the cylinder dist x

  // slanted distance / cos(phi/2)
  float slanted_dist_x = slanted_distance / cos(this->phi / 2.0f);
  this->cylinder_dist_x -= slanted_dist_x;
}

void octahedron::transform_to_octahedron() {
  // if any of the coordinates is greater than 1 then the transformation is not
  // needed
  // find the magnitude of any of the vectors
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
  this->radius -= radius_increment;

  // update the cylinder dist x
  float slanted_dist_x = slanted_distance / cos(this->phi / 2.0f);
  this->cylinder_dist_x += slanted_dist_x;
}
