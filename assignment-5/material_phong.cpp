
#include <iostream>
using std::cout;
using std::endl;

#include <armadillo>
using arma::zeros;

#include "material_phong.hpp"

material_phong::material_phong(const fvec &color,
                               const fvec &spec_color,
                               double spec_exp,
                               double reflect_intns,
                               double refract_intns,
                               double snell_coeff)
 :
    m_color(color),
    m_spec_color(spec_color),
    m_spec_exp(spec_exp),
    m_reflect_intns(reflect_intns),
    m_refract_intns(refract_intns),
    m_snell_coeff(snell_coeff)
{
    
}

fvec material_phong::get_color(const vec &viewdir,
                               const vec &surfpos,
                               const vec &surfnorm,
                               const scene &inscene,
                               int depth) const
{
    fvec total_color;
    total_color.zeros(4);
    total_color(3) = 1.0;

    for(int channel = 0; channel < 3; channel++)
    {
        total_color(channel) += m_color(channel)*0.2;
    }

    
    set<light*>::iterator light_it = inscene.lights().begin();
    for(; light_it != inscene.lights().end(); light_it++)
    {
        fvec lightint = (*light_it)->intensity(surfpos, surfnorm);
        vec  lightdir = (*light_it)->dirtolight(surfpos);

        // Simple shadow calculation
        ray ray_to_light(surfpos + surfnorm * 0.005, lightdir);
        if(inscene.cast_ray(ray_to_light).size() != 0)
        {
            continue;
        }

        double dot_ln = dot(lightdir, surfnorm);

        vec reflected = 2*dot_ln*surfnorm - lightdir;

        double dot_rv = dot(reflected, viewdir);

        // If the light is in front of the surface, apply lighting
        if(dot_ln >= 0.0)
        {
            for(int channel = 0; channel < 3; channel++)
            {
                // Add in diffuse term
                total_color(channel)
                    += m_color(channel)*dot_ln*lightint(channel);

                // Add in specular term
                if(dot_rv >= 0.0)
                {
                    total_color(channel)
                        += m_spec_color(channel)
                        * pow(dot_rv, m_spec_exp)
                        * lightint(channel);
                }
            }
        }
    }

    // If we are still above the depth limit, recur for reflection and
    // refraction
    if(depth > 0)
    {
        // Outset the reflected ray along the normal slightly
        ray reflected(surfpos + 0.005*surfnorm,
                      2*(dot(viewdir, surfnorm))*surfnorm - viewdir);
        
        // Correct for wonky normals
        if(dot(surfnorm, viewdir) <= 0.00000000005)
        {
            reflected.slope = -viewdir;
        }

        set<intersection> reflect_intersections = inscene.cast_ray(reflected);

        total_color += m_reflect_intns
            * inscene.color_intersections(reflect_intersections, depth-1);
    }

    return total_color;
}
