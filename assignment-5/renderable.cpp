
#include "renderable.hpp"

bool intersection::operator<(const intersection &rop) const
{
    return paramval < rop.paramval;
}

