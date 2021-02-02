#include "sfml-engine/physics/physicscontact.h"


gbh::Node* gbh::PhysicsContact::otherNode(gbh::Node* aNode) const
{
    if (nodeA == aNode)
    {
        return nodeB;
    }
    else if (nodeB == aNode)
    {
        return nodeA;
    }
    
    return nullptr;
}


bool gbh::PhysicsContact::containsNode(gbh::Node* aNode) const
{
    return ((nodeA == aNode) || (nodeB == aNode));
}
