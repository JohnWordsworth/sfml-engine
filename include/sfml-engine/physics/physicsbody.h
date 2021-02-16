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
        float restitution = 0.7f;
        float friction = 0.05f;
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
        void addEdgeList(const std::vector<sf::Vector2f> vertices, const PhysicsMaterial& material = PhysicsMaterial());
        
        sf::Vector2f getPosition();
        float getAngle();
        
        PhysicsWorld* getWorld() { return m_world; }
        
        void setNode(gbh::Node* node) { m_node = node; }
        gbh::Node* getNode() const { return m_node; }
        
        void setTransform(const sf::Vector2f& position, float angle);
        void setType(PhysicsBodyType type);
        
        void makeSensor();
        
        // Velocity
        
        sf::Vector2f getLinearVelocity() const;
        void setLinearVelocity(const sf::Vector2f& value);
        
        float getAngularVelocity() const;
        void setAngularVelocity(float value);

        // Methods for applying forces to bodies
        
        void applyForceToCenter(const sf::Vector2f& force, bool wake = true);
        void applyForce(const sf::Vector2f& force, const sf::Vector2f& point, bool wake = true);
        void applyTorque(float torque, bool wake = true);
        void applyImpulseToCenter(const sf::Vector2f& impulse, bool wake = true);
        void applyImpulse(const sf::Vector2f& impulse, const sf::Vector2f& point, bool wake = true);
        void applyAngularImpulse(float impulse, bool wake = true);
        
        // Other Box2D Methods to expose
        
        // Linear Damping
        float getLinearDamping() const              { return m_boxBody->GetLinearDamping(); }
        void setLinearDamping(float value)          { m_boxBody->SetLinearDamping(value); }

        float getAngularDamping() const             { return m_boxBody->GetAngularDamping(); }
        void setAngularDamping(float value)         { m_boxBody->SetAngularDamping(value);}
     
        float getGravityScale() const               { return m_boxBody->GetGravityScale(); }
        void setGravityScale(float value)           { m_boxBody->SetGravityScale(value); }
            
        bool isBullet() const                       { return m_boxBody->IsBullet(); }
        void setIsBullet(bool value)                { m_boxBody->SetBullet(value); }
        
        bool isSleepingAllowed() const              { return m_boxBody->IsSleepingAllowed(); }
        void setIsSleepingAllowed(bool value)       { m_boxBody->SetSleepingAllowed(value); }
        
        bool isAwake() const                        { return m_boxBody->IsAwake(); }
        void setAwake(bool value)                   { m_boxBody->SetAwake(value); }

        bool isEnabled() const                      { return m_boxBody->IsEnabled(); }
        void setEnabled(bool value)                 { m_boxBody->SetEnabled(value); }

        bool isFixedRotation() const                { return m_boxBody->IsFixedRotation(); }
        void setFixedRotation(bool value)           { return m_boxBody->SetFixedRotation(value); }
        
        void destroy();
        
    private:
        static float boxAngleToSfml(float angle);
        static float sfmlAngleToBox(float angle);
        
        gbh::Node* m_node = nullptr;
        PhysicsWorld* m_world = nullptr;
        b2Body* m_boxBody = nullptr;
        std::vector<b2Fixture*> m_fixtures;
    };
}
