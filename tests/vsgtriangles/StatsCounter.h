#pragma once
#include <vsg/commands/Draw.h>
#include <vsg/commands/DrawIndexed.h>
#include <vsg/core/Visitor.h>
#include <vsg/nodes/Node.h>
#include <vsg/nodes/VertexIndexDraw.h>
#include <vsg/state/GraphicsPipeline.h>
#include <vsg/state/InputAssemblyState.h>

class StatsCounter : public vsg::Visitor
{
public:
    uint64_t triangles = 0;
    uint64_t vertices = 0;
    uint64_t indices = 0;
    uint64_t drawCalls = 0;

    VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    void reset()
    {
        triangles = 0;
        vertices = 0;
        indices = 0;
        drawCalls = 0;
    }

    void apply(vsg::Node& node) override
    {
        node.traverse(*this);
    }

    void apply(vsg::BindGraphicsPipeline& bind) override
    {
        auto pipeline = bind.pipeline;

        for (auto& s : pipeline->pipelineStates)
        {
            if (s && s->is_compatible(typeid(vsg::InputAssemblyState)))
            {
                auto inputAssemblyState = s->cast<vsg::InputAssemblyState>();
                topology = inputAssemblyState->topology;
                break;
            }
        }

        bind.traverse(*this);
    }

    uint64_t calcTriangles(uint32_t count)
    {
        switch (topology)
        {
        case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST:
            return count / 3;

        case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP:
        case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN:
            return count >= 3 ? count - 2 : 0;

        case VK_PRIMITIVE_TOPOLOGY_LINE_LIST:
        case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP:
        case VK_PRIMITIVE_TOPOLOGY_POINT_LIST:
            return 0;

        default:
            return 0;
        }
    }

    void apply(vsg::Draw& draw) override
    {
        drawCalls++;

        vertices += draw.vertexCount;

        triangles += calcTriangles(draw.vertexCount);
    }

    void apply(vsg::DrawIndexed& draw) override
    {
        drawCalls++;

        indices += draw.indexCount;

        triangles += calcTriangles(draw.indexCount);
    }

    void apply(vsg::VertexIndexDraw& draw) override
    {
        drawCalls++;

        indices += draw.indexCount;

        triangles += calcTriangles(draw.indexCount);
    }
};


#define DELTA_TIME(now, last) std::chrono::duration<double, std::chrono::milliseconds::period>(now - last).count()
