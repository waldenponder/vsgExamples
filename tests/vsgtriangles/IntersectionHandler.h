#pragma once
#include <vsg/commands/Draw.h>
#include <vsg/commands/DrawIndexed.h>
#include <vsg/core/Visitor.h>
#include <vsg/nodes/Node.h>
#include <vsg/nodes/VertexIndexDraw.h>
#include <vsg/state/GraphicsPipeline.h>
#include <vsg/state/InputAssemblyState.h>
#include <vsg/utils/Builder.h>
#include <vsg/utils/LineSegmentIntersector.h>


class IntersectionHandler : public vsg::Inherit<vsg::Visitor, IntersectionHandler>
{
public:
    vsg::GeometryInfo geom;
    vsg::StateInfo state;

    vsg::ref_ptr<vsg::Viewer> viewer;
    vsg::ref_ptr<vsg::Builder> builder;
    vsg::ref_ptr<vsg::Options> options;
    vsg::ref_ptr<vsg::Camera> camera;
    vsg::ref_ptr<vsg::Group> scenegraph;
    vsg::ref_ptr<vsg::EllipsoidModel> ellipsoidModel;
    vsg::ref_ptr<vsg::Group> highLightGroup;

    double scale = 1.0;
    bool verbose = true;

    IntersectionHandler(vsg::ref_ptr<vsg::Builder> in_builder, vsg::ref_ptr<vsg::Camera> in_camera, vsg::ref_ptr<vsg::Group> in_scenegraph, vsg::ref_ptr<vsg::EllipsoidModel> in_ellipsoidModel, double in_scale, vsg::ref_ptr<vsg::Options> in_options) :
        builder(in_builder),
        options(in_options),
        camera(in_camera),
        scenegraph(in_scenegraph),
        ellipsoidModel(in_ellipsoidModel),
        scale(in_scale)
    {
        geom.cullNode = true;
        builder->verbose = verbose;
        if (scale > 10.0) scale = 10.0;
    }

    void apply(vsg::KeyPressEvent& keyPress) override;


    void apply(vsg::ButtonPressEvent& buttonPressEvent) override;

    void apply(vsg::PointerEvent& pointerEvent) override;


    void intersection_LineSegmentIntersector(vsg::PointerEvent& pointerEvent);

    void intersection_PolytopeIntersector(vsg::PointerEvent& pointerEvent);

protected:
    vsg::ref_ptr<vsg::PointerEvent> lastPointerEvent;
    vsg::ref_ptr<vsg::LineSegmentIntersector::Intersection> lastIntersection;
};

vsg::ref_ptr<vsg::Group> createFromId(int id, bool highLight = false);