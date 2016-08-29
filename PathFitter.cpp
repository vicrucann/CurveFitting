#include "PathFitter.h"

#include <math.h>
#include <algorithm>
#include <memory>

template <typename Container, typename Point2, typename Real>
PathFitter<Container, Point2, Real>::PathFitter()
    : m_dataPoints(0)
{
}

template <typename Container, typename Point2, typename Real>
void PathFitter<Container, Point2, Real>::init(const Container &path)
{
    /* copy points from path and filter out adjacent duplicates */
    Point2 prev = getNANPoint();
    for (unsigned int i=0; i<path.size(); ++i){
        auto point = path.at(i);
        if (prev.isNaN() || prev != point)
        {
            m_dataPoints.push_back(point);
            prev = point;
        }
    }
}

template <typename Container, typename Point2, typename Real>
Point2 PathFitter<Container, Point2, Real>::curveAt(unsigned int index) const
{
    if (index >= m_dataPoints.size())
        return getNANPoint();
    return m_dataPoints.at(index);
}

template <typename Container, typename Point2, typename Real>
size_t PathFitter<Container, Point2, Real>::getLength() const
{
    return m_dataPoints.size();
}

template <typename Container, typename Point2, typename Real>
void PathFitter<Container, Point2, Real>::fitCubic(Container *curvesSet, Real error, int first, int last, const Point2 &tan1, const Point2 &tan2)
{
    /* 2 points case */
    if (last-first == 1){
        auto pt1 = m_dataPoints.at(first);
        auto pt2 = m_dataPoints.at(last);
        auto dist = getDistance(pt1, pt2) / 3.f;
        auto pta = pt1 + tan1 * dist;
        auto ptb = pt2 + tan2 * dist;

        Curve curve(4);
        curve[0] = pt1;
        curve[1] = pta;
        curve[2] = ptb;
        curve[3] = pt2;

        this->addCurve(curvesSet, curve);
        return;
    }

    /* parameterize points and attempt to fit the curve */
    auto uPrime = this->chordLengthParameterize(first, last);
    auto maxError = (std::max)(error, error*error);
    int split = -1;
    bool parametersInOrder =  true;

    /* 4 iterations */
    for (int i=0; i<=4; ++i){
        Curve curve = this->generateBezier(first, last, uPrime, tan1, tan2);

        int index;
        auto merr = this->findMaxError(first, last, curve, uPrime, index);
        if (merr < error && parametersInOrder){
            this->addCurve(curvesSet, curve);
            return;
        }

        split = index;
        if (merr >= maxError)
            break;

        parametersInOrder = this->reparameterize(first, last, uPrime, curve);
        maxError = merr;
    }

    /* fitting failed */
    auto tanCenter = m_dataPoints.at(split-1) - m_dataPoints.at(split+1);
    this->fitCubic(curvesSet, error, first, split, tan1, tanCenter);
    this->fitCubic(curvesSet, error, split, last, -tanCenter, tan2 );
}

template <typename Container, typename Point2, typename Real>
void PathFitter<Container, Point2, Real>::addCurve(Container *curvesSet, const Curve &curve)
{
    curvesSet->push_back( curve[0] );
    curvesSet->push_back( curve[1] );
    curvesSet->push_back( curve[2] );
    curvesSet->push_back( curve[3] );
}

template <typename Container, typename Point2, typename Real>
typename PathFitter<Container, Point2, Real>::Curve PathFitter<Container, Point2, Real>::generateBezier(int first, int last, const std::vector<Real> &uPrime, const Point2 &tan1, const Point2 &tan2)
{
    double epsilon = 1.0e-6;
    auto pt1 = m_dataPoints[first];
    auto pt2 = m_dataPoints[last];

    // C and X matrices
    Real C[2][2];
    Real X[2];

    for (int i=0, l=last-first+1; i<l; ++i){
        auto u = uPrime.at(i);
        auto t = 1-u,
                b = 3*u*t,
                b0 = t * t * t,
                b1 = b * t,
                b2 = b * u,
                b3 = u * u * u;
        auto a1 = tan1 * b1,
                a2 = tan2 * b2,
                tmp = m_dataPoints[first+i] - (pt1 * (b0 + b1)) - (pt2 * (b2 + b3)) ;
        C[0][0] += a1 * a2;
        C[0][1] += a1 * a2;
        C[1][0] = C[0][1];
        C[1][1] += a2 * a2;
        X[0] += a1 * tmp;
        X[1] += a2 * tmp;
    }

    /* determinant of C and X */
    auto detC0C1 = C[0][0] * C[1][1] - C[1][0] * C[0][1];
    Real alpha1, alpha2;
    if (std::fabs(detC0C1) > epsilon){
        /* Kramer's rule */
        auto detC0X = C[0][0] * X[1] - C[1][0] * X[0];
        auto detXC1 = X[0] * C[1][1] - X[1] * C[0][1];

        /* alpha values */
        alpha1 = detXC1 / detC0C1;
        alpha2 = detC0X / detC0C1;
    }
    else {
        /* matrix is under-determined, assume alpha1=alpha2 */
        auto c0 = C[0][0] + C[0][1];
        auto c1 = C[1][0] + C[1][1];
        if (std::fabs(c0) > epsilon)
            alpha1 = alpha2 = X[0] / c0;
        else if (std::fabs(c1)>epsilon)
            alpha1 = alpha2 = X[1] / c1;
        else
            alpha1 = alpha2 = 0;

    }

    auto segLength = this->getDistance(pt2, pt1);
    auto eps = epsilon * segLength;
    Point2 handle1, handle2;
    if (alpha1 < eps || alpha2 < eps)
        alpha1 = alpha2 = segLength / 3.f;
    else {
        auto line = pt2 - pt1;
        handle1 = tan1 * alpha1;
        handle2 = tan2 * alpha2;
        if (handle1 * line - handle2 * line > segLength * segLength ){
            alpha1 = alpha2 = segLength / 3.f;
            handle1 = handle2 = getNANPoint();
        }
    }

    auto pta = handle1.isNaN()? pt1+tan1*alpha1 : pt1+handle1;
    auto ptb = handle2.isNaN() ? pt2 + tan2 * alpha2 : pt2 + handle2;

    return Curve{pt1, pta, ptb, pt2};
}

template <typename Container, typename Point2, typename Real>
bool PathFitter<Container, Point2, Real>::reparameterize(int first, int last, std::vector<Real> &u, const Curve &curve)
{
    for (int i=first; i<=last; ++i){
        u[i-first] = this->findRoot(curve, m_dataPoints[i], u[i-first]);
    }

    for (int i=1, l=u.size(); i<l; ++i ){
        if (u[i] <= u[i-1])
            return false;
    }
    return true;
}

template <typename Container, typename Point2, typename Real>
Real PathFitter<Container, Point2, Real>::findRoot(const Curve &curve, const Point2 &point, Real u)
{
    Curve curve1(3);
    Curve curve2(2);

    /* control vertices for Q' */
    for (int i=0; i<=2; ++i){
        curve1[i] = (curve[i+1] - curve[i]) * 3.f;
    }

    /* control vertices for Q'' */
    for (int i=0; i<=1; ++i){
        curve2[i] = (curve1[i+1] - curve1[i]) * 2.f;
    }

    /* compute Q(u), Q'(u) and Q''(u) */
    auto pt = this->evaluate(3, curve, u);
    auto pt1 = this->evaluate(2, curve1, u);
    auto pt2 = this->evaluate(1, curve2, u);
    auto diff = pt - point;
    auto df = pt1 * pt1 + diff * pt2;

    /* f(u) / f'(u) */
    if (std::fabs(df) < 1.0e-6)
        return u;

    /* u = u - f(u)/f'(u) */
    return u - (diff*pt1) / df;
}

template <typename Container, typename Point2, typename Real>
Point2 PathFitter<Container, Point2, Real>::evaluate(int degree, const Curve &curve, Real t)
{
    Curve tmp(curve);

    /* triangle computation */
    for (int i=1; i<=degree; ++i){
        for (int j=0; j<=degree-i; ++j){
            tmp[j] = tmp[j] * (1-t) + tmp[j+1] * t;
        }
    }
    return tmp[0];
}

template <typename Container, typename Point2, typename Real>
std::vector<Real> PathFitter<Container, Point2, Real>::chordLengthParameterize(int first, int last)
{
    std::vector<Real> u(last-first+1);

    for (int i=first+1; i<=last; ++i){
        u[i-first] = u[i-first-1] + this->getDistance( m_dataPoints[i], m_dataPoints[i-1]);
    }

    for (int i=1, m=last-first; i<=m; ++i ){
        u[i] /= u[m];
    }

    return u;
}

template <typename Container, typename Point2, typename Real>
Real PathFitter<Container, Point2, Real>::findMaxError(int first, int last, const Curve &curve, const std::vector<Real> &u, int &index)
{
    index = std::floor((last-first+1)/2);
    Real maxDist = 0;
    for (int i=first+1; i<last; ++i){
        auto P = this->evaluate(3, curve, u.at(i-first));
        auto v = P - m_dataPoints[i];
        auto dist = v.length2(); // squared distance
        if (dist >= maxDist){
            maxDist = dist;
            index = i;
        }
    }
    return maxDist;
}
