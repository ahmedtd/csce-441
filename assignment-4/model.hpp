
#ifndef MODEL
#define MODEL

#include <istream>
using std::istream;
#include <vector>
using std::vector;

#include "Eigen/Dense"
using Eigen::Vector4f;
using Eigen::Vector3f;
using Eigen::Vector3i;

#include "eigendef.hpp"

enum rendermode
{
    flat,
    pervertex,
    perpixel
};

Vector3f lowerVector(const Vector3f &lop, const Vector3f &rop);

Vector3f upperVector(const Vector3f &lop, const Vector3f &rop);

struct edgedesc
{
    int ycur;
    int ymax;

    Vector3f position;
    Vector3f positioninc;

    Vector3f data;
    Vector3f datainc;

    bool operator<(const edgedesc &other) const
    {
        if(ycur < other.ycur)
        {
            return true;
        }
        else if(ycur > other.ycur)
        {
            return false;
        }
        else
        {
            //cout << "second " << position(0) << " " << other.position(0) << endl;
            if((int)floor(position(0)) < (int)floor(other.position(0)))
            {
                //cout << "taken" << endl;
                return true;
            }
            else if((int)floor(position(0)) > (int)floor(other.position(0)))
            {
                return false;
            }
            else
            {
                return positioninc(0) < other.positioninc(0);
            }
        }
    }
};

class edge_x_compare
{
public:
    bool operator()(const edgedesc &edge1, const edgedesc &edge2)
    {
        return edge1.position(0) < edge2.position(0);
    }
};

class edge_y_selector
{
public:
    edge_y_selector(int y)
    :
    mY(y)
    {
    }

    bool operator()(const edgedesc &edge)
    {
        return edge.ymax == mY;
    }

    int mY;
};

class model
{
public:
    model(istream &source);

    void render(vector<Vector4f> &framebuffer, vector<float> &auxbuffer);
private:
    void renderFace(int faceindex,
                    vector<Vector4f> &framebuffer,
                    vector<float> &auxbuffer);

    void renderTri(bool normalize_data,
                   vector<edgedesc> &edgelist,
                   vector<Vector4f> &framebuffer,
                   vector<float> &auxbuffer);

    vector<Vector3f> mVertices;
    vector<Vector3f> mVertexNormals;

    vector<Vector3i> mFaces;
    vector<Vector3f> mFaceNormals;

public:
    rendermode mRenderMode;
};

#endif
