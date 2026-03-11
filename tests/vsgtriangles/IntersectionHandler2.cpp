#include "IntersectionHandler2.h"
#include "Node.h"
#include <vsg/all.h>
using namespace vsg;
using namespace std;

void makeColorDynamic(vsg::Node* node);


void IntersectionHandler2::selectNode(vsg::Node* node)
{
    if (!node) return;
    makeColorDynamic(node);
    setColor((vsg::Node*)node, selected_color);
    //for (auto& n : _objectNodes)
    //{
    //    vsg::vec4 color = node == n ? selected_color : default_color;
    //    setColor((vsg::Node*)n, color);
    //}
}

void IntersectionHandler2::intersection(vsg::PointerEvent& pointerEvent)
{
    auto intersector = vsg::LineSegmentIntersector::create(*_camera, pointerEvent.x, pointerEvent.y);
    _scene->accept(*intersector);
    // fmt::print("Intersected {} objects\n", intersector->intersections.size());
    if (intersector->intersections.empty()) return;

    // sort the intersections front to back
    std::sort(intersector->intersections.begin(),
              intersector->intersections.end(),
              [](auto& lhs, auto& rhs) { return lhs->ratio<rhs->ratio; });

    auto& intersection = intersector->intersections[0];
    for (const vsg::Node* node : intersection->nodePath)
    {
       // std::string name;
        //            std::cout << "className=" << node->className();
        //node->getValue("name", name);

        // check if the node is in the set of selectable objects
        //if (_objectNodes.find((vsg::Node*)node) != _objectNodes.end())
        {
            // fmt::print("Found selectable object {}\n", name);
            selectNode(const_cast<vsg::Node*>(node));
            break;
        }
    }
}

void IntersectionHandler2::setColor(vsg::Node* node,
                                    vsg::vec4 color)
{
    if (!node) return;
    struct ChangeColor : public vsg::Visitor
    {
        ChangeColor(vsg::vec4 color) :
            _color(color)
        {
        }

        void apply(vsg::Object& object) override
        {
            object.traverse(*this);
        }

        void apply(vsg::VertexIndexDraw& vid) override
        {
            for (auto& buffer : vid.arrays)
            {
                auto data = buffer->data;

                if (data->is_compatible(typeid(vsg::vec4Array))) 
                {
                    // Change the color to green
                    (*(vsg::vec4*)(data->dataPointer(0))) = _color;
                    //   fmt::print("Changed color to ({},{},{},{})\n",
                    //          _color.r, _color.g, _color.b, _color.a);
                    data->dirty();
                }
            }
        }

        vsg::vec4 _color;
    };

    ChangeColor cc(color);
    node->accept(cc);
}


// Change the property of the color mode of the list of node to be
// dynamic. This is necessary in order to change the color when selecting
// them.
void makeColorDynamic(vsg::Node* node)
{
    if (!node) return;

    static std::set<vsg::Node*> sets;

    if (sets.find(node) != sets.end()) return;

    sets.insert(node);


    struct ChangeDynamicProperty : public vsg::Visitor
    {
        ChangeDynamicProperty()
        {
        }

        void apply(vsg::Object& object) override
        {
            object.traverse(*this);
        }

        void apply(vsg::VertexIndexDraw& vid) override
        {
            for (auto& buffer : vid.arrays)
            {
                auto data = buffer->data;

                if (data->is_compatible(typeid(vsg::vec4Array)))
                {
                    // Change the property to dynamic
                    data->properties.dataVariance = vsg::DYNAMIC_DATA;
                    data->dirty();
                }
            }
        }

        vsg::vec4 _color;
    };

    ChangeDynamicProperty cdp;
    node->accept(cdp);
}