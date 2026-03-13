#pragma once
#include <vsg/commands/Draw.h>
#include <vsg/commands/DrawIndexed.h>
#include <vsg/nodes/Node.h>
#include <vsg/nodes/VertexIndexDraw.h>
#include <vsg/state/GraphicsPipeline.h>
#include <vsg/state/InputAssemblyState.h>
#include <vsg/state/material.h>
#include <vsg/utils/Builder.h>
#include <vsg/utils/LineSegmentIntersector.h>
#include "tsl/robin_map.h"
#include <vsg/all.h>
#include "FGeometry.h"
#include "FMaterial.h"
#include "FNodeState.h"

class FNode : public vsg::Inherit<vsg::Object, FNode>
{
public:
    FNode();

    bool visible() const;
    void setVisible(bool v);

    FNodeState& node_state() { return node_state_; }
    const FNodeState& node_state() const { return node_state_; }

private:
    ref_ptr<FMaterial> material;
    ref_ptr<vsg::dmat4> matrix;
    ref_ptr<FGeometry> geometry_;
    FNodeState node_state_;
    int id_ = 0;
};

namespace vsg
{
    VSG_type_name(FNode);
}