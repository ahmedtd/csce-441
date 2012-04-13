
#ifndef FRAMEBUFFER
#define FRAMEBUFFER

#include <vector>
using std::vector;

#include <armadillo>
using arma::fvec;

class framebuffer
{
public:
    framebuffer(int width, int height);

    void reshape(int width, int height);

    // Return an auxiliary memory vector bound to the memory in mBuffer at the
    // correct location.
    float* operator()(int row, int col);

    int width() const;
    int height() const;

    float* pointer();

private:
    int mWidth;
    int mHeight;

    vector<float> mBuffer;
};

#endif
