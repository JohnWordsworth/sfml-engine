#pragma once

#include <SFML/System.hpp>
#include <box2d/box2d.h>
#include <memory>
#include <vector>



namespace gbh
{
    class Node;
    class PhysicsWorld;

    enum class PhysicsBodyType
    {
        Static,
        Kinematic,
        Dynamic
    };

    struct PhysicsMaterial
    {
        PhysicsMaterial() { };
        PhysicsMaterial(float density, float restitution, float friction);
        
        float density = 1.0f;
        float restitution = 0.9f;
        float friction = 0.0f;
    };

    class PhysicsBody
    {
        friend class Node;
        
    public:
        PhysicsBody(PhysicsWorld* world, const sf::Vector2f& position);
        ~PhysicsBody();
        
        void addCircle(float radius, const sf::Vector2f position = sf::Vector2f(), const PhysicsMaterial& material = PhysicsMaterial());
        void addBox(const sf::Vector2f& size, const sf::Vector2f position = sf::Vector2f(), const PhysicsMaterial& material = PhysicsMaterial());
        void addEdgeBox(const sf::Vector2f& size, const sf::Vector2f position = sf::Vector2f(), const PhysicsMaterial& material = PhysicsMaterial());
        
        sf::Vector2f getPosition();
        float getAngle();
        
        PhysicsWorld* getWorld() { return m_world; }
        
        void setTransform(const sf::Vector2f& position, float angle);
        void setType(PhysicsBodyType type);
        
        void destroy();
        
    private:
        PhysicsWorld* m_world = nullptr;
        b2Body* m_boxBody = nullptr;
        std::vector<b2Fixture*> m_fixtures;
    };
}
