
#include <sstream>
using std::istringstream;
#include <stdexcept>
using std::runtime_error;
#include <string>
using std::string;
#include <sstream>
using std::stringstream;
#include <algorithm>
using std::min;
using std::max;
using std::sort;
using std::reverse;
using std::remove_if;
#include <iostream>
using std::cout;
using std::endl;
#include <list>
using std::list;

#include "Eigen/Dense"
using Eigen::Vector3d;

#include "model.hpp"


Vector3f lowerVector(const Vector3f &lop, const Vector3f &rop)
{
    if(lop(1) < rop(1))
        return lop;
    else
        return rop;
}

Vector3f upperVector(const Vector3f &lop, const Vector3f &rop)
{
    if(lop(1) > rop(1))
        return lop;
    else
        return rop;
}

bool edgeflipped(const Vector3f &lop, const Vector3f &rop)
{
    if(lop(1) < rop(1))
        return false;
    else
        return true;
}

Vector3f amblight_color(0.5, 0.5, 0.5);

Vector3f dirlight_color(1.0, 1.0, 1.0);
Vector3f dirlight_dir(-1.0, -1.0, 1.0);

Vector3f mat_ambient(0.1, 0.0, 0.0);
Vector3f mat_diffuse(0.7, 0.0, 0.0);
Vector3f mat_specular(0.5, 0.5, 0.5);
double   mat_exponent(5.0);

Vector3f view_dir(0.0, 0.0, -1.0);

model::model(istream &source)
:
    mRenderMode(flat)
{
    dirlight_dir.normalize();

    int numVertices(0);
    int numFaces(0);

    string line;
    string tok;
    Vector3f zero_vector(0.0, 0.0, 0.0);
    Vector3f temp_vector1(0.0, 0.0, 0.0);
    Vector3f temp_vector2(0.0, 0.0, 0.0);
    Vector3f temp_vector3(0.0, 0.0, 0.0);
    Vector3i temp_face(0,0,0);
    while(getline(source, line))
    {
        stringstream linestream(line);
        linestream >> tok;

        if(tok == "v")
        {
            numVertices++;
            
            // Load coordinates of the vertex
            for(int count = 0; count < 3; count++)
            {
                linestream >> temp_vector1(count);
            }
            mVertices.push_back(temp_vector1);
            mVertexNormals.push_back(zero_vector);
        }
        else if(tok == "f")
        {
            numFaces++;
            
            // Load indices of the vertex, normalizing for a zero-based list
            for(int count = 0; count < 3; count++)
            {
                linestream >> temp_face(count);
                temp_face(count)--;
            }
            mFaces.push_back(temp_face);
            
            // Calculate the normal of the face
            temp_vector1 = (mVertices[temp_face(2)]-mVertices[temp_face(0)]);
            temp_vector2 = (mVertices[temp_face(1)]-mVertices[temp_face(0)]);
            temp_vector3 = temp_vector1.cross(temp_vector2);
            mFaceNormals.push_back(temp_vector3);
            
            for(int count = 0; count < 3; count++)
            {
                mVertexNormals[temp_face(count)] += temp_vector3;
            }
        }
    }

    // Normalize all vertex and face normals
    for(int count = 0; count < mVertexNormals.size(); count++)
    {
        mVertexNormals[count].normalize();
    }
    for(int count = 0; count < mFaceNormals.size(); count++)
    {
        mFaceNormals[count].normalize();
    }

    cout << "Read " << numVertices << "v " << numFaces << "f " << endl;
}

void model::render(vector<Vector4f> &framebuffer, vector<float> &auxbuffer)
{
    for(int count = 0; count < mFaces.size(); count++)
    {
        renderFace(count, framebuffer, auxbuffer);
    }

    // renderFace(6, framebuffer, auxbuffer);
}

void model::renderFace(int faceindex,
                       vector<Vector4f> &framebuffer,
                       vector<float> &auxbuffer)
{
    //cout << "RenderFace" << endl;

    // Cull backfaces because they kill the scan-converter
    if(mFaceNormals[faceindex].dot(view_dir) <= 0.0f)
        return;

    // Scale up the vectors so that they live in the scale of the screen
    // dimensions
    Vector3f offset(200.0, 200.0, 200.0);

    Vector3f v0 = mVertices[mFaces[faceindex](0)];
    Vector3f v1 = mVertices[mFaces[faceindex](1)];
    Vector3f v2 = mVertices[mFaces[faceindex](2)];
    
    Vector3f s0 = v0 * 200.0 + offset;
    Vector3f s1 = v1 * 200.0 + offset;
    Vector3f s2 = v2 * 200.0 + offset;

    // cout << s0 << endl;
    // cout << s1 << endl;
    // cout << s2 << endl;
    // cout << endl;

    Vector3f n0 = mVertexNormals[mFaces[faceindex](0)];
    Vector3f n1 = mVertexNormals[mFaces[faceindex](1)];
    Vector3f n2 = mVertexNormals[mFaces[faceindex](2)];


    Vector3f edge0_min = lowerVector(s0, s1);
    Vector3f edge0_max = upperVector(s0, s1);

    Vector3f edge1_min = lowerVector(s1, s2);
    Vector3f edge1_max = upperVector(s1, s2);
    
    Vector3f edge2_min = lowerVector(s2, s0);
    Vector3f edge2_max = upperVector(s2, s0);

    
    // store the number of steps for each edge
    int steps0 = (int)floor(edge0_max(1)) - (int)floor(edge0_min(1));
    int steps1 = (int)floor(edge1_max(1)) - (int)floor(edge1_min(1));
    int steps2 = (int)floor(edge2_max(1)) - (int)floor(edge2_min(1));

    // compile the edgedesc structures
    edgedesc edge0;
    edge0.ycur = (int) floor(edge0_min(1));
    edge0.ymax = (int) floor(edge0_max(1));
    edge0.position = edge0_min;
    edge0.positioninc = (edge0_max - edge0_min) / ((float) steps0);
    
    edgedesc edge1;
    edge1.ycur = (int) floor(edge1_min(1));
    edge1.ymax = (int) floor(edge1_max(1)); 
    edge1.position = edge1_min;
    edge1.positioninc = (edge1_max - edge1_min) / ((float) steps1);
   
    edgedesc edge2;
    edge2.ycur = (int) floor(edge2_min(1));
    edge2.ymax = (int) floor(edge2_max(1));
    edge2.position = edge2_min;
    edge2.positioninc = (edge2_max - edge2_min) / ((float) steps2);
    
    // Decide what normals to use, based on the rendering method in use
    // To simplify the code, and because this isn't going to be efficient no
    // in any case, I perform per-pixel calculations for all three lighting
    // methods, with the choice of normal vectors at each vertex determining the
    // shading method:
    // per-face: I have the normals at each vertex of the face be the face
    //     normal.
    // per-vertex: I have the normals at each vertex of the face be the area-
    //     weighted vertex normals, and the scan-converter does not normalize
    //     the interpolated vectors.
    // per-pixel: As above, but the scan-converter does normalize the
    //     interpolated vectors.

    if(mRenderMode == flat)
    {
        edge0.data = mFaceNormals[faceindex];
        edge1.data = mFaceNormals[faceindex];
        edge2.data = mFaceNormals[faceindex];
        
        edge0.datainc = Vector3f(0.0, 0.0, 0.0);
        edge1.datainc = Vector3f(0.0, 0.0, 0.0);
        edge2.datainc = Vector3f(0.0, 0.0, 0.0);
    }
    else
    {
        Vector3f ld0 = edgeflipped(s0, s1) ? n1 : n0;
        Vector3f ld1 = edgeflipped(s1, s2) ? n2 : n1;
        Vector3f ld2 = edgeflipped(s2, s0) ? n0 : n2;

        Vector3f ud0 = edgeflipped(s0, s1) ? n0 : n1;
        Vector3f ud1 = edgeflipped(s1, s2) ? n1 : n2;
        Vector3f ud2 = edgeflipped(s2, s0) ? n2 : n0;

        edge0.data = ld0;
        edge1.data = ld1;
        edge2.data = ld2;

        edge0.datainc = (ud0 - ld0) / ((float) steps0);
        edge1.datainc = (ud1 - ld1) / ((float) steps1);
        edge2.datainc = (ud2 - ld2) / ((float) steps2);
    }

    // if the edge is not horizontal, put it into the edgelist
    vector<edgedesc> edgelist;
    if(steps0 != 0)
        edgelist.push_back(edge0);
    if(steps1 != 0)
        edgelist.push_back(edge1);
    if(steps2 != 0)
        edgelist.push_back(edge2);

    // Sort the edgelist
    sort(edgelist.begin(), edgelist.end());
    //reverse(edgelist.begin(), edgelist.end());

    //cout << edgelist[0].ycur << ", " << edgelist[0].ymax << endl;
    //for(int count = 0; count < edgelist.size(); count++)
    /// {
    //     cout << edgelist[count].
    // }

    renderTri(mRenderMode != pervertex, edgelist, framebuffer, auxbuffer);
}

void model::renderTri(bool normalize_data,
                      vector<edgedesc> &edgelist,
                      vector<Vector4f> &framebuffer,
                      vector<float> &auxbuffer)
{
    //cout << "RenderTri" << endl;

    // for(int count = 0; count < edgelist.size(); count++)
    // {
    //     cout << edgelist[count].ycur << " " << edgelist[count].ymax << endl;
    //     cout << edgelist[count].position << endl;
    // }

    list<edgedesc> ael;

    // Scan from bottom to top
    for(int pixrow = 0; pixrow < 400; pixrow++)
    {
        // Remove edges ending here
        edge_y_selector sel(pixrow);
        ael.remove_if(sel);

        // Add edges starting here
        for(int count = 0; count < edgelist.size(); count++)
        {
            if(edgelist[count].ycur == pixrow)
               ael.push_back(edgelist[count]);
        }

        // Sort the ael
        edge_x_compare comp;
        ael.sort(comp);

        //cout << ael.size() << endl;

        // We're done if we've incremented both the left and right edges
        if(ael.size() == 0)
            continue;

        // Determine the position and data at the left and right edges of the
        // current span
        Vector3d curpos = ael.front().position.cast<double>();
        Vector3d posinc = ael.back().position.cast<double>() - curpos;
        posinc = posinc + Vector3d(1.0, 1.0, 1.0) * 1.5;

        //cout << "row: " << pixrow << " curx: " << curpos(0) << " cury: " << curpos(1) << endl;
        
        int numsteps = (int) floor(posinc(0));

        posinc /= (double) numsteps;

        Vector3f curdata = ael.front().data;
        Vector3f datainc = ael.back().data - curdata;
        datainc /= (float) numsteps;

        // Scan from left to right
        int lastxpos = (int) floor(curpos(0));
        for(int count = 0; count < numsteps; count++)
        {
            // Check if current pixel is depthmasked
            if(auxbuffer[400*pixrow + (int)floor(curpos(0))] > curpos(2))
            {
                // Normalize data if required
                Vector3f normed_data = curdata;
                if(mRenderMode == perpixel)
                {
                    //cout << "normalizing" << endl;
                    normed_data.normalize();
                }
                // Perform lighting calculation
                Vector3f color(0.0, 0.0, 0.0);

                // Ambient lighting
                color.array() = amblight_color.array() * mat_ambient.array();

                // Diffuse lighting
                float diffuse_strength = -(dirlight_dir.dot(normed_data));
                //cout << diffuse_strength << endl;
                if(diffuse_strength >= 0.0f)
                {
                    color.array() += dirlight_color.array()
                        * mat_diffuse.array()
                        * diffuse_strength;
                }

                // Specular lighting
                if(diffuse_strength >= 0.0f)
                {
                    Vector3f reflected;
                    reflected = normed_data * -2.0f * (dirlight_dir.dot(normed_data))
                        + dirlight_dir;
                    //reflected.normalize();
                    float specular_strength = view_dir.dot(reflected);
                    //cout << specular_strength << endl;
                    specular_strength = max(0.0f, specular_strength);
                    specular_strength = pow(specular_strength, mat_exponent);
                    
                    color.array() += dirlight_color.array()
                        * mat_specular.array()
                        * specular_strength;
                }

                Vector4f fullcolor;
                fullcolor(0) = color(0);
                fullcolor(1) = color(1);
                fullcolor(2) = color(2);
                fullcolor(3) = 1.0f;
                                
                // Write color
                framebuffer[400*pixrow + (int)floor(curpos(0))] = fullcolor;
               
                 // Write depth
                auxbuffer[400*pixrow + (int)floor(curpos(0))] = curpos(2);
 
                // Correction for floating point inaccuracy
                int curxpos = (int) floor(curpos(0));
                if(curxpos - lastxpos > 1)
                {
                    framebuffer[400*pixrow + (int)floor(curpos(0)) - 1] = fullcolor;
                    auxbuffer[400*pixrow + (int)floor(curpos(0)) - 1] = curpos(2);
                }             
                
                lastxpos = curxpos;
            }

            // Increment data and position
            curpos += posinc;
            curdata += datainc;
        }
        
        // Update the position and data fields
        ael.front().position += ael.front().positioninc;
        ael.back().position += ael.back().positioninc;

        ael.front().data += ael.front().datainc;
        ael.back().data += ael.back().datainc;
    }

    // cout << endl;

    //cout << "RenderTri completed" << endl;
}


