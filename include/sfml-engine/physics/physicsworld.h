#pragma once

#include "physicsbody.h"

#include <SFML/System.hpp>
#include <box2d/box2d.h>
#include <memory>
#include <vector>


namespace gbh
{
    class PhysicsWorld
    {
    public:
        PhysicsWorld(const sf::Vector2f gravity);
        ~PhysicsWorld();
        
        std::shared_ptr<PhysicsBody> createEmptyBody();
        std::shared_ptr<PhysicsBody> createBox(const sf::Vector2f& size, const PhysicsMaterial& material = PhysicsMaterial());
        std::shared_ptr<PhysicsBody> createCircle(float radius, const PhysicsMaterial& material = PhysicsMaterial());
        std::shared_ptr<PhysicsBody> createEdgeBox(const sf::Vector2f& size, const PhysicsMaterial& material = PhysicsMaterial());
        
        void simulate(double deltaTime);
        void destroy();
        
        b2World* getBoxWorld() const { return m_boxWorld.get(); }
        float getPixelsPerMeter() const { return m_pixelsPerMeter; }
        
        b2Vec2 sfmlVectorToBoxWorld(const sf::Vector2f& sfVector) const;
        sf::Vector2f boxVectorToSfmlWorld(const b2Vec2& boxVector) const;
        
    private:
        std::unique_ptr<b2World> m_boxWorld;
        std::vector<std::weak_ptr<PhysicsBody>> m_bodies;
        float m_pixelsPerMeter = 30.0f;
        
        float m_simulatedTime = 0.0f;
        float m_actualTime = 0.0f;
        float m_stepTime = 1.0f/60.0f;
    };
}
