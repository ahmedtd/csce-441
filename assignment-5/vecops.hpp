
#ifndef VECOPS
#define VECOPS

#include <armadillo>
using std::vec;
using std::mat;

mat crosspmatrix(vec in);

mat rotmat(vec axis, double angle);

#endif
