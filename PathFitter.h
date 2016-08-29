#ifndef PATHFITTER_H
#define PATHFITTER_H

/*! \class PathFitter
 * Is a pure virtual class which performs curve fitting based on the paper (with minor modifications):
 * An Algorithm for Automatically Fitting Digitized Curves, Philip J. Schneider
 * "Graphics Gems", Academic Press, 1990
 */

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

    /*! Fit a Bezier curve to a (sub-)set of digitized points */
    void fitCubic(Container* curves, Real error, int first, int last, const Point2& tan1, const Point2& tan2);

    /*! \return distance between two points */
    virtual Real getDistance(const Point2& p1, const Point2& p2) = 0;

    /*! \return 2D point whose values are NAN */
    virtual Point2 getNANPoint() const = 0;

private:
    /*! Push back the current bezier curve to the result set. */
    void addCurve(Container *curvesSet, const Curve& bezier);

    /*! Use least-squares method to find Bezier control points for region. */
    Curve generateBezier(int first, int last, const std::vector<Real>& uPrime, const Point2& tan1, const Point2& tan2);

    /*! Given set of points and their parameterization, try to find a better parameterization. */
    bool reparameterize(int first, int last, std::vector<Real> &u, const Curve& bezier);

    /*! Use Newton-Raphson iteration to find better root */
    Real findRoot(const Curve& bezier, const Point2& point, Real u);

    /*! Evaluate a bezier curve at a particular parameter value */
    Point2 evaluate(int degree, const Curve& bezier, Real t);

    /*! Assign parameter values to digitized points using relative distances between points. */
    std::vector<Real> chordLengthParameterize(int first, int last);

    /*! Find the maximum squared distance of digitized points to fitted curve */
    std::pair<Real,int> findMaxError(int first, int last, const Curve& bezier, const std::vector<Real> &u);

private:
    Curve m_dataPoints;
};

#endif // PATHFITTER_H
