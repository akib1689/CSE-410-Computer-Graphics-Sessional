#define _USE_MATH_DEFINES
#define PI_DEGREE 180.0

#include <iostream>

#include <GL/glut.h> // GLUT, includes glu.h and gl.h

#include <cmath>

#include "octahedron.h"

using namespace std;

octahedron octahedron;

// Global variables

// function
float Q_rsqrt(float);
float *normalize(float[3]);

// camera position
float *camera = new float[3];
// looking position
float *look = new float[3];
// up vector
float *up = new float[3];

/* Initialize OpenGL Graphics */
void initGL() {
  glClearColor(0.0f, 0.0f, 0.0f,
               1.0f);      // Set background color to black and opaque
  glClearDepth(1.0f);      // Set background depth to farthest
  glEnable(GL_DEPTH_TEST); // Enable depth testing for z-culling
  glDepthFunc(GL_LEQUAL);  // Set the type of depth-test
  glShadeModel(GL_SMOOTH); // Enable smooth shading
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,
         GL_NICEST); // Nice perspective corrections
}

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {

  glClear(GL_COLOR_BUFFER_BIT |
          GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
  glMatrixMode(GL_MODELVIEW);   // To operate on model-view matrix

  glLoadIdentity(); // Reset the model-view matrix

  gluLookAt(camera[0], camera[1], camera[2], look[0], look[1], look[2], up[0],
            up[1], up[2]);

  glTranslatef(0.0f, 0.0f, -7.0f); // Move the origin to back
  octahedron.draw_octahedron();
  glTranslatef(0.0f, 0.0f, 7.0f); // Move the origin back to the center
  glutSwapBuffers(); // Swap the front and back frame buffers (double buffering)
}

/* Handler for window re-size event. Called back when the window first appears
   and whenever the window is re-sized with its new width and height */
void reshape(GLsizei width,
             GLsizei height) { // GLsizei for non-negative integer
  // Compute aspect ratio of the new window
  if (height == 0)
    height = 1; // To prevent divide by 0
  GLfloat aspect = (GLfloat)width / (GLfloat)height;

  // Set the viewport to cover the new window
  glViewport(0, 0, width, height);

  // Set the aspect ratio of the clipping volume to match the viewport
  glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
  glLoadIdentity();            // Reset
  // Enable perspective projection with fovy, aspect, zNear and zFar
  gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/**
 * callback function for key press (normal keys)
 */
void key_poressed(unsigned char key, int x, int y) {
  // calculate the look vector
  float *look_vec = new float[3];
  look_vec[0] = look[0] - camera[0];
  look_vec[1] = look[1] - camera[1];
  look_vec[2] = look[2] - camera[2];

  // normalize the up vector and look vector
  up = normalize(up);
  look_vec = normalize(look_vec);

  // now find the cross product of up and look
  float *cross = new float[3];
  cross[0] = up[1] * look_vec[2] - up[2] * look_vec[1];
  cross[1] = up[2] * look_vec[0] - up[0] * look_vec[2];
  cross[2] = up[0] * look_vec[1] - up[1] * look_vec[0];

  // the vectors up look and cross are perpendicular to each other
  switch (key) {
  case 'a':
    octahedron.rotateY(-PI_DEGREE / 12);
    break;
  case 'd':
    octahedron.rotateY(PI_DEGREE / 12);
    break;
  case 'A':
    octahedron.rotateY(-PI_DEGREE / 12);
    break;
  case 'D':
    octahedron.rotateX(PI_DEGREE / 12);
    break;
  case ',':
    octahedron.transform_to_sphere();
    break;
  case '.':
    octahedron.transform_to_octahedron();
    break;
  case '1':
    // rotate the look vector to cross vector
    look_vec[0] = cross[0] * sin(M_PI / 180) + look_vec[0] * cos(M_PI / 180);
    look_vec[1] = cross[1] * sin(M_PI / 180) + look_vec[1] * cos(M_PI / 180);
    look_vec[2] = cross[2] * sin(M_PI / 180) + look_vec[2] * cos(M_PI / 180);
    break;
  case '2':
    // rotate the look vector away from cross vector
    look_vec[0] = cross[0] * sin(-M_PI / 180) + look_vec[0] * cos(-M_PI / 180);
    look_vec[1] = cross[1] * sin(-M_PI / 180) + look_vec[1] * cos(-M_PI / 180);
    look_vec[2] = cross[2] * sin(-M_PI / 180) + look_vec[2] * cos(-M_PI / 180);
    break;
  case '3':
    // rotate the look vector to up vector
    look_vec[0] = up[0] * sin(M_PI / 180) + look_vec[0] * cos(M_PI / 180);
    look_vec[1] = up[1] * sin(M_PI / 180) + look_vec[1] * cos(M_PI / 180);
    look_vec[2] = up[2] * sin(M_PI / 180) + look_vec[2] * cos(M_PI / 180);
    // change the up vector to perpendicular to look vector and cross vector
    up[0] = look_vec[1] * cross[2] - look_vec[2] * cross[1];
    up[1] = look_vec[2] * cross[0] - look_vec[0] * cross[2];
    up[2] = look_vec[0] * cross[1] - look_vec[1] * cross[0];
    break;
  case '4':
    // rotate the look vector away from up vector
    look_vec[0] = up[0] * sin(-M_PI / 180) + look_vec[0] * cos(-M_PI / 180);
    look_vec[1] = up[1] * sin(-M_PI / 180) + look_vec[1] * cos(-M_PI / 180);
    look_vec[2] = up[2] * sin(-M_PI / 180) + look_vec[2] * cos(-M_PI / 180);
    // change the up vector to perpendicular to look vector and cross vector
    up[0] = look_vec[1] * cross[2] - look_vec[2] * cross[1];
    up[1] = look_vec[2] * cross[0] - look_vec[0] * cross[2];
    up[2] = look_vec[0] * cross[1] - look_vec[1] * cross[0];
    break;
  case '5':
    // rotate the up vector to the cross vector
    up[0] = cross[0] * sin(M_PI / 180) + up[0] * cos(M_PI / 180);
    up[1] = cross[1] * sin(M_PI / 180) + up[1] * cos(M_PI / 180);
    up[2] = cross[2] * sin(M_PI / 180) + up[2] * cos(M_PI / 180);
    break;
  case '6':
    // rotate the up vector away from the cross vector
    up[0] = cross[0] * sin(-M_PI / 180) + up[0] * cos(-M_PI / 180);
    up[1] = cross[1] * sin(-M_PI / 180) + up[1] * cos(-M_PI / 180);
    up[2] = cross[2] * sin(-M_PI / 180) + up[2] * cos(-M_PI / 180);
    break;
  case 'v':
    // change the triangle visibility
    octahedron.toggle_triangle_visibility();
    break;
  case 'V':
    // change the vertex visibility
    octahedron.toggle_triangle_visibility();
    break;
  case 'c':
    // change the cylinder visibility
    octahedron.toggle_cylinder_visibility();
    break;
  case 'C':
    // change the cylinder visibility
    octahedron.toggle_cylinder_visibility();
    break;
  case 'b':
    // change the sphere visibility
    octahedron.toggle_sphere_visibility();
    break;
  case 'B':
    // change the sphere visibility
    octahedron.toggle_sphere_visibility();
    break;
  default:
    break;
  }
  look[0] = camera[0] + look_vec[0];
  look[1] = camera[1] + look_vec[1];
  look[2] = camera[2] + look_vec[2];
  // look = normalize(look);
  glutPostRedisplay();
}

/**
 * callback function for key press (special keys)
 */
void special_key_pressed(int key, int x, int y) {
  // find the up vector's perpendicular vector
  // we can find the perpendicular vector by cross product of up vector and look
  // vector
  double up_perp[3], look_vec[3];
  look_vec[0] = look[0] - camera[0];
  look_vec[1] = look[1] - camera[1];
  look_vec[2] = look[2] - camera[2];
  up_perp[0] = up[1] * look_vec[2] - up[2] * look_vec[1];
  up_perp[1] = up[2] * look_vec[0] - up[0] * look_vec[2];
  up_perp[2] = up[0] * look_vec[1] - up[1] * look_vec[0];
  switch (key) {
  case GLUT_KEY_LEFT:
    // move the camera in the direction of up_perp vector
    camera[0] += up_perp[0] * 0.1;
    camera[1] += up_perp[1] * 0.1;
    camera[2] += up_perp[2] * 0.1;
    look[0] += up_perp[0] * 0.1;
    look[1] += up_perp[1] * 0.1;
    look[2] += up_perp[2] * 0.1;
    break;
  case GLUT_KEY_RIGHT:
    // move the camera to the opposite direction of up_perp vector
    camera[0] -= up_perp[0] * 0.1;
    camera[1] -= up_perp[1] * 0.1;
    camera[2] -= up_perp[2] * 0.1;
    look[0] -= up_perp[0] * 0.1;
    look[1] -= up_perp[1] * 0.1;
    look[2] -= up_perp[2] * 0.1;
    break;
  case GLUT_KEY_UP:
    // move the camera in the direction of look vector
    camera[0] += look_vec[0] * 0.1;
    camera[1] += look_vec[1] * 0.1;
    camera[2] += look_vec[2] * 0.1;
    look[0] += look_vec[0] * 0.1;
    look[1] += look_vec[1] * 0.1;
    look[2] += look_vec[2] * 0.1;
    break;
  case GLUT_KEY_DOWN:
    // move the camera in the opposite direction of look vector
    camera[0] -= look_vec[0] * 0.1;
    camera[1] -= look_vec[1] * 0.1;
    camera[2] -= look_vec[2] * 0.1;
    look[0] -= look_vec[0] * 0.1;
    look[1] -= look_vec[1] * 0.1;
    look[2] -= look_vec[2] * 0.1;
    break;
  case GLUT_KEY_PAGE_UP:
    // move the camera in the up vectors direction
    camera[0] += up[0] * 0.1;
    camera[1] += up[1] * 0.1;
    camera[2] += up[2] * 0.1;
    look[0] += up[0] * 0.1;
    look[1] += up[1] * 0.1;
    look[2] += up[2] * 0.1;
    break;
  case GLUT_KEY_PAGE_DOWN:
    // move the camera in the opposite direction of up vector
    camera[0] -= up[0] * 0.1;
    camera[1] -= up[1] * 0.1;
    camera[2] -= up[2] * 0.1;
    look[0] -= up[0] * 0.1;
    look[1] -= up[1] * 0.1;
    look[2] -= up[2] * 0.1;
    break;
  default:
    break;
  }
  glutPostRedisplay();
  // camera = normalize(camera);
  // look = normalize(look);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char **argv) {
  look[0] = 0;
  look[1] = 0;
  look[2] = -1;
  camera[0] = 0;
  camera[1] = 0;
  camera[2] = 0;
  up[0] = 0;
  up[1] = 1;
  up[2] = 0;
  glutInit(&argc, argv);          // Initialize GLUT
  glutInitWindowSize(640, 640);   // Set the window's initial width & height
  glutInitWindowPosition(50, 50); // initial window position
  glutCreateWindow(
      "Offline 1: magic cube"); // Create a window with the given title
  glutDisplayFunc(
      display); // Register display callback handler for window re-paint
  glutReshapeFunc(
      reshape); // Register reshape callback handler for window re-size
  glutKeyboardFunc(key_poressed);       // register keyboard press callback
  glutSpecialFunc(special_key_pressed); // register special key press callback
  initGL();                             // Our own OpenGL initialization
  glutMainLoop(); // Enter the infinitely event-processing loop
  return 0;
}

float Q_rsqrt(float number) {
  long i;
  float x2, y;
  const float threehalfs = 1.5F;

  x2 = number * 0.5F;
  y = number;
  i = *(long *)&y;           // evil floating point bit level hacking
  i = 0x5f3759df - (i >> 1); // what the fuck?
  y = *(float *)&i;
  y = y * (threehalfs - (x2 * y * y)); // 1st iteration
  // y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be
  // removed

  return y;
}

float *normalize(float vector[3]) {
  // sanity check for magnitude 0
  if (vector[0] == 0 && vector[1] == 0 && vector[2] == 0) {
    return vector;
  }

  // magnitude
  float magnitude =
      vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2];
  magnitude = Q_rsqrt(magnitude);

  vector[0] *= magnitude;
  vector[1] *= magnitude;
  vector[2] *= magnitude;

  return vector;
}