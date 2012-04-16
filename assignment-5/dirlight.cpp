
#include <armadillo>
using arma::norm;

#include "dirlight.hpp"

dirlight::dirlight(const vec &cast_direction, const fvec &intensity)
:
    m_intensity(intensity),
    m_cast_direction(cast_direction)
{
    m_cast_direction /= norm(m_cast_direction, 2);
}

fvec dirlight::intensity(const vec &surfpos, const vec &surfnorm) const
{
    return m_intensity;
}

vec dirlight::dirtolight(const vec &surfpos) const
{
    return -m_cast_direction;
}
