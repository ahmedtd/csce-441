
#ifndef MATERIAL_PHONG
#define MATERIAL_PHONG

#include <armadillo>
using arma::vec;
using arma::fvec;

#include "material.hpp"

class material_phong : public material
{
public:
    material_phong(const fvec &color,
                   const fvec &spec_color,
                   double spec_exp,
                   double reflect_intns,
                   double refract_intns,
                   double snell_coeff);
    
    virtual fvec get_color(const vec &viewdir,
                           const vec &surfpos,
                           const vec &surfnorm,
                           const scene &inscene,
                           int depth) const;

private:
    fvec m_color;
    fvec m_spec_color;
    double m_spec_exp;
    double m_reflect_intns;
    double m_refract_intns;
    double m_snell_coeff;
};

#endif
