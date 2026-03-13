#include "IntersectionHandler.h"

#include "Node.h"

#include <iostream>
#include <vsg/ui/KeyEvent.h>
#include <vsg/all.h>
using namespace vsg;

vsg::ref_ptr<vsg::Duplicate> duplicate;
vsg::ref_ptr<vsg::ShaderSet> shaderSet_;
vsg::ref_ptr<vsg::BindGraphicsPipeline> bindPipeline;

class FindBindPipeLine : public vsg::ConstVisitor
{
public:
    void apply(const vsg::Node& node) override
    {
        node.traverse(*this);
    }


    void apply(const vsg::BindGraphicsPipeline& bind) override
    {
        vsg::BindGraphicsPipeline* bd = const_cast<vsg::BindGraphicsPipeline*>(&bind);
        bindPipeline = bd->cast<vsg::BindGraphicsPipeline>();
    }
};

vsg::ref_ptr<vsg::Group> createFromId(int id, bool highLight)
{
    auto nodeDef = Scene::instance().getNode(id);
    if (!nodeDef)
        return nullptr;

    vsg::ref_ptr<NodeDefine> node(new NodeDefine);

#if 0
    node->position = vsg::vec3Array::create(nodeDef->position->size());
    for (int i = 0; i < nodeDef->position->size(); ++i)
    {
        node->position->set(i, nodeDef->position->at(i) + vsg::vec3(1,1,1));
    }

    node->normals = vsg::vec3Array::create(nodeDef->normals->size());
    for (int i = 0; i < nodeDef->normals->size(); ++i)
    {
        node->normals->set(i, nodeDef->normals->at(i));
    }

    node->colors = vsg::vec4Array::create(nodeDef->colors->size());
    for (int i = 0; i < nodeDef->colors->size(); ++i)
    {
        node->colors->set(i, nodeDef->colors->at(i));
    }

    node->indices = vsg::ushortArray::create(nodeDef->indices->size());
    for (int i = 0; i < nodeDef->indices->size(); ++i)
    {
        node->indices->set(i, nodeDef->indices->at(i));
    }
#else
    node->position = nodeDef->position;
    node->colors = nodeDef->colors;
    node->normals = nodeDef->normals;
    node->indices = nodeDef->indices;

#endif 
    node->material.diffuse.set(0.f, 0, 1.f, 1.0f);

    if (highLight)
    {
        node->depthBiasSlopeFactor = -1;
        node->depthBiasConstantFactor = -1;
    }
    Scene::instance().addNode(node);

    vsg::ref_ptr<vsg::Group> group = convert(*node);
    return group;
}

vsg::ref_ptr<vsg::Node> createHighlight(vsg::ref_ptr<vsg::LineSegmentIntersector::Intersection> intersection)
{
    const vsg::VertexIndexDraw* drawCopy = nullptr;

    std::vector<const Node*> nodes = intersection->nodePath;
    std::reverse(nodes.begin(), nodes.end());

    int id = 0;
    for (auto* node : nodes)
    {
        if (node && node->is_compatible(typeid(vsg::StateGroup)))
        {
            auto group = node->cast<vsg::StateGroup>();

            if (group->getValue("id", id))
            {
               // return vsg::ref_ptr<vsg::Node>(const_cast<StateGroup*>(group));
                break;
            }
        }
    }

    auto stateGroup = createFromId(id, true);

    if (!stateGroup) return nullptr;

    vsg::ref_ptr<vsg::Node> node2 = stateGroup;

    for (auto itr = intersection->nodePath.rbegin();
         itr != intersection->nodePath.rend(); ++itr)
    {
        if (auto mt = dynamic_cast<const vsg::MatrixTransform*>(*itr))
        {
            auto newMT = vsg::MatrixTransform::create(mt->matrix);
            newMT->addChild(node2);
            node2 = newMT;
        }
    }

    return node2;
}

#if 0
vsg::ref_ptr<vsg::Node> createHighlight3(vsg::ref_ptr<vsg::LineSegmentIntersector::Intersection> intersection)
{
    if (!shaderSet_) shaderSet_ = vsg::createPhongShaderSet();
    const vsg::VertexIndexDraw* drawCopy = nullptr;


    for (auto itr = intersection->nodePath.rbegin();
         itr != intersection->nodePath.rend(); ++itr)
    {
        drawCopy = dynamic_cast<const vsg::VertexIndexDraw*>(*itr);
        if (drawCopy) break;
    }

    //for (auto* node : nodes)
    //{
    //    if (bindPipeline) break;
    //    FindBindPipeLine find;
    //    node->accept(find);
    //}

    if (!drawCopy) return {};

    VertexIndexDraw* drawCopy2 = const_cast<VertexIndexDraw*>(drawCopy);
    auto refNode = vsg::ref_ptr<vsg::VertexIndexDraw>(drawCopy2);

    auto stateGroup = vsg::StateGroup::create();
    stateGroup->addChild(refNode->clone().cast<vsg::Node>());
    stateGroup->addChild(refNode);

#if 1
    // ---- ÐÂ²ÄÖÊ ----
    //auto highlightMaterial = vsg::PhongMaterialValue::create();

    //highlightMaterial->value().ambient.set(0.0f, 0.3f, 0.0f, 1.0f);
    //highlightMaterial->value().diffuse.set(0.0f, 1.0f, 0.0f, 1.0f); // ÂÌÉ«
    //highlightMaterial->value().specular.set(0.5f, 1.0f, 0.5f, 1.0f);
    //highlightMaterial->value().shininess = 64.0f;

    auto config = vsg::GraphicsPipelineConfigurator::create(shaderSet_);

    auto mat = vsg::PhongMaterialValue::create();
    mat->value().diffuse.set(0, 1, 0, 1);

    auto array = drawCopy->arrays;
    vsg::DataList datas;
    for (auto info : array)
    {
        datas.push_back(info->data);
    }
    config->assignDescriptor("material", mat);

    return stateGroup;

    //vsg::ref_ptr<vsg::vec3Array> normals;
    //auto vertices = array[0]->data.cast<vsg::vec3Array>();
    //auto colors = array[1]->data.cast<vsg::vec4Array>();

    //if (array.size() >= 3)
    //    normals = array[2]->data.cast<vsg::vec3Array>();
    //auto indices = drawCopy->indices->data.cast<vsg::ushortArray>();

    //NodeDefine nodeDef;
    //nodeDef.position = vertices;
    //nodeDef.normals = normals;
    //nodeDef.colors = colors;
    //nodeDef.indices = indices;
    //nodeDef.material = vsg::PhongMaterial{};

    //nodeDef.material.diffuse.set(0.f, 0, 1.f, 1.0f);

    

   // return stateGroup;
    //vsg::DataList vertexArrays;
    //config->assignArray(vertexArrays, "vsg_Vertex", VK_VERTEX_INPUT_RATE_VERTEX, vertices);
    //config->assignArray(vertexArrays, "vsg_Normal", VK_VERTEX_INPUT_RATE_VERTEX, normals);
    //config->assignArray(vertexArrays, "vsg_Color", /**/ VK_VERTEX_INPUT_RATE_INSTANCE, colors);

    //config->init();
    //config->copyTo(stateGroup);
#endif

    //auto highlightMaterial = vsg::PhongMaterialValue::create();
    //highlightMaterial->value().diffuse.set(0, 1, 0, 1);

    //auto descriptor = vsg::DescriptorBuffer::create(highlightMaterial, 0);

    //if (!bindPipeline) return {};

    //ref_ptr<PipelineLayout> layout = bindPipeline->pipeline->layout;
    //std::vector<vsg::ref_ptr<vsg::DescriptorSetLayout>> setLayout = layout->setLayouts;

    //for (int i = 0; i < setLayout.size(); ++i)
    //{
    //    auto bindings = setLayout[i]->bindings;
    //    // for (auto& binding : bindings)
    //    //{
    //    //if (binding.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER || binding.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC)
    //    //{
    //    auto descriptorSet = vsg::DescriptorSet::create(setLayout[i], vsg::Descriptors{descriptor});
    //    stateGroup->add(
    //        vsg::BindDescriptorSet::create(
    //            VK_PIPELINE_BIND_POINT_GRAPHICS,
    //            layout,
    //            0, // set index
    //            descriptorSet));
    //    // }
    //    //}
    //    break;
    //}

    // ·ÀÖ¹ Z-fighting

    //stateGroup->add(depthBias);
 //   auto stateGroup = createFromId(id);

  //  if (!stateGroup) return nullptr;

  /*  vsg::ref_ptr<vsg::Node> node2 = stateGroup;

    for (auto itr = intersection->nodePath.rbegin();
         itr != intersection->nodePath.rend(); ++itr)
    {
        if (auto mt = dynamic_cast<const vsg::MatrixTransform*>(*itr))
        {
            auto newMT = vsg::MatrixTransform::create(mt->matrix);
            newMT->addChild(node2);
            node2 = newMT;
        }
    }

    return node2;*/
}
#endif

void IntersectionHandler::apply(vsg::KeyPressEvent& keyPress)
{
    if (lastPointerEvent)
    {
        intersection_LineSegmentIntersector(*lastPointerEvent);
        if (!lastIntersection) return;

        vsg::info("keyPress.keyModifier = ", keyPress.keyModifier, " keyPress.keyBase = ", keyPress.keyBase);

        geom.dx.set(static_cast<float>(scale), 0.0f, 0.0f);
        geom.dy.set(0.0f, static_cast<float>(scale), 0.0f);
        geom.dz.set(0.0f, 0.0f, static_cast<float>(scale));

        if (keyPress.keyModifier & vsg::MODKEY_Control)
        {
            // when we press the ctrl key we want to enable billboarding of the created shapes
            state.billboard = true;

            // when billboarding the position is the pivot point in local coordinates
            geom.position.set(0.0f, 0.0f, 0.0f);

            // the position is set by positions data, in this case just one position so use a vec4Value, but if needed we can use an array of positions
            auto pos = vsg::vec3(lastIntersection->worldIntersection);
            geom.positions = vsg::vec4Value::create(vsg::vec4(pos.x, pos.y, pos.z, static_cast<float>(scale * 5.0))); // x,y,z and scaleDistance
        }
        else
        {
            geom.position = vsg::vec3(lastIntersection->worldIntersection);
        }

        if (keyPress.keyBase == 'b')
        {
            scenegraph->addChild(builder->createBox(geom, state));
        }
        else if (keyPress.keyBase == 'q')
        {
            scenegraph->addChild(builder->createQuad(geom, state));
        }
        else if (keyPress.keyBase == 'c')
        {
            scenegraph->addChild(builder->createCylinder(geom, state));
        }
        else if (keyPress.keyBase == 'p')
        {
            scenegraph->addChild(builder->createCapsule(geom, state));
        }
        else if (keyPress.keyBase == 's')
        {
            scenegraph->addChild(builder->createSphere(geom, state));
        }
        else if (keyPress.keyBase == 'n')
        {
            scenegraph->addChild(builder->createCone(geom, state));
        }
    }

    if (state.billboard)
    {
        // switch off billboarding so other shapes aren't affected.
        state.billboard = false;
        geom.positions = {};
    }

    if (keyPress.keyBase == 'o')
    {
        vsg::write(scenegraph, "builder.vsgt");
    }
}

void IntersectionHandler::apply(vsg::ButtonPressEvent& buttonPressEvent)
{
    lastPointerEvent = &buttonPressEvent;

    if (buttonPressEvent.button == 1)
    {
        intersection_LineSegmentIntersector(buttonPressEvent);
    }
    else if (buttonPressEvent.button == 2)
    {
        intersection_PolytopeIntersector(buttonPressEvent);
    }
}

void IntersectionHandler::apply(vsg::PointerEvent& pointerEvent)
{
    lastPointerEvent = &pointerEvent;
}


void IntersectionHandler::intersection_LineSegmentIntersector(vsg::PointerEvent& pointerEvent)
{
    auto intersector = vsg::LineSegmentIntersector::create(*camera, pointerEvent.x, pointerEvent.y);

    auto before_intersection = vsg::clock::now();

    scenegraph->accept(*intersector);
    highLightGroup->children = {};

    auto after_intersection = vsg::clock::now();

    if (verbose)
    {
        std::cout << "\nintersection_LineSegmentIntersector(" << pointerEvent.x << ", " << pointerEvent.y << ") " << intersector->intersections.size() << ")";
        std::cout << "time = " << std::chrono::duration<double, std::chrono::milliseconds::period>(after_intersection - before_intersection).count() << "ms" << std::endl;
    }

    if (intersector->intersections.empty()) return;

    // sort the intersections front to back
    std::sort(intersector->intersections.begin(), intersector->intersections.end(), [](auto& lhs, auto& rhs) { return lhs->ratio < rhs->ratio; });

    for (auto& intersection : intersector->intersections)
    {
        auto node = createHighlight(intersection);
        if (node)
        {
            viewer->compileManager->compile(node);
            highLightGroup->addChild(node);
        }

        for (auto& node : intersection->nodePath)
        {
            std::string name;
            std::cout << ", " << node->className();
            if (node->getValue("name", name)) std::cout << ":name=" << name;
        }

        std::cout << "\n\n";
        break;
    }

    lastIntersection = intersector->intersections.front();
}

void IntersectionHandler::intersection_PolytopeIntersector(vsg::PointerEvent& pointerEvent)
{
    double size = 5.0;
    double xMin = static_cast<double>(pointerEvent.x) - size;
    double xMax = static_cast<double>(pointerEvent.x) + size;
    double yMin = static_cast<double>(pointerEvent.y) - size;
    double yMax = static_cast<double>(pointerEvent.y) + size;

    auto intersector = vsg::PolytopeIntersector::create(*camera, xMin, yMin, xMax, yMax);
    scenegraph->accept(*intersector);

    if (verbose) std::cout << "intersection_PolytopeIntersector(" << pointerEvent.x << ", " << pointerEvent.y << ") " << intersector->intersections.size() << ")" << std::endl;

    if (intersector->intersections.empty()) return;
}