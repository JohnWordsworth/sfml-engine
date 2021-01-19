#include "sfml-engine/physics/physicsworld.h"

#include "sfml-engine/physics/physicsbody.h"


gbh::PhysicsWorld::PhysicsWorld(const sf::Vector2f gravity)
{
    b2Vec2 b2Gravity { gravity.x, gravity.y };
    m_boxWorld = std::make_unique<b2World>(b2Gravity);
}


gbh::PhysicsWorld::~PhysicsWorld()
{
    destroy();
}


std::shared_ptr<gbh::PhysicsBody> gbh::PhysicsWorld::createEmptyBody()
{
    std::shared_ptr<gbh::PhysicsBody> body = std::make_shared<gbh::PhysicsBody>(this, sf::Vector2f());
    m_bodies.push_back(body);

    return body;
}


std::shared_ptr<gbh::PhysicsBody> gbh::PhysicsWorld::createBox(const sf::Vector2f& size, const PhysicsMaterial& material)
{
    std::shared_ptr<gbh::PhysicsBody> body = createEmptyBody();
    body->addBox(size, sf::Vector2f(), material);
    
    return body;
}


std::shared_ptr<gbh::PhysicsBody> gbh::PhysicsWorld::createCircle(float radius, const PhysicsMaterial& material)
{
    std::shared_ptr<gbh::PhysicsBody> body = createEmptyBody();
    body->addCircle(radius, sf::Vector2f(), material);
    
    return body;
}


std::shared_ptr<gbh::PhysicsBody> gbh::PhysicsWorld::createEdgeBox(const sf::Vector2f& size, const PhysicsMaterial& material)
{
    std::shared_ptr<gbh::PhysicsBody> body = createEmptyBody();
    body->addEdgeBox(size, sf::Vector2f(), material);

    return body;
}


void gbh::PhysicsWorld::simulate(double deltaTime)
{
    m_actualTime += (float)deltaTime;
    
    while (m_simulatedTime + m_stepTime < m_actualTime)
    {
        m_boxWorld->Step(m_stepTime, 8, 3);
        m_simulatedTime += m_stepTime;
    }
}


void gbh::PhysicsWorld::destroy()
{
    for(int i = 0; i < m_bodies.size(); ++i)
    {
        m_bodies[i]->destroy();
    }
    
    m_bodies.clear();
}
