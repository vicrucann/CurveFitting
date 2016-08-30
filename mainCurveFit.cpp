#include <Windows.h>

#include "math.h"

#include <QtGlobal>
#include <QDebug>

#include <osg/Drawable>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/StateSet>
#include <osgViewer/Viewer>
#include <osg/LineWidth>
#include <osg/BlendFunc>

#include "OsgPathFitter.h"

const int OSG_WIDTH = 1280;
const int OSG_HEIGHT = 960;

osg::Vec3Array* drawCurves(osg::Vec3Array* curves, int samples = 11)
{
    osg::ref_ptr<osg::Vec3Array> sampled = new osg::Vec3Array;
    Q_ASSERT(curves->size() % 4 == 0);
    int nCurves = curves->size() / 4;

    auto delta = 1.f / float(samples);

    for (decltype(curves->size()) i=0; i<curves->size(); i=i+4){
        auto b0 = curves->at(i),
                b1 = curves->at(i+1),
                b2 = curves->at(i+2),
                b3 = curves->at(i+3);
        for (int j=0; j<=samples; ++j){
            auto t = delta * float(j),
                    t2 = t*t,
                    one_minus_t = 1.f - t,
                    one_minus_t2 = one_minus_t * one_minus_t;

            auto Bt = b0 * one_minus_t2 * one_minus_t
                    + b1 * 3.f * t * one_minus_t2
                    + b2 * 3.f * t2 * one_minus_t
                    + b3 * t2 * t;

            sampled->push_back(Bt);
        }
    }
    Q_ASSERT(sampled->size() == (samples+1)*nCurves);
    return sampled.release();
}

osg::Vec3Array* createDataPoints()
{
    osg::ref_ptr<osg::Vec3Array> curve = new osg::Vec3Array;

    curve->push_back(osg::Vec3f(0,0,0));
    curve->push_back(osg::Vec3f(2,0,12));
    curve->push_back(osg::Vec3f(40,0,28));
    curve->push_back(osg::Vec3f(40,0,4));

    osg::ref_ptr<osg::Vec3Array> sampled = drawCurves(curve, 60);

    return sampled.release();
}

osg::Node* createTestScene()
{
    osg::ref_ptr<osg::Vec3Array> path = createDataPoints();
    OsgPathFitter<osg::Vec3Array, osg::Vec3f, float> fitter;
    fitter.init(*path);

    /* threshold: prefer to set it automatically depending on size of bounding box
     * in order to avoid under- and over-fitting. Here, we will set it fixed */
    float tolerance = 1.f;
    osg::ref_ptr<osg::Vec3Array> curves = fitter.fit(tolerance);

    osg::ref_ptr<osg::Vec3Array> sampled = drawCurves(curves.get(), 15);
    qDebug() << "path.segments=" << path->size()/4;
    qDebug() << "curves.segments=" << curves->size()/4;

    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(0.3,0.9,0,1));
    osg::Vec4Array* colorsGT = new osg::Vec4Array;
    colorsGT->push_back(osg::Vec4(1,0,0,1));

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    geom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, sampled->size()));
    geom->setUseDisplayList(false);
    geom->setVertexArray(sampled.get());
    geom->setColorArray(colors, osg::Array::BIND_OVERALL);

    osg::ref_ptr<osg::Geometry> geomGT = new osg::Geometry;
    geomGT->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, path->size()));
    geomGT->setUseDisplayList(false);
    geomGT->setVertexArray(path.get());
    geomGT->setColorArray(colorsGT, osg::Array::BIND_OVERALL);

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(geom.get());
    geode->addDrawable(geomGT.get());

    osg::StateSet* state = geode->getOrCreateStateSet();
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    state->setMode(GL_BLEND, osg::StateAttribute::ON);
    state->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
    osg::LineWidth* lw = new osg::LineWidth;
    lw->setWidth(10.f);
    state->setAttribute(lw, osg::StateAttribute::ON);
    osg::BlendFunc* blendfunc = new osg::BlendFunc();
    state->setAttributeAndModes(blendfunc, osg::StateAttribute::ON);

    return geode.release();
}

int main(int, char**)
{
    ::SetProcessDPIAware();

    osgViewer::Viewer viewer;

    osg::ref_ptr<osg::Group> root = new osg::Group();

    root->addChild(createTestScene());
    viewer.setSceneData(root.get());
    viewer.setUpViewInWindow(100,100,OSG_WIDTH, OSG_HEIGHT);
    return viewer.run();
}
