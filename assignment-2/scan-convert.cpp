
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
vector<color> polygon_colors;

bool clip_mode(false);

bool defining_rect(false);
vec2 mouse_start;
vec2 mouse_move;

vec2 clip_tl(0, 400);
vec2 clip_br(400, 0);

vector<color> pseudo_framebuffer(window_width * window_height);

// Sets pixel x,y to the color RGB 
void setFramebuffer(int x, int y, const color& newcolor)
{
	// changes the origin from the lower-left corner to the upper-left corner
    pseudo_framebuffer[y*window_width+x] = newcolor;
}

void setSpan(int xmin, int xmax, int y, const color& newcolor)
{
    for(int xcur = xmin; xcur < xmax; xcur++)
    {
        pseudo_framebuffer[y*window_width+xcur] = newcolor;
    }
}

vec2 projectToClipRegion(const vec2 &orig, const vec2 &tl, const vec2 &br)
{
    double newx = orig.x();
    double newy = orig.y();
    
    if(newx <= tl.x())
        newx = tl.x();
    else if(newx >= br.x())
        newx = br.x();
    
    if(newy <= br.y())
        newy = br.y();
    else if(newy >= tl.y())
        newy = tl.y();

    return vec2(newx, newy);
}

const int oc_l = 8;
const int oc_r = 1;
const int oc_t = 4;
const int oc_b = 2;
const int oc_hor_mask = oc_l & oc_r;
const int oc_ver_mask = oc_t & oc_b;

int genOutCode(const vec2 &point, const vec2 &tl, const vec2 &br)
{
    int outcode = 0;
    
    if(point.x() < tl.x())
        outcode |= oc_l;
    else if(point.x() > br.x())
        outcode |= oc_r;
    
    if(point.y() < br.y())
        outcode |= oc_b;
    else if(point.y() > tl.y())
        outcode |= oc_t;

    return outcode;
}

void displayCallback()
{   
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Iterate over all polygons
    vector<vector<vec2> >::iterator polygon_it;
    vector<color>::const_iterator   color_it;
    for(polygon_it = polygons.begin(), color_it = polygon_colors.begin();
        polygon_it != polygons.end();
        polygon_it++, color_it++)
    {
        // Check to make sure this polygon is displayable
        if((*polygon_it).size() <= 2)
            continue;

        // Perform analytic clipping
        vector<vec2> clipped_poly;
        
        // Iterate over all edges of the polygon
        vector<vec2>::const_iterator p1_it = (*polygon_it).begin();
        //clipped_poly.push_back(projectToClipRegion(*p1_it, clip_tl, clip_br));
        vector<vec2>::const_iterator p2_it = (*polygon_it).begin();
        p2_it++;
        for(; p1_it != (*polygon_it).end(); p1_it++, p2_it++)
        {
            if(p2_it == polygon_it->end())
                p2_it = polygon_it->begin();

            vec2 p1 = *p1_it;
            vec2 p2 = *p2_it;

            // Get the outcodes for p1 and p2
            int p1_oc = genOutCode(p1, clip_tl, clip_br);
            int p2_oc = genOutCode(p2, clip_tl, clip_br);

            // If both outcodes are 0, then we can trivially accept this edge
            if(p1_oc == 0 && p2_oc == 0)
            {
                clipped_poly.push_back(p2);
                continue;
            }

            // Calculate the projection of p2 into the clipping region.  This is
            // used several places below
            vec2 p2_projected = projectToClipRegion(p2, clip_tl, clip_br);

            // If the outcodes share a bit, then this edge lies fully outside
            // the clipping region
            if( (p1_oc & p2_oc) != 0)
            {
                // Insert an edge into the clipped polygon from the projection
                // of p1 into the clipping rect to the projection of p2 into the
                // clipping rect.
                clipped_poly.push_back(p2_projected);
                continue;
            }

            // Decompose the current edge into parametric form
            vec2 v = p2 - p1;
            vec2 p = p1;

            // Solve for parameter values for top, left, bottom, and right
            // intersections
            double t_t = (clip_tl.y() - p.y()) / v.y();
            double t_l = (clip_tl.x() - p.x()) / v.x();
            double t_b = (clip_br.y() - p.y()) / v.y();
            double t_r = (clip_br.x() - p.x()) / v.x();

            // cout << t_t << " " << t_l << " " << t_b << " " << t_r << endl;

            vec2 isection_t = p + t_t * v;
            vec2 isection_l = p + t_l * v;
            vec2 isection_b = p + t_b * v;
            vec2 isection_r = p + t_r * v;
            
            // cout << isection_t << " " << isection_l << " " << isection_b << " " << isection_r << endl;

            isection_t.y(clip_tl.y());
            isection_l.x(clip_tl.x());
            isection_b.y(clip_br.y());
            isection_r.x(clip_br.x());

            // cout << isection_t << " " << isection_l << " " << isection_b << " " << isection_r << endl;

            // If the outcodes do not share a bit, but the edge does not hit the
            // clip rectangle (determined by seeing if the y-vals of the
            // intersection with the left and right clip edges are outside of
            // the top and bottom clip edges), then we need to put a vertex at
            // the corner the edge passes closest to, and a vertex at the
            // projection of p2 into the clip region.

            int isection_t_oc = genOutCode(isection_t, clip_tl, clip_br);
            int isection_l_oc = genOutCode(isection_l, clip_tl, clip_br);
            int isection_b_oc = genOutCode(isection_b, clip_tl, clip_br);
            int isection_r_oc = genOutCode(isection_r, clip_tl, clip_br);

            // cout << isection_t_oc << " " << isection_l_oc << " " << isection_b_oc << " " << isection_r_oc << endl;
            
            // Check if all intersections with the clipping lines are outside of
            // the clipping region.
            if(   isection_l_oc != 0 && isection_r_oc != 0
               && isection_l_oc == isection_r_oc
               && isection_t_oc != 0 && isection_b_oc != 0
               && isection_t_oc == isection_b_oc)
            {
                // If so, place a point at the corner the line passes nearest
                // to, if it won't replicate a point
 

                if(isection_l_oc == oc_t)
                {
                    if(isection_t_oc == oc_l)
                        clipped_poly.push_back(clip_tl);
                    else
                        clipped_poly.push_back(vec2(clip_br.x(), clip_tl.y()));
                }
                else
                {
                    if(isection_t_oc == oc_l)
                        clipped_poly.push_back(vec2(clip_tl.x(), clip_br.y()));
                    else
                        clipped_poly.push_back(clip_br);
                }

                // And a point at the projection of p2 into the clip rectangle,
                // if that won't replicate a point
                
                clipped_poly.push_back(p2_projected);
            }
            else
            {
                // If we're not avoiding the clip region, then we must be
                // drawing a line.  This will involve putting a point where the
                // edge enters the clip rectangle, a point where the edge leaves
                // the rectangle, and a point at the projection of p2 into the
                // rectangle
                
                // We can get the first two points by looking looking at the
                // clip-plane intersection outcodes for the two that are zero.
                // (Due to fp-math, there is the possibility of zero to all
                // four having outcode zero.  We can resolve this by picking the
                // points that give maximum difference.

                // It may be necessary to alter the scan-conversion below to
                // eliminate sparklies if one of these points is inside the
                // clipping region and the other is outside.

                // Do the outcode checking
                map<double, vec2> params_points;

                if(isection_t_oc == 0)
                    params_points[t_t] = isection_t;

                if(isection_l_oc == 0)
                    params_points[t_l] = isection_l;

                if(isection_b_oc == 0)
                    params_points[t_b] = isection_b;

                if(isection_r_oc == 0)
                    params_points[t_r] = isection_r;

                if(params_points.size() != 2)
                {
                    cout << "Danger, Will Robinson! I haven't put in code to "
                         << "handle all the dangers of not treating "
                         << "floating-point numbers like real numbers.  As a "
                         << "temporary workaround, try not making your polygon "
                         << "edges lie so close to the diagonal of my bounding "
                         << "box.  " << params_points.size() << endl;
                }
                
                // Only one of these two conditionals can be true -- if both
                // were, we would have trivially accepted this edge much earlier

                if(p1_oc != 0)
                    clipped_poly.push_back(params_points.begin()->second);
                
                if(p2_oc != 0)
                    clipped_poly.push_back(params_points.rbegin()->second);

                clipped_poly.push_back(p2_projected);
            }
            
        }

        // Step around the clipped polygon and remove extraneous vertices
        

        // Build the global edge table
        vector<list<edgedesc> > get(window_height);

        // Iterate over all edges of the polygon
        p1_it = clipped_poly.begin();
        p2_it = clipped_poly.begin();
        p2_it++;
        for(; p1_it != clipped_poly.end(); p1_it++, p2_it++)
        {
            // cout << *p1_it << " -> " << *p2_it << endl;

            if(p2_it == clipped_poly.end())
                p2_it = clipped_poly.begin();
            
            // Recategorize the points as upper and lower
            vec2 upper = (p1_it->y() > p2_it->y()) ? *p1_it : *p2_it;
            vec2 lower = (p1_it->y() > p2_it->y()) ? *p2_it : *p1_it;

            // Pull out integer coordinates
            int lowerx = lower.x();
            int lowery = lower.y();
            int upperx = upper.x();
            int uppery = upper.y();
            
            // If this edge is horizontal in pixel coordinates, ignore it
            if(lowery == uppery)
            {
                continue;
            }

            // Get the x-coordinate of the lower point
            int slopex = upperx - lowerx;
            int slopey = uppery - lowery;

            // Pack up this edge
            edgedesc thisedge;
            thisedge.ymax = uppery;
            thisedge.xcur = lowerx;
            thisedge.xcur_frac = 0;
            thisedge.slope_x = slopex;
            thisedge.slope_y = slopey;

            //cout << "get entry lowery: " << lowery << " uppery: " << uppery << endl;

            // Put it in the global edge table
            get[lowery].push_back(thisedge);
            get[lowery].sort();
        }

        // Scan-convert from the global edge table
        list<edgedesc> aet;
        vector<list<edgedesc> >::iterator get_it = get.begin();
        int curPixY = 0;
        for(; get_it != get.end(); get_it++, curPixY++)
        {
            // Copy edges that start here to the active edge table
            aet.merge(*get_it);
                    
            // Remove edges that end here
            edge_y_selector selector(curPixY);
            aet.remove_if(selector);
            
            aet.sort();

            // Debugging check -- ensure that the aet always has an even
            // number of edges
            if(aet.size() % 2 != 0)
            {
                cout << "ERROR! aet # edges = "
                     << aet.size() << " at y = " << curPixY
                     << endl;
            }

            // If the active edge table is non-empty
            if(aet.size() == 0)
                continue;

            // Take pairs of elements from the active edge table
            list<edgedesc>::iterator aet_it = aet.begin();
            list<edgedesc>::iterator lag_it;
            int element_num = 0;
            for(; aet_it != aet.end(); aet_it++, element_num++)
            {
                if(element_num % 2 == 0)
                    lag_it = aet_it;
                else
                    setSpan(lag_it->xcur, aet_it->xcur, curPixY, *color_it);
            }

            // Adjust edges in the aet for the new y value
            aet_it = aet.begin();
            for(; aet_it != aet.end(); aet_it++)
            {
                aet_it->xcur_frac += aet_it->slope_x;
                aet_it->xcur += aet_it->xcur_frac / aet_it->slope_y;
                aet_it->xcur_frac %= aet_it->slope_y;
            }
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

    // If not in clip mode, add edges to polygons
    if(!clip_mode)
    {
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

                polygon_colors.push_back(color::random());
                                    
                cout << "Polygon closed: " << position  << endl;
            }
            else
            {
                cout << "Warning -- can't close polygon: " << position << endl;
            }
        
        }
    }
   
    glutPostRedisplay();
}

void mouseDragCallback(int x, int y)
{
    // Turn window coordinates into canvas coordinates by inverting y
    y = window_height - y - 1;
    
    vec2 position(x, y);
    
    // If in clip mode, adjust the clip rectangle
    if(clip_mode)
    {
        if(!defining_rect)
        {
            defining_rect = true;
            mouse_start = position;
        }
        mouse_move = position;
    
        double top = max(mouse_start.y(), mouse_move.y());
        double left = min(mouse_start.x(), mouse_move.x());
        double bottom = min(mouse_start.y(), mouse_move.y());
        double right = max(mouse_start.x(), mouse_move.x());

        clip_tl.x(left);
        clip_tl.y(top);
    
        clip_br.x(right);
        clip_br.y(bottom);

        glutPostRedisplay();
    }
}

void mouseMoveCallback(int x, int y)
{
    if(clip_mode)
    {
        defining_rect = false;
    }
}

void keyboardCallback(unsigned char key, int x, int y)
{
    if(key == 'c')
        clip_mode = true;
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

    // Set up our first polygon color
    polygon_colors.push_back(color::random());

	glutMainLoop();
	return 0;
}

