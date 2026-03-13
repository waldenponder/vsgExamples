#pragma once
#include <vsg/commands/Draw.h>
#include <vsg/commands/DrawIndexed.h>
#include <vsg/core/Visitor.h>
#include <vsg/nodes/Node.h>
#include <vsg/nodes/VertexIndexDraw.h>
#include <vsg/state/GraphicsPipeline.h>
#include <vsg/state/InputAssemblyState.h>
#include <vsg/state/material.h>
#include <vsg/utils/Builder.h>
#include <vsg/utils/LineSegmentIntersector.h>
#include "tsl/robin_map.h"
#include <vsg/all.h>
#include "Util.h"

class FGeometry : public vsg::Inherit<vsg::Object, FGeometry>
{
public:
    ref_ptr<vec3Array> position;
    ref_ptr<vec3Array> normals;
    ref_ptr<vec2Array> uv;
    ref_ptr<vec4Array> colors; //vertex colors
    ref_ptr<ushortArray> indices;

    vsg::vec4 instanceColor;
    VkPrimitiveTopology topology;
};

namespace FGeometryCreator
{
    ref_ptr<FGeometry> lines(const ref_ptr<vec3Array>& pos, const vsg::vec4& color);

    ref_ptr<FGeometry> lineStrip(const ref_ptr<vec3Array>& pos,const vsg::vec4& color);

    ref_ptr<FGeometry> triangles(const ref_ptr<vec3Array>& pos, const ref_ptr<vec3Array>& normal, const vsg::vec4& color, ref_ptr<vec2Array> uv = nullptr);

    ref_ptr<FGeometry> triangleIndices(const ref_ptr<vec3Array>& pos, const vsg::vec4& color, const ref_ptr<vsg::ushortArray>& indices);

    ref_ptr<FGeometry> triangleStrip(const ref_ptr<vec3Array>& pos, const ref_ptr<vec3Array>& normal, const vsg::vec4& color, const ref_ptr<vec2Array>& uv = nullptr);

    ref_ptr<FGeometry> points(const ref_ptr<vec3Array>& pos,const vsg::vec4& color);

    namespace VertexColor
    {
        ref_ptr<FGeometry> lines(const ref_ptr<vec3Array>& pos, const ref_ptr<vec4Array>& colors);

        ref_ptr<FGeometry> lineStrip(const ref_ptr<vec3Array>& pos,const ref_ptr<vec4Array>& colors);

        ref_ptr<FGeometry> triangles(const ref_ptr<vec3Array>& pos, const ref_ptr<vec3Array>& normal, const ref_ptr<vec4Array>& colors, ref_ptr<vec2Array> uv = nullptr);

        ref_ptr<FGeometry> triangleIndices(const ref_ptr<vec3Array>& pos, const ref_ptr<vec3Array>& normal, const ref_ptr<vec4Array>& colors,
            const ref_ptr<vsg::ushortArray>& indices,  ref_ptr<vec2Array> uv = nullptr);

        ref_ptr<FGeometry> triangleStrip(const ref_ptr<vec3Array>& pos, const ref_ptr<vec3Array>& normal, const ref_ptr<vec4Array>& colors, ref_ptr<vec2Array> uv);

        ref_ptr<FGeometry> points(const ref_ptr<vec3Array>& pos, const ref_ptr<vec4Array>& colors);
    }; // namespace VertexColor
};

namespace vsg
{
    VSG_type_name(FGeometry);
}
