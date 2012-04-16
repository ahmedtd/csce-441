
#include "scene.hpp"

void scene::render(framebuffer &target)
{
    // Choose a viewport
    viewport &active_view(*(*mViewports.begin()));

    for(int row = 0; row < target.height(); row++)
    {
        for(int col = 0; col < target.width(); col++)
        {
            ray initray = active_view.genray(row,
                                             col,
                                             target.width(),
                                             target.height());

            set<intersection> root_intersections = cast_ray(initray);

            memcpy(target(row, col),
                   color_intersections(root_intersections, 10).memptr(),
                   sizeof(float)*4);
        }
    }
}

set<intersection> scene::cast_ray(const ray &root) const
{
    set<intersection> all_intersec;
    
    // Get all intersections along this ray
    set<renderable*>::const_iterator geom_it = mRenderables.begin();
    for(; geom_it != mRenderables.end(); geom_it++)
    {
        set<intersection> this_intersec = (*geom_it)->intersect(root);
        
        all_intersec.insert(this_intersec.begin(), this_intersec.end());
    }

    // Find the first intersection, and all intersections beyond it within an
    // epsilon
    intersection trial_key;
    set<intersection>::const_iterator intersec_lb, intersec_ub;
    
    trial_key.paramval = 0.0;
    intersec_lb = all_intersec.lower_bound(trial_key);
    
    trial_key.paramval = intersec_lb->paramval + 0.0001;
    intersec_ub = all_intersec.upper_bound(trial_key);

    if(intersec_lb == all_intersec.end())
    {
        return (set<intersection>());
    }
    else
    {
        return (set<intersection>(intersec_lb, intersec_ub));
    }
}

fvec scene::color_intersections(const set<intersection> &intersect_set,
                                int depth) const
{
    // If there are no intersections, return the void color
    if(intersect_set.size() == 0)
    {
        return mVoidColor;
    }

    fvec pixcolor;
    pixcolor.zeros(4);
    
    // Average the colors across all the intersections
    set<intersection>::const_iterator intersect_it = intersect_set.begin();
    for(; intersect_it != intersect_set.end(); intersect_it++)
    {
        // Fetch the material in use by the renderable at the intersection
        map<renderable const*, material*>::const_iterator mat_it;
        mat_it = mMaterials.find(intersect_it->target);

        // If a material exists for the object, use it
        if(mat_it != mMaterials.end())
        {
            pixcolor += (*mat_it).second->get_color(
                -(intersect_it->generating_ray.slope),
                intersect_it->surfpos,
                intersect_it->surfnorm,
                *this,
                depth
            );
        }
        else
        {
            // If the renderable doesn't have an assigned material, give it
            // red
            pixcolor += fvec("1 0 0 1");
        }
    }
    pixcolor /= (double) intersect_set.size();

    return pixcolor;
}

set<renderable*>& scene::renderables()
{
    return mRenderables;
}

const set<renderable*>& scene::renderables() const
{
    return mRenderables;
}

map<renderable const*, material*>& scene::materials()
{
    return mMaterials;
}

const map<renderable const*, material*>& scene::materials() const
{
    return mMaterials;
}

set<light*>& scene::lights()
{
    return mLights;
}

const set<light*>& scene::lights() const
{
    return mLights;
}

set<viewport*>& scene::viewports()
{
    return mViewports;
}

const set<viewport*>& scene::viewports() const
{
    return mViewports;
}

fvec& scene::voidcolor()
{
    return mVoidColor;
}

const fvec& scene::voidcolor() const
{
    return mVoidColor;
}
