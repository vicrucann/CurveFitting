#ifndef PATHFITTER_H
#define PATHFITTER_H

#include <osg/Array>

template <typename Container, typename Point2, typename Real>
class PathFitter
{
public:
    typedef std::vector<Point2> Curve; /*!< Curve is a vector of 2d points */

    PathFitter();
    void init(const Container& path);
    virtual Container* fit(Real error = 2.5) = 0;

    Point2 curveAt(unsigned int index) const;
    size_t getLength() const;

protected:
    /* Fit a Bezier curve to a (sub-)set of digitized points */
    void fitCubic(Container* curves, Real error, int first, int last, const Point2& tan1, const Point2& tan2);
    virtual Real getDistance(const Point2& p1, const Point2& p2) = 0;
    virtual Point2 getNANPoint() const = 0;

private:
    void addCurve(Container *curvesSet, const Curve& curve);

    Curve generateBezier(int first, int last, const std::vector<Real>& uPrime, const Point2& tan1, const Point2& tan2);

    bool reparameterize(int first, int last, std::vector<Real> &u, const Curve& curve);
    Real findRoot(const Curve& curve, const Point2& point, Real u);
    Point2 evaluate(int degree, const Curve& curve, Real t);
    std::vector<Real> chordLengthParameterize(int first, int last);
    Real findMaxError(int first, int last, const Curve& curve, const std::vector<Real> &u, int& index);

private:
    Curve m_dataPoints;
};

#endif // PATHFITTER_H
