#define _USE_MATH_DEFINES

#include <GL/glut.h> // GLUT, includes glu.h and gl.h

void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

    // draw a quads
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glBegin(GL_QUADS);
    glVertex2f(-0.8f, 0.3f);
    glVertex2f(-0.8f, -0.1f);
    glVertex2f(-0.6f, -0.1f);
    glVertex2f(-0.6f, 0.3f);
    glEnd();

    glFlush(); // Render now
}

void reshape(int width, int height) {

}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char **argv)
{
    glutInit(&argc, argv);        // Initialize GLUT
    glutInitWindowSize(640, 640); // Set the window's initial width & height

    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner

    glutCreateWindow("OpenGL 2D Drawing"); // Create a window with the given title
    glutDisplayFunc(display);              // Register display callback handler for window re-paint
    glutReshapeFunc(reshape);              // Register callback handler for window re-shape

    glutMainLoop(); // Enter the infinitely event-processing loop
    return 0;
}