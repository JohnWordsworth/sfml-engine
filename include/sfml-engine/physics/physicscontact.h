#pragma once

#include "physicsbody.h"

#include <SFML/System.hpp>
#include <box2d/box2d.h>


namespace gbh
{
    class Node;

    enum class ContactEventType
    {
        BeginContact,
        EndContact
    };


    struct PhysicsContact
    {
        Node* nodeA = nullptr;
        Node* nodeB = nullptr;
        PhysicsBody* bodyA = nullptr;
        PhysicsBody* bodyB = nullptr;
        b2Fixture* fixtureA = nullptr;
        b2Fixture* fixtureB = nullptr;
        
        // Returns the other node to the provided node.
        Node* otherNode(Node* aNode) const;
        
        // Returns true if either of the nodes is equal to the one provided.
        bool containsNode(Node* aNode) const;
    };
}
