#include "sfml-engine/physics/physicsworld.h"

#include "sfml-engine/physics/physicsbody.h"


//----------------------
// ContactEvent
//----------------------

gbh::ContactEvent::ContactEvent(gbh::ContactEventType type, const gbh::PhysicsContact& contactData) :
    eventType(type), contact(contactData)
{
    
}


//----------------------
// PhysicsWorld
//----------------------


gbh::PhysicsWorld::PhysicsWorld(const sf::Vector2f gravity)
{
    b2Vec2 b2Gravity { gravity.x, gravity.y };
    m_boxWorld = std::make_unique<b2World>(b2Gravity);
    m_boxWorld->SetContactListener(this);
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
    m_contactEvents.clear();
    m_actualTime += (float)deltaTime;
    
    while (m_simulatedTime + m_stepTime < m_actualTime)
    {
        m_boxWorld->Step(m_stepTime, 8, 3);
        m_simulatedTime += m_stepTime;
    }

    if (m_contactListener != nullptr)
    {
        for(int i = 0; i < m_contactEvents.size(); ++i)
        {
            ContactEvent event = m_contactEvents[i];
            
            if (event.eventType == ContactEventType::BeginContact)
            {
                m_contactListener->beginContact(event.contact);
            }
            else if (event.eventType == ContactEventType::EndContact)
            {
                m_contactListener->endContact(event.contact);
            }
        }
    }
}


void gbh::PhysicsWorld::destroy()
{
    for(int i = 0; i < m_bodies.size(); ++i)
    {
        if (!m_bodies[i].expired())
        {
            std::shared_ptr<gbh::PhysicsBody> body = m_bodies[i].lock();
            body->destroy();
        }
    }
    
    m_bodies.clear();
}


b2Vec2 gbh::PhysicsWorld::sfmlVectorToBoxWorld(const sf::Vector2f& sfVector) const
{
    return b2Vec2(sfVector.x / m_pixelsPerMeter, -sfVector.y / m_pixelsPerMeter);
}


sf::Vector2f gbh::PhysicsWorld::boxVectorToSfmlWorld(const b2Vec2& boxVector) const
{
    return sf::Vector2f(boxVector.x * m_pixelsPerMeter, -boxVector.y * m_pixelsPerMeter);
}


gbh::PhysicsContact gbh::PhysicsWorld::boxContactToSfml(b2Contact* contact) const
{
    PhysicsContact sfContact;
    sfContact.bodyA = static_cast<gbh::PhysicsBody *>(contact->GetFixtureA()->GetBody()->GetUserData());
    sfContact.bodyB = static_cast<gbh::PhysicsBody *>(contact->GetFixtureB()->GetBody()->GetUserData());
    sfContact.nodeA = sfContact.bodyA->getNode();
    sfContact.nodeB = sfContact.bodyB->getNode();
    sfContact.fixtureA = contact->GetFixtureA();
    sfContact.fixtureB = contact->GetFixtureB();
    
    return sfContact;
}


void gbh::PhysicsWorld::BeginContact(b2Contact* contact)
{
    ContactEvent event = ContactEvent(ContactEventType::BeginContact, boxContactToSfml(contact));
    m_contactEvents.push_back(event);
}


void gbh::PhysicsWorld::EndContact(b2Contact* contact)
{
    ContactEvent event = ContactEvent(ContactEventType::EndContact, boxContactToSfml(contact));
    m_contactEvents.push_back(event);
}


// Not implemented for now
void gbh::PhysicsWorld::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)     { }
void gbh::PhysicsWorld::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)  { }


void gbh::PhysicsWorld::getContactList(std::vector<gbh::PhysicsContact>& list) const
{
    list.clear();
    
    for (b2Contact* c = m_boxWorld->GetContactList(); c; c = c->GetNext())
    {
        list.push_back(boxContactToSfml(c));
    }
}
