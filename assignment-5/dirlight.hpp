
#ifndef DIRLIGHT
#define DIRLIGHT

#include "light.hpp"

class dirlight : public light
{
    virtual fvec intensity(const vec &surfpos) const;
    virtual vec dirtolight(const vec &surfpos) const;
};

#endif
