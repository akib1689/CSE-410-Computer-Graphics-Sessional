#include "octahedron.h"
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

#include <iostream>

using namespace std;

octahedron::octahedron(/* args */) {

}

octahedron::~octahedron() {

}

void octahedron::draw_triangle(float a[3], float b[3], float c[3], float color[3]) {
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


void octahedron::rotateX(float angle) {
    this->angleX += angle;
}

void octahedron::rotateY(float angle) {
    this->angleY += angle;
}

void octahedron::rotateZ(float angle) {
    this->angleZ += angle;
}
