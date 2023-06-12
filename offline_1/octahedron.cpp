#include <GL/glut.h> // GLUT, includes glu.h and gl.h

#include <cmath>
#include <iostream>
#include <vector>

#include "octahedron.h"
using namespace std;

octahedron::octahedron(/* args */) {}

octahedron::~octahedron() {}

void octahedron::draw_triangle(float a[3], float b[3], float c[3],
                               float color[3]) {
  glBegin(GL_TRIANGLES);
  glColor3fv(color);
  glVertex3fv(a);
  glVertex3fv(b);
  glVertex3fv(c);
  glEnd();

  // set the clor back to white
  glColor3f(1.0f, 1.0f, 1.0f);
  // draw a line joining the center of the triangle
  // and center of the base (b, c)
  // draw a point at the origin
  glLineWidth(4.0f);
  glBegin(GL_LINES);
  glVertex3f((a[0] + b[0] + c[0]) / 3.0f, (a[1] + b[1] + c[1]) / 3.0f,
             (a[2] + b[2] + c[2]) / 3.0f);
  glVertex3f((a[0] + b[0]) / 2.0f, (a[1] + b[1]) / 2.0f, (a[2] + b[2]) / 2.0f);
  glEnd();

  // draw the cylinder with the specified radius
  // and height will be the distance between the points a and b
  // and the center of the cylinder will be the center of the line ab
  // find the distance between the points a and b
  float magnitude = (a[0] - b[0]) * (a[0] - b[0]) +
                    (a[1] - b[1]) * (a[1] - b[1]) +
                    (a[2] - b[2]) * (a[2] - b[2]);
  magnitude = sqrt(magnitude);
  // rotate the axis by 45 degrees about the y axis
  glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
  // translate the axis at (1,0,0)
  glTranslatef(this->cylinder_dist_x, 0.0f, 0.0f);
  this->draw_quarter_cylinder(this->radius, magnitude / 2.0f);
  // revert the translation
  glTranslatef(-this->cylinder_dist_x, 0.0f, 0.0f);
  // revert the rotation
  glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
}

void octahedron::draw_octahedron() {

  // apply the transformation
  glRotatef(this->angleX, 1.0f, 0.0f, 0.0f);
  glRotatef(this->angleY, 0.0f, 1.0f, 0.0f);
  glRotatef(this->angleZ, 0.0f, 0.0f, 1.0f);

  // draw the first half
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_1);
  // rotate 90 degrees about the y axis
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_2);
  // rotate 90 degrees about the y axis
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_1);
  // rotate 90 degrees about the y axis
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_2);
  // rotate 90 degrees about the y axis
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

  // now the original state is re stored

  // draw the other half
  // rotate 180 degrees about the x axis
  glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_1);
  // rotate 90 degrees about the y axis
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_2);
  // rotate 90 degrees about the y axis
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_1);
  // rotate 90 degrees about the y axis
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_2);
  // rotate 90 degrees about the y axis
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

  // now the original state is re stored
  // rotate -180 degrees about the x axis
  glRotatef(-180.0f, 1.0f, 0.0f, 0.0f);

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
  if (factor < 0) {
    factor = -factor;
  }

  float radius_increment = slanted_distance * factor;

  // update the radius
  this->radius += radius_increment;

  // update the cylinder dist x

  // the distance will be sqrt(slanted distance * slanted distance + radius *
  // radius)

  // update the cylinder dist x
  this->cylinder_dist_x -=
      sqrt(slanted_distance * slanted_distance + this->radius * this->radius);
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
  if (factor < 0) {
    factor = -factor;
  }

  float radius_increment = slanted_distance * factor;

  // update the radius
  this->radius -= radius_increment;
}

void octahedron::draw_quarter_cylinder(double radius, double height) {
  // generate the vertices
  vector<double> vertices;
  for (int i = 0; i <= this->slices; i++) {
    double theta = i * 2.0 * M_PI / this->slices;
    double x = radius * cos(theta);
    double y = radius * sin(theta);
    double z = height;
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
  }

  // now the vertices are generated
  // draw the quads
  for (int i = 0; i < this->slices; i++) {
    // the points are of quads are like this
    // i'th vertex, i+1'th vertex, i+1'th vertex with -z, i'th vertex with -z
    // there will not be any problem with the last vertex because the
    // above loop generates the last vertex on top of the first vertex
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
}