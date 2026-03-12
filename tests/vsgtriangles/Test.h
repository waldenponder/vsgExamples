#pragma once
#include <vsg/commands/Draw.h>
#include <vsg/commands/DrawIndexed.h>
#include <vsg/core/Visitor.h>
#include <vsg/nodes/Node.h>
#include <vsg/nodes/StateGroup.h>
#include <vsg/nodes/VertexIndexDraw.h>
#include <vsg/state/GraphicsPipeline.h>
#include <vsg/state/InputAssemblyState.h>
#include <vsg/state/material.h>
#include <vsg/utils/Builder.h>
#include <vsg/utils/GraphicsPipelineConfigurator.h>
#include <vsg/utils/LineSegmentIntersector.h>

vsg::ref_ptr<vsg::StateGroup> createMaterialStateGroup(vsg::ref_ptr<vsg::Options> options);

vsg::ref_ptr<vsg::Node> createTriangle();


vsg::ref_ptr<vsg::Node> createQuad();


vsg::ref_ptr<vsg::Group> test3();
