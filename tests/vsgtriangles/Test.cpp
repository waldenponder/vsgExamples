#include "Test.h"

#include "tsl/robin_map.h"

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
    gpc->enableArray("vsg_Translation", VK_VERTEX_INPUT_RATE_INSTANCE, 12);

    auto material = vsg::PhongMaterialValue::create();
    material->value().diffuse = vsg::vec4(1.0f, 1.f, 1.f, 1.0f);

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

    auto colors = vsg::vec4Array::create({{1, 1, 0, 1} , {0, 1, 1, 1}});

    auto indices = vsg::ushortArray::create({0, 1, 2});

    auto instanceMatrices = vsg::mat4Array::create(2);
    (*instanceMatrices)[0] = vsg::translate(-320.0f, 0.0f, 10.0f);
    (*instanceMatrices)[1] = vsg::translate(320.0f, 0.0f, 10.0f);

    auto vid = vsg::VertexIndexDraw::create();
    vid->assignArrays({vertices, normals, colors, instanceMatrices});
    vid->assignIndices(indices);

    vid->indexCount = 3;
    vid->instanceCount = 2;
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

    auto colors = vsg::vec4Array::create({{1, 0, 0, 1}, {0, 1, 0, 1}});

    auto indices = vsg::ushortArray::create({0, 1, 2,
                                             2, 3, 0});
    auto instanceMatrices = vsg::mat4Array::create(2);
    (*instanceMatrices)[0] = vsg::translate(-200.0f, 0.0f, 10000.0f);
    (*instanceMatrices)[1] = vsg::translate(200.0f, 1000.0f, 0.0f);

    auto vid = vsg::VertexIndexDraw::create();

    vid->assignArrays({vertices, normals, colors, instanceMatrices});
    vid->assignIndices(indices);

    vid->indexCount = 6;
    vid->instanceCount = 2;
    return vid;
}

static const size_t N = 1000000;

std::vector<int> generate_keys(size_t n)
{
    std::vector<int> keys(n);
    for (size_t i = 0; i < n; i++)
        keys[i] = i;

    shuffle(keys.begin(), keys.end(), std::mt19937{std::random_device{}()});
    return keys;
}
using namespace std;
template<typename Map>
void test_map(const string& name, const vector<int>& keys)
{
    Map m;

    auto t1 = chrono::high_resolution_clock::now();

    // insert
    for (size_t i = 0; i < keys.size(); i++)
        m[keys[i]] = i;

    auto t2 = chrono::high_resolution_clock::now();

    // find
    for (auto k : keys)
    {
        auto it = m.find(k);
        if (it == m.end())
            cout << "error\n";
    }

    auto t3 = chrono::high_resolution_clock::now();

    // iterate
    long long sum = 0;
    for (auto& kv : m)
        sum += kv.second;

    auto t4 = chrono::high_resolution_clock::now();

    // erase
    for (auto k : keys)
        m.erase(k);

    auto t5 = chrono::high_resolution_clock::now();

    auto insert_time =
        chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();

    auto find_time =
        chrono::duration_cast<chrono::milliseconds>(t3 - t2).count();

    auto iterate_time =
        chrono::duration_cast<chrono::milliseconds>(t4 - t3).count();

    auto erase_time =
        chrono::duration_cast<chrono::milliseconds>(t5 - t4).count();

    cout << "==== " << name << " ====\n";
    cout << "insert  : " << insert_time << " ms\n";
    cout << "find    : " << find_time << " ms\n";
    cout << "iterate : " << iterate_time << " ms\n";
    cout << "erase   : " << erase_time << " ms\n";
    cout << "sum     : " << sum << "\n";
    cout << endl;
}


vsg::ref_ptr<vsg::Group> test3()
{
#if 0
    std::cout << "generate keys...\n";

    auto keys = generate_keys(N);

    test_map<std::unordered_map<int, int>>(
        "std::unordered_map", keys);

    test_map<std::map<int, int>>(
        "std::map", keys);

    test_map<std::unordered_map<int, int>>(
        "std::unordered_map", keys);

    test_map<tsl::robin_map<int, int>>(
        "tsl::robin_map", keys);

    test_map<std::map<int, int>>(
        "std::map", keys);

    test_map<tsl::robin_map<int, int>>(
        "tsl::robin_map", keys);
#endif

    auto root = vsg::MatrixTransform::create(vsg::translate(500.0, 1.0, 10.0));

    auto materialGroup = createMaterialStateGroup();

    materialGroup->addChild(createTriangle());
    materialGroup->addChild(createQuad());

    root->addChild(materialGroup); 

    //auto materialGroup2 = createMaterialStateGroup2();
    //root->addChild(materialGroup2);
    return root; 
}