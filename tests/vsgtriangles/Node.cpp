#include "Node.h"

#include <iostream>
#include <random>
#include <vsg/ui/KeyEvent.h>
#include <vsg/all.h>


vsg::ref_ptr<vsg::ShaderSet> shaderSet;

static int s_id = 0;

void Scene::addNode(vsg::ref_ptr<NodeDefine> def)
{
    ++s_id;
    def->id = s_id;
    map_[s_id] = def;
}

vsg::ref_ptr<NodeDefine> Scene::getNode(int id)
{
    auto itor = map_.find(id);
    if (itor == map_.end())
        return nullptr;
    return itor->second;
}

void Scene::reset()
{
    map_ = {};
    shaderSet = {};
}

//vsg::ref_ptr<vsg::Group> convert(const NodeDefine& node)
//{
//    auto group = vsg::Group::create();
//
//    auto geometry = vsg::Geometry::create();
//
//    // 深拷贝所有数据
//    geometry->assignArrays({vsg::vec3Array::create(*node.position),
//                            vsg::vec3Array::create(*node.normals),
//                            vsg::vec4Array::create(*node.colors)});
//    geometry->assignIndices(vsg::ushortArray::create(*node.indices));
//
//    auto drawCommand = vsg::DrawIndexed::create(
//        node.indices->size(), 1, 0, 0, 0);
//    geometry->addChild(drawCommand);
//
//    group->addChild(geometry);
//    return group;
//}

vsg::ref_ptr<vsg::Group> convert(const NodeDefine& node)
{
    // auto scenegraph = vsg::Group::create();

    //  auto sharedObjects = vsg::SharedObjects::create();
    //1.创建一个Phone模型的渲染管线
    if (!shaderSet) shaderSet = vsg::createPhongShaderSet();
    auto config = vsg::GraphicsPipelineConfigurator::create(shaderSet);
    //2.填充默认材质
    auto mat = vsg::PhongMaterialValue::create(node.material);
    config->assignDescriptor("material", mat);


#if 1
    auto& vertices = node.position;
    auto& normals = node.normals;
    auto& colors = node.colors;
    auto& indices = node.indices;
#else
    auto vertices = vsg::vec3Array::create(*node.position); 
    auto normals = vsg::vec3Array::create(*node.normals);   
    auto colors = vsg::vec4Array::create(*node.colors);     
    auto indices = vsg::ushortArray::create(*node.indices); 
#endif
    vsg::DataList vertexArrays;
    config->assignArray(vertexArrays, "vsg_Vertex", VK_VERTEX_INPUT_RATE_VERTEX, vertices);
    if (normals)
    {
        config->assignArray(vertexArrays, "vsg_Normal", VK_VERTEX_INPUT_RATE_VERTEX, normals);
    }
    //graphicsPipelineConfig->assignArray(vertexArrays, "vsg_TexCoord0", VK_VERTEX_INPUT_RATE_VERTEX, texcoords);
    if (colors)
    {
        config->assignArray(vertexArrays, "vsg_Color", /**/ VK_VERTEX_INPUT_RATE_INSTANCE, colors);
    }

    auto drawCommands = vsg::Commands::create();
    drawCommands->addChild(vsg::BindVertexBuffers::create(config->baseAttributeBinding, vertexArrays));
    drawCommands->addChild(vsg::BindIndexBuffer::create(indices));
    drawCommands->addChild(vsg::DrawIndexed::create(indices->size(), 1, 0, 0, 0));

    auto depthBias = vsg::SetDepthBias::create();
    depthBias->depthBiasConstantFactor = node.depthBiasConstantFactor;
    depthBias->depthBiasSlopeFactor = node.depthBiasConstantFactor;
    drawCommands->addChild(depthBias);

    config->init();
    auto stateGroup = vsg::StateGroup::create();
    config->copyTo(stateGroup);

    stateGroup->addChild(drawCommands);
    stateGroup->setValue("id", node.id);

    //scenegraph->addChild(stateGroup);

    // return scenegraph;
    return stateGroup;
}

vsg::ref_ptr<NodeDefine> createQuadTest(float sz, float z)
{
    vsg::ref_ptr<NodeDefine> node = vsg::ref_ptr<NodeDefine> (new NodeDefine);
    float val = sz;

    static std::default_random_engine eng(time(NULL));
    static std::uniform_real_distribution<double> rd(0, 1);

    node->position = vsg::vec3Array::create({{-val, -val, z},
                                            {val, -val, z},
                                            {val, val, z},
                                            {-val, val, z}});

    node->normals = vsg::vec3Array::create({{0.0f, 0.0f, 1.0f},
                                           {0.0f, 0.0f, 1.0f},
                                           {0.0f, 0.0f, 1.0f},
                                           {0.0f, 0.0f, 1.0f}});

    node->colors = vsg::vec4Array::create({{1.0f, 1.0f, 1.0f, 1.0f}});

    node->indices = vsg::ushortArray::create({0, 1, 2,
                                             0, 2, 3});

    node->material = vsg::PhongMaterial{};
    node->material.diffuse.set(rd(eng), rd(eng), rd(eng), 1.0f);

    node->matrix = vsg::dmat4();

    return node;
}