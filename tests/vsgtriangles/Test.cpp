#include "Test.h"

#include <iostream>
#include <random>
#include <vsg/ui/KeyEvent.h>
#include <vsg/all.h>

vsg::ref_ptr<vsg::StateGroup> createMaterialStateGroup()
{
    auto shaderSet = vsg::createPhongShaderSet();

    auto gpc = vsg::GraphicsPipelineConfigurator::create(shaderSet);

    struct SetPipelineStates : public vsg::Visitor
    {

        SetPipelineStates() = default;

        void apply(vsg::Object& object) override
        {
            object.traverse(*this);
        }

        void apply(vsg::RasterizationState& rs) override
        {
            rs.cullMode = VK_CULL_MODE_NONE;
        }

        void apply(vsg::InputAssemblyState& ias) override
        {
            ias.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        }

        void apply(vsg::ColorBlendState& cbs) override
        {
            //cbs.configureAttachments(si.blending);
        }
    } sps;

    gpc->accept(sps);

    gpc->enableArray("vsg_Vertex", VK_VERTEX_INPUT_RATE_VERTEX, 12);
    gpc->enableArray("vsg_Normal", VK_VERTEX_INPUT_RATE_VERTEX, 12);
    gpc->enableArray("vsg_Color", VK_VERTEX_INPUT_RATE_INSTANCE, 16);

    auto material = vsg::PhongMaterialValue::create();
    material->value().diffuse = vsg::vec4(1.0f, 0.5f, 0.5f, 1.0f);

    gpc->assignDescriptor("material", material);

    gpc->init();

    vsg::StateCommands stateCommands;
    gpc->copyTo(stateCommands);

    auto stateGroup = vsg::StateGroup::create();

    stateGroup->stateCommands = stateCommands;
    stateGroup->prototypeArrayState = gpc->getSuitableArrayState();

    return stateGroup;
}

#if 0
vsg::ref_ptr<vsg::StateGroup> createMaterialStateGroup2()
{
    auto shaderSet = vsg::createPhongShaderSet();

    auto gpc = vsg::GraphicsPipelineConfigurator::create(shaderSet);

    struct SetPipelineStates : public vsg::Visitor
    {

        SetPipelineStates() = default;

        void apply(vsg::Object& object) override
        {
            object.traverse(*this);
        }

        void apply(vsg::RasterizationState& rs) override
        {
            rs.cullMode = VK_CULL_MODE_NONE;
        }

        void apply(vsg::InputAssemblyState& ias) override
        {
            ias.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        }

        void apply(vsg::ColorBlendState& cbs) override
        {
            //cbs.configureAttachments(si.blending);
        }
    } sps;

    gpc->accept(sps);

    float sz = 30;
    auto vertices = vsg::vec3Array::create({{0.0f, sz, 0.0f},
                                            {-sz, -sz, 0.0f},
                                            {sz, -sz, 0.0f}});

    auto normals = vsg::vec3Array::create(3);
    for (auto& n : *normals) n = {0, 0, 1};

    auto colors = vsg::vec4Array::create({{1, 0, 0, 1},
                                          {1, 0, 0, 1},
                                          {1, 0, 0, 1}});

    auto indices = vsg::ushortArray::create({0, 1, 2});

    vsg::DataList vertexArrays;
    gpc->assignArray(vertexArrays, "vsg_Vertex", VK_VERTEX_INPUT_RATE_VERTEX, vertices);
    gpc->assignArray(vertexArrays, "vsg_Normal", VK_VERTEX_INPUT_RATE_VERTEX, normals);
    gpc->assignArray(vertexArrays, "vsg_Color", VK_VERTEX_INPUT_RATE_VERTEX, colors);

    auto vid = vsg::VertexIndexDraw::create();
    vid->assignArrays({vertices, normals, colors});
    vid->assignIndices(indices);
    vid->indexCount = 3;
    vid->instanceCount = 1;

    //auto drawCommands = vsg::Commands::create();
    //drawCommands->addChild(vsg::BindVertexBuffers::create(gpc->baseAttributeBinding, vertexArrays));
    //drawCommands->addChild(vsg::BindIndexBuffer::create(indices));
    //drawCommands->addChild(vsg::DrawIndexed::create(indices->size(), 1, 0, 0, 0));

    // ´´½¨²ÄÖÊ
    auto material = vsg::PhongMaterialValue::create();
    material->value().diffuse = vsg::vec4(1.0f, 0.5f, 0.5f, 1.0f);

    gpc->assignDescriptor("material", material);

    gpc->init();

    //vsg::StateCommands stateCommands;
    //gpc->copyTo(stateCommands);

    auto stateGroup = vsg::StateGroup::create();
    gpc->copyTo(stateGroup);
    stateGroup->addChild(vid);

    //stateGroup->stateCommands = stateCommands;
    //stateGroup->prototypeArrayState = gpc->getSuitableArrayState();

    return stateGroup;
}
#endif

vsg::ref_ptr<vsg::Node> createTriangle()
{
    float sz = 30;
    float z = 10;
    auto vertices = vsg::vec3Array::create({{0.0f, sz, z},
                                            {-sz, -sz, z},
                                            {sz, -sz, z}});

    auto normals = vsg::vec3Array::create(3);
    for (auto& n : *normals) n = {0, 0, 1};

    auto colors = vsg::vec4Array::create({{0, 1, 0, 1}});

    auto indices = vsg::ushortArray::create({0, 1, 2});

    auto vid = vsg::VertexIndexDraw::create();

    vid->assignArrays({vertices, normals, colors});
    vid->assignIndices(indices);

    vid->indexCount = 3;
    vid->instanceCount = 1;
    return vid;
}

vsg::ref_ptr<vsg::Node> createQuad()
{
    float sz = 50;
    float z = 4;
    auto vertices = vsg::vec3Array::create({{-sz, sz, z},
                                            {sz, sz, z},
                                            {sz, -sz, z},
                                            {-sz, -sz, z}});

    auto normals = vsg::vec3Array::create(4);
    for (auto& n : *normals) n = {0, 0, 1};

    auto colors = vsg::vec4Array::create({{1, 1, 0, 1}});

    auto indices = vsg::ushortArray::create({0, 1, 2,
                                             2, 3, 0});

    auto vid = vsg::VertexIndexDraw::create();

    vid->assignArrays({vertices, normals, colors});
    vid->assignIndices(indices);

    vid->indexCount = 6;
    vid->instanceCount = 1;
    return vid;
}

vsg::ref_ptr<vsg::Group> test3()
{
    auto root = vsg::MatrixTransform::create(vsg::translate(500.0, 1.0, 10.0));

    auto materialGroup = createMaterialStateGroup();

    materialGroup->addChild(createTriangle());
    materialGroup->addChild(createQuad());

    root->addChild(materialGroup); 

    //auto materialGroup2 = createMaterialStateGroup2();
    //root->addChild(materialGroup2);
    return root; 
}