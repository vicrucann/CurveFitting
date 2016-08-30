#include "PathFitter.cpp"

#include <osg/Array>

/* define template instance that will be used in code
 * see more: https://isocpp.org/wiki/faq/templates#separate-template-fn-defn-from-decl
*/
template class PathFitter<osg::Vec3Array, osg::Vec3f, float>;
