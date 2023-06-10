#define _USE_MATH_DEFINES
#define ANGLE_INCREMENT 6.0
#define PI 3.14159265358979323846
#define HIGH_BOB_ANGLE 15.0
#define HOUR_ANGLE 30.0
#define HOUR_ANGLE_PER_MINUTES 0.5
#define MINUTE_ANGLE 6.0
#define MINUTE_ANGLE_PER_SECONDS 0.1
#define SECOND_ANGLE 6.0

#include <GL/glut.h> // GLUT, includes glu.h and gl.h
#include <math.h>    // for tanf and cosf function

#include <ctime> // for time function (to get the current time)
#include <iostream>

using namespace std;
// function declarations
void draw_circle(float, float, float, int, bool);
void draw_hour_hand();
void draw_minute_hand();
void draw_second_hand();
void second_handler(int);
void draw_clock();
void draw_pendulum();

// variables
// hour angle
GLfloat hour_angle = 0.0;
// minute angle
GLfloat minute_angle = 0.0;
// second angle
GLfloat second_angle = 0.0;

// pendulum angle
GLfloat pendulum_angle = 0.0;

// pendulum direction
int pendulum_direction = 1;

// last time the idle function was called
clock_t last_idle_time = 0;

/**
 * Handler for window-repaint event. Call back when the window first appears and
 * whenever the window needs to be re-painted.
 */
void display() {
  glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

  // translate the clock to the upper portion
  // of the screen and also scale them down
  glTranslatef(0.0f, 0.5f, 0.0f);
  glScalef(0.5f, 0.5f, 0.5f);
  // draw the clock
  draw_clock();
  // reverse the translation
  glScalef(2.0f, 2.0f, 2.0f);
  glTranslatef(0.0f, -0.5f, 0.0f);
  // translate to the pendulum position
  glRotatef(pendulum_angle, 0.0f, 0.0f, 1.0f);
  // draw the pendulum
  draw_pendulum();
  // reverse the translation
  glRotatef(-pendulum_angle, 0.0f, 0.0f, 1.0f);

  glFlush(); // Render now
}
/**
 * calculation of the pendulum angle:
 * time period of the pendulum = 2 seconds
 * angular frequency = 2 * PI / time period = PI
 * pendulum angle = HIGH_BOB_ANGLE * cos(angular frequency * time)
 */
void idle_handler() {
  // get the current time
  clock_t now = clock();

  // find the difference between two clock ticks
  double time_diff = (double)(now - last_idle_time) / CLOCKS_PER_SEC;
  // debug print
  // cout << "time diff: " << time_diff << endl;
  // calculate the pendulum angle
  pendulum_angle = HIGH_BOB_ANGLE * cos(PI * time_diff);
  // debug print
  // cout << "pendulum angle: " << pendulum_angle << endl;
  glutPostRedisplay();
}

/**
 *  Handler for window re-size event. Called back when the window first appears
 * and whenever the window is re-sized with its new width and height
 */
void reshape(GLsizei width, GLsizei height) { // GLsizei for non-negative integer
  // Compute aspect ratio of the new window
  if (height == 0)
    height = 1; // To prevent divide by 0
  GLfloat aspect = (GLfloat)width / (GLfloat)height;

  // Set the viewport to cover the new window
  glViewport(0, 0, width, height);

  // Set the aspect ratio of the clipping area to match the viewport
  glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
  glLoadIdentity();
  if (width >= height) {
    // aspect >= 1, set the height from -1 to 1, with larger width
    gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
  }
  else {
    // aspect < 1, set the width to -1 to 1, with larger height
    gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
  }
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
  // get the current time
  time_t now = time(0);
  last_idle_time = clock();
  // convert the current time to local time
  tm* local_time = localtime(&now);
  // calculate the degree of rotation for the hour hand, minute hand and second hand
  hour_angle = (local_time->tm_hour % 12) * HOUR_ANGLE + (local_time->tm_min * HOUR_ANGLE_PER_MINUTES);
  minute_angle = local_time->tm_min * MINUTE_ANGLE + (local_time->tm_sec * MINUTE_ANGLE_PER_SECONDS);
  second_angle = local_time->tm_sec * SECOND_ANGLE;
  // adjust the hour angle the hour and minutes hand by 90 degrees
  hour_angle -= 90.0;
  minute_angle -= 90.0;
  second_angle -= 90.0;

  // debug print
  // cout << "hour angle: " << hour_angle << endl;
  // cout << "minute angle: " << minute_angle << endl;

  // print various components of tm structure.
  // cout << "Time: " << local_time->tm_hour << ":"
  //   << local_time->tm_min << ":" << local_time->tm_sec << endl;

  glutInit(&argc, argv);        // Initialize GLUT
  glutInitWindowSize(640, 640); // Set the window's initial width & height

  glutInitWindowPosition(50, 50); // Position the window's initial top-left corner

  glutCreateWindow("Offline 1: Clock"); // Create a window with the given title
  glutDisplayFunc(display);             // Register display callback handler for window re-paint

  glutReshapeFunc(reshape); // Register callback handler for window re-size event

  glutIdleFunc(idle_handler);          // Register callback handler if no other event occurs
  glutTimerFunc(0, second_handler, 0); // First timer call immediately
  glutMainLoop();                      // Enter the infinitely event-processing loop
  return 0;
}

/**
 * This function draws the hour hand of the clock
 * at the center of the screen the shifting and rotation of the hand is done
 * by the caller function
 */
void draw_hour_hand() {
  glBegin(GL_POLYGON);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(0.3f, -0.04f);
  glVertex2f(0.5f, 0.0f);
  glVertex2f(0.3f, 0.04f);
  glEnd();

  draw_circle(0.325f, 0.0f, 0.05f, 10, true);
}
/**
 * This function draws the minute hand of the clock
 * hifting and rotation of the hand is done by the caller function
 */
void draw_minute_hand() {
  glBegin(GL_POLYGON);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(0.5f, -0.025f);
  glVertex2f(0.8f, 0.0f);
  glVertex2f(0.5f, 0.025f);
  glEnd();
}
/**
 * This function draws the second hand of the clock
 * the shifting and rotation of the hand is done by the caller function
 */
void draw_second_hand() {
  glBegin(GL_LINES);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(0.8f, 0.0f);
  glEnd();
}

/**
 * function responsible for entire clock drawing
 * draws the hour hand minute hand and second hand seperately then calls the
 * display function uses local coordinate system so any shifting and rotating
 * applied on the clock is done by the caller function
 */
void draw_clock() {
  // rotate the hour hand by the hour angle
  glRotatef(-hour_angle, 0.0f, 0.0f, 1.0f);
  // draw the hour hand
  draw_hour_hand();
  // rotate the hour hand back to the original position
  glRotatef(+hour_angle, 0.0f, 0.0f, 1.0f);

  // rotate for the minutes hand
  glRotatef(-minute_angle, 0.0f, 0.0f, 1.0f);
  // draw the minute hand
  draw_minute_hand();
  // rotate back to the original position
  glRotatef(+minute_angle, 0.0f, 0.0f, 1.0f);

  // rotate for the seconds hand
  glRotatef(-second_angle, 0.0f, 0.0f, 1.0f);
  // draw the seconds hand
  draw_second_hand();
  // rotate back to the original position
  glRotatef(+second_angle, 0.0f, 0.0f, 1.0f);

  // draw the middle bob
  draw_circle(0.0f, 0.0f, 0.04f, 10, true);

  // draw the outer circle
  draw_circle(0.0f, 0.0f, 0.9f, 100, false);
  draw_circle(0.0f, 0.0f, 1.0f, 100, false);

  // draw the inner markings on hour lines
  for (int i = 0; i < 12; i++) {
    glRotatef(HOUR_ANGLE, 0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    // for the 12, 3, 6, 9 lines 
    if (i % 3 == 2) {
      glVertex2f(0.7f, 0.0f);
      glVertex2f(0.9f, 0.0f);
    }
    else {
      glVertex2f(0.8f, 0.0f);
      glVertex2f(0.9f, 0.0f);
    }
    glEnd();
  }

  // the borders
  glBegin(GL_LINE_LOOP);
  glVertex2f(-1.0f, 1.0f);
  glVertex2f(1.0f, 1.0f);
  glVertex2f(1.0f, -1.5f);
  glVertex2f(0.0f, -2.0f);
  glVertex2f(-1.0f, -1.5f);
  glEnd();

}

/**
 * This function draws a circle at the given center and radius
 * the number of segments determines the smoothness of the circle
 */
void draw_circle(float cx, float cy, float r, int num_segments, bool fill) {
  float theta = PI * 2 / float(num_segments);
  float tangetial_factor = tanf(theta); // calculate the tangential factor

  float radial_factor = cosf(theta); // calculate the radial factor

  float x = r; // we start at angle = 0

  float y = 0;
  glLineWidth(2);
  if (fill)
  {
    glBegin(GL_POLYGON);
  }
  else
  {
    glBegin(GL_LINE_LOOP);
  }
  for (int ii = 0; ii < num_segments; ii++)
  {
    glVertex2f(x + cx, y + cy); // output vertex

    // calculate the tangential vector
    // remember, the radial vector is (x, y)
    // to get the tangential vector we flip those coordinates and negate one of
    // them

    float tx = -y;
    float ty = x;

    // add the tangential vector

    x += tx * tangetial_factor;
    y += ty * tangetial_factor;

    // correct using the radial factor

    x *= radial_factor;
    y *= radial_factor;
  }
  glEnd();
}

/**
 * This function is called every second to update the clock
 * This only changes the value then calls the display function
 */
void second_handler(int value) {
  // time_t now = time(0);
  // tm *local_time = localtime(&now);

  // print various components of tm structure.
  // cout << "Time: " << local_time->tm_hour << ":"
  //      << local_time->tm_min << ":"
  //      << local_time->tm_sec << endl;
  // increment the second angle
  second_angle += ANGLE_INCREMENT;
  // reset the angle if it is greater than 360
  if (second_angle >= 360.0) {
    second_angle -= 360.0;
  }
  // increment the minute angle
  minute_angle += ANGLE_INCREMENT / 60.0;
  // reset
  if (minute_angle >= 360.0) {
    minute_angle -= 360.0;
  }
  // increment the hour angle
  hour_angle += ANGLE_INCREMENT / 3600.0;
  // reset
  if (hour_angle >= 360.0) {
    hour_angle -= 360.0;
  }
  // call the display function
  glutPostRedisplay();
  // call the timer function again
  glutTimerFunc(1000, second_handler, 0);
}

void draw_pendulum()
{
  // draw the pendulum rod
  glBegin(GL_LINES);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(0.0f, -0.7f);
  glEnd();

  // draw the bob
  draw_circle(0.0f, -0.7f, 0.1f, 50, true);
}