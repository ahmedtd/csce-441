
#ifndef SCENE
#define SCENE

#include <armadillo>
using arma::fvec;

#include <map>
using std::map;
#include <set>
using std::set;

class scene;

#include "renderable.hpp"
#include "light.hpp"
#include "viewport.hpp"
#include "framebuffer.hpp"
#include "material.hpp"

class scene
{
public:
    void render(framebuffer &target);

    set<renderable*>& renderables();
    const set<renderable*>& renderables() const;

    map<renderable*, material*>& materials();
    const map<renderable*, material*>& materials() const;

    set<light*>& lights();
    const set<light*>& lights() const;

    set<viewport*>& viewports();
    const set<viewport*>& viewports() const;

    fvec& voidcolor();
    const fvec& voidcolor() const;

    set<intersection> cast_ray(const ray &root) const;
    fvec color_intersections(const set<intersection> &intersec_set,
                             int depth = 10) const;

private:
    fvec mVoidColor;

    set<renderable*> mRenderables;
    map<renderable*, material*> mMaterials;
    set<light*> mLights;
    set<viewport*> mViewports; 
};

#endif
