#include "FGeometry.h"
#include <iostream>
#include <random>
#include <vsg/ui/KeyEvent.h>
#include <vsg/all.h>

namespace
{
    ref_ptr<FGeometry> createGeometry(
        const ref_ptr<vec3Array>& pos,
        const ref_ptr<vec3Array>& normal,
        const ref_ptr<vec2Array>& uv,
        const ref_ptr<vec4Array>& colors,
        const ref_ptr<vsg::ushortArray>& indices,
        const vsg::vec4& instanceColor,
        VkPrimitiveTopology topology)
    {
        auto geom = FGeometry::create();

        geom->position = pos;
        geom->normals = normal;
        geom->uv = uv;
        geom->colors = colors;
        geom->indices = indices;
        geom->instanceColor = instanceColor;
        geom->topology = topology;

        return geom;
    }
} // namespace

ref_ptr<FGeometry> FGeometryCreator::lines(
    const ref_ptr<vec3Array>& pos,
    const vsg::vec4& color)
{
    return createGeometry(
        pos,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        color,
        VK_PRIMITIVE_TOPOLOGY_LINE_LIST);
}

ref_ptr<FGeometry> FGeometryCreator::lineStrip(
    const ref_ptr<vec3Array>& pos,
    const vsg::vec4& color)
{
    return createGeometry(
        pos,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        color,
        VK_PRIMITIVE_TOPOLOGY_LINE_STRIP);
}

ref_ptr<FGeometry> FGeometryCreator::triangles(
    const ref_ptr<vec3Array>& pos,
    const ref_ptr<vec3Array>& normal,
    const vsg::vec4& color,
    ref_ptr<vec2Array> uv)
{
    return createGeometry(
        pos,
        normal,
        uv,
        nullptr,
        nullptr,
        color,
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
}

ref_ptr<FGeometry> FGeometryCreator::triangleIndices(
    const ref_ptr<vec3Array>& pos,
    const vsg::vec4& color,
    const ref_ptr<ushortArray>& indices)
{
    return createGeometry(
        pos,
        nullptr,
        nullptr,
        nullptr,
        indices,
        color,
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
}

ref_ptr<FGeometry> FGeometryCreator::triangleStrip(
    const ref_ptr<vec3Array>& pos, const ref_ptr<vec3Array>& normal,
    const vsg::vec4& color, const ref_ptr<vec2Array>& uv)
{
    return createGeometry(
        pos,
        normal,
        uv,
        nullptr,
        nullptr,
        color,
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);
}

ref_ptr<FGeometry> FGeometryCreator::points(
    const ref_ptr<vec3Array>& pos,
    const vsg::vec4& color)
{
    return createGeometry(
        pos,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        color,
        VK_PRIMITIVE_TOPOLOGY_POINT_LIST);
}

ref_ptr<FGeometry> FGeometryCreator::VertexColor::lines(
    const ref_ptr<vec3Array>& pos,
    const ref_ptr<vec4Array>& colors)
{
    return createGeometry(
        pos,
        nullptr,
        nullptr,
        colors,
        nullptr,
        vsg::vec4(1.0, 1, 1, 1),
        VK_PRIMITIVE_TOPOLOGY_LINE_LIST);
}

ref_ptr<FGeometry> FGeometryCreator::VertexColor::lineStrip(
    const ref_ptr<vec3Array>& pos,
    const ref_ptr<vec4Array>& colors)
{
    return createGeometry(
        pos,
        nullptr,
        nullptr,
        colors,
        nullptr,
        vsg::vec4(1.0, 1, 1, 1),
        VK_PRIMITIVE_TOPOLOGY_LINE_STRIP);
}

ref_ptr<FGeometry> FGeometryCreator::VertexColor::triangles(
    const ref_ptr<vec3Array>& pos,
    const ref_ptr<vec3Array>& normal,
    const ref_ptr<vec4Array>& colors,
    ref_ptr<vec2Array> uv)
{
    return createGeometry(
        pos,
        normal,
        uv,
        colors,
        nullptr,
        vsg::vec4(1.0, 1, 1, 1),
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
}

ref_ptr<FGeometry> FGeometryCreator::VertexColor::triangleIndices(
    const ref_ptr<vec3Array>& pos,
    const ref_ptr<vec3Array>& normal,
    const ref_ptr<vec4Array>& colors,
    const ref_ptr<vsg::ushortArray>& indices,
    ref_ptr<vec2Array> uv)
{
    return createGeometry(
        pos,
        normal,
        uv,
        colors,
        indices,
        vsg::vec4(1.0, 1, 1, 1),
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
}

ref_ptr<FGeometry> FGeometryCreator::VertexColor::triangleStrip(const ref_ptr<vec3Array>& pos, const ref_ptr<vec3Array>& normal, const ref_ptr<vec4Array>& colors, ref_ptr<vec2Array> uv)
{
    return createGeometry(
        pos,
        normal,
        uv,
        colors,
        nullptr,
        vsg::vec4(1.0, 1, 1, 1),
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);
}