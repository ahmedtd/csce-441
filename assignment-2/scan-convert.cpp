
#include <algorithm>
using std::fill;
using std::min;
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;

#include <GL/glut.h>

#include "color.hpp"
#include "geometry.hpp"

int window_width(400);
int window_height(400);

vector<vector<vec2> > finished_polygons;
vector<vec2> polygon_head;

vector<color> pseudo_framebuffer(window_width * window_height);

// Sets pixel x,y to the color RGB 
void setFramebuffer(int x, int y, const color& newcolor)
{
	// changes the origin from the lower-left corner to the upper-left corner
    pseudo_framebuffer[y*window_width+x] = newcolor;
}

void displayCallback()
{   
    cout << "Display: framebuffer " << pseudo_framebuffer.size() << endl;

    glClear(GL_COLOR_BUFFER_BIT);

    color white(1.0, 1.0, 1.0, 1.0);
    for(int count = 0; count < 100; count++)
    {
        setFramebuffer(count, count, white);
    }
    
    // Draw the pseudo-framebuffer
    glDrawPixels(window_width,
                 window_height,
                 GL_RGBA,
                 GL_FLOAT,
                 &(pseudo_framebuffer[0]));

    // Clear the pseudo-framebuffer
    color transp_black;
    fill(pseudo_framebuffer.begin(), pseudo_framebuffer.end(), transp_black);

    glFlush();
}

void reshapeCallback(int new_width, int new_height)
{
    cout << "Reshape: ("
         << window_width << ", " << window_height
         << ") -> ("
         << new_width << ", " << new_height
         << ")" << endl;
    
    // Update the pseudo-framebuffer to reflect the new size
    vector<color> new_framebuffer(new_width*new_height);
    for(int r_ind = 0; r_ind < min(new_height, window_height); r_ind++)
    {
        for(int c_ind = 0; c_ind < min(new_width, window_width); c_ind++)
        {
            new_framebuffer[r_ind*new_width + c_ind] =
                pseudo_framebuffer[r_ind*window_width + c_ind];            
        }
    }
    pseudo_framebuffer = new_framebuffer;
    
    // Update the global information on the window shape
    window_width  = new_width;
    window_height = new_height;
    
    // Update the projection matrix to account for the window shape
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, window_width, window_height, 0, -1.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glFlush();
}

void mouseClickCallback(int button, int state, int x, int y)
{
    // Turn window coordinates into canvas coordinates by inverting y
    y = window_height - y - 1;
    
    vec2 position(x, y);

    // Are we adding a vertex to a polygon?
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        polygon_head.push_back(position);
        
        cout << "New vertex added: " << position << endl;
    }
    
    // Are we finishing a polygon?
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        // Add the current location as a vertex of the current polygon
        polygon_head.push_back(vec2(x, y));
        
        // Check if the polygon has at least three vertices
        if(polygon_head.size() >= 3)
        {
            finished_polygons.push_back(polygon_head);
            polygon_head.clear();
                        
            cout << "Polygon closed: " << position  << endl;
        }
        else
        {
            cout << "Warning -- can't close polygon: " << position << endl;
        }
        
    }
    
}

void mouseDragCallback(int x, int y)
{
    
}

void mouseMoveCallback(int x, int y)
{
    
}

void keyboardCallback(unsigned char key, int x, int y)
{
    
}

int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_ALPHA);
	glutInitWindowSize(window_width, window_height);
	
	glutCreateWindow("Taahir Ahmed - Homework 2");

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Register callbacks
	glutDisplayFunc(       displayCallback);
    glutReshapeFunc(       reshapeCallback);
    glutMouseFunc(         mouseClickCallback);
    glutMotionFunc(        mouseDragCallback);
    glutPassiveMotionFunc( mouseMoveCallback);
    glutKeyboardFunc(      keyboardCallback);

	glutMainLoop();
	return 0;
}

