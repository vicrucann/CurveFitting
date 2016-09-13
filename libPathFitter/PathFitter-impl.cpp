#include "PathFitter.cpp"

#ifdef USE_OPENSCENEGRAPH
#include <osg/Array>
#endif

#include <vector>
#include <utility>

/* define template instance that will be used in code
 * see more: https://isocpp.org/wiki/faq/templates#separate-template-fn-defn-from-decl
*/
#ifdef USE_OPENSCENEGRAPH
template class PathFitter<osg::Vec3Array, osg::Vec3f, float>;
#endif

#ifdef USE_STL
template class PathFitter<std::vector<std::pair<float, float> >, std::pair<float, float>, float>;
#endif
