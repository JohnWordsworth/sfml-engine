#pragma once

#include "physicsbody.h"
#include "physicscontact.h"

#include <SFML/System.hpp>
#include <box2d/box2d.h>
#include <memory>
#include <vector>


namespace gbh
{
    class PhysicsContactListener
    {
    public:
        virtual void beginContact(const PhysicsContact& contact)  { }
        virtual void endContact(const PhysicsContact& contact)    { }
    };


    struct ContactEvent
    {
        ContactEvent(ContactEventType type, const PhysicsContact& contactData);
        
        ContactEventType eventType;
        PhysicsContact contact;
    };


    class PhysicsWorld : public b2ContactListener
    {
    public:
        PhysicsWorld(const sf::Vector2f gravity);
        ~PhysicsWorld();
        
        std::shared_ptr<PhysicsBody> createEmptyBody();
        std::shared_ptr<PhysicsBody> createBox(const sf::Vector2f& size, const PhysicsMaterial& material = PhysicsMaterial());
        std::shared_ptr<PhysicsBody> createCircle(float radius, const PhysicsMaterial& material = PhysicsMaterial());
        std::shared_ptr<PhysicsBody> createEdgeBox(const sf::Vector2f& size, const PhysicsMaterial& material = PhysicsMaterial());
        std::shared_ptr<PhysicsBody> createEdgeList(const std::vector<sf::Vector2f> vertices, const PhysicsMaterial& material = PhysicsMaterial());
        
        void simulate(double deltaTime);
        void destroy();
        
        b2World* getBoxWorld() const                { return m_boxWorld.get(); }
        float getPixelsPerMeter() const             { return m_pixelsPerMeter; }
                
        b2Vec2 sfmlVectorToBoxWorld(const sf::Vector2f& sfVector) const;
        sf::Vector2f boxVectorToSfmlWorld(const b2Vec2& boxVector) const;
        PhysicsContact boxContactToSfml(b2Contact* contact) const;
                
        // b2ContactListener Methods
        void BeginContact(b2Contact* contact) override;
        void EndContact(b2Contact* contact) override;
        void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
        void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
        
        void setContactListener(PhysicsContactListener* listener)       { m_contactListener = listener; }
        PhysicsContactListener* getContactListener() const              { return m_contactListener; }
        
        void getContactList(std::vector<PhysicsContact>& list) const;
        
    private:
        std::unique_ptr<b2World> m_boxWorld;
        std::vector<std::weak_ptr<PhysicsBody>> m_bodies;
        std::vector<ContactEvent> m_contactEvents;
        PhysicsContactListener* m_contactListener = nullptr;
        
        float m_pixelsPerMeter = 30.0f;
        
        float m_simulatedTime = 0.0f;
        float m_actualTime = 0.0f;
        float m_stepTime = 1.0f/60.0f;
    };
}
