# Curve fitter

A C++11-based class that performs curve fitting based on *An algorithm for automatically fitting digitized curves* by Philip J. Schneider which was published in *Graphics gems*, 1990. The algorithm is presented with minor modifications and improvements. 

Although the algorithm is complete, the wrap-up project is still in development and may undergo some changes and improvements, e.g., an example of OpenGL-based visualization only will be added, as well as stl-based usage only (no Qt and OpenSceneGraph dependencies).

# Requirements

Currently the project requires installed Qt5 and OpenSceneGraph (>= 3.4.0), as well as C++11 compatible compiler. 

# Curve fitter class

Use the `OsgPathFitter` class or derive your own using the base `PathFitter`. More details will be added here.

# Licence

See the corresponding [licence file](https://github.com/vicrucann/CurveFitting/blob/master/LICENSE).

# Examples

On the image, 60 sampled curve (red color) is represented by a 1 cubic bezier curve (green color).

![Screenshot](https://github.com/vicrucann/CurveFitting/blob/master/screenshots/curvefit.png)
