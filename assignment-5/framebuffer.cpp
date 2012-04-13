
#include "framebuffer.hpp"

framebuffer::framebuffer(int width, int height)
 :
    mWidth(width),
    mHeight(height),
    mBuffer(4 * width * height, 1.0f)
{
    
}

void framebuffer::reshape(int width, int height)
{
    mWidth = width;
    mHeight = height;
    
    mBuffer.resize(4 * width * height);
}

float* framebuffer::operator()(int row, int col)
{
    return &mBuffer[row*4*mWidth+col*4];
}

int framebuffer::width() const
{
    return mWidth;
}

int framebuffer::height() const
{
    return mHeight;
}

float* framebuffer::pointer()
{
    return &(mBuffer[0]);
}
