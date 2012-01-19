#include <GL/glut.h>
#include <GL/gl.h>

int window_width(800);
int window_height(600);

void displayCallback(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
        glVertex2f(0, 0);
        glVertex2f(window_width - 1, window_height - 1);
    glEnd();
    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
        glVertex2f(0.1 * window_width, 0.1 * window_height);
        glVertex2f(0.9 * window_width, 0.1 * window_height);
        glVertex2f(0.9 * window_width, 0.9 * window_height);
        glVertex2f(0.1 * window_width, 0.9 * window_height);
    glEnd();
    glutSwapBuffers();
}

void reshapeCallback(int width, int height)
{
    // Update the global information on the window shape
    window_width = width;
    window_height = height;
    
    // Update the projection matrix to account for the window shape
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, window_width, window_height, 0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);

    glutPostRedisplay();
}

void keyboardCallback(unsigned char key, int x, int y)
{
    glutPostRedisplay();
}

void mouseMoveCallback(int x, int y)
{
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    // Initialize glut and window
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);
    glutInitWindowSize(window_width, window_height); 
    
    glutCreateWindow ("Taahir Ahmed - Assignment 1");

    // Default to transparent black
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // Switch to modelview mode for drawing
    glMatrixMode(GL_MODELVIEW);

    // Register glut callbacks
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    glutMotionFunc(mouseMoveCallback);
    glutKeyboardFunc(keyboardCallback);
    
    // Pass control to glut
    glutMainLoop();
    return 0;
}
