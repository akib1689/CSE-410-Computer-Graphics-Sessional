#define _USE_MATH_DEFINES
#define PI_DEGREE 180.0

#include <iostream>

#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

#include <cmath>

#include "octahedron.h"


using namespace std;

octahedron octahedron;

// Global variables

// function 
float Q_rsqrt(float);

// camera position
GLdouble camera[] = { 0.0f, 0.0f, 0.0f };
// looking position
GLfloat look[] = { 0.0f, 0.0f, -1.0f };
// up vector
GLfloat up[] = { 0.0f, 1.0f, 0.0f };

/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

    glLoadIdentity();                 // Reset the model-view matrix

    gluLookAt(camera[0], camera[1], camera[2], look[0], look[1], look[2], up[0], up[1], up[2]);

    glTranslatef(0.0f, 0.0f, -7.0f);  // Move the origin to back
    octahedron.draw_octahedron();
    glTranslatef(0.0f, 0.0f, 7.0f);  // Move the origin back to the center
    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}


/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
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
void key_poressed(unsigned char key, int x, int y) {
    // calculate the look vector
    double look_vec[3];
    look_vec[0] = look[0] - camera[0];
    look_vec[1] = look[1] - camera[1];
    look_vec[2] = look[2] - camera[2];
    // magnitude
    float magnitude = look_vec[0] * look_vec[0] + look_vec[1] * look_vec[1] + look_vec[2] * look_vec[2];
    magnitude = Q_rsqrt(magnitude);
    // normalize
    look_vec[0] *= magnitude;
    look_vec[1] *= magnitude;
    look_vec[2] *= magnitude;
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
    case '1':
        // rotate the look vector around xz plane
        // y axis is taken as pivot
        look_vec[0] = look_vec[0] * cos(M_PI / 180) + look_vec[2] * sin(M_PI / 180);
        look_vec[2] = -look_vec[0] * sin(M_PI / 180) + look_vec[2] * cos(M_PI / 180);
        break;
    case '2':
        // rotate the look vector around xz plane
        // y axis is taken as pivot
        look_vec[0] = look_vec[0] * cos(-M_PI / 180) + look_vec[2] * sin(-M_PI / 180);
        look_vec[2] = -look_vec[0] * sin(-M_PI / 180) + look_vec[2] * cos(-M_PI / 180);
        break;
    case '3':
        // rotate the look vector around yz plane
        // x axis is taken as pivot
        look_vec[1] = look_vec[1] * cos(-M_PI / 180) + look_vec[2] * sin(-M_PI / 180);
        look_vec[2] = -look_vec[1] * sin(-M_PI / 180) + look_vec[2] * cos(-M_PI / 180);
        break;
    case '4':
        // rotate the look vector around yz plane
        // x axis is taken as pivot
        look_vec[1] = look_vec[1] * cos(M_PI / 180) + look_vec[2] * sin(M_PI / 180);
        look_vec[2] = -look_vec[1] * sin(M_PI / 180) + look_vec[2] * cos(M_PI / 180);
        break;
    case '5':
        // rotate the up vector around xy plane
        // z axis is taken as pivot
        up[0] = up[0] * cos(M_PI / 180) + up[1] * sin(M_PI / 180);
        up[1] = -up[0] * sin(M_PI / 180) + up[1] * cos(M_PI / 180);
        break;
    case '6':
        // rotate the up vector around xy plane
        // z axis is taken as pivot
        up[0] = up[0] * cos(-M_PI / 180) + up[1] * sin(-M_PI / 180);
        up[1] = -up[0] * sin(-M_PI / 180) + up[1] * cos(-M_PI / 180);
        break;
    default:
        break;
    }
    look[0] = camera[0] + look_vec[0];
    look[1] = camera[1] + look_vec[1];
    look[2] = camera[2] + look_vec[2];
    glutPostRedisplay();

}

/**
 * callback function for key press (special keys)
*/
void special_key_pressed(int key, int x, int y) {
    // first find to which direction the up vector is pointing more
    cout << "up vector: " << up[0] << " " << up[1] << " " << up[2] << endl;
    switch (key) {
    case GLUT_KEY_LEFT:
        // move the camera to left
        camera[0] -= 0.1;
        look[0] -= 0.1;
        glutPostRedisplay();
        break;
    case GLUT_KEY_RIGHT:
        // move the camera to right
        camera[0] += 0.1;
        look[0] += 0.1;
        glutPostRedisplay();
        break;
    case GLUT_KEY_UP:
        // move the camera forward
        camera[2] -= 0.1;
        look[2] -= 0.1;
        glutPostRedisplay();
        break;
    case GLUT_KEY_DOWN:
        // move the camera backward
        camera[2] += 0.1;
        look[2] += 0.1;
        glutPostRedisplay();
        break;
    case GLUT_KEY_PAGE_UP:
        // move the camera upward
        camera[1] += 0.1;
        look[1] += 0.1;
        glutPostRedisplay();
        break;
    case GLUT_KEY_PAGE_DOWN:
        // move the camera downward
        camera[1] -= 0.1;
        look[1] -= 0.1;
        glutPostRedisplay();
        break;
    default:
        break;
    }

}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    glutInit(&argc, argv);         // Initialize GLUT
    glutInitWindowSize(640, 640);  // Set the window's initial width & height
    glutInitWindowPosition(50, 50);  // initial window position
    glutCreateWindow("Offline 1: magic cube");  // Create a window with the given title
    glutDisplayFunc(display);    // Register display callback handler for window re-paint
    glutReshapeFunc(reshape);    // Register reshape callback handler for window re-size
    glutKeyboardFunc(key_poressed); // register keyboard press callback
    glutSpecialFunc(special_key_pressed); // register special key press callback
    initGL();                     // Our own OpenGL initialization
    glutMainLoop();  // Enter the infinitely event-processing loop
    return 0;
}



float Q_rsqrt(float number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = *(long*)&y;                       // evil floating point bit level hacking
    i = 0x5f3759df - (i >> 1);               // what the fuck?
    y = *(float*)&i;
    y = y * (threehalfs - (x2 * y * y));   // 1st iteration
    // y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

    return y;
}