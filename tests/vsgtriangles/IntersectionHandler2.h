#pragma once
#include <vsg/commands/Draw.h>
#include <vsg/commands/DrawIndexed.h>
#include <vsg/core/Visitor.h>
#include <vsg/nodes/Node.h>
#include <vsg/nodes/VertexIndexDraw.h>
#include <vsg/state/GraphicsPipeline.h>
#include <vsg/state/InputAssemblyState.h>
#include <vsg/ui/PointerEvent.h>
#include <vsg/utils/Builder.h>
#include <vsg/utils/LineSegmentIntersector.h>


class IntersectionHandler2 : public vsg::Inherit<vsg::Visitor, IntersectionHandler2>
{
public:
    IntersectionHandler2(vsg::ref_ptr<vsg::Group> scene,
                         vsg::ref_ptr<vsg::Camera> camera,
                         std::set<vsg::Node*> objectNodes = {}) :
        _scene(scene),
        _camera(camera),
        _objectNodes(objectNodes)
    {
        // Set default colors
        selectNode();
    }

    void apply(vsg::ButtonPressEvent& buttonPressEvent) override
    {
        intersection(buttonPressEvent);
    }

    void selectNode(vsg::Node* node = nullptr);


    void intersection(vsg::PointerEvent& pointerEvent);


    void setColor(vsg::Node* node,
                  vsg::vec4 color);


    static constexpr vsg::vec4 default_color = vsg::vec4{0.5, 0.5, 0.5, 1.0}; // Default color
    static constexpr vsg::vec4 selected_color = vsg::vec4{1.0, 0, 0, 1.0};    // Select color

    vsg::ref_ptr<vsg::Node> _scene;
    vsg::ref_ptr<vsg::Camera> _camera;
    std::set<vsg::Node*> _objectNodes;
};