
#include "dirlight.hpp"

fvec dirlight::intensity(const vec &surfpos) const
{
    return fvec("1 1 1");
}

vec dirlight::dirtolight(const vec &surfpos) const
{
    vec direction("0 1 1");
    return direction / norm(direction, 2);
}
