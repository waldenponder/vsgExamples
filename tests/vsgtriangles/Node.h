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


struct NodeDefine : public vsg::Object
{
    vsg::ref_ptr<vsg::vec3Array> position;
    vsg::ref_ptr<vsg::vec3Array> normals;
    vsg::ref_ptr<vsg::vec4Array> colors;
    vsg::ref_ptr<vsg::ushortArray> indices;
    vsg::PhongMaterial material;

    float depthBiasConstantFactor = 0.0f;
    float depthBiasClamp = 0.0f;
    float depthBiasSlopeFactor = 0.0f;

    vsg::dmat4 matrix;
    int id;
};

class Scene
{
public:
    static Scene& instance()
    {
        static Scene instance; 
        return instance;
    }

    void addNode(vsg::ref_ptr<NodeDefine> def);
    vsg::ref_ptr<NodeDefine> getNode(int id);

    void reset();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

private:
    Scene() = default;

    std::unordered_map<int, vsg::ref_ptr<NodeDefine>> map_;
};

vsg::ref_ptr<vsg::Group> convert(const NodeDefine& node);

vsg::ref_ptr<NodeDefine> createQuadTest(float sz, float z);