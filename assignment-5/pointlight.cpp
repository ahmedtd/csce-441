
#include "pointlight.hpp"

pointlight::pointlight(const fvec &color, const vec &position)
:
    mColor(color),
    mPosition(position)
{
    
}

fvec pointlight::intensity(const vec &surfpos) const
{
    return fvec("1 1 1");
}

vec pointlight::dirtolight(const vec &surfpos) const
{
    vec dir = (mPosition - surfpos);
    return dir / norm(dir,2);
}

fvec pointlight::color() const
{
    return mColor;
}

vec pointlight::position() const
{
    return mPosition;
}
