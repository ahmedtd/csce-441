
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

            memcpy(target(row, col),
                   sendray(initray, 10).memptr(),
                   sizeof(float)*4);
        }
    }
}

fvec scene::sendray(const ray &root, int depth) const
{
    set<intersection> all_intersec;

    // Get all intersections along this ray
    set<renderable*>::iterator it = mRenderables.begin();
    for(; it != mRenderables.end(); it++)
    {
        set<intersection> this_intersec = (*it)->intersect(root);
        
        all_intersec.insert(this_intersec.begin(), this_intersec.end());
    }

    // Take the first intersection
    intersection zero_key;
    zero_key.paramval = 0.0;
    set<intersection>::iterator intersec = all_intersec.lower_bound(zero_key);
    
    if(intersec == all_intersec.end())
    {
        return mVoidColor;
    }
    else
    {
        map<renderable*, material*>::const_iterator mat_it = mMaterials.begin();
        mat_it = mMaterials.find(intersec->target);
        
        if(mat_it != mMaterials.end())
        {
            return (*mat_it).second->get_color(-root.slope,
                                               intersec->surfpos,
                                               intersec->surfnorm,
                                               *this,
                                               depth);
        }
        else
        {
            return fvec("1 0 0 1");
        }
                                                       
    }   
}

set<renderable*>& scene::renderables()
{
    return mRenderables;
}

const set<renderable*>& scene::renderables() const
{
    return mRenderables;
}

map<renderable*, material*>& scene::materials()
{
    return mMaterials;
}

const map<renderable*, material*>& scene::materials() const
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
