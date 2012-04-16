
#ifndef RAY
#define RAY

#include <armadillo>
using arma::vec;

class ray
{
public:
    ray();
    ray(const vec &point_in, const vec &slope_in);

    vec evaluate(const double &paramval) const;

    vec point;
    vec slope;
};

#endif
