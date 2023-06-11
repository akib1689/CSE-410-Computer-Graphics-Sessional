#include "octahedron.h"
#include <GL/glut.h> // GLUT, includes glu.h and gl.h

#include <iostream>

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
  // draw a point at the origin
  glPointSize(5.0f);
  glBegin(GL_POINTS);
  glVertex3f((a[0] + b[0] + c[0]) / 3.0f, (a[1] + b[1] + c[1]) / 3.0f,
             (a[2] + b[2] + c[2]) / 3.0f);
  glEnd();
}

void octahedron::draw_octahedron() {

  // apply the transformation
  glRotatef(this->angleX, 1.0f, 0.0f, 0.0f);
  glRotatef(this->angleY, 0.0f, 1.0f, 0.0f);
  glRotatef(this->angleZ, 0.0f, 0.0f, 1.0f);

  // draw the first half
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_1);
  // rotate 90 degrees about the z axis
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_2);
  // rotate 90 degrees about the z axis
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_1);
  // rotate 90 degrees about the z axis
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_2);
  // rotate 90 degrees about the z axis
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);

  // now the original state is re stored

  // draw the other half
  // rotate 180 degrees about the x axis
  glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_1);
  // rotate 90 degrees about the z axis
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_2);
  // rotate 90 degrees about the z axis
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_1);
  // rotate 90 degrees about the z axis
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  this->draw_triangle(this->top, this->left, this->right, this->sides_color_2);
  // rotate 90 degrees about the z axis
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);

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

  // transform the above code to loop
  for (int i = 0; i < 3; i++) {
    this->top[i] = (this->top[i] * 15.0f + 1.0 / 3.0f) / 16.0f;
    this->left[i] = (this->left[i] * 15.0f + 1.0 / 3.0f) / 16.0f;
    this->right[i] = (this->right[i] * 15.0f + 1.0 / 3.0f) / 16.0f;
  }
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
  // reverse the above transformation
  for (int i = 0; i < 3; i++) {
    this->top[i] = (this->top[i] * 16.0f - 1.0 / 3.0f) / 15.0f;
    this->left[i] = (this->left[i] * 16.0f - 1.0 / 3.0f) / 15.0f;
    this->right[i] = (this->right[i] * 16.0f - 1.0 / 3.0f) / 15.0f;
  }
}