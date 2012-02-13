
#include <algorithm>
using std::copy;
using std::fill;
using std::max;
using std::min;
using std::replace_if;
using std::sort;
#include <cmath>
using std::floor;
#include <iostream>
using std::cout;
using std::endl;
#include <list>
using std::list;
#include <map>
using std::map;
#include <vector>
using std::vector;

#include <GL/glut.h>

#include "color.hpp"
#include "geometry.hpp"

int window_width(400);
int window_height(400);

struct edgedesc
{
    int ymax;
    int xcur;
    int xcur_frac;
    int slope_y;
    int slope_x;
};

bool operator<(const edgedesc &lop, const edgedesc &rop)
{
    return lop.xcur < rop.xcur;
}

class edge_y_selector
{
public:
    edge_y_selector(int yval)
    :
        m_yval(yval)
    {
    }
    
    bool operator() (const edgedesc &edge)
    {
        return m_yval == edge.ymax;
    }

private:
    int m_yval;
};

vector<vector<vec2> > polygons(1);

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

    // Build the global edge table
    map<int, list<edgedesc> > get;
    
    // Iterate over all polygons
    vector<vector<vec2> >::iterator polygon_it;
    for(polygon_it = polygons.begin();
        polygon_it != polygons.end();
        polygon_it++)
    {
        // Check to make sure this polygon is displayable
        if((*polygon_it).size() <= 2)
            continue;

        // Iterate over all edges of the polygon
        vector<vec2>::iterator p1_it = (*polygon_it).end()--;
        vector<vec2>::iterator p2_it = (*polygon_it).begin();
        for(; p2_it != (*polygon_it).end(); p1_it++, p2_it++)
        {
            if(p1_it == (*polygon_it).end())
                p1_it = (*polygon_it).begin();
            
            // TEST CODE
            setFramebuffer(p2_it->x(), p2_it->y(), color::white());

            // Unpack the relevant parameters of the edge
            int minPixX = min(floor(p1_it->x()), floor(p2_it->x()));
            int maxPixX = max(floor(p1_it->x()), floor(p2_it->y()));
            int minPixY = min(floor(p1_it->y()), floor(p2_it->y()));
            int maxPixY = max(floor(p1_it->y()), floor(p2_it->y()));
            
            int sign = 1;
            if(p2_it->x() > p1_it->x())
                if(p2_it->y() > p1_it->y())
                    sign = 1;
                else
                    sign = -1;
            else
                if(p2_it->y() > p1_it->y())
                    sign = -1;
                else
                    sign = 1;

            // If this edge is horizontal in pixel coordinates, ignore it
            if(minPixY == maxPixY)
                continue;
            
            // Pack up this edge
            edgedesc thisedge;
            thisedge.ymax = maxPixY;
            thisedge.xcur = minPixX;
            thisedge.xcur_frac = 0;
            thisedge.slope_x = sign * (maxPixX - minPixX);
            thisedge.slope_y = maxPixY - minPixY;

            // Put it in the global edge table
            (get[minPixY]).push_back(thisedge);
            get[minPixY].sort();
        }
    }

    // Scan-convert from the global edge table
    list<edgedesc> aet;
    map<int, list<edgedesc> >::iterator get_it = get.begin();
    int curPixY = get.begin()->first;
    while(curPixY < window_height)
    {
        // If we're on a y value where edges begin or end
        if(curPixY == get_it->first)
        {
            // Copy edges that start here to the active edge table
            copy(get_it->second.begin(), get_it->second.end(), aet.begin());
        
            // Remove edges that end here
            edge_y_selector selector(curPixY);
            remove_if(aet.begin(), aet.end(), selector);
            
            aet.sort();
        }
      
        // Draw the span at this range
        list<edgedesc>::iterator curBound_it = aet.begin();
        int curPixX = curBound_it->xcur;
        curBound_it++;
        bool drawing = true;
        while(true)
        {
            // Have we reached a transition point?
            if(curPixX == curBound_it->xcur)
            {
                drawing = !drawing;
     
                // Advance the current edge
                curBound_it++;
                
                // Check to see if we're at the last edge
                if(curBound_it == aet.end())
                    break;
            }
            
            if(drawing)
            {
                setFramebuffer(curPixX, curPixY, color::white());
            }    
        }
        
        // Increment y by 1
        curPixY++;

        // Increment the global edge table iterator
        if(curPixY == get_it->first)
        {
            get_it++;
            
            // If we've moved past the top of the global edge table, break out
            // of the scan conversion
            if(get_it == get.end())
                break;
        }

        // Adjust edges in the aet for the new y value
        list<edgedesc>::iterator aet_it = aet.begin();
        for(; aet_it != aet.end(); aet_it++)
        {
            aet_it->xcur_frac += aet_it->slope_x;
            aet_it->xcur += aet_it->xcur_frac / aet_it->slope_y;
            aet_it->xcur_frac = aet_it->xcur_frac % aet_it->slope_y;
        }
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
    
    color trans_black;
    pseudo_framebuffer.resize(new_width*new_height, trans_black);

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
        polygons.back().push_back(position);
        
        cout << "New vertex added: " << position << endl;
    }
    
    // Are we finishing a polygon?
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        // Add the current location as a vertex of the current polygon
        polygons.back().push_back(position);
        
        // Check if the polygon has at least three vertices
        if(polygons.back().size() >= 3)
        {
            vector<vec2> empty;
            polygons.push_back(empty);
                                    
            cout << "Polygon closed: " << position  << endl;
        }
        else
        {
            cout << "Warning -- can't close polygon: " << position << endl;
        }
        
    }
    
    glutPostRedisplay();
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

    // Set up our first polygon

	glutMainLoop();
	return 0;
}

