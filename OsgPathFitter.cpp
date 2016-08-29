#include "OsgPathFitter.h"

#include "math.h"

template <typename Container, typename Point2, typename Real>
OsgPathFitter<Container, Point2, Real>::OsgPathFitter()
    : PathFitter<osg::Vec3Array, osg::Vec3f, float>()
{
}

template <typename Container, typename Point2, typename Real>
Container *OsgPathFitter<Container, Point2, Real>::fit(Real error)
{
    osg::ref_ptr<Container> curvesSet = new Container;
    auto length = this->getLength();

    if (length>0){
        if (length > 1){
            auto tan1 = this->curveAt(1) - this->curveAt(0);
            auto tan2 = this->curveAt(length-2) - this->curveAt(length-1);
            tan1.normalize();
            tan2.normalize();
            this->fitCubic(curvesSet.get(), error, 0, length-1,
                           tan1, // left tangent
                           tan2); // right tangent
        }
    }
    return curvesSet.release();
}

template <typename Container, typename Point2, typename Real>
Real OsgPathFitter<Container, Point2, Real>::getDistance(const Point2 &p1, const Point2 &p2)
{
    auto x = p1.x() - p2.x();
    auto z = p1.z() - p2.z();
    return sqrt(x*x + z*z);
}

template <typename Container, typename Point2, typename Real>
Point2 OsgPathFitter<Container, Point2, Real>::getNANPoint() const
{
    return {NAN, NAN, NAN};
}

template class OsgPathFitter<osg::Vec3Array, osg::Vec3f, float>;
