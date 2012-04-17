
#include <cmath>
using std::pow;

#include "pointlight.hpp"

pointlight::pointlight(const fvec &color, const vec &position)
:
    mColor(color),
    mPosition(position)
{
    
}

fvec pointlight::intensity(const vec &surfpos) const
{
    double dist = dist_to_light(surfpos);

    if(dist < 15.0)
    {
        return mColor;
    }
    else
    {
        return 225.0 * mColor / pow(dist,2);
    }
}

vec pointlight::dirtolight(const vec &surfpos) const
{
    vec dir = (mPosition - surfpos);
    return dir / norm(dir,2);
}

double pointlight::dist_to_light(const vec &surfpos) const
{
    return norm(mPosition - surfpos, 2);
}

fvec pointlight::color() const
{
    return mColor;
}

vec pointlight::position() const
{
    return mPosition;
}
