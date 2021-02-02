#include "sfml-engine/physics/physicsbody.h"
#include "sfml-engine/physics/physicsworld.h"
#include "sfml-engine/mathutils.h"

#include <iostream>


gbh::PhysicsMaterial::PhysicsMaterial(float density, float restitution, float friction)
{
    this->density = density;
    this->restitution = restitution;
    this->friction = friction;
}


gbh::PhysicsBody::PhysicsBody(PhysicsWorld* world, const sf::Vector2f& position) :
    m_world(world)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.type = b2_dynamicBody;
    
    m_boxBody = m_world->getBoxWorld()->CreateBody(&bodyDef);
    m_boxBody->SetUserData(this);
}


gbh::PhysicsBody::~PhysicsBody()
{
    destroy();
}


void gbh::PhysicsBody::addCircle(float radius, const sf::Vector2f position, const PhysicsMaterial& material)
{
    if (m_boxBody == nullptr)
    {
        std::cout << "Attempted to add a circle fixture to a null body. Aborting.\n";
        return;
    }
    
    b2CircleShape shape;
    shape.m_radius = radius / m_world->getPixelsPerMeter();
    shape.m_p = b2Vec2(position.x, -position.y);
    shape.m_p *= 1.0f / m_world->getPixelsPerMeter();
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = material.density;
    fixtureDef.restitution = material.restitution;
    fixtureDef.friction = material.friction;
    
    b2Fixture* fixture = m_boxBody->CreateFixture(&fixtureDef);
    
    if (fixture != nullptr)
    {
        m_fixtures.push_back(fixture);
    }
}


void gbh::PhysicsBody::addBox(const sf::Vector2f& size, const sf::Vector2f position, const PhysicsMaterial& material)
{
    if (m_boxBody == nullptr)
    {
        std::cout << "Attempted to add a box fixture to a null body. Aborting.\n";
        return;
    }
    
    // box2d expects half-size here
    b2Vec2 boxSize = b2Vec2(size.x * 0.5f, size.y * 0.5f);
    boxSize *= (1.0f / m_world->getPixelsPerMeter());
    
    b2Vec2 boxPos = b2Vec2(position.x, -position.y);
    boxPos *= (1.0f / m_world->getPixelsPerMeter());
    
    b2PolygonShape shape;
    shape.SetAsBox(boxSize.x, boxSize.y, boxPos, 0.f);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = material.density;
    fixtureDef.restitution = material.restitution;
    fixtureDef.friction = material.friction;
    
    b2Fixture* fixture = m_boxBody->CreateFixture(&fixtureDef);
    
    if (fixture != nullptr)
    {
        m_fixtures.push_back(fixture);
    }
}


void gbh::PhysicsBody::addEdgeBox(const sf::Vector2f& size, const sf::Vector2f position, const PhysicsMaterial& material)
{
    if (m_boxBody == nullptr)
    {
        std::cout << "Attempted to add a box fixture to a null body. Aborting.\n";
        return;
    }
    
    if (abs(position.x) > 0.001f || abs(position.y) > 0.001f )
    {
        std::cout << "Edge Box does not support position yet.\n";
    }

    b2Vec2 vertices[4];
    vertices[0].Set(-size.x * 0.5f, -size.y * 0.5f);
    vertices[1].Set( size.x * 0.5f, -size.y * 0.5f);
    vertices[2].Set( size.x * 0.5f,  size.y * 0.5f);
    vertices[3].Set(-size.x * 0.5f,  size.y * 0.5f);
    
    float ppm = m_world->getPixelsPerMeter();
    
    for(int i = 0; i < 4; ++i) {
        vertices[i].x = vertices[i].x / ppm;
        vertices[i].y = vertices[i].y / ppm;
    }
    
    for(int i = 0; i < 4; ++i) {
        b2EdgeShape shape;
        shape.SetTwoSided(vertices[i], vertices[(i+1) % 4]);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = material.density;
        fixtureDef.restitution = material.restitution;
        fixtureDef.friction = material.friction;
        
        b2Fixture* fixture = m_boxBody->CreateFixture(&fixtureDef);
        
        if (fixture != nullptr)
        {
            m_fixtures.push_back(fixture);
        }
    }
}


sf::Vector2f gbh::PhysicsBody::getPosition()
{
    b2Vec2 position = m_boxBody->GetPosition();
    float ppm = m_world->getPixelsPerMeter();
    return sf::Vector2f(position.x * ppm, position.y * ppm);
}


float gbh::PhysicsBody::getAngle()
{
    return boxAngleToSfml(m_boxBody->GetAngle());
}


void gbh::PhysicsBody::setTransform(const sf::Vector2f& position, float angle)
{
    if (m_boxBody)
    {
        float ppm = m_world->getPixelsPerMeter();
        float boxAngle = sfmlAngleToBox(angle);
        m_boxBody->SetTransform(b2Vec2(position.x / ppm, position.y / ppm), boxAngle);
    }
}


void gbh::PhysicsBody::setType(gbh::PhysicsBodyType type)
{
    switch(type)
    {
        case gbh::PhysicsBodyType::Static:
            m_boxBody->SetType(b2_staticBody);
            break;
            
        case gbh::PhysicsBodyType::Kinematic:
            m_boxBody->SetType(b2_kinematicBody);
            break;
            
        case gbh::PhysicsBodyType::Dynamic:
            m_boxBody->SetType(b2_dynamicBody);
            break;
    }
}


void gbh::PhysicsBody::makeSensor()
{
    if (m_boxBody == nullptr) {
        return;
    }
    
    for (b2Fixture* f = m_boxBody->GetFixtureList(); f; f = f->GetNext())
    {
        f->SetSensor(true);
    }
}


sf::Vector2f gbh::PhysicsBody::getLinearVelocity() const
{
    return m_world->boxVectorToSfmlWorld(m_boxBody->GetLinearVelocity());
}


void gbh::PhysicsBody::setLinearVelocity(const sf::Vector2f& value)
{
    m_boxBody->SetLinearVelocity(m_world->sfmlVectorToBoxWorld(value));
}


float gbh::PhysicsBody::getAngularVelocity() const
{
    return boxAngleToSfml(m_boxBody->GetAngularVelocity());
}


void gbh::PhysicsBody::setAngularVelocity(float value)
{
    m_boxBody->SetAngularVelocity(sfmlAngleToBox(value));
}


void gbh::PhysicsBody::applyForceToCenter(const sf::Vector2f& force, bool wake)
{
    m_boxBody->ApplyForceToCenter(m_world->sfmlVectorToBoxWorld(force), wake);
}


void gbh::PhysicsBody::applyForce(const sf::Vector2f& force, const sf::Vector2f& point, bool wake)
{
    m_boxBody->ApplyForce(m_world->sfmlVectorToBoxWorld(force), m_world->sfmlVectorToBoxWorld(point), wake);
}


void gbh::PhysicsBody::applyTorque(float torque, bool wake)
{
    m_boxBody->ApplyTorque(sfmlAngleToBox(torque), wake);
}


void gbh::PhysicsBody::applyImpulseToCenter(const sf::Vector2f& impulse, bool wake)
{
    m_boxBody->ApplyLinearImpulseToCenter(m_world->sfmlVectorToBoxWorld(impulse), wake);
}


void gbh::PhysicsBody::applyImpulse(const sf::Vector2f& impulse, const sf::Vector2f& point, bool wake)
{
    m_boxBody->ApplyLinearImpulse(m_world->sfmlVectorToBoxWorld(impulse), m_world->sfmlVectorToBoxWorld(point), wake);
}


void gbh::PhysicsBody::applyAngularImpulse(float impulse, bool wake)
{
    m_boxBody->ApplyAngularImpulse(sfmlAngleToBox(impulse), wake);
}



void gbh::PhysicsBody::destroy()
{
    if (m_boxBody != nullptr)
    {
        m_world->getBoxWorld()->DestroyBody(m_boxBody);
        m_boxBody = nullptr;
    }
}


float gbh::PhysicsBody::boxAngleToSfml(float angle)
{
    return -gbh::math::radiansToDegrees(angle);
}


float gbh::PhysicsBody::sfmlAngleToBox(float angle)
{
    return -gbh::math::degreesToRadians(angle);
}

