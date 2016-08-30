#ifndef OSGPATHFITTER_H
#define OSGPATHFITTER_H

#include "PathFitter.h"

#include <osg/ref_ptr>
#include <osg/Array>

template <typename Container, typename Point2, typename Real>
class OsgPathFitter : public PathFitter<osg::Vec3Array, osg::Vec3f, float>
{
public:
    OsgPathFitter();
    virtual Container* fit(Real error = 2.5);

protected:
    virtual Real getDistance(const Point2& p1, const Point2& p2);
    virtual Point2 getNANPoint() const;
};

#endif // OSGPATHFITTER_H
