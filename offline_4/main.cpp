#define _USE_MATH_DEFINES
#define PI_DEGREE 180.0

#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "color.cpp"
#include "vector3d.cpp"

using namespace std;

bool draw_axis_flag = true;

float *normalize(float vector[3]) {
  // sanity check for magnitude 0
  if (vector[0] == 0 && vector[1] == 0 && vector[2] == 0) {
    return vector;
  }

  // magnitude
  float magnitude =
      vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2];
  magnitude = sqrt(magnitude);

  vector[0] *= magnitude;
  vector[1] *= magnitude;
  vector[2] *= magnitude;

  return vector;
}

// global variable
// camera
Vector3D camera;
// look vector
Vector3D look;
// up vector
Vector3D up;
// right vector
Vector3D r;

void draw_axis() {
  // define 3 axis
  Vector3D X(1, 0, 0);
  Vector3D Y(0, 1, 0);
  Vector3D Z(0, 0, 1);
  Vector3D origin(0, 0, 0);

  // 3 colors for 3 axis
  Color red(255, 0, 0);
  Color green(0, 255, 0);
  Color blue(0, 0, 255);

  // draw axis
  if (draw_axis_flag) {
    glBegin(GL_LINES);
    // x axis
    glColor3f(1.0 * red[0] / 255, 1.0 * red[1] / 255, 1.0 * red[2] / 255);
    glVertex3d(origin[0], origin[1], origin[2]);
    glVertex3d(X[0], X[1], X[2]);

    glColor3f(1.0 * green[0] / 255, 1.0 * green[1] / 255, 1.0 * green[2] / 255);
    glVertex3d(origin[0], origin[1], origin[2]);
    glVertex3d(Y[0], Y[1], Y[2]);

    glColor3f(1.0 * blue[0] / 255, 1.0 * blue[1] / 255, 1.0 * blue[2] / 255);
    glVertex3d(origin[0], origin[1], origin[2]);
    glVertex3d(Z[0], Z[1], Z[2]);

    glEnd();
  }
}

/* Initialize OpenGL Graphics */
void initGL() {
  glClearColor(0.0f, 0.0f, 0.0f,
               1.0f);       // Set background color to black and opaque
  glClearDepth(1.0f);       // Set background depth to farthest
  glEnable(GL_DEPTH_TEST);  // Enable depth testing for z-culling
  glDepthFunc(GL_LEQUAL);   // Set the type of depth-test
  glShadeModel(GL_SMOOTH);  // Enable smooth shading
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,
         GL_NICEST);  // Nice perspective corrections
}

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
  glClear(GL_COLOR_BUFFER_BIT |
          GL_DEPTH_BUFFER_BIT);  // Clear color and depth buffers
  glMatrixMode(GL_MODELVIEW);    // To operate on model-view matrix

  glLoadIdentity();  // Reset the model-view matrix

  gluLookAt(camera[0], camera[1], camera[2], look[0], look[1], look[2], up[0],
            up[1], up[2]);

  // draw a triangle to test
  glColor3f(1, 1, 0);
  glBegin(GL_TRIANGLES);
  glVertex3d(1, 0, 0);
  glVertex3d(0, 1, 0);
  glVertex3d(0, 0, 1);
  glEnd();

  draw_axis();
  glPushMatrix();
  glPopMatrix();
  glutSwapBuffers();  // Swap the front and back frame buffers (double
                      // buffering)
}

/* Handler for window re-size event. Called back when the window first appears
   and whenever the window is re-sized with its new width and height */
void reshape(GLsizei width,
             GLsizei height) {  // GLsizei for non-negative integer
  // Compute aspect ratio of the new window
  if (height == 0) height = 1;  // To prevent divide by 0
  GLfloat aspect = (GLfloat)width / (GLfloat)height;

  // Set the viewport to cover the new window
  glViewport(0, 0, width, height);

  // Set the aspect ratio of the clipping volume to match the viewport
  glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
  glLoadIdentity();             // Reset
  // Enable perspective projection with fovy, aspect, zNear and zFar
  gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/**
 * callback function for key press (normal keys)
 */
void key_pressed(unsigned char key, int x, int y) {
  // rate of rotation
  float rate_rotation = M_PI / 60;
  float rate_translation = 0.1;
  // calculate the look vector
  Vector3D look_vec(look - camera);

  // normalize the up vector and look vector
  up.normalize();
  // look_vec = normalize(look_vec);

  // now find the cross product of look_vec and up
  Vector3D cross = look_vec * up;

  // the vectors up look and cross are perpendicular to each other
  switch (key) {
    case '1':
      // rotate the look vector to cross vector
      look_vec[0] =
          cross[0] * sin(rate_rotation) + look_vec[0] * cos(rate_rotation);
      look_vec[1] =
          cross[1] * sin(rate_rotation) + look_vec[1] * cos(rate_rotation);
      look_vec[2] =
          cross[2] * sin(rate_rotation) + look_vec[2] * cos(rate_rotation);
      break;
    case '2':
      // rotate the look vector away from cross vector
      look_vec[0] =
          cross[0] * sin(-rate_rotation) + look_vec[0] * cos(-rate_rotation);
      look_vec[1] =
          cross[1] * sin(-rate_rotation) + look_vec[1] * cos(-rate_rotation);
      look_vec[2] =
          cross[2] * sin(-rate_rotation) + look_vec[2] * cos(-rate_rotation);
      break;
    case '3':
      // rotate the look vector to up vector
      look_vec[0] =
          up[0] * sin(rate_rotation) + look_vec[0] * cos(rate_rotation);
      look_vec[1] =
          up[1] * sin(rate_rotation) + look_vec[1] * cos(rate_rotation);
      look_vec[2] =
          up[2] * sin(rate_rotation) + look_vec[2] * cos(rate_rotation);
      // change the up vector to perpendicular to look vector and cross vector
      up = (cross * look_vec);
      break;
    case '4':
      // rotate the look vector away from up vector
      look_vec[0] =
          up[0] * sin(-rate_rotation) + look_vec[0] * cos(-rate_rotation);
      look_vec[1] =
          up[1] * sin(-rate_rotation) + look_vec[1] * cos(-rate_rotation);
      look_vec[2] =
          up[2] * sin(-rate_rotation) + look_vec[2] * cos(-rate_rotation);
      // change the up vector to perpendicular to look vector and cross vector
      up = (cross * look_vec);
      break;
    case '5':
      // rotate the up vector to the cross vector
      up[0] = cross[0] * sin(rate_rotation) + up[0] * cos(rate_rotation);
      up[1] = cross[1] * sin(rate_rotation) + up[1] * cos(rate_rotation);
      up[2] = cross[2] * sin(rate_rotation) + up[2] * cos(rate_rotation);
      break;
    case '6':
      // rotate the up vector away from the cross vector
      up[0] = cross[0] * sin(-rate_rotation) + up[0] * cos(-rate_rotation);
      up[1] = cross[1] * sin(-rate_rotation) + up[1] * cos(-rate_rotation);
      up[2] = cross[2] * sin(-rate_rotation) + up[2] * cos(-rate_rotation);
      break;
    case 'l':
      // toggle the draw axis flag
      draw_axis_flag = !draw_axis_flag;
      break;
    case 'w':
      // move the camera in the direction of up vector
      // don't change the look position
      camera[0] += up[0] * rate_translation;
      camera[1] += up[1] * rate_translation;
      camera[2] += up[2] * rate_translation;
      // new look vector
      look_vec = look - camera;
      break;
    case 's':
      // move the camera in the opposite direction of up vector
      // don't change the look position
      camera[0] -= up[0] * rate_translation;
      camera[1] -= up[1] * rate_translation;
      camera[2] -= up[2] * rate_translation;
      // new look vector
      look_vec = look - camera;
      break;
    default:
      break;
  }
  // recalculating the look vector
  look = camera + look_vec;
  glutPostRedisplay();
}

/**
 * callback function for key press (special keys)
 */
void special_key_pressed(int key, int x, int y) {
  float rate_translation = 0.1;
  // find the up vector's perpendicular vector
  // we can find the perpendicular vector by cross product of up vector and look
  // vector
  Vector3D look_vec(look - camera);
  Vector3D up_perp = up * look_vec;
  switch (key) {
    case GLUT_KEY_LEFT:
      // move the camera in the direction of up_perp vector
      camera[0] += up_perp[0] * rate_translation;
      camera[1] += up_perp[1] * rate_translation;
      camera[2] += up_perp[2] * rate_translation;
      look[0] += up_perp[0] * rate_translation;
      look[1] += up_perp[1] * rate_translation;
      look[2] += up_perp[2] * rate_translation;
      break;
    case GLUT_KEY_RIGHT:
      // move the camera to the opposite direction of up_perp vector
      camera[0] -= up_perp[0] * rate_translation;
      camera[1] -= up_perp[1] * rate_translation;
      camera[2] -= up_perp[2] * rate_translation;
      look[0] -= up_perp[0] * rate_translation;
      look[1] -= up_perp[1] * rate_translation;
      look[2] -= up_perp[2] * rate_translation;
      break;
    case GLUT_KEY_UP:
      // move the camera in the direction of look vector
      camera[0] += look_vec[0] * rate_translation;
      camera[1] += look_vec[1] * rate_translation;
      camera[2] += look_vec[2] * rate_translation;
      look[0] += look_vec[0] * rate_translation;
      look[1] += look_vec[1] * rate_translation;
      look[2] += look_vec[2] * rate_translation;
      break;
    case GLUT_KEY_DOWN:
      // move the camera in the opposite direction of look vector
      camera[0] -= look_vec[0] * rate_translation;
      camera[1] -= look_vec[1] * rate_translation;
      camera[2] -= look_vec[2] * rate_translation;
      look[0] -= look_vec[0] * rate_translation;
      look[1] -= look_vec[1] * rate_translation;
      look[2] -= look_vec[2] * rate_translation;
      break;
    case GLUT_KEY_PAGE_UP:
      // move the camera in the up vectors direction
      camera[0] += up[0] * rate_translation;
      camera[1] += up[1] * rate_translation;
      camera[2] += up[2] * rate_translation;
      look[0] += up[0] * rate_translation;
      look[1] += up[1] * rate_translation;
      look[2] += up[2] * rate_translation;
      break;
    case GLUT_KEY_PAGE_DOWN:
      // move the camera in the opposite direction of up vector
      camera[0] -= up[0] * rate_translation;
      camera[1] -= up[1] * rate_translation;
      camera[2] -= up[2] * rate_translation;
      look[0] -= up[0] * rate_translation;
      look[1] -= up[1] * rate_translation;
      look[2] -= up[2] * rate_translation;
      break;
    default:
      break;
  }
  glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char **argv) {
  look[0] = 0;
  look[1] = 0;
  look[2] = 0;
  camera[0] = 4;
  camera[1] = 4;
  camera[2] = 4;
  up[0] = 0;
  up[1] = 1;
  up[2] = 0;
  glutInit(&argc, argv);           // Initialize GLUT
  glutInitWindowSize(640, 640);    // Set the window's initial width & height
  glutInitWindowPosition(50, 50);  // initial window position
  glutCreateWindow(
      "Offline 4: Ray Tracing");  // Create a window with the given title
  glutDisplayFunc(
      display);  // Register display callback handler for window re-paint
  glutReshapeFunc(
      reshape);  // Register reshape callback handler for window re-size
  glutKeyboardFunc(key_pressed);         // register keyboard press callback
  glutSpecialFunc(special_key_pressed);  // register special key press callback
  initGL();                              // Our own OpenGL initialization
  glutMainLoop();  // Enter the infinitely event-processing loop
  return 0;
}