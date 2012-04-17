
#ifndef POINTLIGHT
#define POINTLIGHT

#include "light.hpp"

class pointlight : public light
{
public:
    pointlight(const fvec &color, const vec &position);
    
    virtual fvec intensity(const vec &surfpos) const;
    virtual vec dirtolight(const vec &surfpos) const;
    virtual double dist_to_light(const vec &surfpos) const;
    
    fvec color() const;
    vec position() const;
    
private:
    fvec mColor;
    vec  mPosition;
};


#endif
