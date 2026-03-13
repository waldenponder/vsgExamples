#include "FNode.h"

FNode::FNode()
{
}

bool FNode::visible() const
{
    return node_state_.visible();
}

void FNode::setVisible(bool v)
{
    return node_state_.setVisible(v);
}