# Curve fitter

A C++11-based class that performs curve fitting based on *An algorithm for automatically fitting digitized curves* by Philip J. Schneider which was published in *Graphics gems*, 1990. The algorithm is presented with minor modifications and improvements. 

# Requirements

Currently the project requires installed OpenSceneGraph (>= 3.4.0) and C++11 compatible compiler. 

# Curve fitter class

Use the `OsgPathFitter` class or derive your own using the base `PathFitter` class. 

## Sub-classing the `PathFitter`

The `PathFitter` is a template abstract class. Which means, you have to sub-class it and provide your own implementation for the pure virtual functions and for certain operators (e.g., dot product, cross product, etc.). The list of functions and their expected behaviour is described in the following sub-sections.

#### Constructor

The constructor does not require any implementation, but it requires to provide a list of arguments for the used templates. In the case of `OsgPathFitter`, the template arguments are:

* `osg::Vec3Array` is a vector of a 3D component. In the base class it has a template name `Container`.
* `osg::Vec3f` is a 3D (used as 2D) component of type `float`, i.e., given the above `Container = Vec3Array(Vec3f, Vec3f, ...)`. In the base class the template has a name `Point2D`.
* `float` is a real type of which the `Point2D` consists of, i.e., `Vec3f(float, float, float)`. In the base class it has a name `Real`.

#### Method `fit()`

It is the main loop method that launches the fitting algorithm. It is designed to be re-implemented with the option to use smart pointer of choice. In case of `OsgPathFitter`, we chose `osg::ref_ptr<>`.

The main steps that are needed to be in the method are:

1. Allocate new `Container` memory (manage it by means of smart pointer of choice if you wish).
2. Calculate tangents of the extreme points as shown in the `OsgPathFitter::fit()` example using `curveAt()` and `normalize()` functions. 
3. Launch method `PathFitter::fitCubic()` provided a newly allocated container where the results will be saved, the tolerance error, the container's first and last point indexes, and the calculated tangent.
4. Return the pointer on the allocated container which will contain the result points.

#### Method `getDistance()`

Must return an Euclidean distance between two `Point2D`s. Of course, depending on the number of dimensions of your `Point`s, the formula will vary. Refer to the provided `OsgPathFitter` example for more details.

#### Method `getNANPoint()`

Must return a `Point2D` with initialized values of `NAN` (from `math.h`).

#### Implementation of `operator`'s

The base class `PathFitter` uses some notations taken from OpenSceneGraph API: 

* `Real r = V1 * V2` is the operator for dot product between vectors `V1` and `V2`.
* `Vec2 V = V1 ^ V2` is the operator for cross product between vectors `V1` and `V2`.
* `Vec2 V = V0 * s` is the operator for scaling of vector `V0` by real `s`.
* `normalize()` performs normalization of vector so that it length becomes unity.
* `length2()` is the squared length of the vector.

#### Template definitions

In your custom class, it is necessary to provide template definitions. As it was mentioned above, you provide the definitions in the custom constructor. The full definitions must be provided at the end of  your `.cpp` implementation file of the custom path fitter class.

The `PathFitter` class must also contain the full definition to avoid the **LNK2019**, *Unresolved external symbol*. For this purpose, we prepared a file called `PathFitter-impl.cpp` that is to contain such definitions. Note we use the separate file simple for convenience reasons so that do not add custom definitions into the `PathFitter` abstract class.

# Usage of `LibPathFitter` in an external project

The `PathFitter` and its derivatives and compiled into a library `libPathFitter` so that to be easily used from an external project. In this case, you will only need to modify your project's corresponding `CMakeLists.txt` file. For example:

```
add_subdirectory(CurveFitting/libPathFitter)
link_directories(${CMAKE_BINARY_DIR}/src/libNumerics/CurveFitting/libPathFitter)
# ...
target_link_libraries( libExternal
    libPathFitter
    ${QT_LIBRARIES}
    ${OPENSCENEGRAPH_LIBRARIES}
# ... any other libs
)
```
For a live example, feel free to refer to [Cherish](https://github.com/vicrucann/cherish) project and its corresponding [`CMakeLists.txt`](https://github.com/vicrucann/cherish/blob/master/src/libNumerics/CMakeLists.txt).

# Licence

See the corresponding [licence file](https://github.com/vicrucann/CurveFitting/blob/master/LICENSE).

# Examples

On the image, 60 sampled curve (red color) is represented by a 1 cubic Bezier curve (green color).

![Screenshot](https://github.com/vicrucann/CurveFitting/blob/master/screenshots/curvefit.png)

Another example that includes 3 curves representing 180 sampled path.

![Screenshot](https://github.com/vicrucann/CurveFitting/blob/master/screenshots/curvefit-2.png)
