
#ifndef DIRLIGHT
#define DIRLIGHT

#include "light.hpp"

class dirlight : public light
{
public:
    dirlight(const vec &cast_direction, const fvec &intensity);

    virtual fvec intensity(const vec &surfpos) const;
    virtual vec dirtolight(const vec &surfpos) const;
    virtual double dist_to_light(const vec &surfpos) const;

private:
    fvec m_intensity;
    vec  m_cast_direction;
};

#endif
