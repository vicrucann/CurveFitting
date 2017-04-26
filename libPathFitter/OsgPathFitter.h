#ifndef OSGPATHFITTER_H
#define OSGPATHFITTER_H

#include "PathFitter.h"

#include <osg/ref_ptr>
#include <osg/Array>

/*! \class OsgPathFitter
 * \brief OpenSceneGraph-based class to perform path fitting.
 *
 * The container is of type osg::Vec3Array which can be directly used for drawing procedure by
 * attaching it to the corresponding geometry.
 *
 * Each point is represented by osg::Vec3f - a 3D point; all the points lie within the same plane,
 * e.g., they have format (x, 0, z). OpenSceneGraph's implementation of osg::Vec3f include all the
 * linear algebra operators that are used by PathFitter, e.g., inner product as operator*(), cross
 * product as operator^(), etc.
*/

template <typename Container, typename Point2, typename Real>
class OsgPathFitter : public PathFitter<osg::Vec3Array, osg::Vec3f, float>
{
public:
    /*! Constructor */
    OsgPathFitter();

    /*! \return Container type that can be easily used within OpenSceneGraph. */
    virtual Container* fit(Real error = 2.5);

protected:
    /*! \return Euclidian distance between two points. */
    virtual Real getDistance(const Point2& p1, const Point2& p2);

    /*! \return NAN point type. */
    virtual Point2 getNANPoint() const;
};

#endif // OSGPATHFITTER_H
